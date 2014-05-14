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
#line 14 "language.yy"

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


/* Line 268 of yacc.c  */
#line 130 "language.cc"

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 95 "language.yy"

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
    CHAR_IF_CSH(EntityApp)        *entity;
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



/* Line 293 of yacc.c  */
#line 341 "language.cc"
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
#line 180 "language.yy"

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
        if (YYID (N)) {                                     \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;\
            (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos; \
        } else {                                            \
            (Current).first_pos = (Current).last_pos =      \
                YYRHSLOC (Rhs, 0).last_pos;                 \
        }                                                   \
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



/* Line 343 of yacc.c  */
#line 429 "language.cc"

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
#define YYFINAL  213
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1767

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  418
/* YYNRULES -- Number of states.  */
#define YYNSTATES  464

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    13,    14,    17,    20,
      24,    27,    31,    33,    36,    40,    43,    47,    51,    54,
      59,    64,    68,    71,    75,    78,    80,    83,    85,    87,
      90,    93,    97,   102,   104,   108,   110,   112,   114,   117,
     119,   121,   124,   126,   129,   133,   135,   138,   142,   145,
     149,   154,   158,   160,   162,   164,   167,   170,   174,   178,
     183,   185,   187,   189,   191,   194,   197,   199,   202,   204,
     207,   209,   211,   214,   216,   219,   222,   225,   229,   231,
     234,   236,   238,   240,   244,   247,   249,   252,   254,   256,
     258,   260,   263,   265,   267,   269,   271,   274,   277,   280,
     282,   286,   288,   291,   294,   296,   300,   302,   304,   306,
     308,   310,   314,   317,   321,   325,   329,   333,   337,   340,
     344,   347,   351,   354,   356,   360,   363,   367,   370,   372,
     376,   379,   382,   384,   388,   391,   393,   397,   400,   402,
     404,   406,   409,   413,   417,   419,   422,   426,   431,   437,
     439,   442,   445,   448,   450,   453,   457,   462,   468,   475,
     483,   485,   489,   492,   494,   496,   500,   505,   508,   510,
     516,   523,   525,   527,   531,   534,   536,   539,   541,   543,
     545,   549,   552,   556,   560,   564,   568,   572,   574,   577,
     579,   581,   584,   587,   591,   595,   598,   603,   607,   609,
     612,   615,   619,   622,   625,   627,   631,   633,   636,   640,
     642,   645,   647,   649,   653,   656,   659,   661,   664,   668,
     672,   677,   681,   686,   690,   695,   701,   703,   705,   708,
     711,   715,   717,   719,   721,   725,   728,   731,   733,   735,
     739,   744,   747,   749,   751,   753,   757,   760,   762,   764,
     766,   768,   772,   775,   778,   782,   785,   789,   792,   795,
     799,   802,   806,   809,   812,   816,   819,   821,   823,   825,
     827,   829,   831,   833,   835,   837,   839,   841,   843,   845,
     847,   849,   851,   853,   855,   857,   860,   863,   867,   869,
     872,   875,   879,   881,   884,   887,   891,   893,   896,   899,
     903,   905,   908,   911,   915,   917,   920,   923,   927,   929,
     931,   934,   936,   938,   940,   945,   951,   955,   960,   964,
     968,   971,   973,   975,   978,   983,   986,   989,   993,   995,
     997,   999,  1002,  1004,  1006,  1008,  1010,  1013,  1017,  1020,
    1022,  1025,  1029,  1034,  1038,  1041,  1045,  1047,  1050,  1053,
    1055,  1059,  1062,  1066,  1070,  1075,  1079,  1083,  1087,  1090,
    1094,  1096,  1098,  1100,  1102,  1104,  1106,  1108,  1110,  1112,
    1114,  1116,  1118,  1120,  1122,  1124,  1126,  1128,  1130,  1132,
    1134,  1136,  1138,  1140,  1142,  1144,  1146,  1148,  1150,  1152,
    1154,  1156,  1158,  1160,  1162,  1164,  1166,  1168,  1170,  1172,
    1174,  1176,  1178,  1180,  1182,  1184,  1186,  1188,  1190,  1192,
    1194,  1196,  1198,  1200,  1202,  1204,  1206,  1208,  1211
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      73,     0,    -1,    75,    74,    -1,     0,    -1,    60,    -1,
      60,     9,    -1,    -1,    17,    78,    -1,    77,    78,    -1,
      17,     1,    74,    -1,    76,    74,    -1,    76,     1,    74,
      -1,    79,    -1,    79,    13,    -1,    79,    13,    76,    -1,
      17,     7,    -1,    17,     7,   158,    -1,   159,    79,   160,
      -1,   159,   160,    -1,   159,    79,     1,   160,    -1,   159,
      79,     1,     0,    -1,   159,    79,     0,    -1,   159,     0,
      -1,   159,    79,    60,    -1,   159,    60,    -1,    80,    -1,
      80,    83,    -1,    83,    -1,    81,    -1,    80,    81,    -1,
      83,     9,    -1,    83,     9,    71,    -1,    83,     1,     9,
      71,    -1,     9,    -1,    83,     1,     9,    -1,    49,    -1,
      50,    -1,    84,    -1,    82,    84,    -1,    82,    -1,   123,
      -1,   123,   149,    -1,    45,    -1,    45,   123,    -1,    45,
     123,   149,    -1,    51,    -1,    51,   122,    -1,    51,   153,
     122,    -1,    51,   153,    -1,    51,   122,   149,    -1,    51,
     153,   122,   149,    -1,    51,   153,   149,    -1,   149,    -1,
      95,    -1,    90,    -1,    90,    95,    -1,    95,     8,    -1,
      95,     8,    93,    -1,    90,    95,     8,    -1,    90,    95,
       8,    93,    -1,    91,    -1,   112,    -1,   116,    -1,   111,
      -1,    41,    99,    -1,    42,   102,    -1,    42,    -1,    43,
      96,    -1,    43,    -1,    44,   105,    -1,    44,    -1,    38,
      -1,    38,   150,    -1,    39,    -1,    39,   150,    -1,    85,
     149,    -1,    48,   158,    -1,    48,   158,   150,    -1,    40,
      -1,    40,   150,    -1,   144,    -1,   145,    -1,   147,    -1,
      62,    86,    87,    -1,    62,    86,    -1,    62,    -1,    61,
      87,    -1,    61,    -1,    68,    -1,    69,    -1,    88,    -1,
      55,    65,    -1,    55,    -1,    65,    -1,   149,    -1,     5,
      -1,     5,   149,    -1,   154,     6,    -1,     6,   154,    -1,
       6,    -1,   154,     6,   154,    -1,   154,    -1,   154,     6,
      -1,     6,   154,    -1,     6,    -1,   154,     6,   154,    -1,
      57,    -1,    56,    -1,    58,    -1,    59,    -1,    92,    -1,
      91,     8,    92,    -1,    91,     8,    -1,    91,     8,    71,
      -1,    91,     8,     1,    -1,   154,     7,    22,    -1,   154,
       7,     5,    -1,   154,     7,   158,    -1,   154,     7,    -1,
      17,     7,   158,    -1,    17,     7,    -1,    17,    11,   158,
      -1,    17,    11,    -1,    94,    -1,    93,     8,    94,    -1,
      93,     8,    -1,    93,     8,    71,    -1,   154,   149,    -1,
     154,    -1,   154,   149,    78,    -1,   154,    78,    -1,   154,
     149,    -1,   154,    -1,   154,   149,    78,    -1,   154,    78,
      -1,    97,    -1,    96,     8,    97,    -1,    98,   150,    -1,
      98,    -1,   158,    -1,    37,    -1,    98,     8,    -1,    98,
       8,   158,    -1,    98,     8,    37,    -1,   154,    -1,   154,
      12,    -1,   154,    12,   100,    -1,   154,    12,   100,    13,
      -1,   154,    12,   100,     1,    13,    -1,   101,    -1,     1,
     101,    -1,   100,   101,    -1,   100,     1,    -1,    70,    -1,
      70,     5,    -1,    70,     5,     5,    -1,    70,     5,     5,
       5,    -1,    70,     5,     5,     5,     5,    -1,    70,     5,
       5,     5,     5,     5,    -1,    70,     5,     5,     5,     5,
       5,     5,    -1,   104,    -1,   102,     8,   104,    -1,   102,
       8,    -1,    21,    -1,   158,    -1,   157,     7,   103,    -1,
     157,     7,    37,   103,    -1,   157,     7,    -1,   157,    -1,
       3,   106,   107,     9,    13,    -1,     3,   106,   107,     9,
       1,    13,    -1,    12,    -1,   108,    -1,   107,     9,   108,
      -1,    42,   102,    -1,    42,    -1,    43,    96,    -1,    43,
      -1,   109,    -1,   110,    -1,   109,     8,   110,    -1,   109,
       1,    -1,   154,     7,    22,    -1,   154,     7,     5,    -1,
     154,     7,   158,    -1,    17,     7,   158,    -1,    17,    11,
     158,    -1,    78,    -1,   111,    78,    -1,   113,    -1,    46,
      -1,    46,   113,    -1,   112,   118,    -1,   112,   118,   149,
      -1,   112,   118,    78,    -1,   112,    78,    -1,   112,   118,
     149,    78,    -1,   112,   149,    78,    -1,   118,    -1,   118,
     149,    -1,   118,    78,    -1,   118,   149,    78,    -1,    47,
     118,    -1,    47,     1,    -1,    47,    -1,    47,   118,   149,
      -1,   114,    -1,   115,   118,    -1,   115,   118,   149,    -1,
     115,    -1,   115,    78,    -1,    37,    -1,    36,    -1,   154,
     117,   154,    -1,   117,   154,    -1,   154,   117,    -1,   117,
      -1,    53,   154,    -1,    53,   154,     5,    -1,    53,   154,
       6,    -1,    53,   154,     6,     5,    -1,    53,   154,    10,
      -1,    53,   154,    10,     5,    -1,    53,   154,   117,    -1,
      53,   154,     6,   154,    -1,    53,   154,     6,   154,     5,
      -1,    53,    -1,   117,    -1,    16,   117,    -1,   117,    16,
      -1,    16,   117,    16,    -1,   135,    -1,   134,    -1,   136,
      -1,   154,   120,   154,    -1,   120,   154,    -1,   154,   120,
      -1,   120,    -1,   154,    -1,   120,   154,    71,    -1,   154,
     120,   154,    71,    -1,   121,   119,    -1,   121,    -1,   119,
      -1,    35,    -1,   124,    54,   119,    -1,   124,    54,    -1,
     124,    -1,   125,    -1,   126,    -1,   127,    -1,   154,   134,
     154,    -1,   134,   154,    -1,   154,   134,    -1,   125,   137,
     154,    -1,   125,   137,    -1,   154,   135,   154,    -1,   135,
     154,    -1,   154,   135,    -1,   126,   138,   154,    -1,   126,
     138,    -1,   154,   136,   154,    -1,   136,   154,    -1,   154,
     136,    -1,   127,   139,   154,    -1,   127,   139,    -1,    23,
      -1,    26,    -1,    29,    -1,    32,    -1,    24,    -1,    27,
      -1,    30,    -1,    33,    -1,    25,    -1,    28,    -1,    31,
      -1,    34,    -1,   128,    -1,     6,    -1,   129,    -1,     6,
      -1,   130,    -1,     6,    -1,   128,    -1,    16,   128,    -1,
     128,    16,    -1,    16,   128,    16,    -1,   129,    -1,    16,
     129,    -1,   129,    16,    -1,    16,   129,    16,    -1,   130,
      -1,    16,   130,    -1,   130,    16,    -1,    16,   130,    16,
      -1,   131,    -1,    16,   131,    -1,   131,    16,    -1,    16,
     131,    16,    -1,   132,    -1,    16,   132,    -1,   132,    16,
      -1,    16,   132,    16,    -1,   133,    -1,    16,   133,    -1,
     133,    16,    -1,    16,   133,    16,    -1,   141,    -1,   154,
      -1,    55,   119,    -1,    55,    -1,   154,    -1,   156,    -1,
      63,   142,    89,   140,    -1,    63,   142,    89,   140,   140,
      -1,    63,   142,   141,    -1,    63,   142,   141,   140,    -1,
      63,   142,    89,    -1,    63,   142,   154,    -1,    63,   142,
      -1,    63,    -1,   143,    -1,   143,   149,    -1,    64,    53,
     158,   149,    -1,    64,   149,    -1,    64,    53,    -1,    64,
      53,   149,    -1,    65,    -1,    66,    -1,    67,    -1,   146,
     149,    -1,   146,    -1,    19,    -1,    18,    -1,   148,    -1,
     148,   150,    -1,   150,   148,   150,    -1,   150,   148,    -1,
     150,    -1,    14,    15,    -1,    14,   151,    15,    -1,    14,
     151,     1,    15,    -1,    14,     1,    15,    -1,    14,   151,
      -1,    14,   151,     1,    -1,    14,    -1,    14,    71,    -1,
      14,     1,    -1,   152,    -1,   151,     8,   152,    -1,   151,
       8,    -1,   151,     8,    71,    -1,   157,     7,   103,    -1,
     157,     7,    37,   103,    -1,   157,     7,    37,    -1,   157,
       7,     5,    -1,   157,     7,    22,    -1,   157,     7,    -1,
     158,     7,    71,    -1,   158,    -1,    52,    -1,    46,    -1,
       4,    -1,     3,    -1,    70,    -1,    17,    -1,    42,    -1,
      43,    -1,    44,    -1,    40,    -1,    45,    -1,    46,    -1,
      47,    -1,    51,    -1,    49,    -1,    50,    -1,    38,    -1,
      39,    -1,    48,    -1,    53,    -1,    55,    -1,    56,    -1,
      57,    -1,    58,    -1,    59,    -1,    60,    -1,    62,    -1,
      61,    -1,    63,    -1,    64,    -1,    65,    -1,    66,    -1,
      67,    -1,    68,    -1,    69,    -1,    52,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,
      34,    -1,    35,    -1,    36,    -1,   154,    -1,   155,    -1,
     157,    -1,   156,    -1,    20,    -1,    12,    -1,    12,    71,
      -1,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   244,   244,   249,   250,   259,   271,   281,   290,   299,
     310,   318,   332,   333,   342,   356,   368,   386,   394,   405,
     416,   427,   438,   448,   460,   474,   475,   486,   501,   510,
     518,   532,   533,   534,   546,   562,   562,   564,   573,   598,
     617,   625,   637,   649,   664,   682,   699,   712,   726,   745,
     762,   780,   803,   815,   823,   835,   847,   860,   872,   888,
     902,   916,   923,   930,   939,   948,   957,   972,   981,   996,
    1005,  1016,  1026,  1040,  1050,  1064,  1081,  1094,  1111,  1121,
    1135,  1136,  1137,  1138,  1152,  1167,  1181,  1195,  1211,  1211,
    1213,  1214,  1240,  1258,  1275,  1276,  1287,  1298,  1310,  1321,
    1330,  1344,  1369,  1381,  1392,  1402,  1417,  1417,  1417,  1417,
    1419,  1429,  1448,  1461,  1462,  1473,  1492,  1511,  1530,  1550,
    1569,  1589,  1608,  1629,  1635,  1645,  1657,  1660,  1676,  1688,
    1704,  1717,  1735,  1750,  1768,  1784,  1785,  1792,  1840,  1851,
    1871,  1886,  1901,  1922,  1939,  1949,  1960,  1977,  1992,  2009,
    2019,  2031,  2040,  2048,  2064,  2092,  2116,  2142,  2170,  2200,
    2233,  2234,  2244,  2258,  2258,  2260,  2282,  2305,  2323,  2340,
    2356,  2375,  2386,  2387,  2394,  2401,  2415,  2426,  2440,  2442,
    2443,  2449,  2456,  2475,  2494,  2513,  2535,  2559,  2568,  2580,
    2587,  2598,  2609,  2617,  2630,  2638,  2647,  2660,  2676,  2684,
    2697,  2705,  2720,  2735,  2751,  2762,  2781,  2791,  2805,  2825,
    2826,  2833,  2833,  2835,  2849,  2860,  2872,  2882,  2894,  2908,
    2922,  2937,  2950,  2965,  2999,  3015,  3033,  3047,  3058,  3070,
    3082,  3096,  3105,  3114,  3124,  3137,  3147,  3158,  3166,  3179,
    3180,  3183,  3194,  3202,  3221,  3229,  3238,  3253,  3259,  3259,
    3259,  3261,  3274,  3284,  3295,  3305,  3315,  3328,  3338,  3349,
    3359,  3368,  3381,  3391,  3402,  3412,  3421,  3421,  3421,  3421,
    3422,  3422,  3422,  3422,  3423,  3423,  3423,  3423,  3425,  3425,
    3426,  3426,  3427,  3427,  3429,  3439,  3450,  3461,  3475,  3485,
    3496,  3507,  3521,  3531,  3542,  3553,  3567,  3576,  3590,  3604,
    3621,  3630,  3644,  3658,  3675,  3684,  3698,  3712,  3730,  3731,
    3754,  3764,  3778,  3778,  3780,  3816,  3839,  3875,  3899,  3935,
    3972,  4008,  4025,  4032,  4045,  4063,  4077,  4092,  4112,  4119,
    4126,  4134,  4147,  4159,  4166,  4175,  4183,  4191,  4203,  4211,
    4214,  4224,  4234,  4245,  4256,  4267,  4278,  4291,  4295,  4307,
    4314,  4323,  4333,  4335,  4350,  4365,  4380,  4394,  4408,  4422,
    4427,  4439,  4453,  4463,  4470,  4471,  4478,  4478,  4479,  4479,
    4480,  4480,  4480,  4481,  4482,  4482,  4482,  4483,  4483,  4484,
    4484,  4484,  4484,  4484,  4485,  4485,  4485,  4486,  4486,  4486,
    4487,  4487,  4487,  4488,  4488,  4488,  4489,  4491,  4492,  4493,
    4494,  4495,  4496,  4497,  4498,  4499,  4500,  4501,  4502,  4503,
    4511,  4522,  4522,  4524,  4524,  4524,  4526,  4541,  4543
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
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
  "symbol_string", "alpha_string", "string", "scope_open", "scope_close", 0
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
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326
};
# endif

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

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
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

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
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

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -352
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

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -347
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

#define yypact_value_is_default(yystate) \
  ((yystate) == (-352))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

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
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2222 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2231 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2240 "language.cc"
	break;
      case 17: /* "TOK_MSC" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2249 "language.cc"
	break;
      case 18: /* "TOK_COLON_STRING" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2258 "language.cc"
	break;
      case 19: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2267 "language.cc"
	break;
      case 20: /* "TOK_STYLE_NAME" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2276 "language.cc"
	break;
      case 21: /* "TOK_COLORDEF" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2285 "language.cc"
	break;
      case 22: /* "TOK_BOOLEAN" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2294 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_HEADING" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2303 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_NUDGE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2312 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2321 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_DEFSHAPE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2330 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2339 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2348 "language.cc"
	break;
      case 44: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2357 "language.cc"
	break;
      case 45: /* "TOK_COMMAND_BIG" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2366 "language.cc"
	break;
      case 46: /* "TOK_COMMAND_BOX" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2375 "language.cc"
	break;
      case 47: /* "TOK_COMMAND_PIPE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2384 "language.cc"
	break;
      case 48: /* "TOK_COMMAND_MARK" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2393 "language.cc"
	break;
      case 49: /* "TOK_COMMAND_PARALLEL" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2402 "language.cc"
	break;
      case 50: /* "TOK_COMMAND_OVERLAP" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2411 "language.cc"
	break;
      case 51: /* "TOK_VERTICAL" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2420 "language.cc"
	break;
      case 52: /* "TOK_VERTICAL_SHAPE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2429 "language.cc"
	break;
      case 53: /* "TOK_AT" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2438 "language.cc"
	break;
      case 54: /* "TOK_LOST" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2447 "language.cc"
	break;
      case 55: /* "TOK_AT_POS" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2456 "language.cc"
	break;
      case 56: /* "TOK_SHOW" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2465 "language.cc"
	break;
      case 57: /* "TOK_HIDE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2474 "language.cc"
	break;
      case 58: /* "TOK_ACTIVATE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2483 "language.cc"
	break;
      case 59: /* "TOK_DEACTIVATE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2492 "language.cc"
	break;
      case 60: /* "TOK_BYE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2501 "language.cc"
	break;
      case 61: /* "TOK_COMMAND_VSPACE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2510 "language.cc"
	break;
      case 62: /* "TOK_COMMAND_HSPACE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2519 "language.cc"
	break;
      case 63: /* "TOK_COMMAND_SYMBOL" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2528 "language.cc"
	break;
      case 64: /* "TOK_COMMAND_NOTE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2537 "language.cc"
	break;
      case 65: /* "TOK_COMMAND_COMMENT" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2546 "language.cc"
	break;
      case 66: /* "TOK_COMMAND_ENDNOTE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2555 "language.cc"
	break;
      case 67: /* "TOK_COMMAND_FOOTNOTE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2564 "language.cc"
	break;
      case 68: /* "TOK_COMMAND_TITLE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2573 "language.cc"
	break;
      case 69: /* "TOK_COMMAND_SUBTITLE" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2582 "language.cc"
	break;
      case 76: /* "top_level_arclist" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2591 "language.cc"
	break;
      case 78: /* "braced_arclist" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2600 "language.cc"
	break;
      case 79: /* "arclist_maybe_no_semicolon" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2609 "language.cc"
	break;
      case 80: /* "arclist" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2618 "language.cc"
	break;
      case 81: /* "arc_with_parallel_semicolon" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2627 "language.cc"
	break;
      case 82: /* "overlap_or_parallel" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2636 "language.cc"
	break;
      case 83: /* "arc_with_parallel" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2645 "language.cc"
	break;
      case 84: /* "arc" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2654 "language.cc"
	break;
      case 85: /* "titlecommandtoken" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2663 "language.cc"
	break;
      case 86: /* "hspace_location" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2672 "language.cc"
	break;
      case 87: /* "full_arcattrlist_with_label_or_number" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2681 "language.cc"
	break;
      case 88: /* "entityrel" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2690 "language.cc"
	break;
      case 89: /* "markerrel_no_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2699 "language.cc"
	break;
      case 90: /* "entity_command_prefixes" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2708 "language.cc"
	break;
      case 91: /* "optlist" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2717 "language.cc"
	break;
      case 92: /* "opt" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2726 "language.cc"
	break;
      case 93: /* "entitylist" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2735 "language.cc"
	break;
      case 94: /* "entity" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2744 "language.cc"
	break;
      case 95: /* "first_entity" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2753 "language.cc"
	break;
      case 98: /* "stylenamelist" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1391 of yacc.c  */
#line 2762 "language.cc"
	break;
      case 103: /* "color_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2771 "language.cc"
	break;
      case 111: /* "parallel" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1391 of yacc.c  */
#line 2780 "language.cc"
	break;
      case 112: /* "box_list" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1391 of yacc.c  */
#line 2789 "language.cc"
	break;
      case 113: /* "first_box" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2798 "language.cc"
	break;
      case 114: /* "first_pipe" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1391 of yacc.c  */
#line 2807 "language.cc"
	break;
      case 115: /* "pipe_list_no_content" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2816 "language.cc"
	break;
      case 116: /* "pipe_list" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2825 "language.cc"
	break;
      case 118: /* "boxrel" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2834 "language.cc"
	break;
      case 119: /* "vertxpos" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1391 of yacc.c  */
#line 2843 "language.cc"
	break;
      case 121: /* "vertrel_no_xpos" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2852 "language.cc"
	break;
      case 122: /* "vertrel" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2861 "language.cc"
	break;
      case 123: /* "arcrel" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2870 "language.cc"
	break;
      case 124: /* "arcrel_arrow" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2879 "language.cc"
	break;
      case 125: /* "arcrel_to" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2888 "language.cc"
	break;
      case 126: /* "arcrel_from" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2897 "language.cc"
	break;
      case 127: /* "arcrel_bidir" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2906 "language.cc"
	break;
      case 140: /* "extvertxpos" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->extvertxpos);};

/* Line 1391 of yacc.c  */
#line 2915 "language.cc"
	break;
      case 141: /* "extvertxpos_no_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{if (!C_S_H) delete (yyvaluep->extvertxpos);};

/* Line 1391 of yacc.c  */
#line 2924 "language.cc"
	break;
      case 142: /* "symbol_type_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2933 "language.cc"
	break;
      case 143: /* "symbol_command_no_attr" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2942 "language.cc"
	break;
      case 144: /* "symbol_command" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2951 "language.cc"
	break;
      case 145: /* "note" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2960 "language.cc"
	break;
      case 147: /* "comment" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2969 "language.cc"
	break;
      case 148: /* "colon_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2978 "language.cc"
	break;
      case 149: /* "full_arcattrlist_with_label" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2987 "language.cc"
	break;
      case 150: /* "full_arcattrlist" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2996 "language.cc"
	break;
      case 151: /* "arcattrlist" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 3005 "language.cc"
	break;
      case 152: /* "arcattr" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1391 of yacc.c  */
#line 3014 "language.cc"
	break;
      case 154: /* "entity_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3023 "language.cc"
	break;
      case 155: /* "reserved_word_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3032 "language.cc"
	break;
      case 156: /* "symbol_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3041 "language.cc"
	break;
      case 157: /* "alpha_string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3050 "language.cc"
	break;
      case 158: /* "string" */

/* Line 1391 of yacc.c  */
#line 178 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3059 "language.cc"
	break;
      case 160: /* "scope_close" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 3068 "language.cc"
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

/* User initialization code.  */

/* Line 1590 of yacc.c  */
#line 7 "language.yy"
{
  #ifdef C_S_H_IS_COMPILED
    yylloc.first_pos = 0;
    yylloc.last_pos = 0;
  #endif
}

/* Line 1590 of yacc.c  */
#line 3225 "language.cc"
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
        case 2:

/* Line 1806 of yacc.c  */
#line 245 "language.yy"
    {
	YYACCEPT;
}
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 251 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond((yylsp[(1) - (1)]));
  #else
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 260 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond((yylsp[(2) - (2)]));
  #else
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 271 "language.yy"
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

  case 7:

/* Line 1806 of yacc.c  */
#line 282 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 291 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (2)])))
        csh.AddDesignsToHints(true);
  #else
    msc.AddArcs((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 300 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH_Error((yylsp[(2) - (3)]), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (3)])).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free((yyvsp[(1) - (3)].str));
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 311 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 319 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    CshPos pos = (yylsp[(2) - (3)]);
    if (((yylsp[(1) - (3)])).last_pos >= ((yylsp[(2) - (3)])).first_pos)
        pos.first_pos = ((yylsp[(1) - (3)])).last_pos;
    csh.AddCSH_Error(pos, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((yyvsp[(1) - (3)].arclist));
    msc.Error.Error(MSC_POS((yylsp[(2) - (3)])).start, "Could not recognize this as a valid line.");
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 334 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Closing brace missing its opening pair.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Unexpected '}'.");
  #endif
}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 343 "language.yy"
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

  case 15:

/* Line 1806 of yacc.c  */
#line 357 "language.yy"
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

  case 16:

/* Line 1806 of yacc.c  */
#line 369 "language.yy"
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
    ArcBase *dummy = msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)]))));
    if (dummy) delete dummy;
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 387 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(3) - (3)].arcbase)) ((yyvsp[(2) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
  #endif
}
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 395 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arclist) = new ArcList;
    //scope_close should not return here with a CommandNumbering
    //but just in case
    if ((yyvsp[(2) - (2)].arcbase))
        delete((yyvsp[(2) - (2)].arcbase));
  #endif
}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 406 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(3) - (4)]), "Could not recognize this as a valid line.");
  #else
    if ((yyvsp[(4) - (4)].arcbase)) ((yyvsp[(2) - (4)].arclist))->Append((yyvsp[(4) - (4)].arcbase)); //Append any potential CommandNumbering
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(3) - (4)])).start, "Syntax error.");
  #endif
    yyerrok;
}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 417 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(3) - (4)]), "Could not recognize this as a valid line.");
  #else
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(3) - (4)])).start, "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (4)])).start, MSC_POS((yylsp[(3) - (4)])).start, "Here is the corresponding '{'.");
  #endif
}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 428 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(2) - (3)]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(2) - (3)])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).start, MSC_POS((yylsp[(2) - (3)])).end.NextChar(), "Here is the corresponding '{'.");
  #endif
}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 439 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (2)]), "Missing a closing brace ('}').");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(1) - (2)])).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 449 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(3) - (3)]), "The command 'bye' can only be used at the top level.");
  #else
    (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).start, MSC_POS((yylsp[(3) - (3)])).start, "Here is the opening '{'.");
  #endif
  free((yyvsp[(3) - (3)].str));
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 461 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "The command 'bye' can only be used at the top level and not inside curly braces '{' and '}'.");
  #else
    (yyval.arclist) = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
  #endif
  free((yyvsp[(2) - (2)].str));
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 476 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(2) - (2)].arcbase)) ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 487 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 502 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
  #endif
}
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 511 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase)) ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 519 "language.yy"
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

  case 33:

/* Line 1806 of yacc.c  */
#line 535 "language.yy"
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

  case 34:

/* Line 1806 of yacc.c  */
#line 547 "language.yy"
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
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (3)])).start, MSC_POS((yylsp[(1) - (3)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 565 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(1) - (1)].arcbase)) 
		((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 574 "language.yy"
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
        if (CaseInsensitiveEqual((yyvsp[(1) - (2)].str), "parallel"))
            ((yyvsp[(2) - (2)].arcbase))->SetParallel();
        else 
            ((yyvsp[(2) - (2)].arcbase))->SetOverlap();
    }
    (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 599 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
    }
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 618 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase));
  #endif
}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 626 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase));
  #endif
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 638 "language.yy"
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

  case 43:

/* Line 1806 of yacc.c  */
#line 650 "language.yy"
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

  case 44:

/* Line 1806 of yacc.c  */
#line 665 "language.yy"
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

  case 45:

/* Line 1806 of yacc.c  */
#line 683 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "brace", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "bracket", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "range", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "box", HINT_KEYWORD));
    }
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #endif
    free((yyvsp[(1) - (1)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 700 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    if ((yyvsp[(2) - (2)].arcvertarrow)) {
      ((yyvsp[(2) - (2)].arcvertarrow))->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ((yyvsp[(2) - (2)].arcvertarrow))->AddAttributeList(NULL);
      (yyval.arcbase) = ((yyvsp[(2) - (2)].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 713 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
  #else
    if ((yyvsp[(3) - (3)].arcvertarrow)) {
      ((yyvsp[(3) - (3)].arcvertarrow))->SetVerticalShape((yyvsp[(2) - (3)].vshape));
      ((yyvsp[(3) - (3)].arcvertarrow))->AddAttributeList(NULL);
      (yyval.arcbase) = ((yyvsp[(3) - (3)].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 727 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
  #else
	ArcTypePlusDir typeplusdir;
	typeplusdir.arc.type = MSC_ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = MSC_DIR_RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[(2) - (2)].vshape));
    ava->AddAttributeList(NULL);
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 746 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[(2) - (3)].arcvertarrow)) {
      ((yyvsp[(2) - (3)].arcvertarrow))->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ((yyvsp[(2) - (3)].arcvertarrow))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
      (yyval.arcbase) = ((yyvsp[(2) - (3)].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 763 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(4) - (4)])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(4) - (4)])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ((yyvsp[(3) - (4)].arcvertarrow)) {
      ((yyvsp[(3) - (4)].arcvertarrow))->SetVerticalShape((yyvsp[(2) - (4)].vshape));
      ((yyvsp[(3) - (4)].arcvertarrow))->AddAttributeList((yyvsp[(4) - (4)].attriblist));
      (yyval.arcbase) = ((yyvsp[(3) - (4)].arcvertarrow));
    } else (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 781 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
	ArcTypePlusDir typeplusdir;
	typeplusdir.arc.type = MSC_ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = MSC_DIR_RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape((yyvsp[(2) - (3)].vshape));
    ava->AddAttributeList((yyvsp[(3) - (3)].attriblist));
	(yyval.arcbase) = ava;
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 804 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(1) - (1)])))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(1) - (1)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
  #endif
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 816 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 824 "language.yy"
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

  case 55:

/* Line 1806 of yacc.c  */
#line 836 "language.yy"
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

  case 56:

/* Line 1806 of yacc.c  */
#line 848 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity((yyvsp[(1) - (2)].entitylist), &msc, false);
    ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing an entity.");
    (yyval.arcbase) = ce;
  #endif
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 861 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    ((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[(3) - (3)].entitylist), &msc, false);
    delete ((yyvsp[(1) - (3)].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce;
  #endif
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 873 "language.yy"
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

  case 59:

/* Line 1806 of yacc.c  */
#line 889 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
  #else
    ((yyvsp[(4) - (4)].entitylist))->Prepend((yyvsp[(2) - (4)].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[(4) - (4)].entitylist), &msc, false);
    delete ((yyvsp[(2) - (4)].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce->ApplyPrefix((yyvsp[(1) - (4)].str));
  #endif
    free((yyvsp[(1) - (4)].str));
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 903 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "CommandArcList" element used only for this. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new CommandArcList(&msc, (yyvsp[(1) - (1)].arclist)));
        ((yyval.arcbase))->AddAttributeList(NULL);
		delete ((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 917 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 924 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 931 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel));
	if ((yyvsp[(1) - (1)].arcparallel))
        ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 940 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 949 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 958 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 973 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 982 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 997 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 1006 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
	csh.AddCSH_ErrorAfter((yyloc), "Missing design name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a design name to (re-)define.");
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 1017 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 1027 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandEntity(NULL, &msc, false));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 1041 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 1051 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 1065 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, true);
  #else
    const EArcType t = CaseInsensitiveEqual("title", (yyvsp[(1) - (2)].str)) ? MSC_COMMAND_TITLE :
                         CaseInsensitiveEqual("subtitle", (yyvsp[(1) - (2)].str)) ? MSC_COMMAND_SUBTITLE :
                         MSC_ARC_INVALID;
    (yyval.arcbase) = (new ArcDivider(t, &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 1082 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.MarkerNames.insert((yyvsp[(2) - (2)].str));
  #else
    (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (2)].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 1095 "language.yy"
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
    (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), MSC_POS((yyloc)), &msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 1112 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 1122 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 1139 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandHSpace(&msc, (yyvsp[(2) - (3)].namerel)));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 1153 "language.yy"
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

  case 85:

/* Line 1806 of yacc.c  */
#line 1168 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
    }
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing an entity.");
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 1182 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandVSpace(&msc));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 1196 "language.yy"
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

  case 91:

/* Line 1806 of yacc.c  */
#line 1215 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[(1) - (2)].str), "left") || CaseInsensitiveEqual((yyvsp[(1) - (2)].str), "right"))
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error((yylsp[(1) - (2)]), "Use either `left` or `right` to specify which comment column to size.");
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((yylsp[(1) - (2)]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    if (CaseInsensitiveEqual((yyvsp[(1) - (2)].str), "left"))
        (yyval.namerel) = new NamePair(LNOTE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])));
    else if (CaseInsensitiveEqual((yyvsp[(1) - (2)].str), "right"))
        (yyval.namerel) = new NamePair(RNOTE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])));
    else {
        msc.Error.Error(MSC_POS((yylsp[(1) - (2)])).start, "Use either `left` or `right` to specify which comment column to size. Ignoring command.");
        (yyval.namerel) = NULL;
    }
  #endif
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1241 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "left") || CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "right"))
        csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    else
        csh.AddCSH_Error((yylsp[(1) - (1)]), "Use either `left comment` or `right comment` to specify which comment column to size.");
    if (csh.CheckEntityHintAt((yylsp[(1) - (1)]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, "Use <entity>-<entity>, `left comment`, `right comment` to specify horizontal spacing. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 1259 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckEntityHintAt((yylsp[(1) - (1)]))) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "left comment", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "right comment", HINT_KEYWORD));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, "Use `left comment` or `right comment` to specify horizontal spacing for comments. Ignoring command.");
    (yyval.namerel) = NULL;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 1277 "language.yy"
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

  case 96:

/* Line 1806 of yacc.c  */
#line 1288 "language.yy"
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

  case 97:

/* Line 1806 of yacc.c  */
#line 1299 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, FileLineColRange());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 1311 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    csh.CheckEntityHintAt((yylsp[(2) - (2)]));
  #else
    (yyval.namerel) = new NamePair(NULL, FileLineColRange(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 1322 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 1331 "language.yy"
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

  case 101:

/* Line 1806 of yacc.c  */
#line 1345 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (1)].str), MSC_POS((yylsp[(1) - (1)])), NULL, FileLineColRange());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 1370 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), NULL, FileLineColRange());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 1382 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(2) - (2)]), HINT_MARKER);
  #else
    (yyval.namerel) = new NamePair(NULL, FileLineColRange(), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 1393 "language.yy"
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

  case 105:

/* Line 1806 of yacc.c  */
#line 1403 "language.yy"
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

  case 110:

/* Line 1806 of yacc.c  */
#line 1420 "language.yy"
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

  case 111:

/* Line 1806 of yacc.c  */
#line 1430 "language.yy"
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

  case 112:

/* Line 1806 of yacc.c  */
#line 1449 "language.yy"
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

  case 114:

/* Line 1806 of yacc.c  */
#line 1463 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 1474 "language.yy"
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

  case 116:

/* Line 1806 of yacc.c  */
#line 1493 "language.yy"
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

  case 117:

/* Line 1806 of yacc.c  */
#line 1512 "language.yy"
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

  case 118:

/* Line 1806 of yacc.c  */
#line 1531 "language.yy"
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

  case 119:

/* Line 1806 of yacc.c  */
#line 1551 "language.yy"
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

  case 120:

/* Line 1806 of yacc.c  */
#line 1570 "language.yy"
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

  case 121:

/* Line 1806 of yacc.c  */
#line 1590 "language.yy"
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

  case 122:

/* Line 1806 of yacc.c  */
#line 1609 "language.yy"
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

  case 123:

/* Line 1806 of yacc.c  */
#line 1630 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 1636 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #else
    ((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist));
    (yyval.entitylist) = (yyvsp[(3) - (3)].entitylist);
    delete ((yyvsp[(1) - (3)].entitylist));
  #endif
}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 1646 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.entitylist) = ((yyvsp[(1) - (2)].entitylist));
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Expecting an entity here.");
  #endif
}
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 1661 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (2)].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1677 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (1)]));
    csh.AddCSH_EntityName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (1)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1689 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (3)].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1705 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 1718 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (2)].attriblist), NULL, FileLineCol());
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 1736 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
      if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (1)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 1751 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (3)].str), &msc);
    ed->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1769 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
      if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 1786 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1793 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *((yyvsp[(1) - (2)].stringlist)))
        if (csh.ForbiddenStyles.find(str) == csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(str);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (auto a=((yyvsp[(2) - (2)].attriblist))->begin(); a!=((yyvsp[(2) - (2)].attriblist))->end(); a++) {
        std::list<string> problem;
        bool had_generic = false;
        for (auto s = ((yyvsp[(1) - (2)].stringlist))->begin(); s!=((yyvsp[(1) - (2)].stringlist))->end(); s++) {
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
    delete((yyvsp[(2) - (2)].attriblist));
  #endif
    delete((yyvsp[(1) - (2)].stringlist));
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1841 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter((yyloc), "Missing attribute definitons in brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing attribute definitons in brackets ('[' and ']').");
  #endif
    delete((yyvsp[(1) - (1)].stringlist));
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1852 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
    if (strcmp((yyvsp[(1) - (1)].str), "emphasis")==0)
        ((yyval.stringlist))->push_back("box");
    else if (strcmp((yyvsp[(1) - (1)].str), "emptyemphasis")==0)
        ((yyval.stringlist))->push_back("emptybox");
    else ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
    if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(1) - (1)].str), &(yylsp[(1) - (1)]), msc));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 1872 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    (yyval.stringlist) = new std::list<string>;
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[(1) - (1)]), msc));
  #endif
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1887 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
	csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a style name to (re-)define.");
    if (csh.CheckHintAfterPlusOne((yylsp[(1) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
	(yyval.stringlist) = (yyvsp[(1) - (2)].stringlist);
  #else
    (yyval.stringlist) = (yyvsp[(1) - (2)].stringlist);
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 1902 "language.yy"
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
    if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[(1) - (3)].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(3) - (3)].str), &(yylsp[(3) - (3)]), msc));
    (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1923 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_STYLENAME);
    (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
	((yyval.stringlist))->push_back("++");
    if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE)) {
		csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ((yyvsp[(1) - (3)].stringlist))->push_back(ConvertEmphasisToBox("++", &(yylsp[(3) - (3)]), msc));
    (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
  #endif
}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 1940 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (1)].str) ? (yyvsp[(1) - (1)].str) : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (1)].str) ? (yyvsp[(1) - (1)].str) : "") +"'.");
  #endif	
  free((yyvsp[(1) - (1)].str));
}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 1950 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (2)].str) ? (yyvsp[(1) - (2)].str): "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (2)].str) ? (yyvsp[(1) - (2)].str): "") +"'.");
  #endif	
  free((yyvsp[(1) - (2)].str));
}
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 1961 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_BRACE);
    csh.AddCSH_ErrorAfter((yylsp[(3) - (3)]), "Missing a closing brace ('}').");
	csh.AddShapeName((yyvsp[(1) - (3)].str));
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(2) - (3)])).start, MSC_POS((yylsp[(3) - (3)])).end.NextChar(), "Here is the corresponding '{'.");
	if ((yyvsp[(3) - (3)].shape)) {
		msc.Shapes.Add(std::string((yyvsp[(1) - (3)].str)), MSC_POS((yylsp[(1) - (3)])).start, msc.file_url, msc.file_info, (yyvsp[(3) - (3)].shape), msc.Error);
		delete (yyvsp[(3) - (3)].shape);
	}
  #endif	
  free((yyvsp[(1) - (3)].str));
}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 1978 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_BRACE);
	csh.AddShapeName((yyvsp[(1) - (4)].str));
  #else
	if ((yyvsp[(3) - (4)].shape)) {
		msc.Shapes.Add(std::string((yyvsp[(1) - (4)].str)), MSC_POS((yylsp[(1) - (4)])).start, msc.file_url, msc.file_info, (yyvsp[(3) - (4)].shape), msc.Error);
		delete (yyvsp[(3) - (4)].shape);
	}
  #endif	
  free((yyvsp[(1) - (4)].str));
}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 1993 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_ATTRVALUE);
    csh.AddCSH((yylsp[(2) - (5)]), COLOR_BRACE);
    csh.AddCSH((yylsp[(5) - (5)]), COLOR_BRACE);
	csh.AddShapeName((yyvsp[(1) - (5)].str));
    csh.AddCSH_Error((yylsp[(4) - (5)]), "Only numbers can come after shape commands.");
  #else
	if ((yyvsp[(3) - (5)].shape)) {
		msc.Shapes.Add(std::string((yyvsp[(1) - (5)].str)), MSC_POS((yylsp[(1) - (5)])).start, msc.file_url, msc.file_info, (yyvsp[(3) - (5)].shape), msc.Error);
		delete (yyvsp[(3) - (5)].shape);
	}
  #endif	
  free((yyvsp[(1) - (5)].str));
}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 2010 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.shape) = new Shape;
	if ((yyvsp[(1) - (1)].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[(1) - (1)].shapeelement))));
		delete (yyvsp[(1) - (1)].shapeelement);
	}
  #endif	
}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 2020 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(1) - (2)]), "I do not understand this.");
  #else
    (yyval.shape) = new Shape;
	if ((yyvsp[(2) - (2)].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[(2) - (2)].shapeelement))));
		delete (yyvsp[(2) - (2)].shapeelement);
	}
  #endif	
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 2032 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
	if ((yyvsp[(2) - (2)].shapeelement)) {
		((yyvsp[(1) - (2)].shape))->Add(std::move(*((yyvsp[(2) - (2)].shapeelement))));
		delete (yyvsp[(2) - (2)].shapeelement);
	}
  #endif	
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 2041 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Only numbers can come after shape commands.");
  #else
  #endif	
}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 2049 "language.yy"
    {
    const int num_args = 0;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (1)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
	if (should_args != num_args)
		csh.AddCSH_ErrorAfter((yyloc), ShapeElement::ErrorMsg((yyvsp[(1) - (1)].shapecommand), num_args));
  #else
	(yyval.shapeelement) = NULL;
	if (should_args != num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (1)].shapecommand), num_args).append(" Ignoring line."));
	else 
	    (yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (1)].shapecommand));
  #endif	
}
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 2065 "language.yy"
    {
    const int num_args = 1;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (2)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (2)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (2)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (2)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	} else if ((yyvsp[(1) - (2)].shapecommand)>=ShapeElement::SECTION_BG && (((yyvsp[(2) - (2)].str))[0]<'0' || ((yyvsp[(2) - (2)].str))[0]>'2' || ((yyvsp[(2) - (2)].str))[1]!=0))
		csh.AddCSH_Error((yylsp[(2) - (2)]), "S (section) commands require an integer between 0 and 2.");
  #else
	(yyval.shapeelement) = NULL;
	const double a = atof((yyvsp[(2) - (2)].str));
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end, ShapeElement::ErrorMsg((yyvsp[(1) - (2)].shapecommand), num_args).append(" Ignoring line."));
	else if ((yyvsp[(1) - (2)].shapecommand)>=ShapeElement::SECTION_BG && (a!=0 && a!=1 && a!=2))
		msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "S (section) commands require an integer between 0 and 2. Ignoring line.");
	else if ((yyvsp[(1) - (2)].shapecommand)>=ShapeElement::SECTION_BG)
	    (yyval.shapeelement) = new ShapeElement(ShapeElement::Type((yyvsp[(1) - (2)].shapecommand) + unsigned(a)));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (2)].shapecommand), a);
  #endif	
  free((yyvsp[(2) - (2)].str));
}
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 2093 "language.yy"
    {
    const int num_args = 2;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (3)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (3)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (3)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (3)]) + (yylsp[(3) - (3)]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[(3) - (3)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (3)].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (3)].shapecommand), atof((yyvsp[(2) - (3)].str)), atof((yyvsp[(3) - (3)].str)));
  #endif	
  free((yyvsp[(2) - (3)].str));
  free((yyvsp[(3) - (3)].str));
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 2117 "language.yy"
    {
    const int num_args = 3;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (4)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (4)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (4)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (4)]) + (yylsp[(4) - (4)]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[(3) - (4)]) + (yylsp[(4) - (4)]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[(4) - (4)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (4)].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (4)].shapecommand), atof((yyvsp[(2) - (4)].str)), atof((yyvsp[(3) - (4)].str)), atof((yyvsp[(4) - (4)].str)));
  #endif	
  free((yyvsp[(2) - (4)].str));
  free((yyvsp[(3) - (4)].str));
  free((yyvsp[(4) - (4)].str));
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 2143 "language.yy"
    {
    const int num_args = 4;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (5)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (5)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (5)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (5)]) + (yylsp[(5) - (5)]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[(3) - (5)]) + (yylsp[(5) - (5)]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[(4) - (5)]) + (yylsp[(5) - (5)]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[(5) - (5)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (5)].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (5)].shapecommand), atof((yyvsp[(2) - (5)].str)), atof((yyvsp[(3) - (5)].str)), atof((yyvsp[(4) - (5)].str)), atof((yyvsp[(5) - (5)].str)));
  #endif	
  free((yyvsp[(2) - (5)].str));
  free((yyvsp[(3) - (5)].str));
  free((yyvsp[(4) - (5)].str));
  free((yyvsp[(5) - (5)].str));
}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 2171 "language.yy"
    {
    const int num_args = 5;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (6)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (6)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (6)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (6)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (6)]) + (yylsp[(6) - (6)]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[(3) - (6)]) + (yylsp[(6) - (6)]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[(4) - (6)]) + (yylsp[(6) - (6)]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[(5) - (6)]) + (yylsp[(6) - (6)]), msg); break;
		case 4:  csh.AddCSH_Error((yylsp[(6) - (6)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (6)].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (6)].shapecommand), atof((yyvsp[(2) - (6)].str)), atof((yyvsp[(3) - (6)].str)), atof((yyvsp[(4) - (6)].str)), atof((yyvsp[(5) - (6)].str)), atof((yyvsp[(6) - (6)].str)));
  #endif	
  free((yyvsp[(2) - (6)].str));
  free((yyvsp[(3) - (6)].str));
  free((yyvsp[(4) - (6)].str));
  free((yyvsp[(5) - (6)].str));
  free((yyvsp[(6) - (6)].str));
}
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 2201 "language.yy"
    {
    const int num_args = 6;
	const int should_args = ShapeElement::GetNumArgs((yyvsp[(1) - (7)].shapecommand));
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (7)]), COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg((yyvsp[(1) - (7)].shapecommand), num_args);
	    switch (ShapeElement::GetNumArgs((yyvsp[(1) - (7)].shapecommand))) {
		case 0:  csh.AddCSH_Error((yylsp[(2) - (7)]) + (yylsp[(7) - (7)]), msg); break;
		case 1:  csh.AddCSH_Error((yylsp[(3) - (7)]) + (yylsp[(7) - (7)]), msg); break;
		case 2:  csh.AddCSH_Error((yylsp[(4) - (7)]) + (yylsp[(7) - (7)]), msg); break;
		case 3:  csh.AddCSH_Error((yylsp[(5) - (7)]) + (yylsp[(7) - (7)]), msg); break;
		case 4:  csh.AddCSH_Error((yylsp[(6) - (7)]) + (yylsp[(7) - (7)]), msg); break;
		case 5:  csh.AddCSH_Error((yylsp[(7) - (7)]), msg); break;
		default: csh.AddCSH_ErrorAfter((yyloc), msg); break;
		}
	}
  #else
	(yyval.shapeelement) = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS((yyloc)).end, ShapeElement::ErrorMsg((yyvsp[(1) - (7)].shapecommand), num_args).append(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (7)].shapecommand), atof((yyvsp[(2) - (7)].str)), atof((yyvsp[(3) - (7)].str)), atof((yyvsp[(4) - (7)].str)), atof((yyvsp[(5) - (7)].str)), atof((yyvsp[(6) - (7)].str)), atof((yyvsp[(7) - (7)].str)));
  #endif	
  free((yyvsp[(2) - (7)].str));
  free((yyvsp[(3) - (7)].str));
  free((yyvsp[(4) - (7)].str));
  free((yyvsp[(5) - (7)].str));
  free((yyvsp[(6) - (7)].str));
  free((yyvsp[(7) - (7)].str));
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 2235 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    if (csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 2245 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_COMMA);
    if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
}
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 2261 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor((yyvsp[(3) - (3)].str));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[(1) - (3)].str)] = color;
    if (csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 2283 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COLORDEF);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor("++"+string((yyvsp[(4) - (4)].str)));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[(1) - (4)].str)] = color;
    if (csh.CheckHintAt((yylsp[(1) - (4)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore((yylsp[(2) - (4)]), (yylsp[(4) - (4)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (4)].str), "++"+string((yyvsp[(4) - (4)].str)), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 2306 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
    if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter((yyloc), "Missing color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing color definition.");
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 2324 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_COLORNAME);
    if (csh.CheckHintAt((yylsp[(1) - (1)]), HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing equal sign ('=') and a color definition.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing equal sign ('=') and a color definition.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 2341 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_DESIGNNAME);
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

  case 170:

/* Line 1806 of yacc.c  */
#line 2357 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (6)]), COLOR_DESIGNNAME);
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

  case 171:

/* Line 1806 of yacc.c  */
#line 2376 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 2388 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 2395 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 2402 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a color name to (re-)define.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 2416 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ATTR_NAME)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 2427 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_ATTR_VALUE)) {
        csh.AddStylesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter((yyloc), "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Missing a style name to (re-)define.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 2444 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 2450 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 2457 "language.yy"
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

  case 183:

/* Line 1806 of yacc.c  */
#line 2476 "language.yy"
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

  case 184:

/* Line 1806 of yacc.c  */
#line 2495 "language.yy"
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

  case 185:

/* Line 1806 of yacc.c  */
#line 2514 "language.yy"
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

  case 186:

/* Line 1806 of yacc.c  */
#line 2536 "language.yy"
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

  case 187:

/* Line 1806 of yacc.c  */
#line 2560 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 2569 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[(1) - (2)].arcparallel);
    else if ((yyvsp[(1) - (2)].arcparallel))
        (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
    else
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 2581 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
  #endif
}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 2588 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcboxseries) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "The keyword '" + string((yyvsp[(1) - (1)].str)) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 2599 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);   
  #else
    ((yyvsp[(2) - (2)].arcbox))->ExpandLineEnd(MSC_POS((yyloc)));
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(2) - (2)].arcbox));
  #endif
  free((yyvsp[(1) - (2)].str));
}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 2610 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 2618 "language.yy"
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

  case 194:

/* Line 1806 of yacc.c  */
#line 2631 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 2639 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 2648 "language.yy"
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

  case 197:

/* Line 1806 of yacc.c  */
#line 2661 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (3)])), NULL, MSC_POS((yylsp[(1) - (3)])), &msc);
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList((yyvsp[(2) - (3)].attriblist)); //should come after AddFollow
  #endif
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 2677 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 2685 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yyloc)), false);
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox));
  #endif
}
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 2698 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])), false);
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 2706 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (3)])))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (3)])))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (3)].arcbox))->SetLineEnd(MSC_POS2((yylsp[(1) - (3)]), (yylsp[(2) - (3)])), false);
    ((yyvsp[(1) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcbox) = ((yyvsp[(1) - (3)].arcbox));
  #endif
}
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 2721 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
  #else
    (yyval.arcpipe) = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ((yyval.arcpipe))->AddAttributeList(NULL);
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 2736 "language.yy"
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

  case 204:

/* Line 1806 of yacc.c  */
#line 2752 "language.yy"
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

  case 205:

/* Line 1806 of yacc.c  */
#line 2763 "language.yy"
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
    ((yyval.arcpipe))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    ((yyval.arcpipe))->SetLineEnd(MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 2782 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(1) - (1)].arcpipe))
      (yyval.arcpipeseries) = new ArcPipeSeries((yyvsp[(1) - (1)].arcpipe));
    else
      (yyval.arcpipeseries) = NULL;
  #endif
}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 2792 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (2)].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    if ((yyvsp[(1) - (2)].arcpipeseries))
      (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddFollowWithAttributes(ap, NULL);
    else {
      ap->AddAttributeList(NULL);
      (yyval.arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 2806 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(3) - (3)])))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(3) - (3)])))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe((yyvsp[(2) - (3)].arcbox));
    ap->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    if ((yyvsp[(1) - (3)].arcpipeseries))
      (yyval.arcpipeseries) = ((yyvsp[(1) - (3)].arcpipeseries))->AddFollowWithAttributes(ap, (yyvsp[(3) - (3)].attriblist));
    else {
      ap->AddAttributeList((yyvsp[(3) - (3)].attriblist));
      (yyval.arcpipeseries) = new ArcPipeSeries(ap);
    }
  #endif
}
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 2827 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 2836 "language.yy"
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

  case 214:

/* Line 1806 of yacc.c  */
#line 2850 "language.yy"
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

  case 215:

/* Line 1806 of yacc.c  */
#line 2861 "language.yy"
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

  case 216:

/* Line 1806 of yacc.c  */
#line 2873 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 2883 "language.yy"
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

  case 218:

/* Line 1806 of yacc.c  */
#line 2895 "language.yy"
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

  case 219:

/* Line 1806 of yacc.c  */
#line 2909 "language.yy"
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

  case 220:

/* Line 1806 of yacc.c  */
#line 2923 "language.yy"
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

  case 221:

/* Line 1806 of yacc.c  */
#line 2938 "language.yy"
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

  case 222:

/* Line 1806 of yacc.c  */
#line 2951 "language.yy"
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

  case 223:

/* Line 1806 of yacc.c  */
#line 2966 "language.yy"
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
    case MSC_BOX_SOLID:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_BY);
        break;
    case MSC_BOX_DASHED:
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), MSC_POS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_BOX_DOTTED:
        msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        (yyval.vertxpos) = NULL;
        break;
    case MSC_BOX_DOUBLE:
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

  case 224:

/* Line 1806 of yacc.c  */
#line 3000 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.AddCSH_EntityName((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
    csh.AddCSH_EntityName((yylsp[(4) - (4)]), (yyvsp[(4) - (4)].str));
  #else
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), MSC_POS((yylsp[(2) - (4)])), (yyvsp[(4) - (4)].str), MSC_POS((yylsp[(4) - (4)])));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 3016 "language.yy"
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
    (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (5)].str), MSC_POS((yylsp[(2) - (5)])), (yyvsp[(4) - (5)].str), MSC_POS((yylsp[(4) - (5)])), atof((yyvsp[(5) - (5)].str)));
  #endif
    free((yyvsp[(1) - (5)].str));
    free((yyvsp[(2) - (5)].str));
    free((yyvsp[(4) - (5)].str));
    free((yyvsp[(5) - (5)].str));
}
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 3034 "language.yy"
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

  case 227:

/* Line 1806 of yacc.c  */
#line 3048 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[(1) - (1)].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::NOT;
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 3059 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[(2) - (2)].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 3071 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[(1) - (2)].arctype);
	((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 3083 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
	(yyval.arctypeplusdir) = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ((yyval.arctypeplusdir)).arc.type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arctypeplusdir)).arc.lost = EArrowLost::AT_SRC;
    ((yyval.arctypeplusdir)).arc.lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
	((yyval.arctypeplusdir)).dir = MSC_DIR_INDETERMINATE;
  #endif
}
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 3097 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_LEFT;
  #endif
}
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 3106 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_RIGHT;
  #endif
}
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 3115 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_BIDIR;
  #endif
}
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 3125 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (3)]), HINT_MARKER);
    csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctypeplusdir), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), &msc);
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 3138 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]), HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctypeplusdir), MARKER_HERE_STR, (yyvsp[(2) - (2)].str), &msc);
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 3148 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_MARKERNAME);
    csh.CheckHintAt((yylsp[(1) - (2)]), HINT_MARKER);
    csh.CheckHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(2) - (2)].arctypeplusdir), (yyvsp[(1) - (2)].str), MARKER_HERE_STR, &msc);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 3159 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctypeplusdir), MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 3167 "language.yy"
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

  case 239:

/* Line 1806 of yacc.c  */
#line 3179 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 3180 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 3184 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(1) - (2)].arcvertarrow))
        (yyval.arcvertarrow) = ((yyvsp[(1) - (2)].arcvertarrow))->AddXpos((yyvsp[(2) - (2)].vertxpos));
    else
        (yyval.arcvertarrow) = NULL;
    delete (yyvsp[(2) - (2)].vertxpos);
  #endif
}
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 3195 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
	VertXPos vxp(msc);
	(yyval.arcvertarrow) = ((yyvsp[(1) - (1)].arcvertarrow))->AddXpos(&vxp);
  #endif
}
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 3203 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    if ((yyvsp[(1) - (1)].vertxpos)) {
		ArcTypePlusDir typeplusdir;
		typeplusdir.arc.type = MSC_ARC_SOLID;
		typeplusdir.arc.lost = EArrowLost::NOT;
		typeplusdir.dir = MSC_DIR_RIGHT;
		ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
		ava->AddXpos((yyvsp[(1) - (1)].vertxpos));
		(yyval.arcvertarrow) = ava;
		delete (yyvsp[(1) - (1)].vertxpos);
	} else
	    (yyval.arcvertarrow) = NULL;
  #endif

}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 3222 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 3230 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (3)].arcarrow))->AddLostPos((yyvsp[(3) - (3)].vertxpos), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
   free((yyvsp[(2) - (3)].str));
}
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 3239 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing 'at' clause.");
    if (csh.CheckHintAfterPlusOne((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end, "Missing 'at' clause.");
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcarrow));
  #endif
   free((yyvsp[(2) - (2)].str));
}
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 3254 "language.yy"
    {
    //explicit copy here to suppress bison warning (since the two types are different)
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 3262 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), true, MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 3275 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 3285 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arcsegdata), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 3296 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 3306 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 3316 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), false, MSC_POS((yylsp[(1) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 3329 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arcsegdata), (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), LSIDE_ENT_STR, false, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 3339 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arcsegdata), RSIDE_ENT_STR, MSC_POS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), false, MSC_POS((yylsp[(1) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 3350 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 3360 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 3369 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (3)]));
    csh.AddCSH_EntityName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(1) - (3)].str), MSC_POS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), true, MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 3382 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[(1) - (2)]), (yylsp[(2) - (2)]));
    csh.AddCSH_EntityName((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arcsegdata), LSIDE_ENT_STR, MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 3392 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==EOF);
  #else
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arcsegdata), (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), RSIDE_ENT_STR, true, MSC_POS((yylsp[(2) - (2)])));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 3403 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    csh.AddCSH_EntityName((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(2) - (3)].arcsegdata), (yyvsp[(3) - (3)].str), MSC_POS((yylsp[(3) - (3)])), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 3413 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 3425 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 3426 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 3427 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 3430 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 3440 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 3451 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 3462 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]) + (yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 3476 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 3486 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 3497 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 3508 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]) + (yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 3522 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 3532 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 3543 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 3554 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]) + (yylsp[(2) - (3)]), COLOR_SYMBOL);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 3568 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 3577 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 3591 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(2) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 3605 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 3622 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 3631 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 3645 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(2) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 3659 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 3676 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 3685 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (2)])));
  #endif
}
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 3699 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(2) - (2)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (2)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_DST;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 3713 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
    else
        csh.AddCSH_Error((yylsp[(1) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error((yylsp[(3) - (3)]), MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ((yyval.arcsegdata)).type = (yyvsp[(2) - (3)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::AT_SRC;
    ((yyval.arcsegdata)).lost_pos.SetFrom(MSC_POS((yylsp[(1) - (3)])));
  #endif
}
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 3732 "language.yy"
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

  case 310:

/* Line 1806 of yacc.c  */
#line 3755 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 3765 "language.yy"
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

  case 314:

/* Line 1806 of yacc.c  */
#line 3781 "language.yy"
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

  case 315:

/* Line 1806 of yacc.c  */
#line 3817 "language.yy"
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

  case 316:

/* Line 1806 of yacc.c  */
#line 3840 "language.yy"
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

  case 317:

/* Line 1806 of yacc.c  */
#line 3876 "language.yy"
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

  case 318:

/* Line 1806 of yacc.c  */
#line 3900 "language.yy"
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

  case 319:

/* Line 1806 of yacc.c  */
#line 3936 "language.yy"
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

  case 320:

/* Line 1806 of yacc.c  */
#line 3973 "language.yy"
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

  case 321:

/* Line 1806 of yacc.c  */
#line 4009 "language.yy"
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

  case 322:

/* Line 1806 of yacc.c  */
#line 4026 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcbase));
  #endif
}
    break;

  case 323:

/* Line 1806 of yacc.c  */
#line 4033 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase));
  #endif
}
    break;

  case 324:

/* Line 1806 of yacc.c  */
#line 4046 "language.yy"
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
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[(3) - (4)].str), MSC_POS((yylsp[(3) - (4)])));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(4) - (4)].attriblist));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 4064 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 4078 "language.yy"
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

  case 327:

/* Line 1806 of yacc.c  */
#line 4093 "language.yy"
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
    (yyval.arcbase) = new CommandNote(&msc);
    ((yyval.arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 4113 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 4120 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 4127 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 4135 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    (yyval.arcbase) = new CommandNote(&msc, (yyvsp[(1) - (2)].eside));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 4148 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Comments and notes need a label.");
    if (csh.CheckEntityHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
  #else
    (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 4160 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 334:

/* Line 1806 of yacc.c  */
#line 4167 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
	csh.AddColonLabel((yylsp[(1) - (1)]));
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 4176 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 4184 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 4192 "language.yy"
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

  case 338:

/* Line 1806 of yacc.c  */
#line 4204 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 340:

/* Line 1806 of yacc.c  */
#line 4215 "language.yy"
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

  case 341:

/* Line 1806 of yacc.c  */
#line 4225 "language.yy"
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

  case 342:

/* Line 1806 of yacc.c  */
#line 4235 "language.yy"
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

  case 343:

/* Line 1806 of yacc.c  */
#line 4246 "language.yy"
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

  case 344:

/* Line 1806 of yacc.c  */
#line 4257 "language.yy"
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

  case 345:

/* Line 1806 of yacc.c  */
#line 4268 "language.yy"
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

  case 346:

/* Line 1806 of yacc.c  */
#line 4279 "language.yy"
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

  case 347:

/* Line 1806 of yacc.c  */
#line 4292 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 4296 "language.yy"
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

  case 349:

/* Line 1806 of yacc.c  */
#line 4308 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 4315 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 4324 "language.yy"
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

  case 353:

/* Line 1806 of yacc.c  */
#line 4336 "language.yy"
    {
  //string=string
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

  case 354:

/* Line 1806 of yacc.c  */
#line 4351 "language.yy"
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (4)]), (yyvsp[(1) - (4)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (4)]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[(3) - (4)])+(yylsp[(4) - (4)]), (string("++")+(yyvsp[(4) - (4)].str)).c_str(), (yyvsp[(1) - (4)].str));
        csh.CheckHintAt((yylsp[(1) - (4)]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[(2) - (4)]), (yylsp[(3) - (4)])+(yylsp[(4) - (4)]), HINT_ATTR_VALUE, (yyvsp[(1) - (4)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (4)].str), string("++")+(yyvsp[(4) - (4)].str), MSC_POS((yylsp[(1) - (4)])), MSC_POS2((yylsp[(3) - (4)]),(yylsp[(4) - (4)])));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 4366 "language.yy"
    {
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        csh.AddCSH_AttrValue((yylsp[(3) - (3)]), "++", (yyvsp[(1) - (3)].str));
		csh.AddCSH_ErrorAfter((yylsp[(3) - (3)]), "Continue with a color name or definition.");
        csh.CheckHintAt((yylsp[(1) - (3)]), HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_VALUE, (yyvsp[(1) - (3)].str));
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), "++", MSC_POS((yylsp[(1) - (3)])), MSC_POS((yylsp[(3) - (3)])));
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 4381 "language.yy"
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

  case 357:

/* Line 1806 of yacc.c  */
#line 4395 "language.yy"
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

  case 358:

/* Line 1806 of yacc.c  */
#line 4409 "language.yy"
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

  case 359:

/* Line 1806 of yacc.c  */
#line 4423 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 4428 "language.yy"
    {
  //here we accept non alpha strings for "->" and similar style names
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
        csh.CheckHintAt((yylsp[(1) - (1)]), HINT_ATTR_NAME);
  #else
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 4440 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "brace")) (yyval.vshape) = ArcVerticalArrow::BRACE;
    else if (CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "bracket")) (yyval.vshape) = ArcVerticalArrow::BRACKET;
    else if (CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "range")) (yyval.vshape) = ArcVerticalArrow::RANGE;
    else if (CaseInsensitiveEqual((yyvsp[(1) - (1)].str), "pointer")) (yyval.vshape) = ArcVerticalArrow::POINTER; 
    else {
        (yyval.vshape) = ArcVerticalArrow::ARROW_OR_BOX;
        _ASSERT(0);
    }
  #endif
  free((yyvsp[(1) - (1)].str));
}
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 4454 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
	(yyval.vshape) = ArcVerticalArrow::BOX;
  #endif
  free((yyvsp[(1) - (1)].str));
}
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 4464 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString((yylsp[(1) - (1)]));
  #endif
  (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 4472 "language.yy"
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[(1) - (1)].shapecommand)];
	((yyval.str))[1] = 0;
}
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 4491 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 4492 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 4493 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 4494 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 4495 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 4496 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 4497 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 4498 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 4499 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 4500 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 4501 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 4502 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 4504 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 4512 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_BOX_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_BOX_DASHED: (yyval.str) = strdup("++"); break; //will likely not happen
    case MSC_BOX_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_BOX_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 4527 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_BRACE);
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

  case 418:

/* Line 1806 of yacc.c  */
#line 4544 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    (yyval.arcbase) = NULL;
    csh.PopContext();
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_BRACE);
  #else
    std::pair<bool, double> hscale = msc.Contexts.back().hscale;
    (yyval.arcbase) = msc.PopContext();
    if (hscale.first)
        msc.Contexts.back().hscale = hscale;
  #endif
}
    break;



/* Line 1806 of yacc.c  */
#line 9195 "language.cc"
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
#line 4558 "language.yy"



/* END OF FILE */

