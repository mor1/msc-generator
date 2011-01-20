
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
#line 193 "colorsyntax.cc"

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
     TOK_SHOW = 301,
     TOK_HIDE = 302,
     TOK_BYE = 303,
     TOK__NEVER__HAPPENS = 304
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
#define TOK_SHOW 301
#define TOK_HIDE 302
#define TOK_BYE 303
#define TOK__NEVER__HAPPENS 304




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 134 "language.yy"

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
#line 346 "colorsyntax.cc"
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
#line 371 "colorsyntax.cc"

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
#define YYFINAL  139
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   919

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  242
/* YYNRULES -- Number of states.  */
#define YYNSTATES  295

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   304

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
      45,    46,    47,    48,    49
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    14,    17,    19,
      22,    26,    28,    31,    35,    39,    42,    47,    51,    54,
      56,    59,    61,    63,    66,    69,    73,    78,    80,    84,
      86,    89,    91,    94,    97,   101,   104,   108,   110,   112,
     115,   118,   122,   127,   132,   134,   136,   138,   141,   144,
     147,   149,   152,   154,   157,   160,   164,   166,   169,   171,
     175,   178,   182,   186,   190,   194,   198,   201,   205,   208,
     210,   214,   217,   221,   224,   226,   229,   231,   233,   237,
     240,   242,   246,   248,   252,   256,   262,   269,   271,   273,
     277,   280,   283,   285,   287,   291,   294,   298,   302,   306,
     310,   312,   315,   317,   319,   322,   326,   330,   333,   338,
     342,   344,   347,   350,   354,   357,   361,   364,   368,   370,
     372,   375,   379,   382,   385,   387,   390,   394,   398,   402,
     407,   409,   411,   413,   418,   422,   426,   429,   434,   438,
     442,   445,   447,   449,   451,   453,   457,   460,   463,   467,
     470,   474,   477,   480,   484,   487,   491,   494,   497,   501,
     504,   506,   508,   510,   512,   514,   516,   518,   520,   522,
     524,   526,   528,   530,   532,   534,   536,   538,   540,   542,
     544,   546,   549,   553,   556,   558,   561,   565,   570,   574,
     577,   581,   583,   586,   589,   591,   595,   598,   602,   606,
     610,   614,   617,   621,   623,   625,   627,   629,   631,   633,
     635,   637,   639,   641,   643,   645,   647,   649,   651,   653,
     655,   657,   659,   661,   663,   665,   667,   669,   671,   673,
     675,   677,   679,   681,   683,   685,   687,   689,   691,   693,
     695,   697,   700
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      51,     0,    -1,    52,    -1,    52,    48,    -1,    -1,    54,
      55,    -1,    53,    -1,    53,     1,    -1,    56,    -1,    56,
      12,    -1,    56,    12,    53,    -1,    15,    -1,    15,     7,
      -1,    15,     7,   104,    -1,   105,    56,   106,    -1,   105,
     106,    -1,   105,    56,     1,   106,    -1,   105,    56,     1,
      -1,   105,    56,    -1,    57,    -1,    57,    59,    -1,    59,
      -1,    58,    -1,    57,    58,    -1,    59,     9,    -1,    59,
       9,    49,    -1,    59,     1,     9,    49,    -1,     9,    -1,
      59,     1,     9,    -1,    60,    -1,    43,    60,    -1,    86,
      -1,    86,    97,    -1,    40,    86,    -1,    40,    86,    97,
      -1,    44,    85,    -1,    44,    85,    97,    -1,    97,    -1,
      65,    -1,    46,    65,    -1,    47,    65,    -1,    65,     8,
      63,    -1,    46,    65,     8,    63,    -1,    47,    65,     8,
      63,    -1,    61,    -1,    78,    -1,    77,    -1,    37,    69,
      -1,    38,    66,    -1,    39,    71,    -1,    34,    -1,    34,
      98,    -1,    35,    -1,    35,    98,    -1,    42,   104,    -1,
      42,   104,    98,    -1,    36,    -1,    36,    98,    -1,    62,
      -1,    61,     8,    62,    -1,    61,     8,    -1,    61,     8,
      49,    -1,    61,     8,     1,    -1,   101,     7,    19,    -1,
     101,     7,     5,    -1,   101,     7,   104,    -1,   101,     7,
      -1,    15,     7,   104,    -1,    15,     7,    -1,    64,    -1,
      63,     8,    64,    -1,    63,     8,    -1,    63,     8,    49,
      -1,   101,    97,    -1,   101,    -1,   101,    97,    -1,   101,
      -1,    67,    -1,    66,     8,    67,    -1,    68,    98,    -1,
     104,    -1,    68,     8,   104,    -1,    70,    -1,    69,     8,
      70,    -1,     3,     7,   104,    -1,     3,    72,    73,     9,
      12,    -1,     3,    72,    73,     9,     1,    12,    -1,    11,
      -1,    74,    -1,    73,     9,    74,    -1,    37,    69,    -1,
      38,    66,    -1,    75,    -1,    76,    -1,    75,     8,    76,
      -1,    75,     1,    -1,   101,     7,    19,    -1,   101,     7,
       5,    -1,   101,     7,   104,    -1,    15,     7,   104,    -1,
      55,    -1,    77,    55,    -1,    79,    -1,    81,    -1,    78,
      82,    -1,    78,    82,    97,    -1,    78,    82,    55,    -1,
      78,    55,    -1,    78,    82,    97,    55,    -1,    78,    97,
      55,    -1,    82,    -1,    82,    97,    -1,    82,    55,    -1,
      82,    97,    55,    -1,    41,    82,    -1,    41,    82,    97,
      -1,    80,    82,    -1,    80,    82,    97,    -1,    41,    -1,
      80,    -1,    80,    55,    -1,   101,    33,   101,    -1,    33,
     101,    -1,   101,    33,    -1,    33,    -1,    45,   101,    -1,
      45,   101,     6,    -1,    45,   101,    10,    -1,    45,   101,
      33,    -1,    45,   101,     6,   101,    -1,    33,    -1,    90,
      -1,    92,    -1,   101,    84,   101,    83,    -1,    84,   101,
      83,    -1,   101,    84,    83,    -1,    84,    83,    -1,   101,
      91,   101,    83,    -1,    91,   101,    83,    -1,   101,    91,
      83,    -1,    91,    83,    -1,    32,    -1,    87,    -1,    88,
      -1,    89,    -1,   101,    90,   101,    -1,    90,   101,    -1,
     101,    90,    -1,    87,    93,   101,    -1,    87,    93,    -1,
     101,    91,   101,    -1,    91,   101,    -1,   101,    91,    -1,
      88,    94,   101,    -1,    88,    94,    -1,   101,    92,   101,
      -1,    92,   101,    -1,   101,    92,    -1,    89,    95,   101,
      -1,    89,    95,    -1,    20,    -1,    23,    -1,    26,    -1,
      29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,    -1,
      22,    -1,    25,    -1,    28,    -1,    31,    -1,    90,    -1,
       6,    -1,    91,    -1,     6,    -1,    92,    -1,     6,    -1,
      17,    -1,    16,    -1,    96,    -1,    96,    98,    -1,    98,
      96,    98,    -1,    98,    96,    -1,    98,    -1,    13,    14,
      -1,    13,    99,    14,    -1,    13,    99,     1,    14,    -1,
      13,     1,    14,    -1,    13,    99,    -1,    13,    99,     1,
      -1,    13,    -1,    13,    49,    -1,    13,     1,    -1,   100,
      -1,    99,     8,   100,    -1,    99,     8,    -1,    99,     8,
      49,    -1,   104,     7,   104,    -1,   104,     7,     5,    -1,
     104,     7,    19,    -1,   104,     7,    -1,   104,     7,    49,
      -1,   104,    -1,     4,    -1,     3,    -1,    15,    -1,    37,
      -1,    38,    -1,    39,    -1,    36,    -1,    40,    -1,    41,
      -1,    44,    -1,    43,    -1,    34,    -1,    35,    -1,    42,
      -1,    45,    -1,    46,    -1,    47,    -1,    48,    -1,    20,
      -1,    21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,
      -1,    26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,
      -1,    31,    -1,    32,    -1,    33,    -1,   101,    -1,   102,
      -1,   103,    -1,    18,    -1,    11,    -1,    11,    49,    -1,
      12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   196,   196,   197,   208,   218,   228,   235,   247,   248,
     257,   271,   279,   291,   306,   316,   329,   341,   352,   364,
     365,   375,   386,   395,   404,   418,   419,   420,   432,   447,
     455,   469,   476,   487,   501,   518,   527,   540,   551,   558,
     569,   580,   589,   601,   613,   625,   632,   639,   648,   657,
     666,   675,   688,   697,   710,   721,   736,   745,   759,   768,
     786,   799,   800,   811,   831,   851,   878,   898,   917,   938,
     944,   956,   968,   971,   986,   998,  1012,  1029,  1030,  1037,
    1056,  1072,  1090,  1091,  1098,  1114,  1134,  1157,  1168,  1169,
    1176,  1183,  1190,  1192,  1193,  1199,  1206,  1218,  1230,  1242,
    1256,  1265,  1277,  1283,  1288,  1297,  1312,  1321,  1330,  1344,
    1361,  1369,  1382,  1390,  1404,  1414,  1428,  1436,  1449,  1462,
    1463,  1470,  1484,  1495,  1507,  1517,  1529,  1543,  1556,  1588,
    1605,  1605,  1605,  1607,  1620,  1631,  1642,  1651,  1664,  1675,
    1686,  1697,  1705,  1709,  1713,  1718,  1732,  1743,  1755,  1766,
    1777,  1791,  1802,  1814,  1825,  1835,  1849,  1860,  1872,  1883,
    1893,  1893,  1893,  1893,  1894,  1894,  1894,  1894,  1895,  1895,
    1895,  1895,  1897,  1897,  1898,  1898,  1899,  1899,  1902,  1909,
    1918,  1926,  1934,  1946,  1954,  1957,  1967,  1977,  1988,  1999,
    2010,  2021,  2034,  2038,  2050,  2057,  2066,  2076,  2078,  2092,
    2106,  2120,  2134,  2139,  2151,  2151,  2153,  2153,  2154,  2154,
    2155,  2155,  2155,  2156,  2156,  2157,  2157,  2158,  2158,  2158,
    2158,  2158,  2160,  2161,  2162,  2163,  2164,  2165,  2166,  2167,
    2168,  2169,  2170,  2171,  2172,  2179,  2189,  2189,  2189,  2189,
    2191,  2205,  2207
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
  "TOK_VERTICAL", "TOK_AT", "TOK_SHOW", "TOK_HIDE", "TOK_BYE",
  "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye", "msc",
  "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "arc_with_parallel", "arc", "optlist", "opt", "entitylist", "entity",
  "first_entity", "styledeflist", "styledef", "tok_stringlist",
  "colordeflist", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "emphasis_list", "first_emphasis", "pipe_def_list",
  "pipe_emphasis", "emphrel", "vertxpos", "empharcrel_straight", "vertrel",
  "arcrel", "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
  "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont", "colon_string",
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    51,    52,    52,    52,    52,    53,    53,
      53,    54,    54,    54,    55,    55,    55,    55,    55,    56,
      56,    56,    57,    57,    58,    58,    58,    58,    58,    59,
      59,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    61,    61,
      61,    61,    61,    62,    62,    62,    62,    62,    62,    63,
      63,    63,    63,    64,    64,    65,    65,    66,    66,    67,
      68,    68,    69,    69,    70,    71,    71,    72,    73,    73,
      74,    74,    74,    75,    75,    75,    76,    76,    76,    76,
      77,    77,    78,    78,    78,    78,    78,    78,    78,    78,
      79,    79,    79,    79,    80,    80,    80,    80,    80,    81,
      81,    82,    82,    82,    82,    83,    83,    83,    83,    83,
      84,    84,    84,    85,    85,    85,    85,    85,    85,    85,
      85,    86,    86,    86,    86,    87,    87,    87,    87,    87,
      88,    88,    88,    88,    88,    89,    89,    89,    89,    89,
      90,    90,    90,    90,    91,    91,    91,    91,    92,    92,
      92,    92,    93,    93,    94,    94,    95,    95,    96,    96,
      97,    97,    97,    97,    97,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    99,    99,    99,    99,   100,   100,
     100,   100,   100,   100,   101,   101,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   104,   104,   104,   104,
     105,   105,   106
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     1,     2,     1,     2,
       3,     1,     2,     3,     3,     2,     4,     3,     2,     1,
       2,     1,     1,     2,     2,     3,     4,     1,     3,     1,
       2,     1,     2,     2,     3,     2,     3,     1,     1,     2,
       2,     3,     4,     4,     1,     1,     1,     2,     2,     2,
       1,     2,     1,     2,     2,     3,     1,     2,     1,     3,
       2,     3,     3,     3,     3,     3,     2,     3,     2,     1,
       3,     2,     3,     2,     1,     2,     1,     1,     3,     2,
       1,     3,     1,     3,     3,     5,     6,     1,     1,     3,
       2,     2,     1,     1,     3,     2,     3,     3,     3,     3,
       1,     2,     1,     1,     2,     3,     3,     2,     4,     3,
       1,     2,     2,     3,     2,     3,     2,     3,     1,     1,
       2,     3,     2,     2,     1,     2,     3,     3,     3,     4,
       1,     1,     1,     4,     3,     3,     2,     4,     3,     3,
       2,     1,     1,     1,     1,     3,     2,     2,     3,     2,
       3,     2,     2,     3,     2,     3,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     2,     1,     2,     3,     4,     3,     2,
       3,     1,     2,     2,     1,     3,     2,     3,     3,     3,
       3,     2,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,   205,   204,    27,   240,     0,    11,   179,   178,   160,
     164,   168,   161,   165,   169,   162,   166,   170,   163,   167,
     171,   141,   124,    50,    52,    56,     0,     0,     0,     0,
     118,     0,     0,     0,     0,     0,     0,     2,     0,     0,
     100,     8,    19,    22,     0,    29,    44,    58,    38,    46,
      45,   102,   119,   103,   110,    31,   142,   143,   144,     0,
       0,     0,   180,    37,   184,    76,     0,   241,   193,   185,
     206,   239,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   215,   216,   210,   207,
     208,   209,   211,   212,   217,   214,   213,   218,   219,   220,
     221,   192,     0,   194,   236,   237,   238,   203,    68,   122,
      51,    53,    57,     0,    47,    82,    48,    77,     0,    80,
       0,    49,    33,     0,   114,     0,    54,     0,    30,   130,
       0,    35,   131,     0,   132,     0,    39,    76,    40,     1,
       3,     7,     5,     9,    23,     0,     0,    24,     0,     0,
     101,   107,   104,     0,   120,   116,   112,   111,    32,   173,
     172,   149,   175,   174,   154,   177,   176,   159,   146,   151,
     156,   181,   183,    66,   123,   147,   152,   157,    75,   242,
       0,    15,   188,   190,   196,   186,   201,    67,     0,     0,
       0,     0,    79,    87,     0,    34,   115,    55,    68,     0,
     136,     0,    36,   140,     0,     0,     0,     0,     0,    10,
      28,    25,    62,    61,    59,     0,    41,    69,    74,   106,
     105,   109,   117,   113,   148,   153,   158,   182,    64,    63,
      65,   121,   145,   150,   155,    17,    14,   187,   197,   195,
     199,   200,   202,   198,    84,    83,    78,    81,     0,     0,
       0,     0,    88,     0,    93,     0,    67,   125,   134,   138,
     135,     0,   139,     0,    42,    43,    26,    71,    73,   108,
      16,     0,    90,    91,     0,    95,     0,     0,   126,   127,
     128,   133,   137,    72,    70,    99,     0,    85,    89,    94,
      97,    96,    98,   129,    86
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,   216,   217,    48,   116,   117,   118,   114,
     115,   121,   194,   251,   252,   253,   254,    49,    50,    51,
      52,    53,    54,   200,   130,   131,    55,    56,    57,    58,
      59,    60,    61,   161,   164,   167,    62,    63,    64,   102,
     103,   104,   105,   106,   119,    66,   181
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -179
static const yytype_int16 yypact[] =
{
     697,  -179,  -179,  -179,   -43,   211,     8,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,   118,    18,    18,    18,    10,   606,    25,   846,
      21,   606,   787,   832,   118,   118,    41,    17,    19,    34,
    -179,    44,   742,  -179,    70,  -179,    54,  -179,    60,    34,
     176,  -179,   133,  -179,   252,    72,   301,   135,   339,   118,
     118,   118,    18,  -179,   108,   872,   652,  -179,    64,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,   302,  -179,  -179,  -179,  -179,    90,   560,  -179,
    -179,  -179,  -179,    95,   102,  -179,   105,  -179,    56,  -179,
     106,  -179,    72,   817,    72,    96,    18,   124,  -179,  -179,
       1,    72,  -179,     1,  -179,   886,   131,    72,   139,  -179,
    -179,  -179,  -179,   742,  -179,   325,   143,    93,   284,   118,
    -179,  -179,   252,    34,  -179,    72,  -179,    34,  -179,  -179,
    -179,   118,  -179,  -179,   118,  -179,  -179,   118,  -179,  -179,
    -179,  -179,    18,   468,   118,   118,   118,   118,  -179,  -179,
     343,  -179,  -179,   144,   421,  -179,   374,   149,   606,    10,
     606,   606,  -179,  -179,    89,  -179,  -179,  -179,   606,   118,
    -179,   125,  -179,  -179,   125,     1,     1,   118,   118,  -179,
     114,  -179,  -179,  -179,  -179,   165,   170,  -179,    72,  -179,
      34,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,   161,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,  -179,  -179,   174,    10,
     606,   175,  -179,    98,  -179,   181,  -179,    30,  -179,  -179,
    -179,   125,  -179,   125,   170,   170,  -179,     6,  -179,  -179,
    -179,   606,   102,   105,    97,  -179,   142,   514,   118,  -179,
    -179,  -179,  -179,  -179,  -179,  -179,   171,  -179,  -179,  -179,
    -179,  -179,  -179,  -179,  -179
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -179,  -179,  -179,    47,  -179,   -38,   129,  -179,   155,   159,
     172,  -179,    43,   -53,   -65,   134,   -37,    26,  -179,   -32,
      29,  -179,  -179,  -179,   -44,  -179,   -55,  -179,  -179,  -179,
    -179,  -179,    -3,  -110,   126,  -179,   233,  -179,  -179,  -179,
     -12,    15,    -7,  -179,  -179,  -179,   200,   -47,    14,  -179,
      82,     0,  -179,  -179,    12,  -179,  -178
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -192
static const yytype_int16 yytable[] =
{
      65,   142,   236,   153,     1,     2,    67,   157,   158,     1,
       2,   150,   151,   113,   154,   108,   156,   107,   178,    -6,
     141,   132,   109,   203,     1,     2,   134,   124,   120,   123,
     125,     5,    65,   135,   137,   137,   278,   110,   111,   112,
     279,   139,    65,   126,   160,     4,   199,   152,   133,   155,
     125,   166,   125,   175,    22,   283,   143,   270,   177,   168,
     169,   170,   148,   280,   191,   140,    65,    -6,   149,     5,
     -21,   146,   163,   -21,   -21,   195,   171,   196,   182,   147,
     176,   -21,   -21,   -21,   202,     5,   -21,   -21,     7,     8,
     178,   258,     1,     2,   259,   260,   262,   186,   286,   275,
       1,     2,   188,   -21,   248,   220,   276,   -92,   222,   287,
     189,   175,   248,   190,   219,   221,   177,   193,   -21,   223,
     187,     1,     2,   132,     7,     8,   249,   250,   134,   174,
     201,   198,   192,   204,   249,   250,     1,     2,   176,   207,
     197,   162,   211,    65,     4,     1,     2,   208,   215,   218,
     206,   281,   210,   282,   264,   265,    10,   248,   237,    13,
     -13,   224,    16,   266,   225,    19,    22,   226,   136,   138,
     199,   268,   173,   179,   231,   232,   233,   234,   267,     1,
       2,   271,   269,   294,   274,   230,   227,     4,   277,     5,
     209,   214,     7,     8,   255,   180,   107,   144,   243,   257,
     244,   145,   284,   247,   128,   261,   263,   218,   218,    22,
     256,  -191,    68,   273,     1,     2,   246,   272,   245,  -191,
    -191,   289,  -191,  -191,  -191,    69,    70,  -191,  -191,    71,
     288,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   205,   122,     4,   172,     5,   239,   218,     7,     8,
       0,     0,     0,     0,   255,     0,   255,     0,   293,     0,
       0,     0,     0,   285,   -60,   212,     0,     1,     2,   292,
       0,     0,   -60,   -60,     0,   -60,   -60,   -60,     0,   127,
     -60,   -60,  -189,   183,     0,  -189,  -189,   159,     0,     0,
     184,  -189,     0,  -189,  -189,  -189,   185,   -60,  -189,  -189,
       0,     9,     0,     0,    12,   -20,   146,    15,   -20,   -20,
      18,     0,   -60,   213,   147,  -189,   -20,   -20,   -20,     0,
       0,   -20,   -20,   -18,   235,   165,   -18,   -18,     0,     0,
    -189,     0,   -18,     0,   -18,   179,   -18,     0,   -20,   -18,
     -18,    11,     0,     0,    14,     0,     0,    17,     0,     0,
      20,     0,     0,   -20,     0,     0,   -18,     1,     2,   240,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
       0,   -18,    71,   241,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   242,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,     0,     0,    71,
       0,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     238,     1,     2,   228,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    70,     0,     0,    71,   229,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,     1,     2,   290,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    70,
       0,     0,    71,   291,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,     1,     2,     0,     0,     0,     0,     0,
       0,   -12,     0,     0,     0,    70,     0,     0,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,     1,
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    70,     0,     0,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,     1,     2,     0,     0,     0,
       0,     3,     0,     4,   179,     5,     0,   127,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       1,     2,     0,     0,     0,     0,     3,     0,     4,     0,
       5,     0,     6,     7,     8,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,     0,    34,    35,     1,     2,     0,     0,     0,
       0,     3,     0,     4,     0,     5,     0,   127,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     0,    34,    35,
       1,     2,     0,     0,     0,     0,     0,     0,     4,     0,
       5,     0,   127,     7,     8,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       0,    33,     0,    34,    35,     1,     2,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,     1,
       2,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,   129,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   173,
       0,     0,     0,     0,     0,     5,     0,     0,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,     0,   174,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,     0,   129
};

static const yytype_int16 yycheck[] =
{
       0,    39,   180,    50,     3,     4,    49,    54,    55,     3,
       4,    49,    50,     3,    52,     7,    54,     5,    65,     0,
       1,    33,    22,   133,     3,     4,    33,    30,     3,    29,
      30,    13,    32,    33,    34,    35,     6,    23,    24,    25,
      10,     0,    42,    31,    56,    11,    45,    50,    33,    52,
      50,    58,    52,    65,    33,    49,    12,   235,    65,    59,
      60,    61,     8,    33,     8,    48,    66,    48,     8,    13,
       0,     1,    57,     3,     4,   122,    62,   124,    14,     9,
      65,    11,    12,    13,   131,    13,    16,    17,    16,    17,
     137,   201,     3,     4,   204,   205,   206,     7,     1,     1,
       3,     4,     7,    33,    15,   152,     8,     9,   155,    12,
       8,   123,    15,     8,   152,   153,   123,    11,    48,   157,
     108,     3,     4,   135,    16,    17,    37,    38,   135,    33,
     130,     7,   118,   133,    37,    38,     3,     4,   123,     8,
     126,     6,    49,   143,    11,     3,     4,     8,   148,   149,
     135,   261,     9,   263,   207,   208,    21,    15,    14,    24,
      11,   161,    27,    49,   164,    30,    33,   167,    34,    35,
      45,   218,     7,    12,   174,   175,   176,   177,     8,     3,
       4,     7,   220,    12,     9,   173,   172,    11,     7,    13,
     143,   148,    16,    17,   194,    66,   184,    42,   186,   199,
     188,    42,   267,   191,    32,   205,   206,   207,   208,    33,
     198,     0,     1,   250,     3,     4,   190,   249,   189,     8,
       9,   276,    11,    12,    13,    14,    15,    16,    17,    18,
     274,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,   135,    29,    11,    64,    13,   184,   267,    16,    17,
      -1,    -1,    -1,    -1,   274,    -1,   276,    -1,   278,    -1,
      -1,    -1,    -1,   271,     0,     1,    -1,     3,     4,   277,
      -1,    -1,     8,     9,    -1,    11,    12,    13,    -1,    15,
      16,    17,     0,     1,    -1,     3,     4,     6,    -1,    -1,
       8,     9,    -1,    11,    12,    13,    14,    33,    16,    17,
      -1,    20,    -1,    -1,    23,     0,     1,    26,     3,     4,
      29,    -1,    48,    49,     9,    33,    11,    12,    13,    -1,
      -1,    16,    17,     0,     1,     6,     3,     4,    -1,    -1,
      48,    -1,     9,    -1,    11,    12,    13,    -1,    33,    16,
      17,    22,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    48,    -1,    -1,    33,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    48,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    18,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,     3,     4,     5,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,
      -1,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    15,    -1,    -1,    18,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    11,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    47,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    -1,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    46,    47,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      13,    -1,    15,    16,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      -1,    44,    -1,    46,    47,     3,     4,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,     3,
       4,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     7,
      -1,    -1,    -1,    -1,    -1,    13,    -1,    -1,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    11,    13,    15,    16,    17,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    46,    47,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    65,    77,
      78,    79,    80,    81,    82,    86,    87,    88,    89,    90,
      91,    92,    96,    97,    98,   101,   105,    49,     1,    14,
      15,    18,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    99,   100,   101,   102,   103,   104,     7,   101,
      98,    98,    98,     3,    69,    70,    66,    67,    68,   104,
       3,    71,    86,   101,    82,   101,   104,    15,    60,    33,
      84,    85,    90,    91,    92,   101,    65,   101,    65,     0,
      48,     1,    55,    12,    58,    59,     1,     9,     8,     8,
      55,    55,    82,    97,    55,    82,    55,    97,    97,     6,
      90,    93,     6,    91,    94,     6,    92,    95,   101,   101,
     101,    98,    96,     7,    33,    90,    91,    92,    97,    12,
      56,   106,    14,     1,     8,    14,     7,   104,     7,     8,
       8,     8,    98,    11,    72,    97,    97,    98,     7,    45,
      83,   101,    97,    83,   101,    84,    91,     8,     8,    53,
       9,    49,     1,    49,    62,   101,    63,    64,   101,    55,
      97,    55,    97,    55,   101,   101,   101,    98,     5,    19,
     104,   101,   101,   101,   101,     1,   106,    14,    49,   100,
       5,    19,    49,   104,   104,    70,    67,   104,    15,    37,
      38,    73,    74,    75,    76,   101,   104,   101,    83,    83,
      83,   101,    83,   101,    63,    63,    49,     8,    97,    55,
     106,     7,    69,    66,     9,     1,     8,     7,     6,    10,
      33,    83,    83,    49,    64,   104,     1,    12,    74,    76,
       5,    19,   104,   101,    12
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
#line 187 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1755 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1764 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1773 "colorsyntax.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1782 "colorsyntax.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1791 "colorsyntax.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1800 "colorsyntax.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1809 "colorsyntax.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1818 "colorsyntax.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1827 "colorsyntax.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1836 "colorsyntax.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1845 "colorsyntax.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1854 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1863 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1872 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1881 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1890 "colorsyntax.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1899 "colorsyntax.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1908 "colorsyntax.cc"
	break;
      case 44: /* "TOK_VERTICAL" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1917 "colorsyntax.cc"
	break;
      case 45: /* "TOK_AT" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1926 "colorsyntax.cc"
	break;
      case 46: /* "TOK_SHOW" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1935 "colorsyntax.cc"
	break;
      case 47: /* "TOK_HIDE" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1944 "colorsyntax.cc"
	break;
      case 48: /* "TOK_BYE" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1953 "colorsyntax.cc"
	break;
      case 53: /* "top_level_arclist" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1962 "colorsyntax.cc"
	break;
      case 55: /* "braced_arclist" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1971 "colorsyntax.cc"
	break;
      case 56: /* "arclist_maybe_no_semicolon" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1980 "colorsyntax.cc"
	break;
      case 57: /* "arclist" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1989 "colorsyntax.cc"
	break;
      case 58: /* "arc_with_parallel_semicolon" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1998 "colorsyntax.cc"
	break;
      case 59: /* "arc_with_parallel" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2007 "colorsyntax.cc"
	break;
      case 60: /* "arc" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2016 "colorsyntax.cc"
	break;
      case 61: /* "optlist" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 2025 "colorsyntax.cc"
	break;
      case 62: /* "opt" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2034 "colorsyntax.cc"
	break;
      case 63: /* "entitylist" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 2043 "colorsyntax.cc"
	break;
      case 64: /* "entity" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 2052 "colorsyntax.cc"
	break;
      case 65: /* "first_entity" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 2061 "colorsyntax.cc"
	break;
      case 68: /* "tok_stringlist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1000 of yacc.c  */
#line 2070 "colorsyntax.cc"
	break;
      case 77: /* "parallel" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1000 of yacc.c  */
#line 2079 "colorsyntax.cc"
	break;
      case 78: /* "emphasis_list" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2088 "colorsyntax.cc"
	break;
      case 79: /* "first_emphasis" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2097 "colorsyntax.cc"
	break;
      case 80: /* "pipe_def_list" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2106 "colorsyntax.cc"
	break;
      case 81: /* "pipe_emphasis" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2115 "colorsyntax.cc"
	break;
      case 82: /* "emphrel" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2124 "colorsyntax.cc"
	break;
      case 83: /* "vertxpos" */

/* Line 1000 of yacc.c  */
#line 178 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1000 of yacc.c  */
#line 2133 "colorsyntax.cc"
	break;
      case 85: /* "vertrel" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2142 "colorsyntax.cc"
	break;
      case 86: /* "arcrel" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2151 "colorsyntax.cc"
	break;
      case 87: /* "arcrel_to" */

/* Line 1000 of yacc.c  */
#line 180 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2160 "colorsyntax.cc"
	break;
      case 88: /* "arcrel_from" */

/* Line 1000 of yacc.c  */
#line 180 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2169 "colorsyntax.cc"
	break;
      case 89: /* "arcrel_bidir" */

/* Line 1000 of yacc.c  */
#line 180 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2178 "colorsyntax.cc"
	break;
      case 97: /* "full_arcattrlist_with_label" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2187 "colorsyntax.cc"
	break;
      case 98: /* "full_arcattrlist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2196 "colorsyntax.cc"
	break;
      case 99: /* "arcattrlist" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2205 "colorsyntax.cc"
	break;
      case 100: /* "arcattr" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1000 of yacc.c  */
#line 2214 "colorsyntax.cc"
	break;
      case 101: /* "entity_string" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2223 "colorsyntax.cc"
	break;
      case 102: /* "reserved_word_string" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2232 "colorsyntax.cc"
	break;
      case 103: /* "symbol_string" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2241 "colorsyntax.cc"
	break;
      case 104: /* "string" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2250 "colorsyntax.cc"
	break;
      case 106: /* "scope_close" */

/* Line 1000 of yacc.c  */
#line 179 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2259 "colorsyntax.cc"
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
#line 198 "language.yy"
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
#line 208 "language.yy"
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
#line 219 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        csh.AddDesignsToHints();
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
    YYACCEPT;
}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 229 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 236 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (((yylsp[(1) - (2)])).last_pos >= ((yylsp[(2) - (2)])).first_pos)
        ((yylsp[(2) - (2)])).first_pos = ((yylsp[(1) - (2)])).last_pos;
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
  #endif
  YYACCEPT;
}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 249 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_Error((yylsp[(2) - (2)]), "Closing brace missing its opening pair.");
  #else
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 258 "language.yy"
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

  case 11:

/* Line 1455 of yacc.c  */
#line 272 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 280 "language.yy"
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

  case 13:

/* Line 1455 of yacc.c  */
#line 292 "language.yy"
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

  case 14:

/* Line 1455 of yacc.c  */
#line 307 "language.yy"
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

  case 15:

/* Line 1455 of yacc.c  */
#line 317 "language.yy"
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

  case 16:

/* Line 1455 of yacc.c  */
#line 330 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACE);
        csh.AddCSH_Error((yylsp[(3) - (4)]), "Could not recognize this as a valid line.");
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
  #else
        if ((yyvsp[(4) - (4)].arcbase)) ((yyvsp[(2) - (4)].arclist))->Append((yyvsp[(4) - (4)].arcbase)); //Append any potential CommandNumbering
        (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
  #endif
    yyerrok;
}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 342 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        csh.AddCSH_Error((yylsp[(3) - (3)]), "Could not recognize this as a valid line.");
  #else
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
        //Do not pop context, as the missing scope_close would have done
        msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Missing '}'.");
  #endif
}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 353 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACE);
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a semicolon (';').");
  #else
        (yyval.arclist) = (yyvsp[(2) - (2)].arclist);
        //Do not pop context, as the missing scope_close would have done
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing '}'.");
  #endif
}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 366 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a semicolon (';').");
  #else
        if ((yyvsp[(2) - (2)].arcbase)) ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing ';'.");
  #endif
}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 376 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a semicolon (';').");
  #else
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
        msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing ';'.");
  #endif
}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 387 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 396 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 405 "language.yy"
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

  case 27:

/* Line 1455 of yacc.c  */
#line 421 "language.yy"
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

  case 28:

/* Line 1455 of yacc.c  */
#line 433 "language.yy"
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
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).end.NextChar(), "Missing ';'.");
  #endif
}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 448 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((yyvsp[(1) - (1)].arcbase)) ((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 456 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
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

  case 31:

/* Line 1455 of yacc.c  */
#line 470 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 477 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            ArcArrow::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 488 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
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

  case 34:

/* Line 1455 of yacc.c  */
#line 502 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            ArcBigArrow::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
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

  case 35:

/* Line 1455 of yacc.c  */
#line 519 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = ((yyvsp[(2) - (2)].arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 528 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 541 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            ArcDivider::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            ArcDivider::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
  #endif
}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 552 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)), &msc))->AddAttributeList(NULL);
  #endif
}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 559 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    CommandEntity *ce = new CommandEntity((new EntityDefList)->Append((yyvsp[(2) - (2)].entity)), &msc);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyShowHide(true);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 570 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    CommandEntity *ce = new CommandEntity((new EntityDefList)->Append((yyvsp[(2) - (2)].entity)), &msc);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyShowHide(false);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 581 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entity)), &msc);
    (yyval.arcbase) = ce->AddAttributeList(NULL);
  #endif
}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 590 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(4) - (4)].entitylist))->Prepend((yyvsp[(2) - (4)].entity)), &msc);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyShowHide(true);
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 602 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
  #else
    CommandEntity *ce = new CommandEntity(((yyvsp[(4) - (4)].entitylist))->Prepend((yyvsp[(2) - (4)].entity)), &msc);
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyShowHide(false);
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 614 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 626 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
  #endif
}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 633 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel))->AddAttributeList(NULL);
  #endif
}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 640 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 649 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 658 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 667 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 676 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            CommandEntity::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
        (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 689 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 698 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            ArcDivider::AttributeNames(csh, true);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            ArcDivider::AttributeValues(csh.hintAttrName, csh, true);
  #else
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 711 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
  #else
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (2)].str), MSC_POS((yyloc)), &msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 722 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            CommandMark::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), MSC_POS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 737 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 746 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        if (csh.CheckHintLocated(HINT_ATTR_NAME))
            CommandNewpage::AttributeNames(csh);
        else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
            CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 760 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 769 "language.yy"
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

  case 60:

/* Line 1455 of yacc.c  */
#line 787 "language.yy"
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

  case 62:

/* Line 1455 of yacc.c  */
#line 801 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 812 "language.yy"
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

  case 64:

/* Line 1455 of yacc.c  */
#line 832 "language.yy"
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

  case 65:

/* Line 1455 of yacc.c  */
#line 852 "language.yy"
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

  case 66:

/* Line 1455 of yacc.c  */
#line 879 "language.yy"
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

  case 67:

/* Line 1455 of yacc.c  */
#line 899 "language.yy"
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

  case 68:

/* Line 1455 of yacc.c  */
#line 918 "language.yy"
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

  case 69:

/* Line 1455 of yacc.c  */
#line 939 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
  #endif
}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 945 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        if (csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ENTITY)) {
		    csh.AddEntitiesToHints();
			csh.hintStatus = HINT_READY;
		}
  #else
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
  #endif
}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 957 "language.yy"
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

  case 73:

/* Line 1455 of yacc.c  */
#line 972 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 987 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
    (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL);
    ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 999 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
        csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1013 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
        csh.AddCSH_KeywordOrEntity((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1031 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1038 "language.yy"
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

  case 80:

/* Line 1455 of yacc.c  */
#line 1057 "language.yy"
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

  case 81:

/* Line 1455 of yacc.c  */
#line 1073 "language.yy"
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

  case 83:

/* Line 1455 of yacc.c  */
#line 1092 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1099 "language.yy"
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

  case 85:

/* Line 1455 of yacc.c  */
#line 1115 "language.yy"
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

  case 86:

/* Line 1455 of yacc.c  */
#line 1135 "language.yy"
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

  case 87:

/* Line 1455 of yacc.c  */
#line 1158 "language.yy"
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

  case 89:

/* Line 1455 of yacc.c  */
#line 1170 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1177 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1184 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1194 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1200 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1207 "language.yy"
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

  case 97:

/* Line 1455 of yacc.c  */
#line 1219 "language.yy"
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

  case 98:

/* Line 1455 of yacc.c  */
#line 1231 "language.yy"
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

  case 99:

/* Line 1455 of yacc.c  */
#line 1243 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
  #else
        msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yyloc)), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1257 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1266 "language.yy"
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

  case 102:

/* Line 1455 of yacc.c  */
#line 1278 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1284 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1289 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow();
    ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
    ((yyvsp[(2) - (2)].arcemph))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1298 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow();
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
    ((yyvsp[(2) - (3)].arcemph))->AddAttributeList((yyvsp[(3) - (3)].attriblist)); //should come after AddFollow

  #endif
}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1313 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow();
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
    ((yyvsp[(2) - (3)].arcemph))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1322 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1331 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(2) - (4)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (4)]), (yylsp[(3) - (4)])));
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow();
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
    ((yyvsp[(2) - (4)].arcemph))->AddAttributeList((yyvsp[(3) - (4)].attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1345 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (3)])), NULL, MSC_POS((yylsp[(1) - (3)])), &msc);
    temp->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
    temp->AddAttributeList((yyvsp[(2) - (3)].attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 1362 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcemph))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 1370 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
  #endif
}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 1383 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 1391 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
  #endif
}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 1405 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
        ((yyvsp[(2) - (2)].arcemph))->SetPipe()->AddAttributeList(NULL)->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 1415 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ((yyvsp[(2) - (3)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(3) - (3)].attriblist))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = (yyvsp[(2) - (3)].arcemph);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1429 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->SetPipe()->ChangeStyleForFollow()->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
    ((yyvsp[(2) - (2)].arcemph))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 1437 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME))
        ArcEmphasis::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE))
        ArcEmphasis::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ((yyvsp[(2) - (3)].arcemph))->SetPipe()->ChangeStyleForFollow()->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
    ((yyvsp[(2) - (3)].arcemph))->AddAttributeList((yyvsp[(3) - (3)].attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 1450 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
        (yyval.arcemph) = NULL;
        msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "The keyword '" + string((yyvsp[(1) - (1)].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1464 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1471 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1485 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1496 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(2) - (2)])), &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1508 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 1518 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 1530 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
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

  case 127:

/* Line 1455 of yacc.c  */
#line 1544 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    csh.AddCSH_EntityName((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 1557 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
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

  case 129:

/* Line 1455 of yacc.c  */
#line 1589 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
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

  case 133:

/* Line 1455 of yacc.c  */
#line 1608 "language.yy"
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

  case 134:

/* Line 1455 of yacc.c  */
#line 1621 "language.yy"
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

  case 135:

/* Line 1455 of yacc.c  */
#line 1632 "language.yy"
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

  case 136:

/* Line 1455 of yacc.c  */
#line 1643 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 1652 "language.yy"
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

  case 138:

/* Line 1455 of yacc.c  */
#line 1665 "language.yy"
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

  case 139:

/* Line 1455 of yacc.c  */
#line 1676 "language.yy"
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

  case 140:

/* Line 1455 of yacc.c  */
#line 1687 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 1698 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 1706 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 1710 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 1714 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 1719 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 1733 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 1744 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 1756 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAt((yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), true, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 1767 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 1778 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 1792 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1803 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1815 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), false, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1826 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), false, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1836 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1850 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1861 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==EOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1873 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), true, MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1884 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1903 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1910 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1919 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1927 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1935 "language.yy"
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

  case 183:

/* Line 1455 of yacc.c  */
#line 1947 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 1958 "language.yy"
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

  case 186:

/* Line 1455 of yacc.c  */
#line 1968 "language.yy"
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

  case 187:

/* Line 1455 of yacc.c  */
#line 1978 "language.yy"
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

  case 188:

/* Line 1455 of yacc.c  */
#line 1989 "language.yy"
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

  case 189:

/* Line 1455 of yacc.c  */
#line 2000 "language.yy"
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

  case 190:

/* Line 1455 of yacc.c  */
#line 2011 "language.yy"
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

  case 191:

/* Line 1455 of yacc.c  */
#line 2022 "language.yy"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 2035 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 2039 "language.yy"
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

  case 194:

/* Line 1455 of yacc.c  */
#line 2051 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 2058 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 2067 "language.yy"
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

  case 198:

/* Line 1455 of yacc.c  */
#line 2079 "language.yy"
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

  case 199:

/* Line 1455 of yacc.c  */
#line 2093 "language.yy"
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

  case 200:

/* Line 1455 of yacc.c  */
#line 2107 "language.yy"
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

  case 201:

/* Line 1455 of yacc.c  */
#line 2121 "language.yy"
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

  case 202:

/* Line 1455 of yacc.c  */
#line 2135 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 2140 "language.yy"
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

  case 222:

/* Line 1455 of yacc.c  */
#line 2160 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 2161 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 2162 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 2163 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 2164 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 2165 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 2166 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 2167 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 2168 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 2169 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 2170 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 2171 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 2173 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 2180 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 2192 "language.yy"
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

  case 242:

/* Line 1455 of yacc.c  */
#line 2208 "language.yy"
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
#line 5424 "colorsyntax.cc"
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
#line 2218 "language.yy"



/* END OF FILE */

