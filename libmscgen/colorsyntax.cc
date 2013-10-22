/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton implementation for Bison GLR parsers in C
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1


/* Substitute the variable and function names.  */
#define yyparse csh_parse
#define yylex   csh_lex
#define yyerror csh_error
#define yylval  csh_lval
#define yychar  csh_char
#define yydebug csh_debug
#define yynerrs csh_nerrs
#define yylloc  csh_lloc

/* Copy the first part of user declarations.  */

/* Line 172 of glr.c  */
#line 15 "language.yy"

/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
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


/* Line 172 of glr.c  */
#line 124 "colorsyntax.cc"



#include "colorsyntax.h"

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

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template,
   here we set the default value of $$ to a zeroed-out value.
   Since the default value is undefined, this behavior is
   technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */

/* Line 243 of glr.c  */
#line 183 "language.yy"

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
#define YYLLOC_DEFAULT(Current, Rhs, N)                         \
    do                                                          \
        if (YYID (N)) {                                         \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;    \
	        (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos;	\
        } else {                                                \
	        (Current).first_pos = (Current).last_pos =      \
	            YYRHSLOC (Rhs, 0).last_pos;                 \
        }                                                       \
    while (YYID (0))

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



/* Line 243 of glr.c  */
#line 222 "colorsyntax.cc"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(env) setjmp (env)
# define YYLONGJMP(env, val) longjmp (env, val)
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5) \
      || (defined __STRICT_ANSI__ && __STRICT_ANSI__))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#define YYOPTIONAL_LOC(Name) Name

#ifndef YYASSERT
# define YYASSERT(condition) ((void) ((condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  195
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1497

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  80
/* YYNRULES -- Number of rules.  */
#define YYNRULES  368
/* YYNRULES -- Number of states.  */
#define YYNSTATES  423
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 9
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   318

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    12,    15,    17,    20,
      22,    25,    27,    30,    34,    37,    41,    45,    48,    53,
      57,    60,    62,    65,    67,    69,    72,    75,    79,    84,
      86,    90,    92,    95,    97,   100,   102,   105,   109,   111,
     114,   118,   120,   122,   124,   127,   130,   134,   138,   143,
     145,   147,   149,   151,   154,   157,   160,   162,   165,   167,
     170,   173,   176,   180,   182,   185,   187,   189,   191,   195,
     198,   200,   203,   205,   207,   209,   211,   214,   216,   218,
     220,   222,   225,   228,   231,   233,   237,   239,   242,   245,
     247,   251,   253,   255,   257,   259,   261,   265,   268,   272,
     276,   280,   284,   288,   291,   295,   298,   302,   305,   307,
     311,   314,   318,   321,   323,   327,   330,   333,   335,   339,
     342,   344,   348,   351,   353,   357,   359,   363,   367,   373,
     380,   382,   384,   388,   391,   394,   396,   398,   402,   405,
     409,   413,   417,   421,   425,   427,   430,   432,   435,   439,
     443,   446,   451,   455,   457,   460,   463,   467,   470,   473,
     475,   479,   481,   484,   488,   490,   493,   497,   500,   503,
     505,   508,   512,   516,   521,   525,   530,   534,   539,   545,
     547,   549,   551,   553,   555,   559,   562,   565,   567,   571,
     574,   577,   579,   581,   585,   590,   594,   599,   602,   604,
     606,   608,   610,   612,   616,   619,   622,   626,   629,   633,
     636,   639,   643,   646,   650,   653,   656,   660,   663,   665,
     667,   669,   671,   673,   675,   677,   679,   681,   683,   685,
     687,   689,   691,   693,   695,   697,   699,   701,   704,   707,
     711,   713,   716,   719,   723,   725,   728,   731,   735,   737,
     740,   743,   747,   749,   752,   755,   759,   761,   764,   767,
     771,   773,   775,   778,   780,   782,   784,   789,   795,   799,
     804,   808,   812,   815,   817,   819,   822,   827,   830,   833,
     837,   839,   841,   843,   846,   848,   850,   852,   854,   857,
     861,   864,   866,   869,   873,   878,   882,   885,   889,   891,
     894,   897,   899,   903,   906,   910,   914,   919,   926,   932,
     940,   950,   954,   958,   961,   965,   967,   969,   971,   973,
     975,   977,   979,   981,   983,   985,   987,   989,   991,   993,
     995,   997,   999,  1001,  1003,  1005,  1007,  1009,  1011,  1013,
    1015,  1017,  1019,  1021,  1023,  1025,  1027,  1029,  1031,  1033,
    1035,  1037,  1039,  1041,  1043,  1045,  1047,  1049,  1051,  1053,
    1055,  1057,  1059,  1061,  1063,  1065,  1067,  1069,  1072
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
      65,     0,    -1,    66,    -1,    66,    53,    -1,    -1,    17,
      69,    -1,    68,    69,    -1,    17,    -1,    17,     1,    -1,
      67,    -1,    67,     1,    -1,    70,    -1,    70,    13,    -1,
      70,    13,    67,    -1,    17,     7,    -1,    17,     7,   140,
      -1,   142,    70,   143,    -1,   142,   143,    -1,   142,    70,
       1,   143,    -1,   142,    70,     1,    -1,   142,    70,    -1,
      71,    -1,    71,    73,    -1,    73,    -1,    72,    -1,    71,
      72,    -1,    73,     9,    -1,    73,     9,    63,    -1,    73,
       1,     9,    63,    -1,     9,    -1,    73,     1,     9,    -1,
      74,    -1,    45,    74,    -1,   108,    -1,   108,   133,    -1,
      42,    -1,    42,   108,    -1,    42,   108,   133,    -1,    46,
      -1,    46,   107,    -1,    46,   107,   133,    -1,   133,    -1,
      85,    -1,    80,    -1,    80,    85,    -1,    85,     8,    -1,
      85,     8,    83,    -1,    80,    85,     8,    -1,    80,    85,
       8,    83,    -1,    81,    -1,    98,    -1,   102,    -1,    97,
      -1,    39,    89,    -1,    40,    86,    -1,    41,    91,    -1,
      36,    -1,    36,   134,    -1,    37,    -1,    37,   134,    -1,
      75,   133,    -1,    44,   140,    -1,    44,   140,   134,    -1,
      38,    -1,    38,   134,    -1,   128,    -1,   129,    -1,   131,
      -1,    55,    76,    77,    -1,    55,    76,    -1,    55,    -1,
      54,    77,    -1,    54,    -1,    61,    -1,    62,    -1,    78,
      -1,    48,    58,    -1,    48,    -1,    58,    -1,   133,    -1,
       5,    -1,     5,   133,    -1,   137,     6,    -1,     6,   137,
      -1,     6,    -1,   137,     6,   137,    -1,   137,    -1,   137,
       6,    -1,     6,   137,    -1,     6,    -1,   137,     6,   137,
      -1,    50,    -1,    49,    -1,    51,    -1,    52,    -1,    82,
      -1,    81,     8,    82,    -1,    81,     8,    -1,    81,     8,
      63,    -1,    81,     8,     1,    -1,   137,     7,    21,    -1,
     137,     7,     5,    -1,   137,     7,   140,    -1,   137,     7,
      -1,    17,     7,   140,    -1,    17,     7,    -1,    17,    11,
     140,    -1,    17,    11,    -1,    84,    -1,    83,     8,    84,
      -1,    83,     8,    -1,    83,     8,    63,    -1,   137,   133,
      -1,   137,    -1,   137,   133,    69,    -1,   137,    69,    -1,
     137,   133,    -1,   137,    -1,   137,   133,    69,    -1,   137,
      69,    -1,    87,    -1,    86,     8,    87,    -1,    88,   134,
      -1,   140,    -1,    88,     8,   140,    -1,    90,    -1,    89,
       8,    90,    -1,     3,     7,   140,    -1,     3,    92,    93,
       9,    13,    -1,     3,    92,    93,     9,     1,    13,    -1,
      12,    -1,    94,    -1,    93,     9,    94,    -1,    39,    89,
      -1,    40,    86,    -1,    95,    -1,    96,    -1,    95,     8,
      96,    -1,    95,     1,    -1,   137,     7,    21,    -1,   137,
       7,     5,    -1,   137,     7,   140,    -1,    17,     7,   140,
      -1,    17,    11,   140,    -1,    69,    -1,    97,    69,    -1,
      99,    -1,    98,   103,    -1,    98,   103,   133,    -1,    98,
     103,    69,    -1,    98,    69,    -1,    98,   103,   133,    69,
      -1,    98,   133,    69,    -1,   103,    -1,   103,   133,    -1,
     103,    69,    -1,   103,   133,    69,    -1,    43,   103,    -1,
      43,     1,    -1,    43,    -1,    43,   103,   133,    -1,   100,
      -1,   101,   103,    -1,   101,   103,   133,    -1,   101,    -1,
     101,    69,    -1,   137,    35,   137,    -1,    35,   137,    -1,
     137,    35,    -1,    35,    -1,    47,   137,    -1,    47,   137,
       5,    -1,    47,   137,     6,    -1,    47,   137,     6,     5,
      -1,    47,   137,    10,    -1,    47,   137,    10,     5,    -1,
      47,   137,    35,    -1,    47,   137,     6,   137,    -1,    47,
     137,     6,   137,     5,    -1,    47,    -1,   137,    -1,    35,
      -1,   112,    -1,   114,    -1,   137,   105,   137,    -1,   105,
     137,    -1,   137,   105,    -1,   105,    -1,   137,   113,   137,
      -1,   113,   137,    -1,   137,   113,    -1,   113,    -1,   137,
      -1,   105,   137,    63,    -1,   137,   105,   137,    63,    -1,
     119,   137,    63,    -1,   137,   119,   137,    63,    -1,   106,
     104,    -1,   106,    -1,    34,    -1,   109,    -1,   110,    -1,
     111,    -1,   137,   118,   137,    -1,   118,   137,    -1,   137,
     118,    -1,   109,   121,   137,    -1,   109,   121,    -1,   137,
     119,   137,    -1,   119,   137,    -1,   137,   119,    -1,   110,
     122,   137,    -1,   110,   122,    -1,   137,   120,   137,    -1,
     120,   137,    -1,   137,   120,    -1,   111,   123,   137,    -1,
     111,   123,    -1,    22,    -1,    25,    -1,    28,    -1,    31,
      -1,    23,    -1,    26,    -1,    29,    -1,    32,    -1,    24,
      -1,    27,    -1,    30,    -1,    33,    -1,   112,    -1,     6,
      -1,   113,    -1,     6,    -1,   114,    -1,     6,    -1,   112,
      -1,    16,   112,    -1,   112,    16,    -1,    16,   112,    16,
      -1,   113,    -1,    16,   113,    -1,   113,    16,    -1,    16,
     113,    16,    -1,   114,    -1,    16,   114,    -1,   114,    16,
      -1,    16,   114,    16,    -1,   115,    -1,    16,   115,    -1,
     115,    16,    -1,    16,   115,    16,    -1,   116,    -1,    16,
     116,    -1,   116,    16,    -1,    16,   116,    16,    -1,   117,
      -1,    16,   117,    -1,   117,    16,    -1,    16,   117,    16,
      -1,   125,    -1,   137,    -1,    48,   104,    -1,    48,    -1,
     137,    -1,   139,    -1,    56,   126,    79,   124,    -1,    56,
     126,    79,   124,   124,    -1,    56,   126,   125,    -1,    56,
     126,   125,   124,    -1,    56,   126,    79,    -1,    56,   126,
     137,    -1,    56,   126,    -1,    56,    -1,   127,    -1,   127,
     133,    -1,    57,    47,   140,   133,    -1,    57,   133,    -1,
      57,    47,    -1,    57,    47,   133,    -1,    58,    -1,    59,
      -1,    60,    -1,   130,   133,    -1,   130,    -1,    19,    -1,
      18,    -1,   132,    -1,   132,   134,    -1,   134,   132,   134,
      -1,   134,   132,    -1,   134,    -1,    14,    15,    -1,    14,
     135,    15,    -1,    14,   135,     1,    15,    -1,    14,     1,
      15,    -1,    14,   135,    -1,    14,   135,     1,    -1,    14,
      -1,    14,    63,    -1,    14,     1,    -1,   136,    -1,   135,
       8,   136,    -1,   135,     8,    -1,   135,     8,    63,    -1,
     140,     7,   140,    -1,   140,     7,   140,     5,    -1,   140,
       7,   140,     8,     5,     5,    -1,   140,     7,   141,     8,
       5,    -1,   140,     7,     5,     8,     5,     8,     5,    -1,
     140,     7,     5,     8,     5,     8,     5,     8,     5,    -1,
     140,     7,     5,    -1,   140,     7,    21,    -1,   140,     7,
      -1,   140,     7,    63,    -1,   140,    -1,     4,    -1,     3,
      -1,    17,    -1,    39,    -1,    40,    -1,    41,    -1,    38,
      -1,    42,    -1,    43,    -1,    46,    -1,    45,    -1,    36,
      -1,    37,    -1,    44,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    51,    -1,    52,    -1,    53,    -1,    55,
      -1,    54,    -1,    56,    -1,    57,    -1,    58,    -1,    59,
      -1,    60,    -1,    61,    -1,    62,    -1,    22,    -1,    23,
      -1,    24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,
      -1,    29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,
      -1,    34,    -1,    35,    -1,   137,    -1,   138,    -1,   139,
      -1,    20,    -1,     5,    -1,   140,    -1,    12,    -1,    12,
      63,    -1,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   247,   247,   248,   259,   269,   280,   290,   300,   310,
     317,   331,   332,   341,   355,   367,   385,   395,   408,   421,
     433,   446,   447,   458,   473,   482,   490,   504,   505,   506,
     518,   533,   541,   564,   572,   584,   596,   611,   629,   641,
     653,   669,   681,   689,   701,   713,   726,   739,   755,   771,
     784,   791,   798,   806,   815,   824,   833,   843,   857,   867,
     881,   898,   911,   928,   938,   952,   953,   954,   955,   969,
     984,   998,  1012,  1028,  1028,  1030,  1031,  1057,  1075,  1092,
    1093,  1104,  1115,  1127,  1138,  1147,  1161,  1186,  1198,  1209,
    1219,  1234,  1234,  1234,  1234,  1236,  1246,  1265,  1278,  1279,
    1290,  1309,  1328,  1347,  1367,  1386,  1406,  1425,  1446,  1452,
    1463,  1475,  1478,  1494,  1506,  1522,  1535,  1553,  1568,  1586,
    1602,  1603,  1610,  1659,  1675,  1693,  1694,  1701,  1717,  1734,
    1754,  1764,  1765,  1772,  1779,  1786,  1788,  1789,  1795,  1802,
    1821,  1840,  1859,  1881,  1905,  1914,  1926,  1933,  1941,  1954,
    1962,  1971,  1984,  2000,  2008,  2021,  2029,  2044,  2059,  2075,
    2086,  2105,  2115,  2129,  2149,  2150,  2157,  2171,  2182,  2194,
    2204,  2216,  2230,  2244,  2259,  2272,  2287,  2321,  2337,  2355,
    2367,  2383,  2389,  2390,  2392,  2405,  2415,  2426,  2434,  2447,
    2457,  2468,  2476,  2489,  2490,  2491,  2492,  2495,  2506,  2517,
    2525,  2529,  2533,  2538,  2551,  2561,  2572,  2582,  2592,  2605,
    2615,  2626,  2636,  2645,  2658,  2668,  2679,  2689,  2698,  2698,
    2698,  2698,  2699,  2699,  2699,  2699,  2700,  2700,  2700,  2700,
    2702,  2702,  2703,  2703,  2704,  2704,  2706,  2715,  2725,  2735,
    2748,  2757,  2767,  2777,  2790,  2799,  2809,  2819,  2832,  2841,
    2851,  2861,  2874,  2883,  2893,  2903,  2916,  2925,  2935,  2945,
    2959,  2960,  2983,  2993,  3007,  3007,  3009,  3045,  3068,  3104,
    3128,  3164,  3201,  3237,  3254,  3261,  3274,  3292,  3306,  3321,
    3341,  3348,  3355,  3363,  3376,  3388,  3395,  3403,  3411,  3419,
    3431,  3439,  3442,  3452,  3462,  3473,  3484,  3495,  3506,  3519,
    3523,  3535,  3542,  3551,  3561,  3563,  3578,  3596,  3615,  3632,
    3649,  3667,  3681,  3695,  3709,  3714,  3726,  3726,  3728,  3728,
    3729,  3729,  3730,  3730,  3730,  3731,  3731,  3732,  3732,  3733,
    3733,  3733,  3733,  3733,  3734,  3734,  3734,  3735,  3735,  3735,
    3736,  3736,  3736,  3737,  3737,  3737,  3739,  3740,  3741,  3742,
    3743,  3744,  3745,  3746,  3747,  3748,  3749,  3750,  3751,  3759,
    3770,  3770,  3770,  3770,  3772,  3772,  3774,  3788,  3790
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
  "TOK_OSBRACKET", "TOK_CSBRACKET", "TOK_ASTERISK", "TOK_MSC",
  "TOK_COLON_STRING", "TOK_COLON_QUOTED_STRING", "TOK_STYLE_NAME",
  "TOK_BOOLEAN", "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM",
  "TOK_REL_SOLID_BIDIR", "TOK_REL_DOUBLE_TO", "TOK_REL_DOUBLE_FROM",
  "TOK_REL_DOUBLE_BIDIR", "TOK_REL_DASHED_TO", "TOK_REL_DASHED_FROM",
  "TOK_REL_DASHED_BIDIR", "TOK_REL_DOTTED_TO", "TOK_REL_DOTTED_FROM",
  "TOK_REL_DOTTED_BIDIR", "TOK_SPECIAL_ARC", "TOK_EMPH",
  "TOK_COMMAND_HEADING", "TOK_COMMAND_NUDGE", "TOK_COMMAND_NEWPAGE",
  "TOK_COMMAND_DEFCOLOR", "TOK_COMMAND_DEFSTYLE", "TOK_COMMAND_DEFDESIGN",
  "TOK_COMMAND_BIG", "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK",
  "TOK_COMMAND_PARALLEL", "TOK_VERTICAL", "TOK_AT", "TOK_AT_POS",
  "TOK_SHOW", "TOK_HIDE", "TOK_ACTIVATE", "TOK_DEACTIVATE", "TOK_BYE",
  "TOK_COMMAND_VSPACE", "TOK_COMMAND_HSPACE", "TOK_COMMAND_SYMBOL",
  "TOK_COMMAND_NOTE", "TOK_COMMAND_COMMENT", "TOK_COMMAND_ENDNOTE",
  "TOK_COMMAND_FOOTNOTE", "TOK_COMMAND_TITLE", "TOK_COMMAND_SUBTITLE",
  "TOK__NEVER__HAPPENS", "$accept", "msc_with_bye", "msc",
  "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "arc_with_parallel", "arc", "titlecommandtoken", "hspace_location",
  "full_arcattrlist_with_label_or_number", "entityrel",
  "markerrel_no_string", "entity_command_prefixes", "optlist", "opt",
  "entitylist", "entity", "first_entity", "styledeflist", "styledef",
  "tok_stringlist", "colordeflist", "colordef", "designdef",
  "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "box_list", "first_box",
  "first_pipe", "pipe_list_no_content", "pipe_list", "boxrel", "vertxpos",
  "empharcrel_straight", "vertrel_no_xpos", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to_no_loss",
  "relation_from_no_loss", "relation_bidir_no_loss",
  "relation_to_cont_no_loss", "relation_from_cont_no_loss",
  "relation_bidir_cont_no_loss", "relation_to", "relation_from",
  "relation_bidir", "relation_to_cont", "relation_from_cont",
  "relation_bidir_cont", "extvertxpos", "extvertxpos_no_string",
  "symbol_type_string", "symbol_command_no_attr", "symbol_command", "note",
  "comment_command", "comment", "colon_string",
  "full_arcattrlist_with_label", "full_arcattrlist", "arcattrlist",
  "arcattr", "entity_string", "reserved_word_string", "symbol_string",
  "string", "number_or_string", "scope_open", "scope_close", 0
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    64,    65,    65,    66,    66,    66,    66,    66,    66,
      66,    67,    67,    67,    68,    68,    69,    69,    69,    69,
      69,    70,    70,    70,    71,    71,    72,    72,    72,    72,
      72,    73,    73,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    75,    75,    76,    76,    76,    76,    77,
      77,    77,    78,    78,    78,    78,    78,    79,    79,    79,
      79,    80,    80,    80,    80,    81,    81,    81,    81,    81,
      82,    82,    82,    82,    82,    82,    82,    82,    83,    83,
      83,    83,    84,    84,    84,    84,    85,    85,    85,    85,
      86,    86,    87,    88,    88,    89,    89,    90,    91,    91,
      92,    93,    93,    94,    94,    94,    95,    95,    95,    96,
      96,    96,    96,    96,    97,    97,    98,    98,    98,    98,
      98,    98,    98,    99,    99,    99,    99,   100,   100,   100,
     100,   101,   101,   101,   102,   102,   103,   103,   103,   103,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   105,   105,   105,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   107,   107,   108,
     108,   108,   108,   109,   109,   109,   109,   109,   110,   110,
     110,   110,   110,   111,   111,   111,   111,   111,   112,   112,
     112,   112,   113,   113,   113,   113,   114,   114,   114,   114,
     115,   115,   116,   116,   117,   117,   118,   118,   118,   118,
     119,   119,   119,   119,   120,   120,   120,   120,   121,   121,
     121,   121,   122,   122,   122,   122,   123,   123,   123,   123,
     124,   124,   125,   125,   126,   126,   127,   127,   127,   127,
     127,   127,   127,   127,   128,   128,   129,   129,   129,   129,
     130,   130,   130,   131,   131,   132,   132,   133,   133,   133,
     133,   133,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   135,   135,   135,   135,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   137,   137,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     140,   140,   140,   140,   141,   141,   142,   142,   143
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     3,     2,     3,     3,     2,     4,     3,
       2,     1,     2,     1,     1,     2,     2,     3,     4,     1,
       3,     1,     2,     1,     2,     1,     2,     3,     1,     2,
       3,     1,     1,     1,     2,     2,     3,     3,     4,     1,
       1,     1,     1,     2,     2,     2,     1,     2,     1,     2,
       2,     2,     3,     1,     2,     1,     1,     1,     3,     2,
       1,     2,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     2,     2,     2,     1,     3,     1,     2,     2,     1,
       3,     1,     1,     1,     1,     1,     3,     2,     3,     3,
       3,     3,     3,     2,     3,     2,     3,     2,     1,     3,
       2,     3,     2,     1,     3,     2,     2,     1,     3,     2,
       1,     3,     2,     1,     3,     1,     3,     3,     5,     6,
       1,     1,     3,     2,     2,     1,     1,     3,     2,     3,
       3,     3,     3,     3,     1,     2,     1,     2,     3,     3,
       2,     4,     3,     1,     2,     2,     3,     2,     2,     1,
       3,     1,     2,     3,     1,     2,     3,     2,     2,     1,
       2,     3,     3,     4,     3,     4,     3,     4,     5,     1,
       1,     1,     1,     1,     3,     2,     2,     1,     3,     2,
       2,     1,     1,     3,     4,     3,     4,     2,     1,     1,
       1,     1,     1,     3,     2,     2,     3,     2,     3,     2,
       2,     3,     2,     3,     2,     2,     3,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       1,     2,     2,     3,     1,     2,     2,     3,     1,     2,
       2,     3,     1,     2,     2,     3,     1,     2,     2,     3,
       1,     1,     2,     1,     1,     1,     4,     5,     3,     4,
       3,     3,     2,     1,     1,     2,     4,     2,     2,     3,
       1,     1,     1,     2,     1,     1,     1,     1,     2,     3,
       2,     1,     2,     3,     4,     3,     2,     3,     1,     2,
       2,     1,     3,     2,     3,     3,     4,     6,     5,     7,
       9,     3,     3,     2,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned short int yydefact[] =
{
       4,   317,   316,    29,   366,     0,     0,     0,   286,   285,
     218,   222,   226,   219,   223,   227,   220,   224,   228,   221,
     225,   229,   199,   169,    56,    58,    63,     0,     0,     0,
      35,     0,     0,     0,    38,    92,    91,    93,    94,    72,
      70,   273,     0,   280,   281,   282,    73,    74,     0,     2,
       0,     0,   144,    11,    21,    24,     0,    31,     0,    43,
      49,    95,    42,    52,    50,   146,   161,   164,    51,   153,
      33,   200,   201,   202,   236,   240,   244,     0,     0,     0,
     274,    65,    66,   284,    67,   287,    41,   291,   117,     0,
     367,   300,   292,   318,   363,   346,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   327,
     328,   322,   319,   320,   321,   323,   324,   329,   326,   325,
     330,   331,   332,   333,   334,   335,   336,   338,   337,   339,
     340,   341,   342,   343,   344,   345,   299,     0,   301,   360,
     361,   362,   315,   237,   241,   245,     8,   105,   107,     5,
     167,    57,    59,    64,     0,    53,   125,    54,   120,     0,
     123,     0,    55,    36,     0,   158,   157,     0,    61,     0,
      32,     0,   181,   187,   198,    39,   182,   191,   183,     0,
     192,    80,    71,    79,    84,    77,    78,    69,    75,    86,
     272,   264,   265,   278,   277,     1,     3,    10,     6,    12,
      25,     0,     0,    26,    60,    44,   117,     0,    45,   145,
     150,   147,     0,   165,   162,   155,   154,    34,   231,     0,
     230,   248,   207,   233,     0,   232,   252,   212,   235,     0,
     234,   256,   217,   238,   242,   246,   204,   209,   214,   275,
     283,   288,   290,   103,   168,   119,   205,   210,   215,   116,
     368,     0,    17,   295,   297,   303,   293,   313,   239,   243,
     247,   104,   106,     0,     0,     0,     0,   122,   130,     0,
      37,   160,    62,   105,   185,   179,   197,   180,    40,   189,
       0,   186,   190,     0,    81,    83,    76,    68,    82,    89,
     263,   270,   268,   271,   279,     0,    13,    30,    27,    47,
      99,    98,    96,     0,    46,   108,   113,   149,   148,   152,
     163,   156,   249,   250,   206,   253,   254,   211,   257,   258,
     216,   289,   101,   100,   102,   166,   203,   208,   213,   118,
      19,    16,   294,   304,   302,   311,   312,   314,   305,     0,
     127,   126,   121,   124,     0,     0,     0,     0,   131,     0,
     136,     0,   104,   193,   170,   195,   184,   188,     0,    85,
      88,   262,   266,   260,   261,   269,    87,   276,    28,    48,
     110,   115,   112,   151,   251,   255,   259,    18,     0,   306,
       0,     0,     0,     0,   133,   134,     0,   138,     0,     0,
     171,   172,   174,   176,   194,   196,   267,    90,   111,   109,
     114,     0,     0,   308,   142,   143,     0,   128,   132,   137,
     140,   139,   141,   173,   177,   175,     0,   307,   129,   178,
     309,     0,   310
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,   187,   182,   188,   291,    59,    60,    61,   304,
     305,    62,   157,   158,   159,   155,   156,   162,   269,   347,
     348,   349,   350,    63,    64,    65,    66,    67,    68,    69,
     276,   173,   174,   175,    70,    71,    72,    73,    74,    75,
      76,   221,   226,   231,    77,    78,    79,   222,   227,   232,
     362,   363,   190,    80,    81,    82,    83,    84,    85,    86,
      87,   137,   138,   139,   140,   141,   160,   339,    89,   252
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -287
static const short int yypact[] =
{
     697,  -287,  -287,  -287,   -48,   394,   499,    49,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,   107,    12,    15,    69,    88,  1057,    96,
    1400,   157,  1057,  1117,  1353,  -287,  -287,  -287,  -287,    33,
      42,  1367,   106,  -287,  -287,  -287,  -287,  -287,   119,    68,
      18,   128,  -287,   139,   757,  -287,   339,  -287,   149,   198,
     148,  -287,   151,   168,   249,  -287,  -287,   191,  -287,   449,
     179,  1321,   349,   292,   169,   180,   205,   201,   201,   201,
     224,  -287,  -287,   237,  -287,   209,  -287,   196,  1428,   577,
    -287,   213,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,   129,  -287,  -287,
    -287,  -287,   232,   244,   263,   271,  -287,   997,  1177,  -287,
    -287,  -287,  -287,  -287,   264,   223,  -287,   266,  -287,   175,
    -287,   283,  -287,   258,  1464,  -287,   386,   274,   299,     6,
    -287,   291,  -287,   255,    71,   569,  -287,    21,  -287,   201,
    1446,   689,  -287,  -287,   262,   257,  -287,   215,  -287,   320,
      91,  -287,  -287,   637,  -287,  -287,  -287,  -287,  -287,   817,
    -287,  1300,   321,   273,  -287,   336,   452,   293,   282,  -287,
    -287,   630,   128,  -287,   749,  -287,   338,  -287,  -287,   864,
    -287,   355,   300,  -287,   353,  -287,   361,   330,  -287,   294,
    -287,   364,   357,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,   340,   877,   360,  -287,   380,   470,   531,   375,
    -287,  1320,  -287,  -287,   354,   516,  -287,   455,  -287,  -287,
    -287,   381,  -287,  1057,    88,  1057,  1057,  -287,  -287,   111,
    -287,  -287,  -287,  1237,   310,   581,  -287,  -287,  -287,  -287,
     347,   594,   100,   201,  -287,  -287,  -287,  -287,   621,   642,
     102,    17,    24,   393,  -287,   149,  -287,   352,  -287,   649,
    -287,  -287,  -287,   458,   454,  -287,   931,  -287,   457,  -287,
    -287,  -287,   521,  -287,  -287,   566,  -287,  -287,   576,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
     617,  -287,  -287,  -287,  -287,   635,  -287,  -287,    58,   639,
    -287,  -287,  -287,  -287,    46,    88,  1057,   641,  -287,   126,
    -287,   651,  -287,  -287,   181,  -287,   647,  -287,   654,  -287,
    -287,  -287,    40,  -287,  -287,  -287,   701,  -287,  -287,   694,
       5,  -287,   700,  -287,  -287,  -287,  -287,  -287,   713,  -287,
     739,   740,  1057,  1057,   742,   754,   328,  -287,   105,   937,
    -287,   208,   759,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,   762,   760,  -287,  -287,  -287,   764,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,   767,  -287,   773,  -287,  -287,  -287,
     796,   800,  -287
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -287,  -287,  -287,   611,  -287,    29,   733,  -287,   769,   770,
     792,  -287,  -287,   640,  -287,  -287,  -287,  -287,   623,   529,
     462,   778,   492,   599,  -287,   520,   619,  -287,  -287,  -287,
     498,  -287,   497,  -287,  -287,  -287,  -287,  -287,  -287,    -9,
     597,   708,  -287,  -287,   860,  -287,  -287,  -287,    -2,     1,
       8,   672,   669,   667,   -78,   -18,   -77,  -287,  -287,  -287,
    -286,   756,  -287,  -287,  -287,  -287,  -287,  -287,   857,    43,
     -23,  -287,   692,     0,  -287,   907,     7,  -287,  -287,  -246
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -299
static const short int yytable[] =
{
      88,   151,   152,   153,   143,   331,   365,   144,     1,     2,
     246,   248,   142,   273,   145,    90,   179,   148,    -9,   197,
       1,     2,   166,   150,     1,     2,     5,     1,     2,     5,
     164,   167,   176,    88,   180,   177,   149,   234,   181,   168,
     189,   191,   178,     1,     2,     1,     2,     5,   184,    -7,
     146,     8,     9,   382,    88,   211,   147,   383,   214,   206,
     148,     4,   241,   379,   167,   290,   380,   167,   398,   220,
     247,    -9,   290,   225,     1,     2,   396,   236,   237,   238,
     198,   230,   183,     5,   377,   194,   246,   248,   290,    88,
     185,   154,   209,   210,     1,     2,   213,   289,   215,   161,
     186,   204,    -7,     1,     2,     1,     2,   212,     1,     2,
       1,     2,   216,   217,     1,     2,   234,   245,   275,   195,
       5,   196,   344,   239,     8,     9,   240,   387,   344,  -296,
     254,   249,  -296,  -296,   388,  -135,   267,   255,  -296,   290,
       4,  -296,  -296,  -296,   256,   272,   247,  -296,  -296,   275,
     345,   346,   199,   193,   261,   262,   207,  -159,   165,   208,
       1,     2,   283,     5,  -296,  -159,  -159,     8,     9,  -159,
    -159,  -159,   144,   274,   277,  -159,  -159,   279,   176,   280,
       4,   282,  -296,   266,   285,   233,   390,   391,   178,     5,
     293,   392,    23,     5,     1,     2,   234,     8,     9,    88,
     295,     1,     2,     4,     1,     2,   270,   303,   306,   271,
    -159,     1,     2,   413,     8,     9,   393,   220,   278,   321,
     181,   235,   314,     5,   284,   225,    23,   317,   253,     5,
     183,   264,   320,     8,     9,   245,   294,   230,     5,   257,
     307,   309,     8,     9,   325,   311,   326,   327,   328,   249,
     324,     5,     1,     2,   308,     8,     9,   310,     1,     2,
     258,     4,   142,     5,   338,     1,     2,     8,     9,   351,
     340,   263,     5,   343,   265,   354,     8,     9,   329,   259,
     352,   356,   357,   358,    23,     1,     2,   260,   359,   360,
     277,   364,   364,   -97,   300,   268,     1,     2,   228,   306,
     228,   -97,   -97,     1,     2,   -97,   -97,   -97,   229,   244,
     169,   -97,   -97,     5,    11,   286,    12,    14,    12,    15,
      17,    15,    18,    20,    18,    21,   288,    21,   -97,   406,
     297,     1,     2,     1,     2,   371,   298,   373,   367,   -23,
     202,   407,   -23,   -23,   299,   344,   -97,   -23,   203,   372,
       4,   -23,   -23,   -23,     5,   223,   301,   -23,   -23,   223,
       1,     2,   364,     1,     2,   224,   397,   345,   346,   332,
     306,   313,    11,   353,   -23,    14,    11,   316,    17,    14,
     319,    20,    17,     1,     2,    20,   351,     4,   351,   404,
     405,   414,   -23,   -15,  -298,    91,   412,     1,     2,   366,
       5,   400,  -298,  -298,     8,     9,  -298,  -298,  -298,    92,
     355,    93,  -298,  -298,    94,   368,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,     1,     2,
     335,     4,   370,     5,     4,   243,     5,     8,     9,     4,
       8,     9,    93,     1,     2,    94,   336,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   337,     1,
       2,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    93,     1,     2,    94,   374,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   333,
       1,     2,   375,     5,     1,     2,     3,     8,     9,     4,
     250,     5,   376,     6,   169,     8,     9,     1,     2,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,     1,     2,    35,    36,    37,    38,
     250,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       1,     2,     4,   378,     5,     1,     2,   381,     8,     9,
     386,     5,     1,     2,    93,     8,     9,    94,   389,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,   370,     5,     1,     2,     3,     8,     9,     4,
     394,     5,     4,     6,     7,     8,     9,   395,   401,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   402,   403,    35,    36,    37,    38,
     264,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       1,     2,   265,     5,   415,   417,     3,     8,     9,     4,
     416,     5,   419,     6,   169,     8,     9,   418,   420,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   421,   422,    35,    36,    37,    38,
     296,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       1,     2,   251,   200,   201,   170,     3,   287,   369,     4,
     302,     5,   399,     6,   169,     8,     9,   205,   385,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,   342,   384,    35,    36,    37,    38,
     218,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       1,     2,   322,   341,   408,   409,    10,   361,   281,    13,
     163,   312,    16,   315,    93,    19,   318,    94,   323,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,   410,     4,   242,     5,   292,   334,   192,     8,
       9,     0,     0,     0,    93,     0,     0,    94,   411,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,     0,     0,     0,     0,     0,     0,     0,   -14,
       0,     0,     0,     0,    93,     0,     0,    94,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     4,
       0,     5,     0,     6,   169,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,     0,    34,     0,     0,    35,    36,    37,    38,
       0,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,    94,     0,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     -22,   202,     0,   -22,   -22,     0,     0,     0,   -22,   203,
       0,     0,   -22,   -22,   -22,     0,     0,     0,   -22,   -22,
     -20,   330,     0,   -20,   -20,     0,     0,   218,   -20,   -20,
       0,     0,   -20,   250,   -20,   -22,     0,   219,   -20,   -20,
       0,     0,     0,    10,     0,     0,    13,     0,     0,    16,
       0,     0,    19,   -22,     0,   -20,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       1,     2,     0,   -20,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     0,   172,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     0,     0,     0,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   243,     0,     0,     0,     0,
       4,     0,     5,     0,     6,     0,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   171,   244,     0,     0,     0,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       6,   172,     0,     0,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned short int yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,   439,   441,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     389,   391,     0,     0,   207,   210,    15,   393,   395,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   429,   431,    45,    47,    39,     0,     0,
       0,    41,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,     0,     0,     0,   197,   199,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   231,   233,     0,     0,     0,     0,
       0,     0,     0,   371,   374,   385,   387,     0,     0,     0,
      11,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,     0,     0,     0,     0,     0,     0,   157,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,    25,    81,
      27,    29,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,     0,     0,     0,     0,   121,     0,     0,     0,     0,
       0,     0,    31,   115,    99,   101,   123,   117,   119,     0,
       0,    75,    77,   103,     0,     0,     0,     0,     0,     0,
       0,   445,   447,     0,   141,   143,   427,     0,     0,     0,
       0,   125,     0,   139,     0,     0,   105,     0,     0,   225,
       0,   175,     0,   227,   229,     0,     0,     0,   127,     0,
       0,     0,   129,   131,     0,     0,     0,     0,     0,     0,
       0,   133,    85,    87,     0,   135,   137,     0,   193,   195,
     159,    89,     0,    91,     0,   221,   223,    93,    95,     0,
       0,     0,   179,     0,   177,     0,   181,   183,     0,   161,
       0,     0,     0,     0,    97,   269,   271,   163,     0,     0,
       0,     0,     0,     0,   263,     0,   265,   267,     0,     0,
       0,     0,     0,   291,   293,     0,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   297,   299,     0,     0,     0,     0,     0,
      71,     0,     0,     0,   253,     0,     0,     0,    73,     0,
     287,     0,     0,     0,   307,     0,     0,     0,     0,     0,
     303,   305,     0,   317,   319,     0,     0,     0,     0,     0,
       0,   289,     0,     0,     0,     0,     0,   295,     0,     0,
     301,     0,     0,   321,   323,     0,     0,   333,     0,     0,
       0,     0,     0,     0,     0,     1,     0,     3,     5,     0,
     185,     0,     0,     0,   187,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     7,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   347,   349,
       0,   107,   401,   109,   255,     0,   257,   111,   113,   411,
     259,   261,     0,   325,   327,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     351,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   353,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   339,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   329,   331,     0,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   343,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   345,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   201,   363,   365,     0,   203,   205,     0,
       0,     0,   417,     0,     0,     0,     0,   367,   369,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   377,   379,     0,     0,     0,     0,
     419,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     235,   237,   273,   421,   275,   381,   383,     0,   277,   279,
       0,   239,   397,   399,     0,   241,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   215,   433,   435,     0,   217,   219,     0,
       0,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    57,     0,   281,     0,     0,    59,   283,   285,    61,
       0,    63,     0,     0,     0,    65,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   449,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     309,   311,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,   405,     0,     0,     0,   407,
     409,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     167,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   171,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     355,   357,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   359,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     361,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,     0,     0,   251,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   335,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    35,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    21,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,     0,   147,     0,     0,     0,   149,   151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   213,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   298,     0,   298,     0,   298,     0,   298,     0,   298,
       0,   169,     0,   169,     0,    56,     0,    58,     0,    63,
       0,    35,     0,    35,     0,   159,     0,   159,     0,   159,
       0,   159,     0,    38,     0,    38,     0,    38,     0,    72,
       0,    72,     0,    72,     0,    70,     0,    70,     0,   273,
       0,   273,     0,   273,     0,    21,     0,    21,     0,    21,
       0,    21,     0,    21,     0,    21,     0,    21,     0,    21,
       0,    23,     0,    23,     0,    43,     0,    43,     0,    49,
       0,    42,     0,    52,     0,    50,     0,    50,     0,    50,
       0,    50,     0,    50,     0,    50,     0,    50,     0,   164,
       0,   164,     0,   164,     0,   164,     0,   153,     0,   153,
       0,   153,     0,   153,     0,    33,     0,    33,     0,    33,
       0,   236,     0,   240,     0,   244,     0,   274,     0,   274,
       0,   274,     0,   284,     0,   284,     0,   284,     0,   287,
       0,   291,     0,   291,     0,   117,     0,   117,     0,   117,
       0,   117,     0,   117,     0,   296,     0,   296,     0,   237,
       0,   241,     0,   245,     0,   105,     0,   107,     0,   107,
       0,   107,     0,   107,     0,    53,     0,    54,     0,    36,
       0,    36,     0,    36,     0,   157,     0,   157,     0,   157,
       0,    61,     0,   187,     0,   187,     0,   198,     0,   198,
       0,    39,     0,    39,     0,    39,     0,   191,   240,     0,
     191,   240,     0,   192,     0,    80,     0,    80,     0,    80,
       0,    84,     0,    84,     0,    69,     0,    69,     0,    69,
       0,   272,     0,   272,     0,   278,     0,   278,     0,   278,
       0,   278,     0,   278,     0,   278,     0,   278,     0,    22,
       0,    22,     0,    44,     0,   117,     0,   117,     0,   117,
       0,   117,     0,    97,     0,    97,     0,    97,     0,    45,
       0,    45,     0,   147,     0,   147,     0,   147,     0,   147,
       0,   162,     0,   162,     0,   162,     0,   154,     0,   248,
       0,   207,     0,   207,     0,   252,     0,   212,     0,   212,
       0,   256,     0,   217,     0,   217,     0,   290,     0,   103,
       0,   103,     0,   103,     0,   103,     0,   168,     0,   168,
       0,   205,     0,   205,     0,   210,     0,   210,     0,   215,
       0,   215,     0,   116,     0,    20,     0,    20,     0,   303,
       0,   303,     0,   303,     0,   303,     0,   313,     0,   313,
       0,   313,     0,   313,     0,   105,     0,   105,     0,   105,
       0,   105,     0,   179,     0,   179,     0,   186,     0,   186,
       0,   190,   240,     0,   190,   240,     0,    82,     0,    82,
       0,    89,     0,    89,     0,   263,     0,   263,     0,   270,
       0,   270,     0,   268,     0,   268,     0,    47,     0,    47,
       0,    46,     0,   113,     0,   113,     0,   113,     0,   113,
       0,   148,     0,   249,     0,   253,     0,   257,     0,    19,
       0,   311,   364,     0,   305,   365,     0,   170,     0,   266,
       0,   266,     0,    87,     0,    87,     0,    48,     0,   110,
       0,   110,     0,   112,     0,   172,     0,   172,     0,   309,
       0
};

static const short int yycheck[] =
{
       0,    24,    25,    26,     6,   251,   292,     6,     3,     4,
      88,    88,     5,     7,     6,    63,    34,    11,     0,     1,
       3,     4,    31,    23,     3,     4,    14,     3,     4,    14,
      30,    31,    34,    33,    34,    34,     7,    16,     5,    32,
      40,    41,    34,     3,     4,     3,     4,    14,     6,     0,
       1,    18,    19,     7,    54,    64,     7,    11,    67,    59,
      11,    12,    85,     5,    64,    48,     8,    67,    63,    71,
      88,    53,    48,    72,     3,     4,   362,    77,    78,    79,
      51,    73,    39,    14,   330,    42,   164,   164,    48,    89,
      48,     3,    63,    64,     3,     4,    67,     6,    69,     3,
      58,    58,    53,     3,     4,     3,     4,    64,     3,     4,
       3,     4,    69,    70,     3,     4,    16,    88,    47,     0,
      14,    53,    17,    80,    18,    19,    83,     1,    17,     0,
       1,    88,     3,     4,     8,     9,   159,     8,     9,    48,
      12,    12,    13,    14,    15,   168,   164,    18,    19,    47,
      39,    40,    13,    47,   147,   148,     8,     0,     1,     8,
       3,     4,   180,    14,    35,     8,     9,    18,    19,    12,
      13,    14,   171,   173,   174,    18,    19,   177,   180,   179,
      12,   180,    53,     8,   184,    16,     5,     6,   180,    14,
     190,    10,    35,    14,     3,     4,    16,    18,    19,   199,
     193,     3,     4,    12,     3,     4,   163,   207,   208,   166,
      53,     3,     4,     5,    18,    19,    35,   219,   175,   242,
       5,    16,   222,    14,   181,   224,    35,   227,    15,    14,
     187,     8,   232,    18,    19,   206,   193,   229,    14,     7,
     211,   212,    18,    19,   244,   216,   246,   247,   248,   206,
     243,    14,     3,     4,   211,    18,    19,   214,     3,     4,
      16,    12,   255,    14,   257,     3,     4,    18,    19,   269,
     263,     7,    14,   266,     8,   275,    18,    19,   249,    16,
     273,   281,   282,   283,    35,     3,     4,    16,   288,   289,
     290,   291,   292,     0,     1,    12,     3,     4,     6,   299,
       6,     8,     9,     3,     4,    12,    13,    14,    16,    35,
      17,    18,    19,    14,    23,    58,    24,    26,    24,    27,
      29,    27,    30,    32,    30,    33,     6,    33,    35,     1,
       9,     3,     4,     3,     4,   306,    63,   308,   295,     0,
       1,    13,     3,     4,     8,    17,    53,     8,     9,   306,
      12,    12,    13,    14,    14,     6,    63,    18,    19,     6,
       3,     4,   362,     3,     4,    16,   366,    39,    40,    15,
     370,    16,    23,    63,    35,    26,    23,    16,    29,    26,
      16,    32,    29,     3,     4,    32,   386,    12,   388,   382,
     383,   391,    53,    12,     0,     1,   389,     3,     4,     6,
      14,   372,     8,     9,    18,    19,    12,    13,    14,    15,
      63,    17,    18,    19,    20,    63,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,     3,     4,
       5,    12,     8,    14,    12,     7,    14,    18,    19,    12,
      18,    19,    17,     3,     4,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,     3,
       4,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    17,     3,     4,    20,    16,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
       3,     4,    16,    14,     3,     4,     9,    18,    19,    12,
      13,    14,    16,    16,    17,    18,    19,     3,     4,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     3,     4,    49,    50,    51,    52,
      13,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    12,     8,    14,     3,     4,     8,    18,    19,
       9,    14,     3,     4,    17,    18,    19,    20,     7,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,     8,    14,     3,     4,     9,    18,    19,    12,
      63,    14,    12,    16,    17,    18,    19,    63,     5,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     5,     5,    49,    50,    51,    52,
       8,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,     8,    14,     5,     5,     9,    18,    19,    12,
       8,    14,     5,    16,    17,    18,    19,    13,     5,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,     8,     5,    49,    50,    51,    52,
     199,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    89,    54,    54,    33,     9,   187,   299,    12,
     207,    14,   370,    16,    17,    18,    19,    59,   346,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,   265,   345,    49,    50,    51,    52,
       6,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,     5,   264,   386,   388,    22,   290,   180,    25,
      30,   219,    28,   224,    17,    31,   229,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,     5,    12,    87,    14,   190,   255,    41,    18,
      19,    -1,    -1,    -1,    17,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    46,    -1,    -1,    49,    50,    51,    52,
      -1,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,     8,     9,
      -1,    -1,    12,    13,    14,    -1,    -1,    -1,    18,    19,
       0,     1,    -1,     3,     4,    -1,    -1,     6,     8,     9,
      -1,    -1,    12,    13,    14,    35,    -1,    16,    18,    19,
      -1,    -1,    -1,    22,    -1,    -1,    25,    -1,    -1,    28,
      -1,    -1,    31,    53,    -1,    35,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
       3,     4,    -1,    53,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,     7,    -1,    -1,    -1,    -1,
      12,    -1,    14,    -1,    16,    -1,    18,    19,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    16,    35,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      16,    35,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     4,     9,    12,    14,    16,    17,    18,    19,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    49,    50,    51,    52,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    80,
      81,    82,    85,    97,    98,    99,   100,   101,   102,   103,
     108,   109,   110,   111,   112,   113,   114,   118,   119,   120,
     127,   128,   129,   130,   131,   132,   133,   134,   137,   142,
      63,     1,    15,    17,    20,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,   135,   136,   137,
     138,   139,   140,   112,   113,   114,     1,     7,    11,    69,
     137,   134,   134,   134,     3,    89,    90,    86,    87,    88,
     140,     3,    91,   108,   137,     1,   103,   137,   140,    17,
      74,    16,    35,   105,   106,   107,   112,   113,   114,   119,
     137,     5,    77,   133,     6,    48,    58,    76,    78,   137,
     126,   137,   139,    47,   133,     0,    53,     1,    69,    13,
      72,    73,     1,     9,   133,    85,   137,     8,     8,    69,
      69,   103,   133,    69,   103,    69,   133,   133,     6,    16,
     112,   115,   121,     6,    16,   113,   116,   122,     6,    16,
     114,   117,   123,    16,    16,    16,   137,   137,   137,   133,
     133,   134,   132,     7,    35,    69,   118,   119,   120,   133,
      13,    70,   143,    15,     1,     8,    15,     7,    16,    16,
      16,   140,   140,     7,     8,     8,     8,   134,    12,    92,
     133,   133,   134,     7,   137,    47,   104,   137,   133,   137,
     137,   105,   113,   119,   133,   137,    58,    77,     6,     6,
      48,    79,   125,   137,   133,   140,    67,     9,    63,     8,
       1,    63,    82,   137,    83,    84,   137,    69,   133,    69,
     133,    69,   115,    16,   137,   116,    16,   137,   117,    16,
     137,   134,     5,    21,   140,   137,   137,   137,   137,    69,
       1,   143,    15,    63,   136,     5,    21,    63,   140,   141,
     140,    90,    87,   140,    17,    39,    40,    93,    94,    95,
      96,   137,   140,    63,   137,    63,   137,   137,   137,   137,
     137,   104,   124,   125,   137,   124,     6,   133,    63,    83,
       8,    69,   133,    69,    16,    16,    16,   143,     8,     5,
       8,     8,     7,    11,    89,    86,     9,     1,     8,     7,
       5,     6,    10,    35,    63,    63,   124,   137,    63,    84,
      69,     5,     5,     5,   140,   140,     1,    13,    94,    96,
       5,    21,   140,     5,   137,     5,     8,     5,    13,     5,
       5,     8,     5
};


/* Prevent warning if -Wmissing-prototypes.  */
int yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner);

/* Error token number */
#define YYTERROR 1

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */


#define YYRHSLOC(Rhs, K) ((Rhs)[K].yystate.yyloc)
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))							\
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

/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# define YY_LOCATION_PRINT(File, Loc)			\
    fprintf (File, "%d.%d-%d.%d",			\
	     (Loc).first_line, (Loc).first_column,	\
	     (Loc).last_line,  (Loc).last_column)
#endif


#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex (&yylval, &yylloc, yyscanner)


#undef yynerrs
#define yynerrs (yystackp->yyerrcnt)
#undef yychar
#define yychar (yystackp->yyrawchar)
#undef yylval
#define yylval (yystackp->yyval)
#undef yylloc
#define yylloc (yystackp->yyloc)
#define csh_nerrs yynerrs
#define csh_char yychar
#define csh_lval yylval
#define csh_lloc yylloc

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)							     \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }	     \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
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

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
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

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			    \
do {									    \
  if (yydebug)								    \
    {									    \
      YYFPRINTF (stderr, "%s ", Title);					    \
      yy_symbol_print (stderr, Type,					    \
		       Value, Location, RESULT, yyscanner);  \
      YYFPRINTF (stderr, "\n");						    \
    }									    \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined __cplusplus \
      || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	  && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)			\
  do {							\
    if (Yystack->yyspaceLeft < YYHEADROOM)		\
      yyexpandGLRStack (Yystack);			\
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)			\
  do {							\
    if (Yystack->yyspaceLeft < YYHEADROOM)		\
      yyMemoryExhausted (Yystack);			\
  } while (YYID (0))
#endif


#if YYERROR_VERBOSE

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
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
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
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state.  */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;
  /* To compute the location of the error token.  */
  yyGLRStackItem yyerror_range[3];

  int yyerrcnt;
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;

  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static void yyFail (yyGLRStack* yystackp, YYLTYPE *yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, YYLTYPE *yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner, const char* yymsg)
{
  if (yymsg != NULL)
    yyerror (yylocp, RESULT, yyscanner, yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystackp)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  yyGLRState* s;
  int i;
  s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
	      YYSTYPE* yyvalp,
	      YYLTYPE* YYOPTIONAL_LOC (yylocp),
	      yyGLRStack* yystackp
	      , YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == NULL);
  int yylow;
  YYUSE (RESULT);
  YYUSE (yyscanner);
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)						     \
  return yyerror (yylocp, RESULT, yyscanner, YY_("syntax error: cannot back up")),     \
	 yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT ((*yylocp), (yyvsp - yyrhslen), yyrhslen);
  yystackp->yyerror_range[1].yystate.yyloc = *yylocp;

  switch (yyn)
    {
        case 3:

/* Line 936 of glr.c  */
#line 249 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
    YYACCEPT; //ignore anything after bye
}
    break;

  case 4:

/* Line 936 of glr.c  */
#line 259 "language.yy"
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

/* Line 936 of glr.c  */
#line 270 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
    msc.AddArcs((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));

    YYACCEPT;
}
    break;

  case 6:

/* Line 936 of glr.c  */
#line 281 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)))
        csh.AddDesignsToHints(true);
  #else
    msc.AddArcs((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
    YYACCEPT;
}
    break;

  case 7:

/* Line 936 of glr.c  */
#line 291 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 8:

/* Line 936 of glr.c  */
#line 301 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 9:

/* Line 936 of glr.c  */
#line 311 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arclist));
  #endif
}
    break;

  case 10:

/* Line 936 of glr.c  */
#line 318 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    CshPos pos = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc);
    if (((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)).last_pos >= ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).first_pos)
        pos.first_pos = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)).last_pos;
    csh.AddCSH_Error(pos, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist));
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start, "Could not recognize this as a valid line.");
  #endif
  YYACCEPT;
}
    break;

  case 12:

/* Line 936 of glr.c  */
#line 333 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Closing brace missing its opening pair.");
  #else
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start, "Unexpected '}'.");
  #endif
}
    break;

  case 13:

/* Line 936 of glr.c  */
#line 342 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Closing brace missing its opening pair.");
  #else
    //Merge $3 into $1
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist))->splice(((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist))->end(), *((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist)));
    delete ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist));
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "Unexpected '}'.");
  #endif
}
    break;

  case 14:

/* Line 936 of glr.c  */
#line 356 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a design name.");
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing design name.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 15:

/* Line 936 of glr.c  */
#line 368 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_DESIGNNAME);
    csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, "msc");
    std::string msg = csh.SetDesignTo((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), true);
    if (msg.length())
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msg.c_str());
  #else
    ArcBase *dummy = msc.AddAttribute(Attribute("msc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
	if (dummy) delete dummy;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 16:

/* Line 936 of glr.c  */
#line 386 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_BRACE);
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arcbase)) ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arclist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arcbase)); //Append any potential CommandNumbering
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arclist);
  #endif
}
    break;

  case 17:

/* Line 936 of glr.c  */
#line 396 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_BRACE);
  #else
    ((*yyvalp).arclist) = new ArcList;
    //scope_close should not return here with a CommandNumbering
    //but just in case
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase))
        delete((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase));
  #endif
}
    break;

  case 18:

/* Line 936 of glr.c  */
#line 409 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), "Could not recognize this as a valid line.");
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), COLOR_BRACE);
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.arcbase)) ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.arclist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.arcbase)); //Append any potential CommandNumbering
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.arclist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc)).start, "Syntax error.");
  #endif
    yyerrok;
}
    break;

  case 19:

/* Line 936 of glr.c  */
#line 422 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "Could not recognize this as a valid line.");
  #else
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "Missing '}'.");
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)).start, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "Here is the corresponding '{'.");
  #endif
}
    break;

  case 20:

/* Line 936 of glr.c  */
#line 434 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a closing brace ('}').");
  #else
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)).start, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Here is the corresponding '{'.");
  #endif
}
    break;

  case 22:

/* Line 936 of glr.c  */
#line 448 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a semicolon (';').");
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase)) ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase));
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 23:

/* Line 936 of glr.c  */
#line 459 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing a semicolon (';').");
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase))
        ((*yyvalp).arclist) = (new ArcList)->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase)); /* New list */
    else
        ((*yyvalp).arclist) = new ArcList;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).start, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 24:

/* Line 936 of glr.c  */
#line 474 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase))
        ((*yyvalp).arclist) = (new ArcList)->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase)); /* New list */
    else
        ((*yyvalp).arclist) = new ArcList;
  #endif
}
    break;

  case 25:

/* Line 936 of glr.c  */
#line 483 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase)) ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase));     /* Add to existing list */
    ((*yyvalp).arclist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
}
    break;

  case 26:

/* Line 936 of glr.c  */
#line 491 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbase);
  #endif
}
    break;

  case 29:

/* Line 936 of glr.c  */
#line 507 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SEMICOLON);
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase)=NULL;
  #endif
}
    break;

  case 30:

/* Line 936 of glr.c  */
#line 519 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_SEMICOLON);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "I am not sure what is coming here.");
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcbase);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)).end.NextChar(), "Expecting a keyword, entity name, a valid arrow, box or divider symbol or an opening brace.");
  #endif
}
    break;

  case 31:

/* Line 936 of glr.c  */
#line 534 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase)) ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase))->SetLineEnd(MSC_POS((*yylocp)));
        ((*yyvalp).arcbase)=(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase);
  #endif
}
    break;

  case 32:

/* Line 936 of glr.c  */
#line 542 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_LINE_START)) {
	    csh.AddLineBeginToHints(false);
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
    }
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase)) {
        ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase))->SetLineEnd(MSC_POS((*yylocp)));
        ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase))->SetParallel();
    }
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 33:

/* Line 936 of glr.c  */
#line 565 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase))->AddAttributeList(NULL);
    ((*yyvalp).arcbase)=((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase));
  #endif
}
    break;

  case 34:

/* Line 936 of glr.c  */
#line 573 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
    ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbase));
  #endif
}
    break;

  case 35:

/* Line 936 of glr.c  */
#line 585 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing an arrow specification.");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing an arrow specification.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    ((*yyvalp).arcbase) = NULL;
}
    break;

  case 36:

/* Line 936 of glr.c  */
#line 597 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBase *arc = msc.CreateArcBigArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase));
    if (arc)
        arc->AddAttributeList(NULL);
    delete (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase);
    ((*yyvalp).arcbase) = arc;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 37:

/* Line 936 of glr.c  */
#line 612 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbase));
    if (arrow) arrow->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
    ((*yyvalp).arcbase) = arrow;
    delete (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbase);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 38:

/* Line 936 of glr.c  */
#line 630 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing a marker.");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing a marker.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    ((*yyvalp).arcbase) = NULL;
}
    break;

  case 39:

/* Line 936 of glr.c  */
#line 642 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase)) {
	  ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase))->AddAttributeList(NULL);
	  ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbase));
    } else ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 40:

/* Line 936 of glr.c  */
#line 654 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbase)) {
	  ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
	  ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbase));
    } else ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 41:

/* Line 936 of glr.c  */
#line 670 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ((*yyvalp).arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.attriblist));
  #endif
}
    break;

  case 42:

/* Line 936 of glr.c  */
#line 682 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((*yyvalp).arcbase) = (new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.entitylist), &msc, false));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 43:

/* Line 936 of glr.c  */
#line 690 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing an entity.");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing an entity.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    ((*yyvalp).arcbase) = NULL;
}
    break;

  case 44:

/* Line 936 of glr.c  */
#line 702 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
  #else
    CommandEntity *ce = new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.entitylist), &msc, false);
    ce->AddAttributeList(NULL);
	((*yyvalp).arcbase) = ce->ApplyPrefix((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 45:

/* Line 936 of glr.c  */
#line 714 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.entitylist), &msc, false);
	ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing an entity.");
    ((*yyvalp).arcbase) = ce;
  #endif
}
    break;

  case 46:

/* Line 936 of glr.c  */
#line 727 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.entitylist))->Prepend((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.entitylist));
    CommandEntity *ce = new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.entitylist), &msc, false);
    delete ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.entitylist));
    ce->AddAttributeList(NULL);
	((*yyvalp).arcbase) = ce;
  #endif
}
    break;

  case 47:

/* Line 936 of glr.c  */
#line 740 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    ((*yyvalp).arcbase) = ce->ApplyPrefix((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).end.NextChar(), "Missing an entity.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 48:

/* Line 936 of glr.c  */
#line 756 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc));
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc));
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.entitylist))->Prepend((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.entitylist));
    CommandEntity *ce = new CommandEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.entitylist), &msc, false);
    delete ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.entitylist));
    ce->AddAttributeList(NULL);
    ((*yyvalp).arcbase) = ce->ApplyPrefix((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 49:

/* Line 936 of glr.c  */
#line 772 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "CommandArcList" element used only for this. */
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arclist)) {
        ((*yyvalp).arcbase) = (new CommandArcList(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arclist)));
		(((*yyvalp).arcbase))->AddAttributeList(NULL);
    } else
        ((*yyvalp).arcbase) = NULL;
  #endif
}
    break;

  case 50:

/* Line 936 of glr.c  */
#line 785 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 51:

/* Line 936 of glr.c  */
#line 792 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 52:

/* Line 936 of glr.c  */
#line 799 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcparallel));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 53:

/* Line 936 of glr.c  */
#line 807 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 54:

/* Line 936 of glr.c  */
#line 816 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 55:

/* Line 936 of glr.c  */
#line 825 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 56:

/* Line 936 of glr.c  */
#line 834 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = (new CommandEntity(NULL, &msc, false));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 57:

/* Line 936 of glr.c  */
#line 844 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcbase) = (new CommandEntity(NULL, &msc, false));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 58:

/* Line 936 of glr.c  */
#line 858 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 59:

/* Line 936 of glr.c  */
#line 868 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    ((*yyvalp).arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 60:

/* Line 936 of glr.c  */
#line 882 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, true);
  #else
    const EArcType t = CaseInsensitiveEqual("title", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str)) ? MSC_COMMAND_TITLE :
                         CaseInsensitiveEqual("subtitle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str)) ? MSC_COMMAND_SUBTITLE :
                         MSC_ARC_INVALID;
    ((*yyvalp).arcbase) = (new ArcDivider(t, &msc));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 61:

/* Line 936 of glr.c  */
#line 899 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.MarkerNames.insert((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcbase) = (new CommandMark((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), &msc));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 62:

/* Line 936 of glr.c  */
#line 912 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.MarkerNames.insert((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcbase) = (new CommandMark((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), &msc));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 63:

/* Line 936 of glr.c  */
#line 929 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
  #else
    ((*yyvalp).arcbase) = (new CommandNewpage(&msc, true, NULL));
	(((*yyvalp).arcbase))->AddAttributeList(NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 64:

/* Line 936 of glr.c  */
#line 939 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcbase) = (new CommandNewpage(&msc, true, NULL));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 68:

/* Line 936 of glr.c  */
#line 956 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcbase) = (new CommandHSpace(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.namerel)));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 69:

/* Line 936 of glr.c  */
#line 970 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), "Missing either a number or a label.");
  #else
    ((*yyvalp).arcbase) = new CommandHSpace(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.namerel)); //Will trigger an error: either label or space attr is needed
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 70:

/* Line 936 of glr.c  */
#line 985 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF)) {
		csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
		csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
	}
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing an entity.");
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 71:

/* Line 936 of glr.c  */
#line 999 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcbase) = (new CommandVSpace(&msc));
	(((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 72:

/* Line 936 of glr.c  */
#line 1013 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing either a number or a label.");
  #else
    ((*yyvalp).arcbase) = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 76:

/* Line 936 of glr.c  */
#line 1032 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), "left") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), "right"))
		csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
	else
	    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), "Use either `left` or `right` to specify which comment column to size.");
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), "left"))
		((*yyvalp).namerel) = new NamePair(LNOTE_ENT_STR, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
	else if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), "right"))
		((*yyvalp).namerel) = new NamePair(RNOTE_ENT_STR, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
	else {
	    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)).start, "Use either `left` or `right` to specify which comment column to size. Ignoring command.");
	    ((*yyvalp).namerel) = NULL;
	}
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 77:

/* Line 936 of glr.c  */
#line 1058 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), "left") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), "right"))
		csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
	else
	    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Use either `left comment` or `right comment` to specify which comment column to size.");
    if (csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).start, "Use <entity>-<entity>, `left comment`, `right comment` to specify horizontal spacing. Ignoring command.");
	((*yyvalp).namerel) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 78:

/* Line 936 of glr.c  */
#line 1076 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).start, "Use `left comment` or `right comment` to specify horizontal spacing for comments. Ignoring command.");
	((*yyvalp).namerel) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 80:

/* Line 936 of glr.c  */
#line 1094 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("space", atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str)));
    ((*yyvalp).attriblist) = al;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 81:

/* Line 936 of glr.c  */
#line 1105 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist))->Append(new Attribute("space", atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str)));
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 82:

/* Line 936 of glr.c  */
#line 1116 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).namerel) = new NamePair((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, FileLineColRange());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 83:

/* Line 936 of glr.c  */
#line 1128 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
  #else
    ((*yyvalp).namerel) = new NamePair(NULL, FileLineColRange(), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 84:

/* Line 936 of glr.c  */
#line 1139 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).namerel) = NULL;
  #endif
}
    break;

  case 85:

/* Line 936 of glr.c  */
#line 1148 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
  #else
    ((*yyvalp).namerel) = new NamePair((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 86:

/* Line 936 of glr.c  */
#line 1162 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc));
  #else
    ((*yyvalp).namerel) = new NamePair((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)), NULL, FileLineColRange());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 87:

/* Line 936 of glr.c  */
#line 1187 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).namerel) = new NamePair((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, FileLineColRange());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 88:

/* Line 936 of glr.c  */
#line 1199 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).namerel) = new NamePair(NULL, FileLineColRange(), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 89:

/* Line 936 of glr.c  */
#line 1210 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).namerel) = NULL;
  #endif
}
    break;

  case 90:

/* Line 936 of glr.c  */
#line 1220 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_MARKER);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).namerel) = new NamePair((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 95:

/* Line 936 of glr.c  */
#line 1237 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase)) {
        ((*yyvalp).arclist) = (new ArcList)->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase)); /* New list */
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        ((*yyvalp).arclist) = NULL;
  #endif
}
    break;

  case 96:

/* Line 936 of glr.c  */
#line 1247 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
    if (csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arcbase)) {
        if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist))
            ((*yyvalp).arclist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arcbase));     /* Add to existing list */
        else
            ((*yyvalp).arclist) = (new ArcList)->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arcbase)); /* New list */
        //($3)->MakeMeLastNotable(); Do not make chart options notable
    } else
        ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist);
  #endif
}
    break;

  case 97:

/* Line 936 of glr.c  */
#line 1266 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_COMMA);
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arclist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Expecting an option here.");
  #endif
}
    break;

  case 99:

/* Line 936 of glr.c  */
#line 1280 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "An option expected here.");
  #else
    ((*yyvalp).arclist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arclist);
  #endif
}
    break;

  case 100:

/* Line 936 of glr.c  */
#line 1291 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase) = msc.AddAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), str2bool((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 101:

/* Line 936 of glr.c  */
#line 1310 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase) = msc.AddAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 102:

/* Line 936 of glr.c  */
#line 1329 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH_AttrValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase) = msc.AddAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 103:

/* Line 936 of glr.c  */
#line 1348 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing option value.");
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing option value.");
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 104:

/* Line 936 of glr.c  */
#line 1368 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
            csh.hintStatus = HINT_READY;
        }
        std::string msg = csh.SetDesignTo((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), true);
        if (msg.length())
            csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msg.c_str());
  #else
        ((*yyvalp).arcbase) = msc.AddAttribute(Attribute("msc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 105:

/* Line 936 of glr.c  */
#line 1387 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing option value.");
        if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints(true);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing option value.");
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 106:

/* Line 936 of glr.c  */
#line 1407 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
            csh.hintStatus = HINT_READY;
        }
        std::string msg = csh.SetDesignTo((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), false);
        if (msg.length())
            csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msg.c_str());
  #else
        ((*yyvalp).arcbase) = msc.AddAttribute(Attribute("msc+", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 107:

/* Line 936 of glr.c  */
#line 1426 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing option value.");
        if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc+")) {
            csh.AddDesignsToHints(false);
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing option value.");
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 108:

/* Line 936 of glr.c  */
#line 1447 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).entitylist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.entitylist));
  #endif
}
    break;

  case 109:

/* Line 936 of glr.c  */
#line 1453 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.entitylist))->Prepend((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.entitylist));
    ((*yyvalp).entitylist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.entitylist);
    delete ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.entitylist));
  #endif
}
    break;

  case 110:

/* Line 936 of glr.c  */
#line 1464 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_COMMA);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).entitylist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.entitylist));
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Expecting an entity here.");
  #endif
}
    break;

  case 112:

/* Line 936 of glr.c  */
#line 1479 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist), NULL, FileLineCol());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 113:

/* Line 936 of glr.c  */
#line 1495 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 114:

/* Line 936 of glr.c  */
#line 1507 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 115:

/* Line 936 of glr.c  */
#line 1523 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList(NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 116:

/* Line 936 of glr.c  */
#line 1536 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist), NULL, FileLineCol());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 117:

/* Line 936 of glr.c  */
#line 1554 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 118:

/* Line 936 of glr.c  */
#line 1569 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 119:

/* Line 936 of glr.c  */
#line 1587 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), &msc);
    ed->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).entitylist) = ed->AddAttributeList(NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 121:

/* Line 936 of glr.c  */
#line 1604 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
  #endif
}
    break;

  case 122:

/* Line 936 of glr.c  */
#line 1611 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.stringlist)))
        if (csh.ForbiddenStyles.find(str) != csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(str);
	if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (auto a=((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist))->begin(); a!=((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist))->end(); a++) {
        std::list<string> problem;
        bool had_generic = false;
        for (auto s = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.stringlist))->begin(); s!=((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.stringlist))->end(); s++) {
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
        } else if (problem.size() == ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist))->size()) {
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
    delete((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.stringlist));
    delete((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
}
    break;

  case 123:

/* Line 936 of glr.c  */
#line 1660 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_STYLENAME);
        ((*yyvalp).stringlist) = new std::list<string>;
        if (strcmp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), "emphasis")==0)
            (((*yyvalp).stringlist))->push_back("box");
        else if (strcmp((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), "emptyemphasis")==0)
            (((*yyvalp).stringlist))->push_back("emptybox");
        else (((*yyvalp).stringlist))->push_back((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).stringlist) = new std::list<string>;
        (((*yyvalp).stringlist))->push_back(ConvertEmphasisToBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), &(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), msc));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 124:

/* Line 936 of glr.c  */
#line 1676 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_STYLENAME);
        ((*yyvalp).stringlist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.stringlist);
        if (strcmp((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), "emphasis")==0)
            (((*yyvalp).stringlist))->push_back("box");
        else if (strcmp((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), "emptyemphasis")==0)
            (((*yyvalp).stringlist))->push_back("emptybox");
        else (((*yyvalp).stringlist))->push_back((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
        ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.stringlist))->push_back(ConvertEmphasisToBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), &(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msc));
        ((*yyvalp).stringlist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.stringlist);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 126:

/* Line 936 of glr.c  */
#line 1695 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
  #endif
}
    break;

  case 127:

/* Line 936 of glr.c  */
#line 1702 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_COLORNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
    if (color.valid)
        csh.Contexts.back().Colors[(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str)] = color;
  #else
    msc.Contexts.back().colors.AddColor((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), msc.Error, MSC_POS((*yylocp)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 128:

/* Line 936 of glr.c  */
#line 1718 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), COLOR_DESIGNNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yyloc), COLOR_SEMICOLON);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yyloc), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str)] += csh.Contexts.back();
    csh.PopContext();
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
}
    break;

  case 129:

/* Line 936 of glr.c  */
#line 1735 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc), COLOR_DESIGNNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yyloc), COLOR_BRACE);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yyloc), COLOR_SEMICOLON);
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yyloc), "Could not recognize this as part of a design definition.");
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yyloc), COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str)] = csh.Contexts.back();
    csh.PopContext();
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str)] += msc.Contexts.back();
    msc.PopContext();
  #endif
}
    break;

  case 130:

/* Line 936 of glr.c  */
#line 1755 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
    break;

  case 132:

/* Line 936 of glr.c  */
#line 1766 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SEMICOLON);
  #endif
}
    break;

  case 133:

/* Line 936 of glr.c  */
#line 1773 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 134:

/* Line 936 of glr.c  */
#line 1780 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 137:

/* Line 936 of glr.c  */
#line 1790 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
  #endif
}
    break;

  case 138:

/* Line 936 of glr.c  */
#line 1796 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 139:

/* Line 936 of glr.c  */
#line 1803 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), str2bool((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 140:

/* Line 936 of glr.c  */
#line 1822 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 141:

/* Line 936 of glr.c  */
#line 1841 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_OPTIONNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 142:

/* Line 936 of glr.c  */
#line 1860 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues("msc", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), true);
    if (msg.length())
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 143:

/* Line 936 of glr.c  */
#line 1882 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_DESIGNNAME);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str))) {
        Msc::AttributeValues("msc+", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), false);
    if (msg.length())
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc+", (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc))));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 144:

/* Line 936 of glr.c  */
#line 1906 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arclist))
        ((*yyvalp).arcparallel) = new ArcParallel(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arclist));
    else
        ((*yyvalp).arcparallel) = NULL;
  #endif
}
    break;

  case 145:

/* Line 936 of glr.c  */
#line 1915 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist)==NULL)
        ((*yyvalp).arcparallel) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcparallel);
    else if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcparallel))
        ((*yyvalp).arcparallel) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcparallel))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
    else
        ((*yyvalp).arcparallel) = new ArcParallel(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
}
    break;

  case 146:

/* Line 936 of glr.c  */
#line 1927 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).arcboxseries) = new ArcBoxSeries((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbox));
  #endif
}
    break;

  case 147:

/* Line 936 of glr.c  */
#line 1934 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcboxseries))->AddFollow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbox));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 148:

/* Line 936 of glr.c  */
#line 1942 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcboxseries))->AddFollow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 149:

/* Line 936 of glr.c  */
#line 1955 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist))->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcboxseries))->AddFollow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 150:

/* Line 936 of glr.c  */
#line 1963 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), &msc);
    temp->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 151:

/* Line 936 of glr.c  */
#line 1972 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc)))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc)))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.arcbox))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.arclist))->SetLineEnd(MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc)));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.arcboxseries))->AddFollow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.arcbox));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.arcbox))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 152:

/* Line 936 of glr.c  */
#line 1985 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), &msc);
    temp->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist))->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)));
    ((*yyvalp).arcboxseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcboxseries))->AddFollow(temp);
    temp->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 153:

/* Line 936 of glr.c  */
#line 2001 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbox))->AddAttributeList(NULL);
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).arcbox) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbox);
  #endif
}
    break;

  case 154:

/* Line 936 of glr.c  */
#line 2009 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS((*yylocp)));
    ((*yyvalp).arcbox) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox));
  #endif
}
    break;

  case 155:

/* Line 936 of glr.c  */
#line 2022 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox))->AddAttributeList(NULL);
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
    ((*yyvalp).arcbox) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbox))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
}
    break;

  case 156:

/* Line 936 of glr.c  */
#line 2030 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcbox))->SetLineEnd(MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)));
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcbox))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arclist))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist));
    ((*yyvalp).arcbox) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcbox));
  #endif
}
    break;

  case 157:

/* Line 936 of glr.c  */
#line 2045 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
  #else
    ((*yyvalp).arcpipe) = new ArcPipe((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbox));
    (((*yyvalp).arcpipe))->AddAttributeList(NULL);
	(((*yyvalp).arcpipe))->SetLineEnd(MSC_POS((*yylocp)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 158:

/* Line 936 of glr.c  */
#line 2060 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a box symbol.");
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing a box symbol. Ignoring pipe.");
    ((*yyvalp).arcpipe) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 159:

/* Line 936 of glr.c  */
#line 2076 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcpipe) = NULL;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "The keyword '" + string((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 160:

/* Line 936 of glr.c  */
#line 2087 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    ((*yyvalp).arcpipe) = new ArcPipe((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox));
    (((*yyvalp).arcpipe))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
	(((*yyvalp).arcpipe))->SetLineEnd(MSC_POS((*yylocp)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 161:

/* Line 936 of glr.c  */
#line 2106 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcpipe))
      ((*yyvalp).arcpipeseries) = new ArcPipeSeries((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcpipe));
    else
      ((*yyvalp).arcpipeseries) = NULL;
  #endif
}
    break;

  case 162:

/* Line 936 of glr.c  */
#line 2116 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcbox));
    ap->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcpipeseries))
      ((*yyvalp).arcpipeseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcpipeseries))->AddFollowWithAttributes(ap, NULL);
    else {
      ap->AddAttributeList(NULL);
      ((*yyvalp).arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
    break;

  case 163:

/* Line 936 of glr.c  */
#line 2130 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcbox));
    ap->SetLineEnd(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)));
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcpipeseries))
      ((*yyvalp).arcpipeseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcpipeseries))->AddFollowWithAttributes(ap, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
    else {
      ap->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
      ((*yyvalp).arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
    break;

  case 165:

/* Line 936 of glr.c  */
#line 2151 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).arcpipeseries) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcpipeseries))->AddArcList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arclist));
  #endif
}
    break;

  case 166:

/* Line 936 of glr.c  */
#line 2158 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcbox) = new ArcBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 167:

/* Line 936 of glr.c  */
#line 2172 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcbox) = new ArcBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 168:

/* Line 936 of glr.c  */
#line 2183 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcbox) = new ArcBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 169:

/* Line 936 of glr.c  */
#line 2195 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcbox) = new ArcBox((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)), &msc);
  #endif
}
    break;

  case 170:

/* Line 936 of glr.c  */
#line 2205 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 171:

/* Line 936 of glr.c  */
#line 2217 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), VertXPos::POS_AT, atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 172:

/* Line 936 of glr.c  */
#line 2231 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), VertXPos::POS_LEFT_SIDE);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 173:

/* Line 936 of glr.c  */
#line 2245 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc)), VertXPos::POS_LEFT_SIDE, -atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 174:

/* Line 936 of glr.c  */
#line 2260 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_SYMBOL);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), VertXPos::POS_RIGHT_SIDE);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 175:

/* Line 936 of glr.c  */
#line 2273 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), COLOR_SYMBOL);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc)), VertXPos::POS_RIGHT_SIDE, atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 176:

/* Line 936 of glr.c  */
#line 2288 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_SYMBOL);
  #else
    switch ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.arctype)) {
    default:
        _ASSERT(0);
    case MSC_BOX_SOLID:
        ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), VertXPos::POS_LEFT_BY);
        break;
    case MSC_BOX_DASHED:
        ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_BOX_DOTTED:
        msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        ((*yyvalp).vertxpos) = NULL;
        break;
    case MSC_BOX_DOUBLE:
        msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start,
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        ((*yyvalp).vertxpos) = NULL;
        break;
    }
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 177:

/* Line 936 of glr.c  */
#line 2322 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 178:

/* Line 936 of glr.c  */
#line 2338 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.str));
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yyloc), COLOR_ATTRVALUE);
  #else
    ((*yyvalp).vertxpos) = new VertXPos(msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yyloc)), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.str)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.str));
}
    break;

  case 179:

/* Line 936 of glr.c  */
#line 2356 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing an entity name.");
  #else
    ((*yyvalp).vertxpos) = NULL;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing an entity name.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 180:

/* Line 936 of glr.c  */
#line 2368 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveBeginsWith("at", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str)) && csh.cursor_pos == ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).last_pos)
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD_PARTIAL);
    if (csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).vertxpos) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 181:

/* Line 936 of glr.c  */
#line 2384 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SYMBOL);
  #endif
}
    break;

  case 184:

/* Line 936 of glr.c  */
#line 2393 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 185:

/* Line 936 of glr.c  */
#line 2406 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype), MARKER_HERE_STR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 186:

/* Line 936 of glr.c  */
#line 2416 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MARKER_HERE_STR, &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 187:

/* Line 936 of glr.c  */
#line 2427 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 188:

/* Line 936 of glr.c  */
#line 2435 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 189:

/* Line 936 of glr.c  */
#line 2448 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MARKER_HERE_STR, &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 190:

/* Line 936 of glr.c  */
#line 2458 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_MARKER);
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype), MARKER_HERE_STR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), &msc);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 191:

/* Line 936 of glr.c  */
#line 2469 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    ((*yyvalp).arcvertarrow) = new ArcVerticalArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 192:

/* Line 936 of glr.c  */
#line 2477 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_MARKERNAME);
    csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_MARKER);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing a box or arrow symbol.");
  #else
    ((*yyvalp).arcvertarrow) = NULL;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 193:

/* Line 936 of glr.c  */
#line 2489 "language.yy"
    {free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str)); ((*yyvalp).arcvertarrow)=NULL;}
    break;

  case 194:

/* Line 936 of glr.c  */
#line 2490 "language.yy"
    {free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str)); ((*yyvalp).arcvertarrow)=NULL;}
    break;

  case 195:

/* Line 936 of glr.c  */
#line 2491 "language.yy"
    {free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str)); ((*yyvalp).arcvertarrow)=NULL;}
    break;

  case 196:

/* Line 936 of glr.c  */
#line 2492 "language.yy"
    {free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str)); free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str)); ((*yyvalp).arcvertarrow)=NULL;}
    break;

  case 197:

/* Line 936 of glr.c  */
#line 2496 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcvertarrow))
        ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcvertarrow))->AddXpos((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.vertxpos));
    else
        ((*yyvalp).arcbase) = NULL;
    delete (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.vertxpos);
  #endif
}
    break;

  case 198:

/* Line 936 of glr.c  */
#line 2507 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing the 'at' keyword.");
  #else
    ((*yyvalp).arcbase) = NULL;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing the 'at' keyword.");
    if ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcvertarrow)) delete (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcvertarrow);
  #endif
}
    break;

  case 199:

/* Line 936 of glr.c  */
#line 2518 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_SYMBOL);
  #else
    ((*yyvalp).arcbase) = new ArcDivider((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype), &msc);
  #endif
}
    break;

  case 200:

/* Line 936 of glr.c  */
#line 2526 "language.yy"
    {
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcarrow);
}
    break;

  case 201:

/* Line 936 of glr.c  */
#line 2530 "language.yy"
    {
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcarrow);
}
    break;

  case 202:

/* Line 936 of glr.c  */
#line 2534 "language.yy"
    {
    ((*yyvalp).arcbase) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcarrow);
}
    break;

  case 203:

/* Line 936 of glr.c  */
#line 2539 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 204:

/* Line 936 of glr.c  */
#line 2552 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcsegdata), LSIDE_ENT_STR, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 205:

/* Line 936 of glr.c  */
#line 2562 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), RSIDE_ENT_STR, true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 206:

/* Line 936 of glr.c  */
#line 2573 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 207:

/* Line 936 of glr.c  */
#line 2583 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 208:

/* Line 936 of glr.c  */
#line 2593 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), false, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 209:

/* Line 936 of glr.c  */
#line 2606 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), LSIDE_ENT_STR, false, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 210:

/* Line 936 of glr.c  */
#line 2616 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), RSIDE_ENT_STR, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), false, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 211:

/* Line 936 of glr.c  */
#line 2627 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 212:

/* Line 936 of glr.c  */
#line 2637 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 213:

/* Line 936 of glr.c  */
#line 2646 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 214:

/* Line 936 of glr.c  */
#line 2659 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcsegdata), LSIDE_ENT_STR, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 215:

/* Line 936 of glr.c  */
#line 2669 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==EOF);
  #else
    ((*yyvalp).arcarrow) = msc.CreateArcArrow((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), RSIDE_ENT_STR, true, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 216:

/* Line 936 of glr.c  */
#line 2680 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc));
    csh.AddCSH_EntityName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arcsegdata), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 217:

/* Line 936 of glr.c  */
#line 2690 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF);
  #else
    ((*yyvalp).arcarrow) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcarrow))->AddSegment((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arcsegdata), NULL, MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 231:

/* Line 936 of glr.c  */
#line 2702 "language.yy"
    {((*yyvalp).arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 233:

/* Line 936 of glr.c  */
#line 2703 "language.yy"
    {((*yyvalp).arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 235:

/* Line 936 of glr.c  */
#line 2704 "language.yy"
    {((*yyvalp).arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 236:

/* Line 936 of glr.c  */
#line 2707 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 237:

/* Line 936 of glr.c  */
#line 2716 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 238:

/* Line 936 of glr.c  */
#line 2726 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 239:

/* Line 936 of glr.c  */
#line 2736 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 240:

/* Line 936 of glr.c  */
#line 2749 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 241:

/* Line 936 of glr.c  */
#line 2758 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 242:

/* Line 936 of glr.c  */
#line 2768 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 243:

/* Line 936 of glr.c  */
#line 2778 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 244:

/* Line 936 of glr.c  */
#line 2791 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 245:

/* Line 936 of glr.c  */
#line 2800 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 246:

/* Line 936 of glr.c  */
#line 2810 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 247:

/* Line 936 of glr.c  */
#line 2820 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 248:

/* Line 936 of glr.c  */
#line 2833 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 249:

/* Line 936 of glr.c  */
#line 2842 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 250:

/* Line 936 of glr.c  */
#line 2852 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 251:

/* Line 936 of glr.c  */
#line 2862 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 252:

/* Line 936 of glr.c  */
#line 2875 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 253:

/* Line 936 of glr.c  */
#line 2884 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 254:

/* Line 936 of glr.c  */
#line 2894 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 255:

/* Line 936 of glr.c  */
#line 2904 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 256:

/* Line 936 of glr.c  */
#line 2917 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 257:

/* Line 936 of glr.c  */
#line 2926 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 258:

/* Line 936 of glr.c  */
#line 2936 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((*yylocp), COLOR_SYMBOL);
  #else
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_DST;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)));
  #endif
}
    break;

  case 259:

/* Line 936 of glr.c  */
#line 2946 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_SYMBOL);
	csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "One arrow may be lost only once. Use a single asterisk ('*').");
  #else
	msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
	(((*yyvalp).arcsegdata)).type = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.arctype);
	(((*yyvalp).arcsegdata)).lost = EArrowLost::AT_SRC;
	(((*yyvalp).arcsegdata)).lost_pos.SetFrom(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)));
  #endif
}
    break;

  case 261:

/* Line 936 of glr.c  */
#line 2961 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_KEYWORD)) {
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
    ((*yyvalp).extvertxpos) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 262:

/* Line 936 of glr.c  */
#line 2984 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));

  #else
    ((*yyvalp).extvertxpos) = new ExtVertXPos((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.vertxpos));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 263:

/* Line 936 of glr.c  */
#line 2994 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).extvertxpos) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 266:

/* Line 936 of glr.c  */
#line 3010 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.namerel), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.extvertxpos), NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 267:

/* Line 936 of glr.c  */
#line 3046 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.namerel), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.extvertxpos), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.extvertxpos));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.str));
}
    break;

  case 268:

/* Line 936 of glr.c  */
#line 3069 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.extvertxpos), NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 269:

/* Line 936 of glr.c  */
#line 3105 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), HINT_MARKER);
  #else
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.extvertxpos), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.extvertxpos));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 270:

/* Line 936 of glr.c  */
#line 3129 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.namerel), NULL, NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 271:

/* Line 936 of glr.c  */
#line 3165 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_MARKER)) {
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
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), NULL, NULL, NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 272:

/* Line 936 of glr.c  */
#line 3202 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), "arc") || CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), "rectangle") ||
        CaseInsensitiveEqual((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), "..."))
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("rectangle", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str))==0 &&
             CaseInsensitiveBeginsWith("...", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str))==0)
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_MARKER)) {
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
    ((*yyvalp).arcbase) = new CommandSymbol(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), NULL, NULL, NULL);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 273:

/* Line 936 of glr.c  */
#line 3238 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end, "Missing a symbol type.", "Use 'arc', '...' or 'rectangle'.");
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 274:

/* Line 936 of glr.c  */
#line 3255 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase))->AddAttributeList(NULL);
    ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arcbase));
  #endif
}
    break;

  case 275:

/* Line 936 of glr.c  */
#line 3262 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
    ((*yyvalp).arcbase) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.arcbase));
  #endif
}
    break;

  case 276:

/* Line 936 of glr.c  */
#line 3275 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_EntityOrMarkerName((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc)))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc)))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ((*yyvalp).arcbase) = new CommandNote(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc)));
    (((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 277:

/* Line 936 of glr.c  */
#line 3293 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ((*yyvalp).arcbase) = new CommandNote(&msc);
    (((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 278:

/* Line 936 of glr.c  */
#line 3307 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing an entity or marker name.");
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 279:

/* Line 936 of glr.c  */
#line 3322 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Missing an entity or marker name.");
    if (csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    ((*yyvalp).arcbase) = new CommandNote(&msc);
    (((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 280:

/* Line 936 of glr.c  */
#line 3342 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).eside)= ESide::LEFT;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 281:

/* Line 936 of glr.c  */
#line 3349 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).eside)= ESide::END;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 282:

/* Line 936 of glr.c  */
#line 3356 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((*yyvalp).eside)= ESide::END;
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 283:

/* Line 936 of glr.c  */
#line 3364 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    ((*yyvalp).arcbase) = new CommandNote(&msc, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.eside));
    (((*yyvalp).arcbase))->AddAttributeList((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist));
  #endif
}
    break;

  case 284:

/* Line 936 of glr.c  */
#line 3377 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Comments and notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    ((*yyvalp).arcbase) = NULL;
  #endif
}
    break;

  case 285:

/* Line 936 of glr.c  */
#line 3389 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), false);
  #endif
    ((*yyvalp).str) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str);
}
    break;

  case 286:

/* Line 936 of glr.c  */
#line 3396 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), true);
  #endif
    ((*yyvalp).str) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str);
}
    break;

  case 287:

/* Line 936 of glr.c  */
#line 3404 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        ((*yyvalp).attriblist) = (new AttributeList)->Append(new Attribute("label", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)), MSC_POS((*yylocp)).IncStartCol()));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 288:

/* Line 936 of glr.c  */
#line 3412 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        ((*yyvalp).attriblist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist))->Prepend(new Attribute("label", (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc)).IncStartCol()));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 289:

/* Line 936 of glr.c  */
#line 3420 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        ((*yyvalp).attriblist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.attriblist))->Append(new Attribute("label", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc)).IncStartCol()));
        //Merge $3 at the end of $1
        ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.attriblist))->splice(((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.attriblist))->end(), *((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist)));
        delete ((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attriblist)); //empty list now
        ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.attriblist);
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 290:

/* Line 936 of glr.c  */
#line 3432 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        ((*yyvalp).attriblist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.attriblist))->Append(new Attribute("label", (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).IncStartCol()));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 292:

/* Line 936 of glr.c  */
#line 3443 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_BRACKET);
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = new AttributeList;
  #endif
}
    break;

  case 293:

/* Line 936 of glr.c  */
#line 3453 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_BRACKET);
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist);
  #endif
}
    break;

  case 294:

/* Line 936 of glr.c  */
#line 3463 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), "Extra stuff after an attribute list. Maybe missing a comma?");
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), COLOR_BRACKET);
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.attriblist);
  #endif
}
    break;

  case 295:

/* Line 936 of glr.c  */
#line 3474 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), "Could not recognize this as an attribute.");
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_BRACKET);
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = new AttributeList;
  #endif
}
    break;

  case 296:

/* Line 936 of glr.c  */
#line 3485 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a square bracket (']').");
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.attriblist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Missing ']'.");
  #endif
}
    break;

  case 297:

/* Line 936 of glr.c  */
#line 3496 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), "Missing a ']'.");
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.attriblist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)).start, "Missing ']'.");
  #endif
}
    break;

  case 298:

/* Line 936 of glr.c  */
#line 3507 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_ErrorAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), "Missing a square bracket (']').");
        csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc)).end.NextChar(), "Missing ']'.");
  #endif
}
    break;

  case 299:

/* Line 936 of glr.c  */
#line 3520 "language.yy"
    {
    ((*yyvalp).attriblist) = NULL;
}
    break;

  case 300:

/* Line 936 of glr.c  */
#line 3524 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), COLOR_BRACKET);
        csh.AddCSH_Error((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), "Missing a ']'.");
        csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = new AttributeList;
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).start, "Missing ']'.");
  #endif
}
    break;

  case 301:

/* Line 936 of glr.c  */
#line 3536 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((*yyvalp).attriblist) = (new AttributeList)->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.attrib));
  #endif
}
    break;

  case 302:

/* Line 936 of glr.c  */
#line 3543 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_COMMA);
    csh.CheckHintBetween((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.attriblist))->Append((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.attrib));
  #endif
}
    break;

  case 303:

/* Line 936 of glr.c  */
#line 3552 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_COMMA);
    csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    ((*yyvalp).attriblist) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.attriblist);
    msc.Error.Error(MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc)).end.NextChar(), "Expecting an entity here.");
  #endif
}
    break;

  case 305:

/* Line 936 of glr.c  */
#line 3564 "language.yy"
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 306:

/* Line 936 of glr.c  */
#line 3579 "language.yy"
    {
  //string=string+number
  //string=string-number
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrColorValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
  #else
        //Ensure the NUMBER starts with either + or -
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str)) + (atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str))>=0 && ((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str))[0]!='+' ? "+" : "") + (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.str));
}
    break;

  case 307:

/* Line 936 of glr.c  */
#line 3597 "language.yy"
    {
  //string=string,number+number
  //string=string,number-number
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrColorValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str));
  #else
        //Ensure the NUMBER starts with either + or -
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.str)) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.str) + (atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.str))>=0 && ((((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.str))[0]!='+' ? "+" : "") +(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.str));
}
    break;

  case 308:

/* Line 936 of glr.c  */
#line 3616 "language.yy"
    {
  //string=string,number
  //string=number,number
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrColorValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.str)) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.str));
}
    break;

  case 309:

/* Line 936 of glr.c  */
#line 3633 "language.yy"
    {
  //string=number,number,number
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrColorValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.str), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.str)) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.str) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.str));
}
    break;

  case 310:

/* Line 936 of glr.c  */
#line 3650 "language.yy"
    {
  //string=number,number,number,number
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH_AttrColorValue((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yyloc) + (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (9))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yysemantics.yysval.str), string((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yysemantics.yysval.str)) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.str) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.str) + "," + (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.str), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yyloc)), MSC_POS2((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yyloc)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (9))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (9))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (9))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (9))].yystate.yysemantics.yysval.str));
	free((((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (9))].yystate.yysemantics.yysval.str));
}
    break;

  case 311:

/* Line 936 of glr.c  */
#line 3668 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), atof((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 312:

/* Line 936 of glr.c  */
#line 3682 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), COLOR_EQUAL);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), COLOR_ATTRVALUE);
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc), HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str), str2bool((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str)), MSC_POS((*yylocp)), MSC_POS((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 313:

/* Line 936 of glr.c  */
#line 3696 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), COLOR_ATTRNAME);
        csh.AddCSH((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), COLOR_EQUAL);
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc), HINT_ATTR_NAME);
        csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_ATTR_VALUE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str), (char*)NULL, MSC_POS((*yylocp)), MSC_POS((*yylocp)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.str));
}
    break;

  case 314:

/* Line 936 of glr.c  */
#line 3710 "language.yy"
    {
    ((*yyvalp).attrib) = NULL;
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.str));
}
    break;

  case 315:

/* Line 936 of glr.c  */
#line 3715 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
        csh.CheckHintAt((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), HINT_ATTR_NAME);
  #else
        ((*yyvalp).attrib) = new Attribute((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str), MSC_POS((*yylocp)));
  #endif
    free((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.str));
}
    break;

  case 346:

/* Line 936 of glr.c  */
#line 3739 "language.yy"
    {((*yyvalp).str) = strdup("->");}
    break;

  case 347:

/* Line 936 of glr.c  */
#line 3740 "language.yy"
    {((*yyvalp).str) = strdup("<-");}
    break;

  case 348:

/* Line 936 of glr.c  */
#line 3741 "language.yy"
    {((*yyvalp).str) = strdup("<->");}
    break;

  case 349:

/* Line 936 of glr.c  */
#line 3742 "language.yy"
    {((*yyvalp).str) = strdup("=>");}
    break;

  case 350:

/* Line 936 of glr.c  */
#line 3743 "language.yy"
    {((*yyvalp).str) = strdup("<=");}
    break;

  case 351:

/* Line 936 of glr.c  */
#line 3744 "language.yy"
    {((*yyvalp).str) = strdup("<=>");}
    break;

  case 352:

/* Line 936 of glr.c  */
#line 3745 "language.yy"
    {((*yyvalp).str) = strdup(">>");}
    break;

  case 353:

/* Line 936 of glr.c  */
#line 3746 "language.yy"
    {((*yyvalp).str) = strdup("<<");}
    break;

  case 354:

/* Line 936 of glr.c  */
#line 3747 "language.yy"
    {((*yyvalp).str) = strdup("<<>>");}
    break;

  case 355:

/* Line 936 of glr.c  */
#line 3748 "language.yy"
    {((*yyvalp).str) = strdup(">");}
    break;

  case 356:

/* Line 936 of glr.c  */
#line 3749 "language.yy"
    {((*yyvalp).str) = strdup("<");}
    break;

  case 357:

/* Line 936 of glr.c  */
#line 3750 "language.yy"
    {((*yyvalp).str) = strdup("<>");}
    break;

  case 358:

/* Line 936 of glr.c  */
#line 3752 "language.yy"
    {
    switch ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype)) {
    case MSC_ARC_DIVIDER:  ((*yyvalp).str) = strdup("---"); break;
    case MSC_ARC_DISCO:    ((*yyvalp).str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 359:

/* Line 936 of glr.c  */
#line 3760 "language.yy"
    {
    switch ((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.arctype)) {
    case MSC_BOX_SOLID:  ((*yyvalp).str) = strdup("--"); break;
    case MSC_BOX_DASHED: ((*yyvalp).str) = strdup("++"); break;
    case MSC_BOX_DOTTED: ((*yyvalp).str) = strdup(".."); break;
    case MSC_BOX_DOUBLE: ((*yyvalp).str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
    break;

  case 366:

/* Line 936 of glr.c  */
#line 3775 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext();
    if (csh.CheckHintAfter((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.PushContext();
  #endif
}
    break;

  case 368:

/* Line 936 of glr.c  */
#line 3791 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    ((*yyvalp).arcbase) = NULL;
    csh.PopContext();
  #else
    std::pair<bool, double> hscale = msc.Contexts.back().hscale;
    ((*yyvalp).arcbase) = msc.PopContext();
    if (hscale.first)
        msc.Contexts.back().hscale = hscale;
  #endif
}
    break;



/* Line 936 of glr.c  */
#line 6811 "colorsyntax.cc"
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


/*ARGSUSED*/ static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
      
      default: break;
    }
}

			      /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
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

  switch (yytype)
    {
      case 3: /* "TOK_STRING" */

/* Line 966 of glr.c  */
#line 173 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6867 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 966 of glr.c  */
#line 173 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6876 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6885 "colorsyntax.cc"
	break;
      case 17: /* "TOK_MSC" */

/* Line 966 of glr.c  */
#line 174 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6894 "colorsyntax.cc"
	break;
      case 18: /* "TOK_COLON_STRING" */

/* Line 966 of glr.c  */
#line 173 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6903 "colorsyntax.cc"
	break;
      case 19: /* "TOK_COLON_QUOTED_STRING" */

/* Line 966 of glr.c  */
#line 173 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6912 "colorsyntax.cc"
	break;
      case 20: /* "TOK_STYLE_NAME" */

/* Line 966 of glr.c  */
#line 173 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6921 "colorsyntax.cc"
	break;
      case 21: /* "TOK_BOOLEAN" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6930 "colorsyntax.cc"
	break;
      case 36: /* "TOK_COMMAND_HEADING" */

/* Line 966 of glr.c  */
#line 176 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6939 "colorsyntax.cc"
	break;
      case 37: /* "TOK_COMMAND_NUDGE" */

/* Line 966 of glr.c  */
#line 176 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6948 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_NEWPAGE" */

/* Line 966 of glr.c  */
#line 176 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6957 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 966 of glr.c  */
#line 175 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6966 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 966 of glr.c  */
#line 175 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6975 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 966 of glr.c  */
#line 175 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6984 "colorsyntax.cc"
	break;
      case 42: /* "TOK_COMMAND_BIG" */

/* Line 966 of glr.c  */
#line 174 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 6993 "colorsyntax.cc"
	break;
      case 43: /* "TOK_COMMAND_PIPE" */

/* Line 966 of glr.c  */
#line 174 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7002 "colorsyntax.cc"
	break;
      case 44: /* "TOK_COMMAND_MARK" */

/* Line 966 of glr.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7011 "colorsyntax.cc"
	break;
      case 45: /* "TOK_COMMAND_PARALLEL" */

/* Line 966 of glr.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7020 "colorsyntax.cc"
	break;
      case 46: /* "TOK_VERTICAL" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7029 "colorsyntax.cc"
	break;
      case 47: /* "TOK_AT" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7038 "colorsyntax.cc"
	break;
      case 48: /* "TOK_AT_POS" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7047 "colorsyntax.cc"
	break;
      case 49: /* "TOK_SHOW" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7056 "colorsyntax.cc"
	break;
      case 50: /* "TOK_HIDE" */

/* Line 966 of glr.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7065 "colorsyntax.cc"
	break;
      case 51: /* "TOK_ACTIVATE" */

/* Line 966 of glr.c  */
#line 179 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7074 "colorsyntax.cc"
	break;
      case 52: /* "TOK_DEACTIVATE" */

/* Line 966 of glr.c  */
#line 179 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7083 "colorsyntax.cc"
	break;
      case 53: /* "TOK_BYE" */

/* Line 966 of glr.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7092 "colorsyntax.cc"
	break;
      case 54: /* "TOK_COMMAND_VSPACE" */

/* Line 966 of glr.c  */
#line 179 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7101 "colorsyntax.cc"
	break;
      case 56: /* "TOK_COMMAND_SYMBOL" */

/* Line 966 of glr.c  */
#line 179 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7110 "colorsyntax.cc"
	break;
      case 57: /* "TOK_COMMAND_NOTE" */

/* Line 966 of glr.c  */
#line 180 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7119 "colorsyntax.cc"
	break;
      case 58: /* "TOK_COMMAND_COMMENT" */

/* Line 966 of glr.c  */
#line 180 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7128 "colorsyntax.cc"
	break;
      case 59: /* "TOK_COMMAND_ENDNOTE" */

/* Line 966 of glr.c  */
#line 180 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7137 "colorsyntax.cc"
	break;
      case 60: /* "TOK_COMMAND_FOOTNOTE" */

/* Line 966 of glr.c  */
#line 180 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7146 "colorsyntax.cc"
	break;
      case 61: /* "TOK_COMMAND_TITLE" */

/* Line 966 of glr.c  */
#line 181 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7155 "colorsyntax.cc"
	break;
      case 62: /* "TOK_COMMAND_SUBTITLE" */

/* Line 966 of glr.c  */
#line 181 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7164 "colorsyntax.cc"
	break;
      case 67: /* "top_level_arclist" */

/* Line 966 of glr.c  */
#line 168 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 966 of glr.c  */
#line 7173 "colorsyntax.cc"
	break;
      case 69: /* "braced_arclist" */

/* Line 966 of glr.c  */
#line 168 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 966 of glr.c  */
#line 7182 "colorsyntax.cc"
	break;
      case 70: /* "arclist_maybe_no_semicolon" */

/* Line 966 of glr.c  */
#line 168 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 966 of glr.c  */
#line 7191 "colorsyntax.cc"
	break;
      case 71: /* "arclist" */

/* Line 966 of glr.c  */
#line 168 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 966 of glr.c  */
#line 7200 "colorsyntax.cc"
	break;
      case 72: /* "arc_with_parallel_semicolon" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7209 "colorsyntax.cc"
	break;
      case 73: /* "arc_with_parallel" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7218 "colorsyntax.cc"
	break;
      case 74: /* "arc" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7227 "colorsyntax.cc"
	break;
      case 77: /* "full_arcattrlist_with_label_or_number" */

/* Line 966 of glr.c  */
#line 171 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 966 of glr.c  */
#line 7236 "colorsyntax.cc"
	break;
      case 81: /* "optlist" */

/* Line 966 of glr.c  */
#line 168 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 966 of glr.c  */
#line 7245 "colorsyntax.cc"
	break;
      case 82: /* "opt" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7254 "colorsyntax.cc"
	break;
      case 83: /* "entitylist" */

/* Line 966 of glr.c  */
#line 169 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 966 of glr.c  */
#line 7263 "colorsyntax.cc"
	break;
      case 84: /* "entity" */

/* Line 966 of glr.c  */
#line 169 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 966 of glr.c  */
#line 7272 "colorsyntax.cc"
	break;
      case 85: /* "first_entity" */

/* Line 966 of glr.c  */
#line 169 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 966 of glr.c  */
#line 7281 "colorsyntax.cc"
	break;
      case 88: /* "tok_stringlist" */

/* Line 966 of glr.c  */
#line 170 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 966 of glr.c  */
#line 7290 "colorsyntax.cc"
	break;
      case 97: /* "parallel" */

/* Line 966 of glr.c  */
#line 167 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 966 of glr.c  */
#line 7299 "colorsyntax.cc"
	break;
      case 98: /* "box_list" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 966 of glr.c  */
#line 7308 "colorsyntax.cc"
	break;
      case 99: /* "first_box" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 966 of glr.c  */
#line 7317 "colorsyntax.cc"
	break;
      case 100: /* "first_pipe" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 966 of glr.c  */
#line 7326 "colorsyntax.cc"
	break;
      case 101: /* "pipe_list_no_content" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 966 of glr.c  */
#line 7335 "colorsyntax.cc"
	break;
      case 102: /* "pipe_list" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 966 of glr.c  */
#line 7344 "colorsyntax.cc"
	break;
      case 103: /* "boxrel" */

/* Line 966 of glr.c  */
#line 166 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 966 of glr.c  */
#line 7353 "colorsyntax.cc"
	break;
      case 104: /* "vertxpos" */

/* Line 966 of glr.c  */
#line 161 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 966 of glr.c  */
#line 7362 "colorsyntax.cc"
	break;
      case 106: /* "vertrel_no_xpos" */

/* Line 966 of glr.c  */
#line 162 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 966 of glr.c  */
#line 7371 "colorsyntax.cc"
	break;
      case 107: /* "vertrel" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7380 "colorsyntax.cc"
	break;
      case 108: /* "arcrel" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7389 "colorsyntax.cc"
	break;
      case 109: /* "arcrel_to" */

/* Line 966 of glr.c  */
#line 165 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 966 of glr.c  */
#line 7398 "colorsyntax.cc"
	break;
      case 110: /* "arcrel_from" */

/* Line 966 of glr.c  */
#line 165 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 966 of glr.c  */
#line 7407 "colorsyntax.cc"
	break;
      case 111: /* "arcrel_bidir" */

/* Line 966 of glr.c  */
#line 165 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 966 of glr.c  */
#line 7416 "colorsyntax.cc"
	break;
      case 126: /* "symbol_type_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7425 "colorsyntax.cc"
	break;
      case 127: /* "symbol_command_no_attr" */

/* Line 966 of glr.c  */
#line 164 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7434 "colorsyntax.cc"
	break;
      case 128: /* "symbol_command" */

/* Line 966 of glr.c  */
#line 164 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7443 "colorsyntax.cc"
	break;
      case 129: /* "note" */

/* Line 966 of glr.c  */
#line 164 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7452 "colorsyntax.cc"
	break;
      case 131: /* "comment" */

/* Line 966 of glr.c  */
#line 164 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7461 "colorsyntax.cc"
	break;
      case 132: /* "colon_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7470 "colorsyntax.cc"
	break;
      case 133: /* "full_arcattrlist_with_label" */

/* Line 966 of glr.c  */
#line 170 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 966 of glr.c  */
#line 7479 "colorsyntax.cc"
	break;
      case 134: /* "full_arcattrlist" */

/* Line 966 of glr.c  */
#line 170 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 966 of glr.c  */
#line 7488 "colorsyntax.cc"
	break;
      case 135: /* "arcattrlist" */

/* Line 966 of glr.c  */
#line 170 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 966 of glr.c  */
#line 7497 "colorsyntax.cc"
	break;
      case 136: /* "arcattr" */

/* Line 966 of glr.c  */
#line 170 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 966 of glr.c  */
#line 7506 "colorsyntax.cc"
	break;
      case 137: /* "entity_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7515 "colorsyntax.cc"
	break;
      case 138: /* "reserved_word_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7524 "colorsyntax.cc"
	break;
      case 139: /* "symbol_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7533 "colorsyntax.cc"
	break;
      case 140: /* "string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7542 "colorsyntax.cc"
	break;
      case 141: /* "number_or_string" */

/* Line 966 of glr.c  */
#line 172 "language.yy"
	{free((yyvaluep->str));};

/* Line 966 of glr.c  */
#line 7551 "colorsyntax.cc"
	break;
      case 143: /* "scope_close" */

/* Line 966 of glr.c  */
#line 163 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 966 of glr.c  */
#line 7560 "colorsyntax.cc"
	break;

      default:
	break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
		&yys->yysemantics.yysval, &yys->yyloc, RESULT, yyscanner);
  else
    {
#if YYDEBUG
      if (yydebug)
	{
	  if (yys->yysemantics.yyfirstVal)
	    YYFPRINTF (stderr, "%s unresolved ", yymsg);
	  else
	    YYFPRINTF (stderr, "%s incomplete ", yymsg);
	  yy_symbol_print (stderr, yystos[yys->yylrState],
			   NULL, &yys->yyloc, RESULT, yyscanner);
	  YYFPRINTF (stderr, "\n");
	}
#endif

      if (yys->yysemantics.yyfirstVal)
	{
	  yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
	  yyGLRState *yyrh;
	  int yyn;
	  for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
	       yyn > 0;
	       yyrh = yyrh->yypred, yyn -= 1)
	    yydestroyGLRState (yymsg, yyrh, RESULT, yyscanner);
	}
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(yystate) \
  ((yystate) == (-287))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(yytable_value) \
  YYID (0)

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
		int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

				/* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
		     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
      yynewOption->yyloc = yylloc;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

				/* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = NULL;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = NULL;
  yystackp->yylastDeleted = NULL;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yysize, yynewSize;
  size_t yyn;
  yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
	{
	  yyGLRState* yys0 = &yyp0->yystate;
	  yyGLRState* yys1 = &yyp1->yystate;
	  if (yys0->yypred != NULL)
	    yys1->yypred =
	      YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
	  if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != NULL)
	    yys1->yysemantics.yyfirstVal =
	      YYRELOC(yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
	}
      else
	{
	  yySemanticOption* yyv0 = &yyp0->yyoption;
	  yySemanticOption* yyv1 = &yyp1->yyoption;
	  if (yyv0->yystate != NULL)
	    yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
	  if (yyv0->yynext != NULL)
	    yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
	}
    }
  if (yystackp->yysplitPoint != NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
				 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != NULL)
      yystackp->yytops.yystates[yyn] =
	YYRELOC (yystackp->yyitems, yynewItems,
		 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == NULL)
	{
	  if (yyi == yyj)
	    {
	      YYDPRINTF ((stderr, "Removing dead stacks.\n"));
	    }
	  yystackp->yytops.yysize -= 1;
	}
      else
	{
	  yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
	  /* In the current implementation, it's unnecessary to copy
	     yystackp->yytops.yylookaheadNeeds[yyi] since, after
	     yyremoveDeletes returns, the parser immediately either enters
	     deterministic operation or shifts a token.  However, it doesn't
	     hurt, and the code might evolve to need it.  */
	  yystackp->yytops.yylookaheadNeeds[yyj] =
	    yystackp->yytops.yylookaheadNeeds[yyi];
	  if (yyj != yyi)
	    {
	      YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
			  (unsigned long int) yyi, (unsigned long int) yyj));
	    }
	  yyj += 1;
	}
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
	    size_t yyposn,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yynewState->yyloc = *yylocp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
		 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
	    YYSTYPE* yyvalp, YYLTYPE* yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs,
			   yyvalp, yylocp, yystackp, RESULT, yyscanner);
    }
  else
    {
      /* At present, doAction is never called in nondeterministic
       * mode, so this branch is never taken.  It is here in
       * anticipation of a future feature that will allow immediate
       * evaluation of selected actions in nondeterministic mode.  */
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
	= yystackp->yytops.yystates[yyk];
      if (yynrhs == 0)
	/* Set default location.  */
	yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yys->yyloc;
      for (yyi = 0; yyi < yynrhs; yyi += 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystackp, RESULT, yyscanner);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)		\
do {					\
  if (yydebug)				\
    yy_reduce_print Args;		\
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
		 YYSTYPE* yyvalp, YYLTYPE* yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYUSE (yylocp);
  YYUSE (RESULT);
  YYUSE (yyscanner);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
	     (unsigned long int) yyk, yyrule - 1,
	     (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
		       , &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yyloc)		       , RESULT, yyscanner);
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK.  In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
	     yybool yyforceEval, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval, &yyloc, RESULT, yyscanner));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval,
			 &yyloc, RESULT, yyscanner));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
		  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
				 yylhsNonterm (yyrule)),
		  yyposn, &yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
	   0 < yyn; yyn -= 1)
	{
	  yys = yys->yypred;
	  YYASSERT (yys);
	}
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
		  "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
		  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
	if (yyi != yyk && yystackp->yytops.yystates[yyi] != NULL)
	  {
	    yyGLRState* yyp, *yysplit = yystackp->yysplitPoint;
	    yyp = yystackp->yytops.yystates[yyi];
	    while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
	      {
		if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
		  {
		    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
		    yymarkStackDeleted (yystackp, yyk);
		    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
				(unsigned long int) yyk,
				(unsigned long int) yyi));
		    return yyok;
		  }
		yyp = yyp->yypred;
	      }
	  }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = NULL;

      if (yystackp->yytops.yycapacity
	  > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
	(yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
				  (yystackp->yytops.yycapacity
				   * sizeof yynewStates[0]));
      if (yynewStates == NULL)
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
	(yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
			     (yystackp->yytops.yycapacity
			      * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == NULL)
	yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
	   yyn = yyrhsLength (yyy0->yyrule);
	   yyn > 0;
	   yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
	if (yys0->yyposn != yys1->yyposn)
	  return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
	break;
      else if (yys0->yyresolved)
	{
	  yys1->yyresolved = yytrue;
	  yys1->yysemantics.yysval = yys0->yysemantics.yysval;
	}
      else if (yys1->yyresolved)
	{
	  yys0->yyresolved = yytrue;
	  yys0->yysemantics.yysval = yys1->yysemantics.yysval;
	}
      else
	{
	  yySemanticOption** yyz0p;
	  yySemanticOption* yyz1;
	  yyz0p = &yys0->yysemantics.yyfirstVal;
	  yyz1 = yys1->yysemantics.yyfirstVal;
	  while (YYID (yytrue))
	    {
	      if (yyz1 == *yyz0p || yyz1 == NULL)
		break;
	      else if (*yyz0p == NULL)
		{
		  *yyz0p = yyz1;
		  break;
		}
	      else if (*yyz0p < yyz1)
		{
		  yySemanticOption* yyz = *yyz0p;
		  *yyz0p = yyz1;
		  yyz1 = yyz1->yynext;
		  (*yyz0p)->yynext = yyz;
		}
	      yyz0p = &(*yyz0p)->yynext;
	    }
	  yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
	}
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
	return 0;
      else
	return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
				   yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
		 yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp, RESULT, yyscanner));
      if (! yys->yyresolved)
	YYCHK (yyresolveValue (yys, yystackp, RESULT, yyscanner));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
		 YYSTYPE* yyvalp, YYLTYPE* yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs;
  int yychar_current;
  YYSTYPE yylval_current;
  YYLTYPE yylloc_current;
  YYRESULTTAG yyflag;

  yynrhs = yyrhsLength (yyopt->yyrule);
  yyflag = yyresolveStates (yyopt->yystate, yynrhs, yystackp, RESULT, yyscanner);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
	yydestroyGLRState ("Cleanup: popping", yys, RESULT, yyscanner);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  if (yynrhs == 0)
    /* Set default location.  */
    yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yyopt->yystate->yyloc;
  yychar_current = yychar;
  yylval_current = yylval;
  yylloc_current = yylloc;
  yychar = yyopt->yyrawchar;
  yylval = yyopt->yyval;
  yylloc = yyopt->yyloc;
  yyflag = yyuserAction (yyopt->yyrule, yynrhs,
			   yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
			   yyvalp, yylocp, yystackp, RESULT, yyscanner);
  yychar = yychar_current;
  yylval = yylval_current;
  yylloc = yylloc_current;
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
	       yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
	       yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
	       (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
	{
	  if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
	    YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
	  else
	    YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
		       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
		       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
		       (unsigned long int) yystates[yyi]->yyposn);
	}
      else
	yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
		   yySemanticOption* yyx1, YYLTYPE *yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (yylocp, RESULT, yyscanner, YY_("syntax is ambiguous"));
  return yyabort;
}

/** Starting at and including state S1, resolve the location for each of the
 *  previous N1 states that is unresolved.  The first semantic option of a state
 *  is always chosen.  */
static void
yyresolveLocations (yyGLRState* yys1, int yyn1,
		    yyGLRStack *yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  if (0 < yyn1)
    {
      yyresolveLocations (yys1->yypred, yyn1 - 1, yystackp, RESULT, yyscanner);
      if (!yys1->yyresolved)
	{
	  yySemanticOption *yyoption;
	  yyGLRStackItem yyrhsloc[1 + YYMAXRHS];
	  int yynrhs;
	  int yychar_current;
	  YYSTYPE yylval_current;
	  YYLTYPE yylloc_current;
	  yyoption = yys1->yysemantics.yyfirstVal;
	  YYASSERT (yyoption != NULL);
	  yynrhs = yyrhsLength (yyoption->yyrule);
	  if (yynrhs > 0)
	    {
	      yyGLRState *yys;
	      int yyn;
	      yyresolveLocations (yyoption->yystate, yynrhs,
				  yystackp, RESULT, yyscanner);
	      for (yys = yyoption->yystate, yyn = yynrhs;
		   yyn > 0;
		   yys = yys->yypred, yyn -= 1)
		yyrhsloc[yyn].yystate.yyloc = yys->yyloc;
	    }
	  else
	    {
	      /* Both yyresolveAction and yyresolveLocations traverse the GSS
		 in reverse rightmost order.  It is only necessary to invoke
		 yyresolveLocations on a subforest for which yyresolveAction
		 would have been invoked next had an ambiguity not been
		 detected.  Thus the location of the previous state (but not
		 necessarily the previous state itself) is guaranteed to be
		 resolved already.  */
	      yyGLRState *yyprevious = yyoption->yystate;
	      yyrhsloc[0].yystate.yyloc = yyprevious->yyloc;
	    }
	  yychar_current = yychar;
	  yylval_current = yylval;
	  yylloc_current = yylloc;
	  yychar = yyoption->yyrawchar;
	  yylval = yyoption->yyval;
	  yylloc = yyoption->yyloc;
	  YYLLOC_DEFAULT ((yys1->yyloc), yyrhsloc, yynrhs);
	  yychar = yychar_current;
	  yylval = yylval_current;
	  yylloc = yylloc_current;
	}
    }
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest;
  yySemanticOption** yypp;
  yybool yymerge;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;
  YYLTYPE *yylocp = &yys->yyloc;

  yybest = yyoptionList;
  yymerge = yyfalse;
  for (yypp = &yyoptionList->yynext; *yypp != NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
	{
	  yymergeOptionSets (yybest, yyp);
	  *yypp = yyp->yynext;
	}
      else
	{
	  switch (yypreference (yybest, yyp))
	    {
	    case 0:
	      yyresolveLocations (yys, 1, yystackp, RESULT, yyscanner);
	      return yyreportAmbiguity (yybest, yyp, yylocp, RESULT, yyscanner);
	      break;
	    case 1:
	      yymerge = yytrue;
	      break;
	    case 2:
	      break;
	    case 3:
	      yybest = yyp;
	      yymerge = yyfalse;
	      break;
	    default:
	      /* This cannot happen so it is not worth a YYASSERT (yyfalse),
		 but some compilers complain if the default case is
		 omitted.  */
	      break;
	    }
	  yypp = &yyp->yynext;
	}
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval,
				yylocp, RESULT, yyscanner);
      if (yyflag == yyok)
	for (yyp = yybest->yynext; yyp != NULL; yyp = yyp->yynext)
	  {
	    if (yyprec == yydprec[yyp->yyrule])
	      {
		YYSTYPE yysval_other;
		YYLTYPE yydummy;
		yyflag = yyresolveAction (yyp, yystackp, &yysval_other,
					  &yydummy, RESULT, yyscanner);
		if (yyflag != yyok)
		  {
		    yydestruct ("Cleanup: discarding incompletely merged value for",
				yystos[yys->yylrState],
				&yysval, yylocp, RESULT, yyscanner);
		    break;
		  }
		yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
	      }
	  }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp, RESULT, yyscanner);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  if (yystackp->yysplitPoint != NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
	   yys != yystackp->yysplitPoint;
	   yys = yys->yypred, yyn += 1)
	continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
			     , RESULT, yyscanner));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = NULL;
  yystackp->yylastDeleted = NULL;

  while (yyr != NULL)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
		   size_t yyposn, YYLTYPE *yylocp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != NULL)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
		  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
	{
	  yyrule = yydefaultAction (yystate);
	  if (yyrule == 0)
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystackp, yyk);
	      return yyok;
	    }
	  YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse, RESULT, yyscanner));
	}
      else
	{
	  yySymbol yytoken;
	  yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
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

	  yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

	  while (*yyconflicts != 0)
	    {
	      size_t yynewStack = yysplitStack (yystackp, yyk);
	      YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
			  (unsigned long int) yynewStack,
			  (unsigned long int) yyk));
	      YYCHK (yyglrReduce (yystackp, yynewStack,
				  *yyconflicts, yyfalse, RESULT, yyscanner));
	      YYCHK (yyprocessOneStack (yystackp, yynewStack,
					yyposn, yylocp, RESULT, yyscanner));
	      yyconflicts += 1;
	    }

	  if (yyisShiftAction (yyaction))
	    break;
	  else if (yyisErrorAction (yyaction))
	    {
	      YYDPRINTF ((stderr, "Stack %lu dies.\n",
			  (unsigned long int) yyk));
	      yymarkStackDeleted (yystackp, yyk);
	      break;
	    }
	  else
	    YYCHK (yyglrReduce (yystackp, yyk, -yyaction,
				yyfalse, RESULT, yyscanner));
	}
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error"));
#else
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (NULL, yytokenName (yytoken));
  size_t yysize = yysize0;
  size_t yysize1;
  yybool yysize_overflow = yyfalse;
  char* yymsg = NULL;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
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
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
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
                yyarg[yycount++] = yytokenName (yyx);
                yysize1 = yysize + yytnamerr (NULL, yytokenName (yyx));
                yysize_overflow |= yysize1 < yysize;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  yysize1 = yysize + strlen (yyformat);
  yysize_overflow |= yysize1 < yysize;
  yysize = yysize1;

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
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
      yyerror (&yylloc, RESULT, yyscanner, yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (&yylloc, RESULT, yyscanner, YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp, YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID (yytrue))
      {
	yySymbol yytoken;
	if (yychar == YYEOF)
	  yyFail (yystackp, &yylloc, RESULT, yyscanner, NULL);
	if (yychar != YYEMPTY)
	  {
	    /* We throw away the lookahead, but the error range
	       of the shifted error token must take it into account.  */
	    yyGLRState *yys = yystackp->yytops.yystates[0];
	    yyGLRStackItem yyerror_range[3];
	    yyerror_range[1].yystate.yyloc = yys->yyloc;
	    yyerror_range[2].yystate.yyloc = yylloc;
	    YYLLOC_DEFAULT ((yys->yyloc), yyerror_range, 2);
	    yytoken = YYTRANSLATE (yychar);
	    yydestruct ("Error: discarding",
			yytoken, &yylval, &yylloc, RESULT, yyscanner);
	  }
	YYDPRINTF ((stderr, "Reading a token: "));
	yychar = YYLEX;
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
	yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
	if (yypact_value_is_default (yyj))
	  return;
	yyj += yytoken;
	if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
	  {
	    if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
	      return;
	  }
	else if (! yytable_value_is_error (yytable[yyj]))
	  return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, &yylloc, RESULT, yyscanner, NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
	{
	  yyj += YYTERROR;
	  if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
	      && yyisShiftAction (yytable[yyj]))
	    {
	      /* Shift the error token having adjusted its location.  */
	      YYLTYPE yyerrloc;
	      yystackp->yyerror_range[2].yystate.yyloc = yylloc;
	      YYLLOC_DEFAULT (yyerrloc, (yystackp->yyerror_range), 2);
	      YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
			       &yylval, &yyerrloc);
	      yyglrShift (yystackp, 0, yytable[yyj],
			  yys->yyposn, &yylval, &yyerrloc);
	      yys = yystackp->yytops.yystates[0];
	      break;
	    }
	}
      yystackp->yyerror_range[1].yystate.yyloc = yys->yyloc;
      if (yys->yypred != NULL)
	yydestroyGLRState ("Error: popping", yys, RESULT, yyscanner);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == NULL)
    yyFail (yystackp, &yylloc, RESULT, yyscanner, NULL);
}

#define YYCHK1(YYE)							     \
  do {									     \
    switch (YYE) {							     \
    case yyok:								     \
      break;								     \
    case yyabort:							     \
      goto yyabortlab;							     \
    case yyaccept:							     \
      goto yyacceptlab;							     \
    case yyerr:								     \
      goto yyuser_error;						     \
    default:								     \
      goto yybuglab;							     \
    }									     \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (YYMSC_RESULT_TYPE &RESULT, void *yyscanner)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif


  /* User initialization code.  */
  
/* Line 2356 of glr.c  */
#line 8 "language.yy"
{
  #ifdef C_S_H_IS_COMPILED
    yylloc.first_pos = 0;
    yylloc.last_pos = 0;
  #endif
}

/* Line 2356 of glr.c  */
#line 8970 "colorsyntax.cc"

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval, &yylloc);
  yyposn = 0;

  while (YYID (yytrue))
    {
      /* For efficiency, we have two loops, the first of which is
	 specialized to deterministic operation (single stack, no
	 potential ambiguity).  */
      /* Standard mode */
      while (YYID (yytrue))
	{
	  yyRuleNum yyrule;
	  int yyaction;
	  const short int* yyconflicts;

	  yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
	  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
	  if (yystate == YYFINAL)
	    goto yyacceptlab;
	  if (yyisDefaultedState (yystate))
	    {
	      yyrule = yydefaultAction (yystate);
	      if (yyrule == 0)
		{
		  yystack.yyerror_range[1].yystate.yyloc = yylloc;
		  yyreportSyntaxError (&yystack, RESULT, yyscanner);
		  goto yyuser_error;
		}
	      YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue, RESULT, yyscanner));
	    }
	  else
	    {
	      yySymbol yytoken;
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

	      yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
	      if (*yyconflicts != 0)
		break;
	      if (yyisShiftAction (yyaction))
		{
		  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
		  yychar = YYEMPTY;
		  yyposn += 1;
		  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval, &yylloc);
		  if (0 < yystack.yyerrState)
		    yystack.yyerrState -= 1;
		}
	      else if (yyisErrorAction (yyaction))
		{
		  yystack.yyerror_range[1].yystate.yyloc = yylloc;
		  yyreportSyntaxError (&yystack, RESULT, yyscanner);
		  goto yyuser_error;
		}
	      else
		YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue, RESULT, yyscanner));
	    }
	}

      while (YYID (yytrue))
	{
	  yySymbol yytoken_to_shift;
	  size_t yys;

	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

	  /* yyprocessOneStack returns one of three things:

	      - An error flag.  If the caller is yyprocessOneStack, it
		immediately returns as well.  When the caller is finally
		yyparse, it jumps to an error label via YYCHK1.

	      - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
		(&yystack, yys), which sets the top state of yys to NULL.  Thus,
		yyparse's following invocation of yyremoveDeletes will remove
		the stack.

	      - yyok, when ready to shift a token.

	     Except in the first case, yyparse will invoke yyremoveDeletes and
	     then shift the next token onto all remaining stacks.  This
	     synchronization of the shift (that is, after all preceding
	     reductions on all stacks) helps prevent double destructor calls
	     on yylval in the event of memory exhaustion.  */

	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn, &yylloc, RESULT, yyscanner));
	  yyremoveDeletes (&yystack);
	  if (yystack.yytops.yysize == 0)
	    {
	      yyundeleteLastStack (&yystack);
	      if (yystack.yytops.yysize == 0)
		yyFail (&yystack, &yylloc, RESULT, yyscanner, YY_("syntax error"));
	      YYCHK1 (yyresolveStack (&yystack, RESULT, yyscanner));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yystack.yyerror_range[1].yystate.yyloc = yylloc;
	      yyreportSyntaxError (&yystack, RESULT, yyscanner);
	      goto yyuser_error;
	    }

	  /* If any yyglrShift call fails, it will fail after shifting.  Thus,
	     a copy of yylval will already be on stack 0 in the event of a
	     failure in the following loop.  Thus, yychar is set to YYEMPTY
	     before the loop to make sure the user destructor for yylval isn't
	     called twice.  */
	  yytoken_to_shift = YYTRANSLATE (yychar);
	  yychar = YYEMPTY;
	  yyposn += 1;
	  for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
	    {
	      int yyaction;
	      const short int* yyconflicts;
	      yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
	      yygetLRActions (yystate, yytoken_to_shift, &yyaction,
			      &yyconflicts);
	      /* Note that yyconflicts were handled by yyprocessOneStack.  */
	      YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
	      YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
	      yyglrShift (&yystack, yys, yyaction, yyposn,
			  &yylval, &yylloc);
	      YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
			  (unsigned long int) yys,
			  yystack.yytops.yystates[yys]->yylrState));
	    }

	  if (yystack.yytops.yysize == 1)
	    {
	      YYCHK1 (yyresolveStack (&yystack, RESULT, yyscanner));
	      YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
	      yycompressStack (&yystack);
	      break;
	    }
	}
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, RESULT, yyscanner);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (&yylloc, RESULT, yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
		YYTRANSLATE (yychar),
		&yylval, &yylloc, RESULT, yyscanner);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
	{
	  size_t yysize = yystack.yytops.yysize;
	  size_t yyk;
	  for (yyk = 0; yyk < yysize; yyk += 1)
	    if (yystates[yyk])
	      {
		while (yystates[yyk])
		  {
		    yyGLRState *yys = yystates[yyk];
		    yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
		    if (yys->yypred != NULL)
		      yydestroyGLRState ("Cleanup: popping", yys, RESULT, yyscanner);
		    yystates[yyk] = yys->yypred;
		    yystack.yynextFree -= 1;
		    yystack.yyspaceLeft += 1;
		  }
		break;
	      }
	}
      yyfreeGLRStack (&yystack);
    }

  /* Make sure YYID is used.  */
  return YYID (yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack (yyGLRStack* yystackp, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystackp) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == NULL)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)							     \
    ((YYX) == NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
	{
	  YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
		     yyp->yystate.yyresolved, yyp->yystate.yylrState,
		     (unsigned long int) yyp->yystate.yyposn,
		     (long int) YYINDEX (yyp->yystate.yypred));
	  if (! yyp->yystate.yyresolved)
	    YYFPRINTF (stderr, ", firstVal: %ld",
		       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
	}
      else
	{
	  YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
		     yyp->yyoption.yyrule - 1,
		     (long int) YYINDEX (yyp->yyoption.yystate),
		     (long int) YYINDEX (yyp->yyoption.yynext));
	}
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
	       (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif



/* Line 2659 of glr.c  */
#line 3804 "language.yy"



/* END OF FILE */

