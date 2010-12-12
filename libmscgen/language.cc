
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
      if (YYID (N)) {					        \
        (Current).first_pos =     YYRHSLOC(Rhs,1).first_pos;	\
	(Current).last_pos  =     YYRHSLOC(Rhs,N).last_pos;	\
        (Current).hint_can_come = YYRHSLOC(Rhs,N).hint_can_come;\
        (Current).hint_type =     YYRHSLOC(Rhs,N).hint_type;    \
        (Current).hint_attr_name =YYRHSLOC(Rhs,N).hint_attr_name;\
      } else {							\
	  (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
          (Current).hint_can_come = false;                      \
      }							        \
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
#line 189 "language.cc"

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
#line 137 "language.yy"

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
#line 334 "language.cc"
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
#line 359 "language.cc"

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
#define YYFINAL  130
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   777

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  211
/* YYNRULES -- Number of states.  */
#define YYNSTATES  291

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
       0,     0,     3,     4,     7,     9,    11,    14,    17,    21,
      23,    26,    30,    34,    37,    42,    46,    48,    51,    53,
      56,    58,    61,    65,    69,    74,    78,    83,    86,    89,
      94,    97,   100,   103,   107,   111,   115,   118,   122,   125,
     129,   133,   138,   141,   145,   147,   151,   155,   159,   163,
     167,   169,   173,   176,   178,   181,   183,   185,   189,   192,
     194,   198,   200,   204,   208,   214,   221,   223,   225,   229,
     232,   235,   237,   239,   243,   246,   250,   254,   258,   262,
     264,   267,   269,   271,   274,   278,   282,   285,   290,   294,
     296,   299,   302,   306,   308,   311,   314,   317,   321,   323,
     326,   330,   333,   336,   338,   341,   345,   349,   353,   358,
     360,   362,   364,   369,   373,   377,   380,   385,   389,   393,
     396,   398,   400,   402,   404,   408,   411,   414,   418,   421,
     425,   428,   431,   435,   438,   442,   445,   448,   452,   455,
     457,   459,   461,   463,   465,   467,   469,   471,   473,   475,
     477,   479,   481,   483,   485,   487,   489,   491,   493,   495,
     497,   500,   504,   507,   509,   512,   516,   521,   525,   529,
     532,   534,   538,   542,   546,   550,   553,   555,   557,   559,
     561,   563,   565,   567,   569,   571,   573,   575,   577,   579,
     581,   583,   585,   587,   589,   591,   593,   595,   597,   599,
     601,   603,   605,   607,   609,   611,   613,   615,   617,   619,
     621,   623
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    -1,    49,    50,    -1,    48,    -1,    51,
      -1,    51,     1,    -1,    51,    12,    -1,    51,    12,    48,
      -1,    15,    -1,    15,     7,    -1,    15,     7,    98,    -1,
      99,    51,   100,    -1,    99,   100,    -1,    99,    51,     1,
     100,    -1,    99,    51,     1,    -1,    52,    -1,    51,    52,
      -1,    53,    -1,    43,    53,    -1,     9,    -1,    80,     9,
      -1,    80,    91,     9,    -1,    40,    80,     9,    -1,    40,
      80,    91,     9,    -1,    44,    79,     9,    -1,    44,    79,
      91,     9,    -1,    91,     9,    -1,    58,     9,    -1,    58,
       8,    56,     9,    -1,    54,     9,    -1,    71,     9,    -1,
      70,     9,    -1,    37,    62,     9,    -1,    38,    59,     9,
      -1,    39,    64,     9,    -1,    34,     9,    -1,    34,    92,
       9,    -1,    35,     9,    -1,    35,    92,     9,    -1,    42,
      98,     9,    -1,    42,    98,    92,     9,    -1,    36,     9,
      -1,    36,    92,     9,    -1,    55,    -1,    54,     8,    55,
      -1,    95,     7,    19,    -1,    95,     7,     5,    -1,    95,
       7,    98,    -1,    15,     7,    98,    -1,    57,    -1,    56,
       8,    57,    -1,    95,    91,    -1,    95,    -1,    95,    91,
      -1,    95,    -1,    60,    -1,    59,     8,    60,    -1,    61,
      92,    -1,    98,    -1,    61,     8,    98,    -1,    63,    -1,
      62,     8,    63,    -1,     3,     7,    98,    -1,     3,    65,
      66,     9,    12,    -1,     3,    65,    66,     9,     1,    12,
      -1,    11,    -1,    67,    -1,    66,     9,    67,    -1,    37,
      62,    -1,    38,    59,    -1,    68,    -1,    69,    -1,    68,
       8,    69,    -1,    68,     1,    -1,    95,     7,    19,    -1,
      95,     7,     5,    -1,    95,     7,    98,    -1,    15,     7,
      98,    -1,    50,    -1,    70,    50,    -1,    72,    -1,    75,
      -1,    71,    76,    -1,    71,    76,    91,    -1,    71,    76,
      50,    -1,    71,    50,    -1,    71,    76,    91,    50,    -1,
      71,    91,    50,    -1,    76,    -1,    76,    91,    -1,    76,
      50,    -1,    76,    91,    50,    -1,    76,    -1,    76,    91,
      -1,    41,    73,    -1,    74,    73,    -1,    74,    41,    73,
      -1,    74,    -1,    74,    50,    -1,    95,    33,    95,    -1,
      33,    95,    -1,    95,    33,    -1,    33,    -1,    45,    95,
      -1,    45,    95,     6,    -1,    45,    95,    10,    -1,    45,
      95,    33,    -1,    45,    95,     6,    95,    -1,    33,    -1,
      84,    -1,    86,    -1,    95,    78,    95,    77,    -1,    78,
      95,    77,    -1,    95,    78,    77,    -1,    78,    77,    -1,
      95,    85,    95,    77,    -1,    85,    95,    77,    -1,    95,
      85,    77,    -1,    85,    77,    -1,    32,    -1,    81,    -1,
      82,    -1,    83,    -1,    95,    84,    95,    -1,    84,    95,
      -1,    95,    84,    -1,    81,    87,    95,    -1,    81,    87,
      -1,    95,    85,    95,    -1,    85,    95,    -1,    95,    85,
      -1,    82,    88,    95,    -1,    82,    88,    -1,    95,    86,
      95,    -1,    86,    95,    -1,    95,    86,    -1,    83,    89,
      95,    -1,    83,    89,    -1,    20,    -1,    23,    -1,    26,
      -1,    29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,
      -1,    22,    -1,    25,    -1,    28,    -1,    31,    -1,    84,
      -1,     6,    -1,    85,    -1,     6,    -1,    86,    -1,     6,
      -1,    17,    -1,    16,    -1,    90,    -1,    90,    92,    -1,
      92,    90,    92,    -1,    92,    90,    -1,    92,    -1,    13,
      14,    -1,    13,    93,    14,    -1,    13,    93,     1,    14,
      -1,    13,     1,    14,    -1,    13,    93,     1,    -1,    13,
       1,    -1,    94,    -1,    93,     8,    94,    -1,    98,     7,
      98,    -1,    98,     7,     5,    -1,    98,     7,    19,    -1,
      98,     7,    -1,    98,    -1,     4,    -1,     3,    -1,    15,
      -1,    37,    -1,    38,    -1,    39,    -1,    36,    -1,    40,
      -1,    41,    -1,    44,    -1,    43,    -1,    34,    -1,    35,
      -1,    42,    -1,    45,    -1,    20,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    33,    -1,    95,    -1,    96,    -1,    97,    -1,    18,
      -1,    11,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   199,   199,   202,   213,   221,   222,   230,   239,   253,
     261,   273,   291,   301,   314,   326,   338,   347,   356,   364,
     378,   386,   394,   411,   423,   446,   456,   475,   492,   500,
     510,   523,   531,   539,   549,   559,   569,   579,   589,   599,
     609,   621,   633,   643,   654,   663,   678,   691,   704,   724,
     745,   751,   763,   781,   793,   803,   816,   817,   824,   843,
     859,   877,   878,   885,   899,   919,   942,   953,   954,   961,
     968,   975,   977,   978,   984,   991,  1003,  1015,  1027,  1041,
    1050,  1062,  1068,  1073,  1080,  1088,  1095,  1103,  1111,  1122,
    1129,  1137,  1144,  1153,  1159,  1167,  1177,  1184,  1196,  1197,
    1204,  1216,  1226,  1236,  1245,  1256,  1268,  1280,  1311,  1326,
    1326,  1326,  1328,  1341,  1352,  1363,  1372,  1385,  1396,  1407,
    1418,  1426,  1430,  1434,  1439,  1451,  1461,  1471,  1481,  1491,
    1503,  1513,  1523,  1533,  1542,  1554,  1564,  1574,  1584,  1593,
    1593,  1593,  1593,  1594,  1594,  1594,  1594,  1595,  1595,  1595,
    1595,  1597,  1597,  1598,  1598,  1599,  1599,  1602,  1609,  1618,
    1626,  1634,  1647,  1656,  1660,  1670,  1681,  1693,  1704,  1715,
    1730,  1737,  1749,  1763,  1777,  1791,  1806,  1818,  1818,  1820,
    1820,  1821,  1821,  1822,  1822,  1822,  1823,  1823,  1824,  1824,
    1825,  1825,  1827,  1828,  1829,  1830,  1831,  1832,  1833,  1834,
    1835,  1836,  1837,  1838,  1839,  1846,  1856,  1856,  1856,  1856,
    1858,  1867
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
  "msckey", "braced_arclist", "arclist", "arc_with_parallel", "arc",
  "optlist", "opt", "entitylist", "entity", "first_entity", "styledeflist",
  "styledef", "tok_stringlist", "colordeflist", "colordef", "designdef",
  "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "emphasis_list",
  "first_emphasis", "pipe_def", "pipe_def_list", "pipe_emphasis",
  "emphrel", "vertxpos", "empharcrel_straight", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
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
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    47,    48,    48,    48,    48,    49,
      49,    49,    50,    50,    50,    50,    51,    51,    52,    52,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    58,    58,    59,    59,    60,    61,
      61,    62,    62,    63,    64,    64,    65,    66,    66,    67,
      67,    67,    68,    68,    68,    69,    69,    69,    69,    70,
      70,    71,    71,    71,    71,    71,    71,    71,    71,    72,
      72,    72,    72,    73,    73,    74,    74,    74,    75,    75,
      76,    76,    76,    76,    77,    77,    77,    77,    77,    78,
      78,    78,    79,    79,    79,    79,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    81,    81,    81,    82,
      82,    82,    82,    82,    83,    83,    83,    83,    83,    84,
      84,    84,    84,    85,    85,    85,    85,    86,    86,    86,
      86,    87,    87,    88,    88,    89,    89,    90,    90,    91,
      91,    91,    91,    91,    92,    92,    92,    92,    92,    92,
      93,    93,    94,    94,    94,    94,    94,    95,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    98,    98,
      99,   100
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     2,     2,     3,     1,
       2,     3,     3,     2,     4,     3,     1,     2,     1,     2,
       1,     2,     3,     3,     4,     3,     4,     2,     2,     4,
       2,     2,     2,     3,     3,     3,     2,     3,     2,     3,
       3,     4,     2,     3,     1,     3,     3,     3,     3,     3,
       1,     3,     2,     1,     2,     1,     1,     3,     2,     1,
       3,     1,     3,     3,     5,     6,     1,     1,     3,     2,
       2,     1,     1,     3,     2,     3,     3,     3,     3,     1,
       2,     1,     1,     2,     3,     3,     2,     4,     3,     1,
       2,     2,     3,     1,     2,     2,     2,     3,     1,     2,
       3,     2,     2,     1,     2,     3,     3,     3,     4,     1,
       1,     1,     4,     3,     3,     2,     4,     3,     3,     2,
       1,     1,     1,     1,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     3,     2,     1,     2,     3,     4,     3,     3,     2,
       1,     3,     3,     3,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,   178,   177,    20,   210,     0,     9,   158,   157,   139,
     143,   147,   140,   144,   148,   141,   145,   149,   142,   146,
     150,   120,   103,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     0,    79,     0,    16,
      18,     0,    44,     0,     0,     0,    81,    98,    82,    89,
       0,   121,   122,   123,     0,     0,     0,   159,     0,   163,
      55,     0,   169,   164,   179,   209,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     188,   189,   183,   180,   181,   182,   184,   185,   190,   187,
     186,   191,     0,   170,   206,   207,   208,   176,    10,   101,
      36,     0,    38,     0,    42,     0,     0,     0,    61,     0,
      56,     0,    59,     0,     0,     0,     0,    95,    93,     0,
       0,     0,    19,   109,     0,     0,   110,     0,   111,     0,
       1,     3,     6,     7,    17,     0,    30,     0,    28,    32,
      80,    31,    86,    83,     0,     0,    99,    96,    91,    90,
      21,     0,   152,   151,   128,   154,   153,   133,   156,   155,
     138,   125,   130,   135,   160,    27,   162,     0,   102,   126,
     131,   136,    54,   211,     0,    13,   167,   168,     0,   165,
     175,    49,    37,    39,    43,     0,     0,    33,     0,    34,
       0,    58,    66,     0,    35,    23,     0,    94,    40,     0,
       0,     0,   115,     0,    25,     0,   119,     0,     0,     0,
       8,    45,     0,     0,    50,    53,    85,    84,    88,    97,
      92,    22,   127,   132,   137,   161,    47,    46,    48,   100,
     124,   129,   134,    15,    12,   166,   171,   173,   174,   172,
      63,    62,    57,    60,     0,     0,     0,     0,    67,     0,
      72,     0,    24,    41,    49,   104,   113,    26,   117,   114,
       0,   118,     0,     0,    29,    52,    87,    14,     0,    69,
      70,     0,    74,     0,     0,   105,   106,   107,   112,   116,
      51,    78,     0,    64,    68,    73,    76,    75,    77,   108,
      65
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
     213,   214,    43,   109,   110,   111,   107,   108,   114,   193,
     247,   248,   249,   250,    44,    45,    46,   117,    47,    48,
      49,   202,   124,   125,    50,    51,    52,    53,    54,    55,
      56,   154,   157,   160,    57,    58,    59,    92,    93,    94,
      95,    96,   112,    61,   175
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -154
static const yytype_int16 yypact[] =
{
     564,  -154,  -154,  -154,  -154,   263,    24,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,    73,    40,   113,   142,    10,   479,    33,   704,
      83,   479,   648,   690,    50,  -154,    53,  -154,   210,  -154,
    -154,   123,  -154,   156,    84,   172,  -154,    37,  -154,   244,
     104,   133,   122,   119,    73,    73,    73,    89,    96,   161,
     730,   522,    93,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,    80,  -154,  -154,  -154,  -154,   103,   479,  -154,
    -154,   125,  -154,   139,  -154,   149,   180,   171,  -154,   207,
    -154,    31,  -154,   196,   183,   181,   675,  -154,   150,   179,
     191,   213,  -154,  -154,    23,   186,  -154,    23,  -154,   744,
    -154,  -154,  -154,   606,  -154,    86,  -154,    73,  -154,  -154,
    -154,  -154,  -154,   244,    53,    83,  -154,  -154,  -154,    53,
    -154,   215,  -154,  -154,    73,  -154,  -154,    73,  -154,  -154,
      73,  -154,  -154,  -154,  -154,  -154,    89,   350,    73,    73,
      73,    73,  -154,  -154,   308,  -154,  -154,   214,   479,  -154,
     393,   218,  -154,  -154,  -154,   479,    10,  -154,   479,  -154,
     479,  -154,  -154,    54,  -154,  -154,   247,  -154,  -154,   249,
     479,    73,  -154,   217,  -154,   250,  -154,   217,    23,    23,
    -154,  -154,   258,   209,  -154,   150,  -154,    53,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,   256,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,   262,    10,   479,   261,  -154,    58,
    -154,   265,  -154,  -154,  -154,    18,  -154,  -154,  -154,  -154,
     217,  -154,   217,    73,  -154,  -154,  -154,  -154,   479,   268,
     271,    59,  -154,   115,   436,    73,  -154,  -154,  -154,  -154,
    -154,  -154,   270,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,   177,  -154,   -35,   252,   -34,   282,  -154,   187,
    -154,    52,  -154,    70,   130,  -154,    81,   141,  -154,  -154,
    -154,    85,  -154,    87,  -154,  -154,  -154,   -42,  -154,  -154,
     -22,  -124,   228,  -154,   329,  -154,  -154,  -154,   -17,    13,
     -18,  -154,  -154,  -154,   300,   -43,    -5,  -154,   184,     0,
    -154,  -154,     6,  -154,  -153
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -72
static const yytype_int16 yytable[] =
{
      60,   131,   144,   206,   134,   147,   149,   151,   118,   140,
     142,    97,   146,   106,   148,   128,   126,   172,   101,   103,
     105,   234,    99,   143,   275,   118,     1,     2,   276,   116,
     119,    98,    60,   129,   153,   159,   113,   120,    60,   190,
       1,     2,   171,   169,     5,   119,   127,   119,     4,   100,
     130,   277,   164,     5,   161,   162,   163,     1,     2,   272,
     282,    60,     1,     2,     4,   156,   273,   -71,   201,   244,
      22,   283,   196,   170,   244,   197,     1,     2,   145,   256,
     267,   177,   205,   258,   259,   261,     1,     2,   178,     1,
       2,   245,   246,   139,   179,     4,   245,   246,   171,   169,
     217,   121,     5,   219,   181,   165,   191,   176,   216,   218,
     180,   128,   126,   150,   220,   199,    22,     5,     1,     2,
       7,     8,   102,   118,   203,   158,     5,   207,   155,   170,
     244,   135,   136,    60,   182,   212,   278,   215,   279,   152,
     134,    11,   209,    10,    14,   119,    13,    17,   183,    16,
      20,   104,    19,     9,   222,     5,    12,   223,   184,    15,
     224,   225,    18,     5,   137,   138,     7,     8,   229,   230,
     231,   232,   265,   228,    60,     1,     2,     7,     8,   186,
     187,   141,   266,     4,    97,     5,   239,   185,     7,     8,
     195,   240,   194,   251,     5,   204,   243,     7,     8,     5,
     198,   255,     7,     8,     5,    22,   254,   192,   260,   262,
      -5,   132,   168,     1,     2,   188,   189,   263,   264,     3,
     200,     4,   133,     5,   221,   121,     7,     8,   235,   -11,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,     4,   252,     5,   253,   257,
       7,     8,   201,   215,    62,   167,     1,     2,   173,   268,
     271,   251,   274,   251,   281,   289,   186,    63,    64,   188,
     288,    65,   290,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   233,
     210,     1,     2,   174,   122,   280,   270,     3,   242,     4,
     173,     5,   211,   121,     7,     8,   269,   241,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     1,     2,   226,   284,   208,   115,   166,
     285,     0,   236,     0,     0,    64,     0,     0,    65,   227,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,     1,     2,   237,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,     0,
       0,    65,   238,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,     1,
       2,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,     0,     0,    65,   287,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,     1,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,     0,     0,    65,     0,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,     1,     2,     0,     0,     0,
       0,     3,     0,     4,   173,     5,     0,   121,     7,     8,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     1,     2,     0,
       0,     0,     0,     3,     0,     4,     0,     5,     0,     6,
       7,     8,     0,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,     1,
       2,     0,     0,     0,     0,     3,     0,     4,     0,     5,
       0,   121,     7,     8,     0,     0,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,     1,     2,     0,     0,     0,     0,     3,     0,     4,
       0,     5,     0,   121,     7,     8,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,     0,    33,     1,     2,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     1,     2,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,     0,   123,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   167,     0,     0,
       0,     0,     0,     5,     0,     0,     7,     8,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,     0,   168,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     0,   123
};

static const yytype_int16 yycheck[] =
{
       0,    36,    45,   127,    38,    47,    49,    50,    30,    44,
      45,     5,    47,     3,    49,    33,    33,    60,    23,    24,
      25,   174,    22,    45,     6,    47,     3,     4,    10,    29,
      30,     7,    32,    33,    51,    53,     3,    31,    38,     8,
       3,     4,    60,    60,    13,    45,    33,    47,    11,     9,
       0,    33,    57,    13,    54,    55,    56,     3,     4,     1,
       1,    61,     3,     4,    11,    52,     8,     9,    45,    15,
      33,    12,   115,    60,    15,   118,     3,     4,    41,   203,
     233,     1,   125,   207,   208,   209,     3,     4,     8,     3,
       4,    37,    38,     9,    14,    11,    37,    38,   116,   116,
     143,    15,    13,   145,    98,     9,   111,    14,   143,   144,
       7,   129,   129,     9,   149,   120,    33,    13,     3,     4,
      16,    17,     9,   145,   124,     6,    13,   127,     6,   116,
      15,     8,     9,   133,     9,   135,   260,   137,   262,     6,
     174,    22,   129,    21,    25,   145,    24,    28,     9,    27,
      31,     9,    30,    20,   154,    13,    23,   157,     9,    26,
     160,   166,    29,    13,     8,     9,    16,    17,   168,   169,
     170,   171,   215,   167,   174,     3,     4,    16,    17,     8,
       9,     9,   217,    11,   178,    13,   180,     7,    16,    17,
       9,   185,     9,   193,    13,     9,   190,    16,    17,    13,
       9,   201,    16,    17,    13,    33,   200,    11,   208,   209,
       0,     1,    33,     3,     4,     8,     9,     8,     9,     9,
       7,    11,    12,    13,     9,    15,    16,    17,    14,    11,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    11,     9,    13,     9,     9,
      16,    17,    45,   263,     1,     7,     3,     4,    12,     7,
       9,   271,     7,   273,   268,   275,     8,    14,    15,     8,
     274,    18,    12,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     1,
     133,     3,     4,    61,    32,   263,   246,     9,   188,    11,
      12,    13,   135,    15,    16,    17,   245,   186,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,     3,     4,     5,   271,   129,    29,    59,
     273,    -1,   178,    -1,    -1,    15,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,     3,     4,    -1,
      -1,    -1,    -1,     9,    -1,    11,    -1,    13,    -1,    15,
      16,    17,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     3,
       4,    -1,    -1,    -1,    -1,     9,    -1,    11,    -1,    13,
      -1,    15,    16,    17,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,    11,
      -1,    13,    -1,    15,    16,    17,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,     3,     4,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     3,     4,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,     7,    -1,    -1,
      -1,    -1,    -1,    13,    -1,    -1,    16,    17,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     9,    11,    13,    15,    16,    17,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    58,    70,    71,    72,    74,    75,    76,
      80,    81,    82,    83,    84,    85,    86,    90,    91,    92,
      95,    99,     1,    14,    15,    18,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    93,    94,    95,    96,    97,    98,     7,    95,
       9,    92,     9,    92,     9,    92,     3,    62,    63,    59,
      60,    61,    98,     3,    64,    80,    95,    73,    76,    95,
      98,    15,    53,    33,    78,    79,    84,    85,    86,    95,
       0,    50,     1,    12,    52,     8,     9,     8,     9,     9,
      50,     9,    50,    76,    91,    41,    50,    73,    50,    91,
       9,    91,     6,    84,    87,     6,    85,    88,     6,    86,
      89,    95,    95,    95,    92,     9,    90,     7,    33,    84,
      85,    86,    91,    12,    51,   100,    14,     1,     8,    14,
       7,    98,     9,     9,     9,     7,     8,     9,     8,     9,
       8,    92,    11,    65,     9,     9,    91,    91,     9,    92,
       7,    45,    77,    95,     9,    91,    77,    95,    78,    85,
      48,    55,    95,    56,    57,    95,    50,    91,    50,    73,
      50,     9,    95,    95,    95,    92,     5,    19,    98,    95,
      95,    95,    95,     1,   100,    14,    94,     5,    19,    98,
      98,    63,    60,    98,    15,    37,    38,    66,    67,    68,
      69,    95,     9,     9,    98,    95,    77,     9,    77,    77,
      95,    77,    95,     8,     9,    91,    50,   100,     7,    62,
      59,     9,     1,     8,     7,     6,    10,    33,    77,    77,
      57,    98,     1,    12,    67,    69,     5,    19,    98,    95,
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
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1693 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1702 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1000 of yacc.c  */
#line 194 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1711 "language.cc"
	break;
      case 15: /* "TOK_MSC" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1720 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1729 "language.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1738 "language.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */

/* Line 1000 of yacc.c  */
#line 190 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1747 "language.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */

/* Line 1000 of yacc.c  */
#line 194 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1756 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1765 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1774 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1000 of yacc.c  */
#line 193 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1783 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1792 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1801 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1000 of yacc.c  */
#line 192 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1810 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1819 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1828 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */

/* Line 1000 of yacc.c  */
#line 194 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1837 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */

/* Line 1000 of yacc.c  */
#line 191 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 1846 "language.cc"
	break;
      case 48: /* "top_level_arclist" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1855 "language.cc"
	break;
      case 50: /* "braced_arclist" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1864 "language.cc"
	break;
      case 51: /* "arclist" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1873 "language.cc"
	break;
      case 52: /* "arc_with_parallel" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1882 "language.cc"
	break;
      case 53: /* "arc" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1891 "language.cc"
	break;
      case 54: /* "optlist" */

/* Line 1000 of yacc.c  */
#line 186 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1000 of yacc.c  */
#line 1900 "language.cc"
	break;
      case 55: /* "opt" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 1909 "language.cc"
	break;
      case 56: /* "entitylist" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1000 of yacc.c  */
#line 1918 "language.cc"
	break;
      case 57: /* "entity" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 1927 "language.cc"
	break;
      case 58: /* "first_entity" */

/* Line 1000 of yacc.c  */
#line 187 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};

/* Line 1000 of yacc.c  */
#line 1936 "language.cc"
	break;
      case 61: /* "tok_stringlist" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1000 of yacc.c  */
#line 1945 "language.cc"
	break;
      case 70: /* "parallel" */

/* Line 1000 of yacc.c  */
#line 185 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1000 of yacc.c  */
#line 1954 "language.cc"
	break;
      case 71: /* "emphasis_list" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 1963 "language.cc"
	break;
      case 72: /* "first_emphasis" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 1972 "language.cc"
	break;
      case 73: /* "pipe_def" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 1981 "language.cc"
	break;
      case 74: /* "pipe_def_list" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 1990 "language.cc"
	break;
      case 75: /* "pipe_emphasis" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 1999 "language.cc"
	break;
      case 76: /* "emphrel" */

/* Line 1000 of yacc.c  */
#line 184 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};

/* Line 1000 of yacc.c  */
#line 2008 "language.cc"
	break;
      case 77: /* "vertxpos" */

/* Line 1000 of yacc.c  */
#line 181 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1000 of yacc.c  */
#line 2017 "language.cc"
	break;
      case 79: /* "vertrel" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2026 "language.cc"
	break;
      case 80: /* "arcrel" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2035 "language.cc"
	break;
      case 81: /* "arcrel_to" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2044 "language.cc"
	break;
      case 82: /* "arcrel_from" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2053 "language.cc"
	break;
      case 83: /* "arcrel_bidir" */

/* Line 1000 of yacc.c  */
#line 183 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1000 of yacc.c  */
#line 2062 "language.cc"
	break;
      case 91: /* "full_arcattrlist_with_label" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2071 "language.cc"
	break;
      case 92: /* "full_arcattrlist" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2080 "language.cc"
	break;
      case 93: /* "arcattrlist" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1000 of yacc.c  */
#line 2089 "language.cc"
	break;
      case 94: /* "arcattr" */

/* Line 1000 of yacc.c  */
#line 188 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1000 of yacc.c  */
#line 2098 "language.cc"
	break;
      case 95: /* "entity_string" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2107 "language.cc"
	break;
      case 96: /* "reserved_word_string" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2116 "language.cc"
	break;
      case 97: /* "symbol_string" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2125 "language.cc"
	break;
      case 98: /* "string" */

/* Line 1000 of yacc.c  */
#line 189 "language.yy"
	{free((yyvaluep->str));};

/* Line 1000 of yacc.c  */
#line 2134 "language.cc"
	break;
      case 100: /* "scope_close" */

/* Line 1000 of yacc.c  */
#line 182 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1000 of yacc.c  */
#line 2143 "language.cc"
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
#line 199 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 203 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintCanComeBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]))) {
        csh.AddDesignsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
        msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 214 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 223 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #else
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
  #endif
}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 231 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #else
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 240 "language.yy"
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

  case 9:

/* Line 1455 of yacc.c  */
#line 254 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 262 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        ((yyloc)).hint_can_come = true;
        ((yyloc)).hint_type = HINT_DESIGNS;
  #else
        msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end, "Missing design name.");
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 274 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_DESIGNS)) {
            csh.AddDesignsToHints();
            csh.hintStatus = HINT_READY;
        }
        csh.SetDesignTo((yyvsp[(1) - (3)].str));
  #else
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 292 "language.yy"
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

  case 13:

/* Line 1455 of yacc.c  */
#line 302 "language.yy"
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

  case 14:

/* Line 1455 of yacc.c  */
#line 315 "language.yy"
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

  case 15:

/* Line 1455 of yacc.c  */
#line 327 "language.yy"
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

  case 16:

/* Line 1455 of yacc.c  */
#line 339 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 348 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 357 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((yyvsp[(1) - (1)].arcbase)) ((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 365 "language.yy"
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

  case 20:

/* Line 1455 of yacc.c  */
#line 379 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase)=NULL;
  #endif
}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 387 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
  #endif
}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 395 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
        csh.CheckHintCanComeBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
        if (csh.CursorIn((yyloc)) && csh.hintStatus == HINT_LOCATED) {
            switch (csh.hintType) {
            case HINT_ATTR_NAME:  ArcArrow::AttributeNames(csh); break;
            case HINT_ATTR_VALUE: ArcArrow::AttributeValues(csh.hintAttrName, csh); break;
            default: _ASSERT(0);
            }
            csh.hintStatus = HINT_READY;
        }
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (3)].arcbase))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
  #endif
}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 412 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        (yyval.arcbase) = msc.CreateArcBigArrow((yyvsp[(2) - (3)].arcbase));
        delete (yyvsp[(2) - (3)].arcbase);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 424 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_SEMICOLON);
        csh.CheckHintCanComeBetween((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
        if (csh.CursorIn((yyloc)) && csh.hintStatus == HINT_LOCATED) {
            switch (csh.hintType) {
            case HINT_ATTR_NAME:  ArcBigArrow::AttributeNames(csh); break;
            case HINT_ATTR_VALUE: ArcBigArrow::AttributeValues(csh.hintAttrName, csh); break;
            default: _ASSERT(0);
            }
            csh.hintStatus = HINT_READY;
        }
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[(2) - (4)].arcbase));
        if (arrow) arrow->AddAttributeList((yyvsp[(3) - (4)].attriblist));
        (yyval.arcbase) = arrow;
        delete (yyvsp[(2) - (4)].arcbase);
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 447 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = (yyvsp[(2) - (3)].arcbase);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 457 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_SEMICOLON);
        csh.CheckHintCanComeBetween((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
        if (csh.CursorIn((yyloc)) && csh.hintStatus == HINT_LOCATED) {
            switch (csh.hintType) {
            case HINT_ATTR_NAME:  ArcVerticalArrow::AttributeNames(csh); break;
            case HINT_ATTR_VALUE: ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh); break;
            default: _ASSERT(0);
            }
            csh.hintStatus = HINT_READY;
        }
  #else
        (yyval.arcbase) = ((yyvsp[(2) - (4)].arcbase))->AddAttributeList((yyvsp[(3) - (4)].attriblist));
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 476 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
        csh.CheckHintCanComeBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
        if (csh.CursorIn((yyloc)) && csh.hintStatus == HINT_LOCATED) {
            switch (csh.hintType) {
            case HINT_ATTR_NAME:  ArcDivider::AttributeNames(csh); break;
            case HINT_ATTR_VALUE: ArcDivider::AttributeValues(csh.hintAttrName, csh); break;
            default: _ASSERT(0);
            }
            csh.hintStatus = HINT_READY;
        }
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (2)].attriblist));
  #endif
}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 493 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
    (yyval.arcbase) = new CommandEntity((new EntityDefList)->Append((yyvsp[(1) - (2)].entity)), &msc);
  #endif
}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 501 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_COMMA);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_SEMICOLON);
    csh.CheckHintCanComeBetween((yylsp[(1) - (4)]),(yylsp[(2) - (4)]));
  #else
    (yyval.arcbase) = new CommandEntity(((yyvsp[(3) - (4)].entitylist))->Prepend((yyvsp[(1) - (4)].entity)), &msc);
  #endif
}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 511 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (2)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (2)].arclist));
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 524 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
    (yyval.arcbase) = (yyvsp[(1) - (2)].arcemph); //to remove warning for downcast
  #endif
}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 532 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
    (yyval.arcbase) = (yyvsp[(1) - (2)].arcparallel);
  #endif
}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 540 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 550 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 560 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 570 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = new CommandEntity(NULL, &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 580 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 590 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 600 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 610 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = new CommandMark((yyvsp[(2) - (3)].str), MSC_POS((yyloc)), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 622 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_MARKERNAME);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (4)].str), MSC_POS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (4)].attriblist));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 634 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = new CommandNewpage(&msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 644 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
  #else
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 655 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
  #endif
}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 664 "language.yy"
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

  case 46:

/* Line 1455 of yacc.c  */
#line 679 "language.yy"
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

  case 47:

/* Line 1455 of yacc.c  */
#line 692 "language.yy"
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

  case 48:

/* Line 1455 of yacc.c  */
#line 705 "language.yy"
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

  case 49:

/* Line 1455 of yacc.c  */
#line 725 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_DESIGNS)) {
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

  case 50:

/* Line 1455 of yacc.c  */
#line 746 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
  #endif
}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 752 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ENTITY);
        csh.CheckHintCanComeBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
  #else
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
  #endif
}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 764 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        if (csh.CursorIn((yyloc)) && csh.hintStatus == HINT_LOCATED) {
            switch (csh.hintType) {
            case HINT_ATTR_NAME:  EntityDef::AttributeNames(csh); break;
            case HINT_ATTR_VALUE: EntityDef::AttributeValues(csh.hintAttrName, csh); break;
            default: _ASSERT(0);
            }
            csh.hintStatus = HINT_READY;
        }
  #else
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
        ((yyval.entity))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 782 "language.yy"
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

  case 54:

/* Line 1455 of yacc.c  */
#line 794 "language.yy"
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

  case 55:

/* Line 1455 of yacc.c  */
#line 804 "language.yy"
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

  case 57:

/* Line 1455 of yacc.c  */
#line 818 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 825 "language.yy"
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

  case 59:

/* Line 1455 of yacc.c  */
#line 844 "language.yy"
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

  case 60:

/* Line 1455 of yacc.c  */
#line 860 "language.yy"
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

  case 62:

/* Line 1455 of yacc.c  */
#line 879 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 886 "language.yy"
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

  case 64:

/* Line 1455 of yacc.c  */
#line 900 "language.yy"
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

  case 65:

/* Line 1455 of yacc.c  */
#line 920 "language.yy"
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

  case 66:

/* Line 1455 of yacc.c  */
#line 943 "language.yy"
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

  case 68:

/* Line 1455 of yacc.c  */
#line 955 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 962 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 969 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 979 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 985 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
  #endif
}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 992 "language.yy"
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

  case 76:

/* Line 1455 of yacc.c  */
#line 1004 "language.yy"
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

  case 77:

/* Line 1455 of yacc.c  */
#line 1016 "language.yy"
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

  case 78:

/* Line 1455 of yacc.c  */
#line 1028 "language.yy"
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

  case 79:

/* Line 1455 of yacc.c  */
#line 1042 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 1051 "language.yy"
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

  case 81:

/* Line 1455 of yacc.c  */
#line 1063 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1069 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1074 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
  #endif
}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1081 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
  #endif
}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1089 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
  #endif
}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1096 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
  #endif
}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1104 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (4)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(2) - (4)]), (yylsp[(3) - (4)])));
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
  #endif
}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1112 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (3)])), NULL, MSC_POS((yylsp[(1) - (3)])), &msc);
    temp->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
  #endif
}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1123 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
  #endif
}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1130 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
  #endif
}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1138 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1145 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (3)].arcemph))->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
  #endif
}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1154 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
  #endif
}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1160 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
  #endif
}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1168 "language.yy"
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

  case 96:

/* Line 1455 of yacc.c  */
#line 1178 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcemph))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
  #endif
}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1185 "language.yy"
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

  case 99:

/* Line 1455 of yacc.c  */
#line 1198 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1205 "language.yy"
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

  case 101:

/* Line 1455 of yacc.c  */
#line 1217 "language.yy"
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

  case 102:

/* Line 1455 of yacc.c  */
#line 1227 "language.yy"
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

  case 103:

/* Line 1455 of yacc.c  */
#line 1237 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1246 "language.yy"
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

  case 105:

/* Line 1455 of yacc.c  */
#line 1257 "language.yy"
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

  case 106:

/* Line 1455 of yacc.c  */
#line 1269 "language.yy"
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

  case 107:

/* Line 1455 of yacc.c  */
#line 1281 "language.yy"
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

  case 108:

/* Line 1455 of yacc.c  */
#line 1312 "language.yy"
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

  case 112:

/* Line 1455 of yacc.c  */
#line 1329 "language.yy"
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

  case 113:

/* Line 1455 of yacc.c  */
#line 1342 "language.yy"
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

  case 114:

/* Line 1455 of yacc.c  */
#line 1353 "language.yy"
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

  case 115:

/* Line 1455 of yacc.c  */
#line 1364 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 1373 "language.yy"
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

  case 117:

/* Line 1455 of yacc.c  */
#line 1386 "language.yy"
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

  case 118:

/* Line 1455 of yacc.c  */
#line 1397 "language.yy"
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

  case 119:

/* Line 1455 of yacc.c  */
#line 1408 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 1419 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 1427 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 1431 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 1435 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 1440 "language.yy"
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

  case 125:

/* Line 1455 of yacc.c  */
#line 1452 "language.yy"
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

  case 126:

/* Line 1455 of yacc.c  */
#line 1462 "language.yy"
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

  case 127:

/* Line 1455 of yacc.c  */
#line 1472 "language.yy"
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

  case 128:

/* Line 1455 of yacc.c  */
#line 1482 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 1492 "language.yy"
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

  case 130:

/* Line 1455 of yacc.c  */
#line 1504 "language.yy"
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

  case 131:

/* Line 1455 of yacc.c  */
#line 1514 "language.yy"
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

  case 132:

/* Line 1455 of yacc.c  */
#line 1524 "language.yy"
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

  case 133:

/* Line 1455 of yacc.c  */
#line 1534 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), false, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 1543 "language.yy"
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

  case 135:

/* Line 1455 of yacc.c  */
#line 1555 "language.yy"
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

  case 136:

/* Line 1455 of yacc.c  */
#line 1565 "language.yy"
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

  case 137:

/* Line 1455 of yacc.c  */
#line 1575 "language.yy"
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

  case 138:

/* Line 1455 of yacc.c  */
#line 1585 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, MSC_POS((yylsp[(2) - (2)])), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1603 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 1610 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 1619 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1627 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1635 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.CheckHintCanComeBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
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

  case 162:

/* Line 1455 of yacc.c  */
#line 1648 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.CheckHintCanComeBetween((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1661 "language.yy"
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

  case 165:

/* Line 1455 of yacc.c  */
#line 1671 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintCanComeBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
  #endif
}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1682 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (4)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (4)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (4)]), (yylsp[(2) - (4)]), HINT_ATTR_NAME);
        csh.CheckHintCanComeBetween((yylsp[(2) - (4)]), (yylsp[(3) - (4)]));
  #else
    (yyval.attriblist) = (yyvsp[(2) - (4)].attriblist);
  #endif
}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1694 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1705 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(3) - (3)]), COLOR_ERROR);
        csh.CheckHintBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintCanComeBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
  #endif
}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1716 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_ERROR);
        if (csh.CursorIn(((yylsp[(1) - (2)])).last_pos+1,((yylsp[(2) - (2)])).last_pos)) {
            ((yyloc)).hint_can_come = true;
            ((yyloc)).hint_type = HINT_ATTR_NAME;
        }
  #else
    (yyval.attriblist) = new AttributeList;
  #endif
}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1731 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1738 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintCanComeBetween((yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 172:

/* Line 1455 of yacc.c  */
#line 1750 "language.yy"
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

  case 173:

/* Line 1455 of yacc.c  */
#line 1764 "language.yy"
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

  case 174:

/* Line 1455 of yacc.c  */
#line 1778 "language.yy"
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

  case 175:

/* Line 1455 of yacc.c  */
#line 1792 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
        if (!csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_NAME)) {
            ((yyloc)).hint_can_come = true;
            ((yyloc)).hint_type = HINT_ATTR_VALUE;
            ((yyloc)).hint_attr_name = (yyvsp[(1) - (2)].str);
        }
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, MSC_POS((yyloc)), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1807 "language.yy"
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

  case 192:

/* Line 1455 of yacc.c  */
#line 1827 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 1828 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 1829 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 1830 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 1831 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 1832 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 1833 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 1834 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 1835 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 1836 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 1837 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 1838 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 1840 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 1847 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 1859 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext();
  #else
    msc.PushContext();
  #endif
}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 1868 "language.yy"
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
#line 4882 "language.cc"
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
#line 1878 "language.yy"



/* END OF FILE */

