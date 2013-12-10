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
#line 14 "language.yy"

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
#line 138 "colorsyntax.cc"

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
#line 94 "language.yy"

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
#line 347 "colorsyntax.cc"
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
#line 179 "language.yy"

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
#line 435 "colorsyntax.cc"

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
#define YYFINAL  209
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1796

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  88
/* YYNRULES -- Number of rules.  */
#define YYNRULES  398
/* YYNRULES -- Number of states.  */
#define YYNSTATES  446

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
       0,     0,     3,     5,     8,     9,    12,    15,    17,    20,
      22,    25,    27,    30,    34,    37,    41,    45,    48,    53,
      57,    60,    62,    65,    67,    69,    72,    75,    79,    84,
      86,    90,    92,    94,    96,    99,   101,   103,   106,   108,
     111,   115,   117,   120,   123,   127,   131,   136,   138,   140,
     142,   145,   148,   152,   156,   161,   163,   165,   167,   169,
     172,   175,   178,   181,   183,   186,   188,   191,   194,   197,
     201,   203,   206,   208,   210,   212,   216,   219,   221,   224,
     226,   228,   230,   232,   235,   237,   239,   241,   243,   246,
     249,   252,   254,   258,   260,   263,   266,   268,   272,   274,
     276,   278,   280,   282,   286,   289,   293,   297,   301,   305,
     309,   312,   316,   319,   323,   326,   328,   332,   335,   339,
     342,   344,   348,   351,   354,   356,   360,   363,   365,   369,
     372,   374,   378,   380,   383,   387,   392,   394,   397,   399,
     402,   406,   411,   417,   424,   432,   441,   443,   447,   449,
     451,   455,   460,   466,   473,   475,   477,   481,   484,   487,
     489,   491,   495,   498,   502,   506,   510,   514,   518,   520,
     523,   525,   527,   530,   533,   537,   541,   544,   549,   553,
     555,   558,   561,   565,   568,   571,   573,   577,   579,   582,
     586,   588,   591,   593,   595,   599,   602,   605,   607,   610,
     614,   618,   623,   627,   632,   636,   641,   647,   649,   651,
     653,   656,   659,   663,   665,   667,   669,   673,   676,   679,
     681,   683,   687,   692,   695,   697,   699,   703,   706,   708,
     710,   712,   714,   718,   721,   724,   728,   731,   735,   738,
     741,   745,   748,   752,   755,   758,   762,   765,   767,   769,
     771,   773,   775,   777,   779,   781,   783,   785,   787,   789,
     791,   793,   795,   797,   799,   801,   803,   806,   809,   813,
     815,   818,   821,   825,   827,   830,   833,   837,   839,   842,
     845,   849,   851,   854,   857,   861,   863,   866,   869,   873,
     875,   877,   880,   882,   884,   886,   891,   897,   901,   906,
     910,   914,   917,   919,   921,   924,   929,   932,   935,   939,
     941,   943,   945,   948,   950,   952,   954,   956,   959,   963,
     966,   968,   971,   975,   980,   984,   987,   991,   993,   996,
     999,  1001,  1005,  1008,  1012,  1016,  1021,  1025,  1029,  1033,
    1036,  1040,  1042,  1044,  1046,  1048,  1050,  1052,  1054,  1056,
    1058,  1060,  1062,  1064,  1066,  1068,  1070,  1072,  1074,  1076,
    1078,  1080,  1082,  1084,  1086,  1088,  1090,  1092,  1094,  1096,
    1098,  1100,  1102,  1104,  1106,  1108,  1110,  1112,  1114,  1116,
    1118,  1120,  1122,  1124,  1126,  1128,  1130,  1132,  1134,  1136,
    1138,  1140,  1142,  1144,  1146,  1148,  1150,  1152,  1155
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      73,     0,    -1,    74,    -1,    74,    60,    -1,    -1,    17,
      77,    -1,    76,    77,    -1,    17,    -1,    17,     1,    -1,
      75,    -1,    75,     1,    -1,    78,    -1,    78,    13,    -1,
      78,    13,    75,    -1,    17,     7,    -1,    17,     7,   157,
      -1,   158,    78,   159,    -1,   158,   159,    -1,   158,    78,
       1,   159,    -1,   158,    78,     1,    -1,   158,    78,    -1,
      79,    -1,    79,    82,    -1,    82,    -1,    80,    -1,    79,
      80,    -1,    82,     9,    -1,    82,     9,    71,    -1,    82,
       1,     9,    71,    -1,     9,    -1,    82,     1,     9,    -1,
      49,    -1,    50,    -1,    83,    -1,    81,    83,    -1,    81,
      -1,   122,    -1,   122,   148,    -1,    45,    -1,    45,   122,
      -1,    45,   122,   148,    -1,    51,    -1,    51,   152,    -1,
      51,   121,    -1,    51,   152,   121,    -1,    51,   121,   148,
      -1,    51,   152,   121,   148,    -1,   148,    -1,    94,    -1,
      89,    -1,    89,    94,    -1,    94,     8,    -1,    94,     8,
      92,    -1,    89,    94,     8,    -1,    89,    94,     8,    92,
      -1,    90,    -1,   111,    -1,   115,    -1,   110,    -1,    41,
      98,    -1,    42,   101,    -1,    43,    95,    -1,    44,   104,
      -1,    38,    -1,    38,   149,    -1,    39,    -1,    39,   149,
      -1,    84,   148,    -1,    48,   157,    -1,    48,   157,   149,
      -1,    40,    -1,    40,   149,    -1,   143,    -1,   144,    -1,
     146,    -1,    62,    85,    86,    -1,    62,    85,    -1,    62,
      -1,    61,    86,    -1,    61,    -1,    68,    -1,    69,    -1,
      87,    -1,    55,    65,    -1,    55,    -1,    65,    -1,   148,
      -1,     5,    -1,     5,   148,    -1,   153,     6,    -1,     6,
     153,    -1,     6,    -1,   153,     6,   153,    -1,   153,    -1,
     153,     6,    -1,     6,   153,    -1,     6,    -1,   153,     6,
     153,    -1,    57,    -1,    56,    -1,    58,    -1,    59,    -1,
      91,    -1,    90,     8,    91,    -1,    90,     8,    -1,    90,
       8,    71,    -1,    90,     8,     1,    -1,   153,     7,    22,
      -1,   153,     7,     5,    -1,   153,     7,   157,    -1,   153,
       7,    -1,    17,     7,   157,    -1,    17,     7,    -1,    17,
      11,   157,    -1,    17,    11,    -1,    93,    -1,    92,     8,
      93,    -1,    92,     8,    -1,    92,     8,    71,    -1,   153,
     148,    -1,   153,    -1,   153,   148,    77,    -1,   153,    77,
      -1,   153,   148,    -1,   153,    -1,   153,   148,    77,    -1,
     153,    77,    -1,    96,    -1,    95,     8,    96,    -1,    97,
     149,    -1,   157,    -1,    97,     8,   157,    -1,   153,    -1,
     153,    12,    -1,   153,    12,    99,    -1,   153,    12,    99,
      13,    -1,   100,    -1,    99,   100,    -1,    70,    -1,    70,
       5,    -1,    70,     5,     5,    -1,    70,     5,     5,     5,
      -1,    70,     5,     5,     5,     5,    -1,    70,     5,     5,
       5,     5,     5,    -1,    70,     5,     5,     5,     5,     5,
       5,    -1,    70,     5,     5,     5,     5,     5,     5,     1,
      -1,   103,    -1,   101,     8,   103,    -1,    21,    -1,   157,
      -1,   156,     7,   102,    -1,   156,     7,    37,   102,    -1,
       3,   105,   106,     9,    13,    -1,     3,   105,   106,     9,
       1,    13,    -1,    12,    -1,   107,    -1,   106,     9,   107,
      -1,    42,   101,    -1,    43,    95,    -1,   108,    -1,   109,
      -1,   108,     8,   109,    -1,   108,     1,    -1,   153,     7,
      22,    -1,   153,     7,     5,    -1,   153,     7,   157,    -1,
      17,     7,   157,    -1,    17,    11,   157,    -1,    77,    -1,
     110,    77,    -1,   112,    -1,    46,    -1,    46,   112,    -1,
     111,   117,    -1,   111,   117,   148,    -1,   111,   117,    77,
      -1,   111,    77,    -1,   111,   117,   148,    77,    -1,   111,
     148,    77,    -1,   117,    -1,   117,   148,    -1,   117,    77,
      -1,   117,   148,    77,    -1,    47,   117,    -1,    47,     1,
      -1,    47,    -1,    47,   117,   148,    -1,   113,    -1,   114,
     117,    -1,   114,   117,   148,    -1,   114,    -1,   114,    77,
      -1,    37,    -1,    36,    -1,   153,   116,   153,    -1,   116,
     153,    -1,   153,   116,    -1,   116,    -1,    53,   153,    -1,
      53,   153,     5,    -1,    53,   153,     6,    -1,    53,   153,
       6,     5,    -1,    53,   153,    10,    -1,    53,   153,    10,
       5,    -1,    53,   153,   116,    -1,    53,   153,     6,   153,
      -1,    53,   153,     6,   153,     5,    -1,    53,    -1,   153,
      -1,   116,    -1,    16,   116,    -1,   116,    16,    -1,    16,
     116,    16,    -1,   134,    -1,   133,    -1,   135,    -1,   153,
     119,   153,    -1,   119,   153,    -1,   153,   119,    -1,   119,
      -1,   153,    -1,   119,   153,    71,    -1,   153,   119,   153,
      71,    -1,   120,   118,    -1,   120,    -1,    35,    -1,   123,
      54,   118,    -1,   123,    54,    -1,   123,    -1,   124,    -1,
     125,    -1,   126,    -1,   153,   133,   153,    -1,   133,   153,
      -1,   153,   133,    -1,   124,   136,   153,    -1,   124,   136,
      -1,   153,   134,   153,    -1,   134,   153,    -1,   153,   134,
      -1,   125,   137,   153,    -1,   125,   137,    -1,   153,   135,
     153,    -1,   135,   153,    -1,   153,   135,    -1,   126,   138,
     153,    -1,   126,   138,    -1,    23,    -1,    26,    -1,    29,
      -1,    32,    -1,    24,    -1,    27,    -1,    30,    -1,    33,
      -1,    25,    -1,    28,    -1,    31,    -1,    34,    -1,   127,
      -1,     6,    -1,   128,    -1,     6,    -1,   129,    -1,     6,
      -1,   127,    -1,    16,   127,    -1,   127,    16,    -1,    16,
     127,    16,    -1,   128,    -1,    16,   128,    -1,   128,    16,
      -1,    16,   128,    16,    -1,   129,    -1,    16,   129,    -1,
     129,    16,    -1,    16,   129,    16,    -1,   130,    -1,    16,
     130,    -1,   130,    16,    -1,    16,   130,    16,    -1,   131,
      -1,    16,   131,    -1,   131,    16,    -1,    16,   131,    16,
      -1,   132,    -1,    16,   132,    -1,   132,    16,    -1,    16,
     132,    16,    -1,   140,    -1,   153,    -1,    55,   118,    -1,
      55,    -1,   153,    -1,   155,    -1,    63,   141,    88,   139,
      -1,    63,   141,    88,   139,   139,    -1,    63,   141,   140,
      -1,    63,   141,   140,   139,    -1,    63,   141,    88,    -1,
      63,   141,   153,    -1,    63,   141,    -1,    63,    -1,   142,
      -1,   142,   148,    -1,    64,    53,   157,   148,    -1,    64,
     148,    -1,    64,    53,    -1,    64,    53,   148,    -1,    65,
      -1,    66,    -1,    67,    -1,   145,   148,    -1,   145,    -1,
      19,    -1,    18,    -1,   147,    -1,   147,   149,    -1,   149,
     147,   149,    -1,   149,   147,    -1,   149,    -1,    14,    15,
      -1,    14,   150,    15,    -1,    14,   150,     1,    15,    -1,
      14,     1,    15,    -1,    14,   150,    -1,    14,   150,     1,
      -1,    14,    -1,    14,    71,    -1,    14,     1,    -1,   151,
      -1,   150,     8,   151,    -1,   150,     8,    -1,   150,     8,
      71,    -1,   156,     7,   102,    -1,   156,     7,    37,   102,
      -1,   156,     7,    37,    -1,   156,     7,     5,    -1,   156,
       7,    22,    -1,   156,     7,    -1,   157,     7,    71,    -1,
     157,    -1,    52,    -1,     4,    -1,     3,    -1,    70,    -1,
      17,    -1,    42,    -1,    43,    -1,    44,    -1,    40,    -1,
      45,    -1,    46,    -1,    47,    -1,    51,    -1,    49,    -1,
      50,    -1,    38,    -1,    39,    -1,    48,    -1,    53,    -1,
      55,    -1,    56,    -1,    57,    -1,    58,    -1,    59,    -1,
      60,    -1,    62,    -1,    61,    -1,    63,    -1,    64,    -1,
      65,    -1,    66,    -1,    67,    -1,    68,    -1,    69,    -1,
      52,    -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,
      27,    -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,
      32,    -1,    33,    -1,    34,    -1,    35,    -1,    36,    -1,
     153,    -1,   154,    -1,   156,    -1,   155,    -1,    20,    -1,
      12,    -1,    12,    71,    -1,    13,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   243,   243,   244,   255,   265,   276,   286,   296,   306,
     313,   327,   328,   337,   351,   363,   381,   391,   404,   417,
     429,   442,   443,   454,   469,   478,   486,   500,   501,   502,
     514,   529,   529,   531,   539,   564,   583,   591,   603,   615,
     630,   648,   664,   678,   691,   705,   722,   740,   752,   760,
     772,   784,   797,   810,   826,   842,   855,   862,   869,   877,
     886,   895,   904,   913,   923,   937,   947,   961,   978,   991,
    1008,  1018,  1032,  1033,  1034,  1035,  1049,  1064,  1078,  1092,
    1108,  1108,  1110,  1111,  1137,  1155,  1172,  1173,  1184,  1195,
    1207,  1218,  1227,  1241,  1266,  1278,  1289,  1299,  1314,  1314,
    1314,  1314,  1316,  1326,  1345,  1358,  1359,  1370,  1389,  1408,
    1427,  1447,  1466,  1486,  1505,  1526,  1532,  1543,  1555,  1558,
    1574,  1586,  1602,  1615,  1633,  1648,  1666,  1682,  1683,  1690,
    1739,  1755,  1773,  1783,  1793,  1809,  1823,  1833,  1843,  1860,
    1885,  1904,  1924,  1945,  1967,  1990,  2018,  2019,  2026,  2026,
    2028,  2043,  2061,  2078,  2098,  2108,  2109,  2116,  2123,  2130,
    2132,  2133,  2139,  2146,  2165,  2184,  2203,  2225,  2249,  2258,
    2270,  2276,  2287,  2297,  2305,  2318,  2326,  2335,  2348,  2364,
    2372,  2385,  2393,  2408,  2423,  2439,  2450,  2469,  2479,  2493,
    2513,  2514,  2521,  2521,  2523,  2537,  2548,  2560,  2570,  2582,
    2596,  2610,  2625,  2638,  2653,  2687,  2703,  2721,  2733,  2749,
    2760,  2772,  2784,  2798,  2807,  2816,  2826,  2839,  2849,  2860,
    2868,  2881,  2882,  2885,  2896,  2907,  2915,  2924,  2939,  2945,
    2945,  2945,  2947,  2960,  2970,  2981,  2991,  3001,  3014,  3024,
    3035,  3045,  3054,  3067,  3077,  3088,  3098,  3107,  3107,  3107,
    3107,  3108,  3108,  3108,  3108,  3109,  3109,  3109,  3109,  3111,
    3111,  3112,  3112,  3113,  3113,  3115,  3125,  3136,  3147,  3161,
    3171,  3182,  3193,  3207,  3217,  3228,  3239,  3253,  3262,  3276,
    3290,  3307,  3316,  3330,  3344,  3361,  3370,  3384,  3398,  3416,
    3417,  3440,  3450,  3464,  3464,  3466,  3502,  3525,  3561,  3585,
    3621,  3658,  3694,  3711,  3718,  3731,  3749,  3763,  3778,  3798,
    3805,  3812,  3820,  3833,  3845,  3852,  3860,  3868,  3876,  3888,
    3896,  3899,  3909,  3919,  3930,  3941,  3952,  3963,  3976,  3980,
    3992,  3999,  4008,  4018,  4020,  4035,  4050,  4065,  4079,  4093,
    4107,  4112,  4124,  4141,  4141,  4142,  4149,  4149,  4150,  4150,
    4151,  4151,  4151,  4152,  4153,  4153,  4153,  4154,  4154,  4155,
    4155,  4155,  4155,  4155,  4156,  4156,  4156,  4157,  4157,  4157,
    4158,  4158,  4158,  4159,  4159,  4159,  4160,  4162,  4163,  4164,
    4165,  4166,  4167,  4168,  4169,  4170,  4171,  4172,  4173,  4174,
    4182,  4193,  4193,  4195,  4195,  4195,  4197,  4211,  4213
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
  "msc", "top_level_arclist", "msckey", "braced_arclist",
  "arclist_maybe_no_semicolon", "arclist", "arc_with_parallel_semicolon",
  "overlap_or_parallel", "arc_with_parallel", "arc", "titlecommandtoken",
  "hspace_location", "full_arcattrlist_with_label_or_number", "entityrel",
  "markerrel_no_string", "entity_command_prefixes", "optlist", "opt",
  "entitylist", "entity", "first_entity", "styledeflist", "styledef",
  "tok_stringlist", "shapedef", "shapedeflist", "shapeline",
  "colordeflist", "color_string", "colordef", "designdef",
  "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "box_list", "first_box",
  "first_pipe", "pipe_list_no_content", "pipe_list", "emphrel", "boxrel",
  "vertxpos", "empharcrel_straight", "vertrel_no_xpos", "vertrel",
  "arcrel", "arcrel_arrow", "arcrel_to", "arcrel_from", "arcrel_bidir",
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
       0,    72,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    75,    75,    75,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    79,    79,    80,    80,    80,    80,
      80,    81,    81,    82,    82,    82,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      84,    84,    85,    85,    85,    85,    86,    86,    86,    87,
      87,    87,    87,    87,    88,    88,    88,    88,    89,    89,
      89,    89,    90,    90,    90,    90,    90,    91,    91,    91,
      91,    91,    91,    91,    91,    92,    92,    92,    92,    93,
      93,    93,    93,    94,    94,    94,    94,    95,    95,    96,
      97,    97,    98,    98,    98,    98,    99,    99,   100,   100,
     100,   100,   100,   100,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   105,   106,   106,   107,   107,   107,
     108,   108,   108,   109,   109,   109,   109,   109,   110,   110,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   112,
     112,   112,   112,   113,   113,   113,   113,   114,   114,   114,
     115,   115,   116,   116,   117,   117,   117,   117,   118,   118,
     118,   118,   118,   118,   118,   118,   118,   118,   118,   119,
     119,   119,   119,   119,   119,   119,   120,   120,   120,   120,
     120,   120,   120,   121,   121,   122,   122,   122,   122,   123,
     123,   123,   124,   124,   124,   124,   124,   125,   125,   125,
     125,   125,   126,   126,   126,   126,   126,   127,   127,   127,
     127,   128,   128,   128,   128,   129,   129,   129,   129,   130,
     130,   131,   131,   132,   132,   133,   133,   133,   133,   134,
     134,   134,   134,   135,   135,   135,   135,   136,   136,   136,
     136,   137,   137,   137,   137,   138,   138,   138,   138,   139,
     139,   140,   140,   141,   141,   142,   142,   142,   142,   142,
     142,   142,   142,   143,   143,   144,   144,   144,   144,   145,
     145,   145,   146,   146,   147,   147,   148,   148,   148,   148,
     148,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     150,   150,   150,   150,   151,   151,   151,   151,   151,   151,
     151,   151,   152,   153,   153,   153,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   156,   156,   157,   157,   157,   158,   158,   159
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     2,     2,     1,     2,     1,
       2,     1,     2,     3,     2,     3,     3,     2,     4,     3,
       2,     1,     2,     1,     1,     2,     2,     3,     4,     1,
       3,     1,     1,     1,     2,     1,     1,     2,     1,     2,
       3,     1,     2,     2,     3,     3,     4,     1,     1,     1,
       2,     2,     3,     3,     4,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     2,     1,     2,     2,     2,     3,
       1,     2,     1,     1,     1,     3,     2,     1,     2,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     2,     2,
       2,     1,     3,     1,     2,     2,     1,     3,     1,     1,
       1,     1,     1,     3,     2,     3,     3,     3,     3,     3,
       2,     3,     2,     3,     2,     1,     3,     2,     3,     2,
       1,     3,     2,     2,     1,     3,     2,     1,     3,     2,
       1,     3,     1,     2,     3,     4,     1,     2,     1,     2,
       3,     4,     5,     6,     7,     8,     1,     3,     1,     1,
       3,     4,     5,     6,     1,     1,     3,     2,     2,     1,
       1,     3,     2,     3,     3,     3,     3,     3,     1,     2,
       1,     1,     2,     2,     3,     3,     2,     4,     3,     1,
       2,     2,     3,     2,     2,     1,     3,     1,     2,     3,
       1,     2,     1,     1,     3,     2,     2,     1,     2,     3,
       3,     4,     3,     4,     3,     4,     5,     1,     1,     1,
       2,     2,     3,     1,     1,     1,     3,     2,     2,     1,
       1,     3,     4,     2,     1,     1,     3,     2,     1,     1,
       1,     1,     3,     2,     2,     3,     2,     3,     2,     2,
       3,     2,     3,     2,     2,     3,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     1,
       2,     2,     3,     1,     2,     2,     3,     1,     2,     2,
       3,     1,     2,     2,     3,     1,     2,     2,     3,     1,
       1,     2,     1,     1,     1,     4,     5,     3,     4,     3,
       3,     2,     1,     1,     2,     4,     2,     2,     3,     1,
       1,     1,     2,     1,     1,     1,     1,     2,     3,     2,
       1,     2,     3,     4,     3,     2,     3,     1,     2,     2,
       1,     3,     2,     3,     3,     4,     3,     3,     3,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
       4,   344,   343,    29,   396,     0,     0,     0,   315,   314,
     247,   251,   255,   248,   252,   256,   249,   253,   257,   250,
     254,   258,   225,   193,   192,    63,    65,    70,     0,     0,
       0,     0,    38,   171,     0,     0,    31,    32,    41,    99,
      98,   100,   101,    79,    77,   302,     0,   309,   310,   311,
      80,    81,   345,     0,     2,     0,     0,   168,    11,    21,
      24,    35,     0,    33,     0,    49,    55,   102,    48,    58,
      56,   170,   187,   190,    57,   197,   179,    36,   228,   229,
     230,   231,   265,   269,   273,     0,     0,     0,   303,    72,
      73,   313,    74,   316,    47,   320,   124,     0,   397,   329,
     321,   346,   395,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   357,   358,   350,
     347,   348,   349,   351,   352,   353,   359,   355,   356,   354,
     376,   360,   361,   362,   363,   364,   365,   366,   368,   367,
     369,   370,   371,   372,   373,   374,   375,   328,     0,   330,
     391,   392,   394,   393,   341,   266,   270,   274,     8,   112,
     114,     5,    64,    66,    71,    59,   132,    60,   146,     0,
      61,   127,     0,   393,   130,     0,    62,    39,     0,   172,
       0,   184,   183,    68,     0,   342,   209,   219,   224,    43,
     214,   213,   215,    42,   220,    87,    78,    86,    91,    84,
      85,    76,    82,    93,   301,   293,   294,   307,   306,     1,
       3,    10,     6,    12,     0,    25,     0,    34,     0,    26,
      67,    50,   124,     0,    51,   169,   176,   173,     0,   191,
     188,   195,   181,   180,    37,   227,   260,     0,   259,   277,
     236,   262,     0,   261,   281,   241,   264,     0,   263,   285,
     246,   267,   271,   275,   233,   238,   243,   304,   312,   317,
     319,   110,   126,   196,   234,   239,   244,   123,   398,     0,
      17,   324,   326,   332,   322,   339,     0,   268,   272,   276,
     111,   113,   133,     0,     0,     0,     0,   129,   154,     0,
      40,   186,    69,   210,   211,   217,   207,   223,   208,    45,
      44,   218,    88,    90,    83,    75,    89,    96,   292,   299,
     297,   300,   308,     0,    13,   112,    30,    27,    53,   106,
     105,   103,     0,    52,   115,   120,   175,   174,   178,   189,
     182,   226,   278,   279,   235,   282,   283,   240,   286,   287,
     245,   318,   108,   107,   109,   194,   232,   237,   242,   125,
      19,    16,   323,   333,   331,   337,   148,   338,   336,   334,
     149,   340,   138,   134,   136,   147,     0,   150,   128,   131,
       0,     0,     0,     0,   155,     0,   160,     0,   212,   221,
     198,    46,   216,    92,    95,   291,   295,   289,   290,   298,
      94,   305,   111,    28,    54,   117,   122,   119,   177,   280,
     284,   288,    18,   335,   139,   135,   137,   151,     0,     0,
     157,   158,     0,   162,     0,     0,   199,   200,   202,   204,
     222,   296,    97,   118,   116,   121,   140,   166,   167,     0,
     152,   156,   161,   164,   163,   165,   201,   205,   203,   141,
     153,   206,   142,   143,     0,   145
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,   201,   196,   202,   309,    65,    66,    67,
     323,   324,    68,   170,   171,   172,   165,   363,   364,   167,
     359,   168,   176,   289,   373,   374,   375,   376,    69,    70,
      71,    72,    73,    74,    75,    76,   297,   187,   188,   189,
      77,    78,    79,    80,    81,    82,    83,    84,   239,   244,
     249,    85,    86,    87,   240,   245,   250,   386,   387,   204,
      88,    89,    90,    91,    92,    93,    94,    95,   148,   149,
     193,   150,   151,   152,   173,   360,    97,   270
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -295
static const yytype_int16 yypact[] =
{
     879,  -295,  -295,  -295,   -51,   469,  1133,   168,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,    11,    11,    11,    26,  1559,
    1423,    34,  1710,    14,   388,  1423,  -295,  -295,  1627,  -295,
    -295,  -295,  -295,   265,    43,  1724,   124,  -295,  -295,  -295,
    -295,  -295,  -295,    48,    -6,    23,    50,  -295,    44,   947,
    -295,  1491,   542,  -295,   176,    26,    63,  -295,    66,    50,
     215,  -295,  -295,   100,  -295,    26,    81,   176,    51,   337,
     413,   299,    86,   107,   123,    26,    26,    26,   176,  -295,
    -295,   176,  -295,    11,  -295,    92,  1759,   743,  -295,   125,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,   116,  -295,
    -295,  -295,  -295,   138,   140,   147,   162,   180,  -295,  1287,
    1423,  -295,  -295,  -295,  -295,  -295,   172,   151,  -295,   190,
     192,  -295,    27,  -295,  -295,   194,  -295,   176,   564,  -295,
      90,  -295,   176,    11,   623,  -295,   198,    26,   129,   176,
    -295,  -295,  -295,  1662,  1554,   176,  -295,  -295,    26,   150,
    -295,   265,  -295,   211,    52,  -295,  -295,  1015,  -295,  -295,
    -295,  -295,  -295,   947,    61,  -295,   563,  -295,   213,   149,
    -295,   217,    81,   338,    26,  -295,  -295,    81,    50,  -295,
     176,  -295,  -295,    50,  -295,   129,  -295,   230,  -295,   210,
      26,  -295,   352,  -295,   216,    26,  -295,   269,  -295,   221,
      26,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
      11,  1083,  -295,    26,    26,    26,    26,    50,  -295,   606,
    -295,  -295,   234,   674,  -295,   811,   183,  -295,  -295,  -295,
     243,  -295,   191,  1559,  1151,  1423,  1423,  -295,  -295,   204,
    -295,  -295,  -295,   244,  -295,   197,    26,  -295,  -295,  -295,
     176,    26,  -295,  -295,  -295,  -295,    26,    26,   129,    39,
      39,   270,  -295,   176,  -295,  1423,   200,  -295,    26,  -295,
    -295,  -295,   279,   280,  -295,    81,  -295,    50,  -295,  -295,
    -295,  -295,   261,  -295,  -295,   271,  -295,  -295,   275,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  -295,
     285,  -295,  -295,  -295,  -295,  -295,  -295,  -295,  1355,  -295,
    -295,  -295,   288,     6,  -295,  -295,  1355,  -295,  -295,  -295,
     143,  1559,  1423,   290,  -295,   201,  -295,   295,  -295,  -295,
     206,  -295,   233,  -295,  -295,  -295,    39,  -295,  -295,  -295,
      26,  -295,  -295,  -295,   280,    18,  -295,    50,  -295,  -295,
    -295,  -295,  -295,  -295,   309,  -295,  -295,  -295,  1423,  1423,
     151,   192,   188,  -295,    36,  1219,  -295,   169,   311,  -295,
    -295,  -295,  -295,  -295,  -295,  -295,   312,  -295,  -295,   307,
    -295,  -295,  -295,  -295,  -295,  -295,  -295,   316,  -295,   317,
    -295,  -295,   318,   323,   626,  -295
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -295,  -295,  -295,   118,  -295,    45,   232,  -295,   273,  -295,
     276,   287,  -295,  -295,   133,  -295,  -295,  -295,  -295,   113,
      31,   -41,   294,   -11,    77,  -295,  -295,  -295,     2,     9,
    -276,    84,  -295,  -295,  -295,   -48,  -295,   -43,  -295,  -295,
     340,  -295,  -295,  -295,   -36,    -7,  -231,   187,  -295,   184,
     351,  -295,  -295,  -295,  -295,     1,    -1,    -3,   156,   142,
     152,   -32,   -29,   -27,  -295,  -295,  -295,  -294,   199,  -295,
    -295,  -295,  -295,  -295,  -295,   292,   249,   -12,  -295,   121,
    -295,     0,  -295,   365,     7,    -4,  -295,  -259
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -328
static const yytype_int16 yytable[] =
{
      96,   154,   186,   157,   331,   156,   190,   155,   367,   191,
     351,   192,   153,   162,   163,   164,   389,     1,     2,   405,
      98,     1,     2,    -9,   211,     5,   174,   182,   166,     1,
       2,   183,   178,   180,   180,   286,   169,   175,   194,     1,
       2,     5,     1,     2,   203,   205,     1,     2,   209,   198,
      23,    24,   161,   370,   210,     1,     2,   213,   307,    96,
     263,    96,     4,   227,   264,   222,   230,   265,   315,   266,
     180,   223,   160,   180,   224,   231,   362,   385,   248,   243,
     238,   259,   403,    -9,    52,   254,   255,   256,    52,   423,
     407,   402,   421,     4,   308,     5,    52,    96,   199,     8,
       9,   212,   251,     1,     2,   235,    52,   308,   200,    52,
       8,     9,     4,    52,   225,   226,  -325,   272,   229,  -325,
    -325,   232,    52,   252,   273,  -325,    23,    24,  -325,  -325,
    -325,   274,     1,     2,  -325,  -325,    23,    24,     5,   253,
     271,   262,     8,     9,   263,   275,   264,   276,   293,   265,
     408,   266,  -325,  -325,   409,   280,   281,   186,   186,   283,
     287,   190,   190,   277,   191,   191,   192,   192,    -7,   158,
      52,   292,     1,     2,   436,   159,  -325,   207,   278,   160,
       4,   157,   296,   156,   282,   155,  -325,   295,   298,   429,
       5,     1,     2,   194,     8,     9,   279,   284,   303,    52,
     285,   430,   413,   313,   311,   370,   288,     1,     2,   414,
    -159,   416,   417,    96,   294,   304,   418,   306,     1,     2,
     317,   370,   316,   322,   325,   318,   333,     4,    -7,     5,
     371,   372,   336,     8,     9,   298,   236,   339,   238,    52,
     334,   243,    23,    24,   248,   337,   371,   372,   341,   352,
     340,    23,    24,    10,   361,   -15,    13,   344,    52,    16,
     378,   362,    19,   345,   346,   347,   348,   262,   379,   154,
     195,   393,   326,   328,    52,   246,   390,   399,   330,     5,
     153,   174,   369,     8,     9,    52,   261,   400,   395,   377,
     169,   401,   197,   404,    12,   208,   380,    15,   268,   412,
      18,   382,   415,    21,   420,   246,   383,   384,   298,   388,
     388,   392,   349,   220,   426,   247,   438,   439,   325,   228,
     440,   441,   442,   443,    12,   233,   234,    15,   444,   269,
      18,   314,   215,    21,   305,   216,   321,   257,  -104,   319,
     258,     1,     2,   236,   419,   267,  -104,  -104,   217,   394,
    -104,  -104,  -104,   237,   424,   214,  -104,  -104,   241,   221,
      10,   411,   368,    13,   431,   406,    16,   365,   174,    19,
     396,   432,   398,   179,  -104,  -104,    11,   300,   169,    14,
     410,   301,    17,   177,   335,    20,   388,   260,  -185,   181,
     422,     1,     2,   332,   354,   325,  -185,  -185,  -104,   338,
    -185,  -185,  -185,   310,   427,   428,  -185,  -185,    52,   320,
     206,   435,   377,     0,   377,     0,     0,   437,     0,   241,
       0,     0,     0,     0,    23,    24,   290,     0,     0,   242,
       0,   291,     0,     0,     0,     0,     0,    11,   299,     0,
      14,     0,   425,    17,   302,     0,    20,     0,  -185,     0,
     197,     0,     0,     0,     0,     0,   312,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -327,
      99,   267,     1,     2,     0,     0,   327,  -327,  -327,   329,
       0,  -327,  -327,  -327,   100,     0,   101,  -327,  -327,   102,
       0,     0,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,  -327,   117,   118,   119,
       0,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    52,
     147,     0,   -23,   218,     0,   -23,   -23,     0,     0,   381,
     -23,   219,     0,     0,   -23,   -23,   -23,     0,     0,     0,
     -23,   -23,   391,   -22,   218,     0,   -22,   -22,     0,     0,
       0,   -22,   219,     0,   397,   -22,   -22,   -22,   -23,   -23,
       6,   -22,   -22,     0,     0,     0,     0,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   -22,
     -22,     0,   -23,     0,     0,     0,   -20,   350,     0,   -20,
     -20,     0,   -23,     0,   -20,   -20,     0,     0,   -20,   268,
     -20,     0,     0,   -22,   -20,   -20,  -144,   445,     0,  -144,
    -144,     0,     0,   -22,  -144,  -144,     0,     0,  -144,  -144,
    -144,     0,   -20,   -20,  -144,  -144,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,    23,
      24,     0,  -144,  -144,     0,     0,   -20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -20,     1,     2,     0,
       0,     0,     0,     0,     0,     0,  -144,     0,     0,     0,
       0,   101,     0,     0,   102,     0,  -144,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,   117,   118,   119,     0,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,    52,   353,     1,     2,     0,     0,
       0,     0,     3,     0,     0,     4,   268,     5,     0,     6,
     214,     8,     9,     0,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,     0,     0,     0,     0,    39,
      40,    41,    42,     0,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,     1,     2,   355,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   356,   357,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   358,   117,
     118,   119,     0,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    52,     1,     2,     0,     0,     0,     0,     3,     0,
       0,     4,     0,     5,     0,     6,     7,     8,     9,     0,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,     0,     0,     0,     0,    39,    40,    41,    42,     0,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       1,     2,     0,     0,     0,     0,     3,     0,     0,     4,
       0,     5,     0,     6,   214,     8,     9,     0,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,     0,
       0,     0,     0,    39,    40,    41,    42,     0,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     0,   101,     8,     9,   102,     0,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    52,     1,     2,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,     0,   343,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,     0,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    52,     1,     2,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   101,     0,
       0,   102,   356,     0,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   366,   117,
     118,   119,     0,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     0,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,    52,     1,     2,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
       0,   434,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,     0,   117,   118,   119,
       0,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    52,
       1,     2,     0,     0,     0,     0,     0,     0,     0,   -14,
       0,     0,     0,     0,   101,     0,     0,   102,     0,     0,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,     0,   117,   118,   119,     0,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,    52,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   101,     0,     0,   102,   356,     0,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,     0,   117,   118,   119,     0,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     0,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,    52,     1,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,     0,     0,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
       0,   117,   118,   119,     0,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,    52,     1,     2,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     5,     0,     6,   214,     8,
       9,     0,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
       0,     0,    38,     0,     0,     0,     0,    39,    40,    41,
      42,     0,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,     1,     2,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,     0,     0,   101,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
      23,    24,     0,     0,     0,     0,     0,   117,   118,   119,
       0,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     0,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,    52,
       1,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,     0,     0,     0,     0,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,    23,    24,     1,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,   185,
       0,     0,     0,     0,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    52,    23,    24,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     2,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     1,     2,     0,
       0,     0,    52,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     0,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,     0,     0,     0,     0,   261,     0,     0,     0,
       0,     4,     0,     5,     0,     6,     0,     8,     9,     0,
      52,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    52,    23,    24
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-295))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,     5,    38,     6,   235,     6,    38,     6,   284,    38,
     269,    38,     5,    25,    26,    27,   310,     3,     4,    13,
      71,     3,     4,     0,     1,    14,    30,    34,    28,     3,
       4,    35,    32,    33,    34,     8,    29,     3,    38,     3,
       4,    14,     3,     4,    44,    45,     3,     4,     0,     6,
      36,    37,     7,    17,    60,     3,     4,    13,     6,    59,
      96,    61,    12,    70,    96,    65,    73,    96,     7,    96,
      70,     8,    11,    73,     8,    75,    70,   308,    81,    80,
      79,    93,   358,    60,    70,    85,    86,    87,    70,    71,
     366,   350,   386,    12,    55,    14,    70,    97,    55,    18,
      19,    56,    16,     3,     4,    54,    70,    55,    65,    70,
      18,    19,    12,    70,    69,    70,     0,     1,    73,     3,
       4,    76,    70,    16,     8,     9,    36,    37,    12,    13,
      14,    15,     3,     4,    18,    19,    36,    37,    14,    16,
      15,    96,    18,    19,   180,     7,   178,     7,   184,   178,
       7,   178,    36,    37,    11,   159,   160,   193,   194,     8,
     172,   193,   194,    16,   193,   194,   193,   194,     0,     1,
      70,   183,     3,     4,     5,     7,    60,    53,    16,    11,
      12,   184,    53,   184,    12,   184,    70,   187,   188,     1,
      14,     3,     4,   193,    18,    19,    16,     7,   198,    70,
       8,    13,     1,   207,   204,    17,    12,     3,     4,     8,
       9,     5,     6,   213,    16,    65,    10,     6,     3,     4,
      71,    17,     9,   223,   224,     8,    16,    12,    60,    14,
      42,    43,    16,    18,    19,   235,     6,    16,   237,    70,
     240,   242,    36,    37,   247,   245,    42,    43,   260,    15,
     250,    36,    37,    23,    71,    12,    26,   261,    70,    29,
      16,    70,    32,   263,   264,   265,   266,   222,    71,   273,
       5,    71,   227,   228,    70,     6,     6,    16,   233,    14,
     273,   285,   286,    18,    19,    70,     7,    16,     8,   289,
     283,    16,    43,     5,    25,    46,   296,    28,    13,     9,
      31,   301,     7,    34,    71,     6,   306,   307,   308,   309,
     310,   315,   267,    64,     5,    16,     5,     5,   318,    70,
      13,     5,     5,     5,    25,    76,    77,    28,     5,    97,
      31,   213,    59,    34,   201,    59,   223,    88,     0,     1,
      91,     3,     4,     6,   380,    96,     8,     9,    61,   318,
      12,    13,    14,    16,   395,    17,    18,    19,     6,    65,
      23,   372,   285,    26,   412,   363,    29,   283,   372,    32,
     325,   414,   327,    33,    36,    37,    24,   193,   371,    27,
     371,   194,    30,    32,   242,    33,   386,    95,     0,     1,
     390,     3,     4,   237,   273,   395,     8,     9,    60,   247,
      12,    13,    14,   204,   408,   409,    18,    19,    70,    71,
      45,   415,   412,    -1,   414,    -1,    -1,   417,    -1,     6,
      -1,    -1,    -1,    -1,    36,    37,   177,    -1,    -1,    16,
      -1,   182,    -1,    -1,    -1,    -1,    -1,    24,   189,    -1,
      27,    -1,   397,    30,   195,    -1,    33,    -1,    60,    -1,
     201,    -1,    -1,    -1,    -1,    -1,   207,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,   222,     3,     4,    -1,    -1,   227,     8,     9,   230,
      -1,    12,    13,    14,    15,    -1,    17,    18,    19,    20,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,     0,     1,    -1,     3,     4,    -1,    -1,   300,
       8,     9,    -1,    -1,    12,    13,    14,    -1,    -1,    -1,
      18,    19,   313,     0,     1,    -1,     3,     4,    -1,    -1,
      -1,     8,     9,    -1,   325,    12,    13,    14,    36,    37,
      16,    18,    19,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    36,
      37,    -1,    60,    -1,    -1,    -1,     0,     1,    -1,     3,
       4,    -1,    70,    -1,     8,     9,    -1,    -1,    12,    13,
      14,    -1,    -1,    60,    18,    19,     0,     1,    -1,     3,
       4,    -1,    -1,    70,     8,     9,    -1,    -1,    12,    13,
      14,    -1,    36,    37,    18,    19,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    36,
      37,    -1,    36,    37,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,     3,     4,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    17,    -1,    -1,    20,    -1,    70,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    -1,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    -1,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,     3,     4,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    12,    13,    14,    -1,    16,
      17,    18,    19,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,    56,
      57,    58,    59,    -1,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,     3,     4,     5,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    12,    -1,    14,    -1,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    12,
      -1,    14,    -1,    16,    17,    18,    19,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    -1,
      -1,    -1,    -1,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,     3,     4,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    17,    -1,
      -1,    20,    21,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    17,    -1,    -1,    20,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,    -1,    20,    21,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,     3,     4,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    -1,    -1,    20,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    -1,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,     3,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    14,    -1,    16,    17,    18,
      19,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    17,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    -1,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    36,    37,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    52,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    70,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    16,     3,     4,    -1,
      -1,    -1,    70,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    -1,     7,    -1,    -1,    -1,
      -1,    12,    -1,    14,    -1,    16,    -1,    18,    19,    -1,
      70,    -1,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    70,    36,    37
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
      68,    69,    70,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    89,    90,    91,    94,   110,
     111,   112,   113,   114,   115,   116,   117,   122,   123,   124,
     125,   126,   127,   128,   129,   133,   134,   135,   142,   143,
     144,   145,   146,   147,   148,   149,   153,   158,    71,     1,
      15,    17,    20,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    38,    39,    40,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    71,   150,   151,
     153,   154,   155,   156,   157,   127,   128,   129,     1,     7,
      11,    77,   149,   149,   149,    98,   153,   101,   103,   156,
      95,    96,    97,   156,   157,     3,   104,   122,   153,   112,
     153,     1,   117,   157,    16,    52,   116,   119,   120,   121,
     133,   134,   135,   152,   153,     5,    86,   148,     6,    55,
      65,    85,    87,   153,   141,   153,   155,    53,   148,     0,
      60,     1,    77,    13,    17,    80,    82,    83,     1,     9,
     148,    94,   153,     8,     8,    77,    77,   117,   148,    77,
     117,   153,    77,   148,   148,    54,     6,    16,   127,   130,
     136,     6,    16,   128,   131,   137,     6,    16,   129,   132,
     138,    16,    16,    16,   153,   153,   153,   148,   148,   149,
     147,     7,    77,   116,   133,   134,   135,   148,    13,    78,
     159,    15,     1,     8,    15,     7,     7,    16,    16,    16,
     157,   157,    12,     8,     7,     8,     8,   149,    12,   105,
     148,   148,   149,   116,    16,   153,    53,   118,   153,   148,
     121,   119,   148,   153,    65,    86,     6,     6,    55,    88,
     140,   153,   148,   157,    75,     7,     9,    71,     8,     1,
      71,    91,   153,    92,    93,   153,    77,   148,    77,   148,
      77,   118,   130,    16,   153,   131,    16,   153,   132,    16,
     153,   149,     5,    22,   157,   153,   153,   153,   153,    77,
       1,   159,    15,    71,   151,     5,    21,    22,    37,   102,
     157,    71,    70,    99,   100,   103,    37,   102,    96,   157,
      17,    42,    43,   106,   107,   108,   109,   153,    16,    71,
     153,   148,   153,   153,   153,   118,   139,   140,   153,   139,
       6,   148,   157,    71,    92,     8,    77,   148,    77,    16,
      16,    16,   159,   102,     5,    13,   100,   102,     7,    11,
     101,    95,     9,     1,     8,     7,     5,     6,    10,   116,
      71,   139,   153,    71,    93,    77,     5,   157,   157,     1,
      13,   107,   109,     5,    22,   157,     5,   153,     5,     5,
      13,     5,     5,     5,     5,     1
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
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2214 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2223 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2232 "colorsyntax.cc"
	break;
      case 17: /* "TOK_MSC" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2241 "colorsyntax.cc"
	break;
      case 18: /* "TOK_COLON_STRING" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2250 "colorsyntax.cc"
	break;
      case 19: /* "TOK_COLON_QUOTED_STRING" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2259 "colorsyntax.cc"
	break;
      case 20: /* "TOK_STYLE_NAME" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2268 "colorsyntax.cc"
	break;
      case 21: /* "TOK_COLORDEF" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2277 "colorsyntax.cc"
	break;
      case 22: /* "TOK_BOOLEAN" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2286 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_HEADING" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2295 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_NUDGE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2304 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_NEWPAGE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2313 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_DEFSHAPE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2322 "colorsyntax.cc"
	break;
      case 42: /* "TOK_COMMAND_DEFCOLOR" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2331 "colorsyntax.cc"
	break;
      case 43: /* "TOK_COMMAND_DEFSTYLE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2340 "colorsyntax.cc"
	break;
      case 44: /* "TOK_COMMAND_DEFDESIGN" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2349 "colorsyntax.cc"
	break;
      case 45: /* "TOK_COMMAND_BIG" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2358 "colorsyntax.cc"
	break;
      case 46: /* "TOK_COMMAND_BOX" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2367 "colorsyntax.cc"
	break;
      case 47: /* "TOK_COMMAND_PIPE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2376 "colorsyntax.cc"
	break;
      case 48: /* "TOK_COMMAND_MARK" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2385 "colorsyntax.cc"
	break;
      case 49: /* "TOK_COMMAND_PARALLEL" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2394 "colorsyntax.cc"
	break;
      case 50: /* "TOK_COMMAND_OVERLAP" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2403 "colorsyntax.cc"
	break;
      case 51: /* "TOK_VERTICAL" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2412 "colorsyntax.cc"
	break;
      case 52: /* "TOK_VERTICAL_SHAPE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2421 "colorsyntax.cc"
	break;
      case 53: /* "TOK_AT" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2430 "colorsyntax.cc"
	break;
      case 54: /* "TOK_LOST" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2439 "colorsyntax.cc"
	break;
      case 55: /* "TOK_AT_POS" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2448 "colorsyntax.cc"
	break;
      case 56: /* "TOK_SHOW" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2457 "colorsyntax.cc"
	break;
      case 57: /* "TOK_HIDE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2466 "colorsyntax.cc"
	break;
      case 58: /* "TOK_ACTIVATE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2475 "colorsyntax.cc"
	break;
      case 59: /* "TOK_DEACTIVATE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2484 "colorsyntax.cc"
	break;
      case 60: /* "TOK_BYE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2493 "colorsyntax.cc"
	break;
      case 61: /* "TOK_COMMAND_VSPACE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2502 "colorsyntax.cc"
	break;
      case 62: /* "TOK_COMMAND_HSPACE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2511 "colorsyntax.cc"
	break;
      case 63: /* "TOK_COMMAND_SYMBOL" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2520 "colorsyntax.cc"
	break;
      case 64: /* "TOK_COMMAND_NOTE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2529 "colorsyntax.cc"
	break;
      case 65: /* "TOK_COMMAND_COMMENT" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2538 "colorsyntax.cc"
	break;
      case 66: /* "TOK_COMMAND_ENDNOTE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2547 "colorsyntax.cc"
	break;
      case 67: /* "TOK_COMMAND_FOOTNOTE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2556 "colorsyntax.cc"
	break;
      case 68: /* "TOK_COMMAND_TITLE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2565 "colorsyntax.cc"
	break;
      case 69: /* "TOK_COMMAND_SUBTITLE" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2574 "colorsyntax.cc"
	break;
      case 75: /* "top_level_arclist" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2583 "colorsyntax.cc"
	break;
      case 77: /* "braced_arclist" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2592 "colorsyntax.cc"
	break;
      case 78: /* "arclist_maybe_no_semicolon" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2601 "colorsyntax.cc"
	break;
      case 79: /* "arclist" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2610 "colorsyntax.cc"
	break;
      case 80: /* "arc_with_parallel_semicolon" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2619 "colorsyntax.cc"
	break;
      case 81: /* "overlap_or_parallel" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2628 "colorsyntax.cc"
	break;
      case 82: /* "arc_with_parallel" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2637 "colorsyntax.cc"
	break;
      case 83: /* "arc" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2646 "colorsyntax.cc"
	break;
      case 84: /* "titlecommandtoken" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2655 "colorsyntax.cc"
	break;
      case 85: /* "hspace_location" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2664 "colorsyntax.cc"
	break;
      case 86: /* "full_arcattrlist_with_label_or_number" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2673 "colorsyntax.cc"
	break;
      case 87: /* "entityrel" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2682 "colorsyntax.cc"
	break;
      case 88: /* "markerrel_no_string" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->namerel);};

/* Line 1391 of yacc.c  */
#line 2691 "colorsyntax.cc"
	break;
      case 89: /* "entity_command_prefixes" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2700 "colorsyntax.cc"
	break;
      case 90: /* "optlist" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};

/* Line 1391 of yacc.c  */
#line 2709 "colorsyntax.cc"
	break;
      case 91: /* "opt" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2718 "colorsyntax.cc"
	break;
      case 92: /* "entitylist" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2727 "colorsyntax.cc"
	break;
      case 93: /* "entity" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2736 "colorsyntax.cc"
	break;
      case 94: /* "first_entity" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};

/* Line 1391 of yacc.c  */
#line 2745 "colorsyntax.cc"
	break;
      case 97: /* "tok_stringlist" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};

/* Line 1391 of yacc.c  */
#line 2754 "colorsyntax.cc"
	break;
      case 102: /* "color_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2763 "colorsyntax.cc"
	break;
      case 110: /* "parallel" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};

/* Line 1391 of yacc.c  */
#line 2772 "colorsyntax.cc"
	break;
      case 111: /* "box_list" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcboxseries);};

/* Line 1391 of yacc.c  */
#line 2781 "colorsyntax.cc"
	break;
      case 112: /* "first_box" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2790 "colorsyntax.cc"
	break;
      case 113: /* "first_pipe" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipe);};

/* Line 1391 of yacc.c  */
#line 2799 "colorsyntax.cc"
	break;
      case 114: /* "pipe_list_no_content" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2808 "colorsyntax.cc"
	break;
      case 115: /* "pipe_list" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcpipeseries);};

/* Line 1391 of yacc.c  */
#line 2817 "colorsyntax.cc"
	break;
      case 117: /* "boxrel" */

/* Line 1391 of yacc.c  */
#line 174 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbox);};

/* Line 1391 of yacc.c  */
#line 2826 "colorsyntax.cc"
	break;
      case 118: /* "vertxpos" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};

/* Line 1391 of yacc.c  */
#line 2835 "colorsyntax.cc"
	break;
      case 120: /* "vertrel_no_xpos" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2844 "colorsyntax.cc"
	break;
      case 121: /* "vertrel" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcvertarrow);};

/* Line 1391 of yacc.c  */
#line 2853 "colorsyntax.cc"
	break;
      case 122: /* "arcrel" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2862 "colorsyntax.cc"
	break;
      case 123: /* "arcrel_arrow" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2871 "colorsyntax.cc"
	break;
      case 124: /* "arcrel_to" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2880 "colorsyntax.cc"
	break;
      case 125: /* "arcrel_from" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2889 "colorsyntax.cc"
	break;
      case 126: /* "arcrel_bidir" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};

/* Line 1391 of yacc.c  */
#line 2898 "colorsyntax.cc"
	break;
      case 139: /* "extvertxpos" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->extvertxpos);};

/* Line 1391 of yacc.c  */
#line 2907 "colorsyntax.cc"
	break;
      case 140: /* "extvertxpos_no_string" */

/* Line 1391 of yacc.c  */
#line 176 "language.yy"
	{if (!C_S_H) delete (yyvaluep->extvertxpos);};

/* Line 1391 of yacc.c  */
#line 2916 "colorsyntax.cc"
	break;
      case 141: /* "symbol_type_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2925 "colorsyntax.cc"
	break;
      case 142: /* "symbol_command_no_attr" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2934 "colorsyntax.cc"
	break;
      case 143: /* "symbol_command" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2943 "colorsyntax.cc"
	break;
      case 144: /* "note" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2952 "colorsyntax.cc"
	break;
      case 146: /* "comment" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 2961 "colorsyntax.cc"
	break;
      case 147: /* "colon_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 2970 "colorsyntax.cc"
	break;
      case 148: /* "full_arcattrlist_with_label" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2979 "colorsyntax.cc"
	break;
      case 149: /* "full_arcattrlist" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2988 "colorsyntax.cc"
	break;
      case 150: /* "arcattrlist" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};

/* Line 1391 of yacc.c  */
#line 2997 "colorsyntax.cc"
	break;
      case 151: /* "arcattr" */

/* Line 1391 of yacc.c  */
#line 175 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};

/* Line 1391 of yacc.c  */
#line 3006 "colorsyntax.cc"
	break;
      case 153: /* "entity_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3015 "colorsyntax.cc"
	break;
      case 154: /* "reserved_word_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3024 "colorsyntax.cc"
	break;
      case 155: /* "symbol_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3033 "colorsyntax.cc"
	break;
      case 156: /* "alpha_string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3042 "colorsyntax.cc"
	break;
      case 157: /* "string" */

/* Line 1391 of yacc.c  */
#line 177 "language.yy"
	{free((yyvaluep->str));};

/* Line 1391 of yacc.c  */
#line 3051 "colorsyntax.cc"
	break;
      case 159: /* "scope_close" */

/* Line 1391 of yacc.c  */
#line 173 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};

/* Line 1391 of yacc.c  */
#line 3060 "colorsyntax.cc"
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
#line 3217 "colorsyntax.cc"
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
        case 3:

/* Line 1806 of yacc.c  */
#line 245 "language.yy"
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
#line 255 "language.yy"
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
#line 266 "language.yy"
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
#line 277 "language.yy"
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
#line 287 "language.yy"
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
#line 297 "language.yy"
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
#line 307 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
  #endif
}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 314 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    CshPos pos = (yylsp[(2) - (2)]);
    if (((yylsp[(1) - (2)])).last_pos >= ((yylsp[(2) - (2)])).first_pos)
        pos.first_pos = ((yylsp[(1) - (2)])).last_pos;
    csh.AddCSH_Error(pos, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).start, "Could not recognize this as a valid line.");
  #endif
  YYACCEPT;
}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 329 "language.yy"
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
#line 338 "language.yy"
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
#line 352 "language.yy"
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
#line 364 "language.yy"
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

  case 16:

/* Line 1806 of yacc.c  */
#line 382 "language.yy"
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

/* Line 1806 of yacc.c  */
#line 392 "language.yy"
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
#line 405 "language.yy"
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

/* Line 1806 of yacc.c  */
#line 418 "language.yy"
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
#line 430 "language.yy"
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
#line 444 "language.yy"
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

/* Line 1806 of yacc.c  */
#line 455 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a semicolon (';').");
  #else
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).start, MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 470 "language.yy"
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

/* Line 1806 of yacc.c  */
#line 479 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(2) - (2)].arcbase)) ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
  #endif
}
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 487 "language.yy"
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
#line 503 "language.yy"
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
#line 515 "language.yy"
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

  case 33:

/* Line 1806 of yacc.c  */
#line 532 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        if ((yyvsp[(1) - (1)].arcbase)) ((yyvsp[(1) - (1)].arcbase))->SetLineEnd(MSC_POS((yyloc)));
        (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
  #endif
}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 540 "language.yy"
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

  case 35:

/* Line 1806 of yacc.c  */
#line 565 "language.yy"
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

  case 36:

/* Line 1806 of yacc.c  */
#line 584 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    ((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase)=((yyvsp[(1) - (1)].arcbase));
  #endif
}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 592 "language.yy"
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

  case 38:

/* Line 1806 of yacc.c  */
#line 604 "language.yy"
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

  case 39:

/* Line 1806 of yacc.c  */
#line 616 "language.yy"
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

  case 40:

/* Line 1806 of yacc.c  */
#line 631 "language.yy"
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

  case 41:

/* Line 1806 of yacc.c  */
#line 649 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "brace", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "bracket", HINT_KEYWORD));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "range", HINT_KEYWORD));
    }
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing a marker.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing a marker.");
  #endif
    free((yyvsp[(1) - (1)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 665 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(2) - (2)]), COLOR_KEYWORD);
    csh.CheckHintAfterPlusOne((yylsp[(2) - (2)]), yylloc, yychar==YYEOF, HINT_MARKER);
    csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), "Missing a marker.");
  #else
    msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end.NextChar(), "Missing a marker.");
  #endif
    free((yyvsp[(1) - (2)].str));
    (yyval.arcbase) = NULL;
    (yyvsp[(2) - (2)].vshape); //to suppress bison warning of unused parameter
}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 679 "language.yy"
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

  case 44:

/* Line 1806 of yacc.c  */
#line 692 "language.yy"
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

  case 45:

/* Line 1806 of yacc.c  */
#line 706 "language.yy"
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

  case 46:

/* Line 1806 of yacc.c  */
#line 723 "language.yy"
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

  case 47:

/* Line 1806 of yacc.c  */
#line 741 "language.yy"
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

  case 48:

/* Line 1806 of yacc.c  */
#line 753 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc, false));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 761 "language.yy"
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

  case 50:

/* Line 1806 of yacc.c  */
#line 773 "language.yy"
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

  case 51:

/* Line 1806 of yacc.c  */
#line 785 "language.yy"
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

  case 52:

/* Line 1806 of yacc.c  */
#line 798 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    ((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist));
    CommandEntity *ce = new CommandEntity((yyvsp[(3) - (3)].entitylist), &msc, false);
    delete ((yyvsp[(1) - (3)].entitylist));
    ce->AddAttributeList(NULL);
    (yyval.arcbase) = ce;
  #endif
}
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 811 "language.yy"
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

  case 54:

/* Line 1806 of yacc.c  */
#line 827 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne((yylsp[(1) - (4)]), (yylsp[(2) - (4)]));
    csh.CheckEntityHintAtAndBefore((yylsp[(3) - (4)]), (yylsp[(4) - (4)]));
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

  case 55:

/* Line 1806 of yacc.c  */
#line 843 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in an "CommandArcList" element used only for this. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new CommandArcList(&msc, (yyvsp[(1) - (1)].arclist)));
        ((yyval.arcbase))->AddAttributeList(NULL);
    } else
        (yyval.arcbase) = NULL;
  #endif
}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 856 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcboxseries); //to remove warning for downcast
  #endif
}
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 863 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcpipeseries); //to remove warning for downcast
  #endif
}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 870 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcparallel));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 878 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 887 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 896 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 905 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = NULL;
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 914 "language.yy"
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

  case 64:

/* Line 1806 of yacc.c  */
#line 924 "language.yy"
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

  case 65:

/* Line 1806 of yacc.c  */
#line 938 "language.yy"
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

  case 66:

/* Line 1806 of yacc.c  */
#line 948 "language.yy"
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

  case 67:

/* Line 1806 of yacc.c  */
#line 962 "language.yy"
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

  case 68:

/* Line 1806 of yacc.c  */
#line 979 "language.yy"
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

  case 69:

/* Line 1806 of yacc.c  */
#line 992 "language.yy"
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

  case 70:

/* Line 1806 of yacc.c  */
#line 1009 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true, NULL));
    ((yyval.arcbase))->AddAttributeList(NULL);
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 1019 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, (yylsp[(2) - (2)])))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    (yyval.arcbase) = (new CommandNewpage(&msc, true, NULL));
    ((yyval.arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 1036 "language.yy"
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

  case 76:

/* Line 1806 of yacc.c  */
#line 1050 "language.yy"
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

  case 77:

/* Line 1806 of yacc.c  */
#line 1065 "language.yy"
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

  case 78:

/* Line 1806 of yacc.c  */
#line 1079 "language.yy"
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

  case 79:

/* Line 1806 of yacc.c  */
#line 1093 "language.yy"
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

  case 83:

/* Line 1806 of yacc.c  */
#line 1112 "language.yy"
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

  case 84:

/* Line 1806 of yacc.c  */
#line 1138 "language.yy"
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

  case 85:

/* Line 1806 of yacc.c  */
#line 1156 "language.yy"
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

  case 87:

/* Line 1806 of yacc.c  */
#line 1174 "language.yy"
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

  case 88:

/* Line 1806 of yacc.c  */
#line 1185 "language.yy"
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

  case 89:

/* Line 1806 of yacc.c  */
#line 1196 "language.yy"
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

  case 90:

/* Line 1806 of yacc.c  */
#line 1208 "language.yy"
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

  case 91:

/* Line 1806 of yacc.c  */
#line 1219 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.namerel) = NULL;
  #endif
}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 1228 "language.yy"
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

  case 93:

/* Line 1806 of yacc.c  */
#line 1242 "language.yy"
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

  case 94:

/* Line 1806 of yacc.c  */
#line 1267 "language.yy"
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

  case 95:

/* Line 1806 of yacc.c  */
#line 1279 "language.yy"
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

  case 96:

/* Line 1806 of yacc.c  */
#line 1290 "language.yy"
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

  case 97:

/* Line 1806 of yacc.c  */
#line 1300 "language.yy"
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

  case 102:

/* Line 1806 of yacc.c  */
#line 1317 "language.yy"
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

  case 103:

/* Line 1806 of yacc.c  */
#line 1327 "language.yy"
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

  case 104:

/* Line 1806 of yacc.c  */
#line 1346 "language.yy"
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

  case 106:

/* Line 1806 of yacc.c  */
#line 1360 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.AddCSH_Error((yylsp[(3) - (3)]), "An option expected here.");
  #else
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
  #endif
}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 1371 "language.yy"
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

  case 108:

/* Line 1806 of yacc.c  */
#line 1390 "language.yy"
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

  case 109:

/* Line 1806 of yacc.c  */
#line 1409 "language.yy"
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

  case 110:

/* Line 1806 of yacc.c  */
#line 1428 "language.yy"
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

  case 111:

/* Line 1806 of yacc.c  */
#line 1448 "language.yy"
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

  case 112:

/* Line 1806 of yacc.c  */
#line 1467 "language.yy"
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

  case 113:

/* Line 1806 of yacc.c  */
#line 1487 "language.yy"
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

  case 114:

/* Line 1806 of yacc.c  */
#line 1506 "language.yy"
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

  case 115:

/* Line 1806 of yacc.c  */
#line 1527 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.entitylist) = ((yyvsp[(1) - (1)].entitylist));
  #endif
}
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 1533 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore((yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
  #else
    ((yyvsp[(3) - (3)].entitylist))->Prepend((yyvsp[(1) - (3)].entitylist));
    (yyval.entitylist) = (yyvsp[(3) - (3)].entitylist);
    delete ((yyvsp[(1) - (3)].entitylist));
  #endif
}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 1544 "language.yy"
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

  case 119:

/* Line 1806 of yacc.c  */
#line 1559 "language.yy"
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

  case 120:

/* Line 1806 of yacc.c  */
#line 1575 "language.yy"
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

  case 121:

/* Line 1806 of yacc.c  */
#line 1587 "language.yy"
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
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 1603 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt((yylsp[(1) - (2)]));
    csh.AddCSH_EntityName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 1616 "language.yy"
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

  case 124:

/* Line 1806 of yacc.c  */
#line 1634 "language.yy"
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

  case 125:

/* Line 1806 of yacc.c  */
#line 1649 "language.yy"
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
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList((yyvsp[(2) - (3)].attriblist), (yyvsp[(3) - (3)].arclist), MSC_POS((yylsp[(3) - (3)])).start);
  #endif
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 1667 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
      if (csh.CheckHintAt((yylsp[(1) - (2)]), HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp((yyvsp[(1) - (2)].str), &msc);
    ed->SetLineEnd(MSC_POS((yyloc)));
    (yyval.entitylist) = ed->AddAttributeList(NULL, (yyvsp[(2) - (2)].arclist), MSC_POS((yylsp[(2) - (2)])).start);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 1684 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 1691 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *((yyvsp[(1) - (2)].stringlist)))
        if (csh.ForbiddenStyles.find(str) != csh.ForbiddenStyles.end())
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
    delete((yyvsp[(1) - (2)].stringlist));
    delete((yyvsp[(2) - (2)].attriblist));
  #endif
}
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 1740 "language.yy"
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

  case 131:

/* Line 1806 of yacc.c  */
#line 1756 "language.yy"
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

  case 132:

/* Line 1806 of yacc.c  */
#line 1774 "language.yy"
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

  case 133:

/* Line 1806 of yacc.c  */
#line 1784 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yyloc), ("Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (2)].str) ? (yyvsp[(1) - (2)].str): "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS((yyloc)).end, "Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string((yyvsp[(1) - (2)].str) ? (yyvsp[(1) - (2)].str): "") +"'.");
  #endif	
  free((yyvsp[(1) - (2)].str));
}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 1794 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter((yylsp[(3) - (3)]), "Missing a closing brace ('}').");
	csh.AddShapeName((yyvsp[(1) - (3)].str));
  #else
    msc.Error.Error(MSC_POS((yylsp[(3) - (3)])).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS((yylsp[(2) - (3)])).start, MSC_POS((yylsp[(2) - (3)])).end.NextChar(), "Here is the corresponding '{'.");
	if ((yyvsp[(3) - (3)].shape)) {
		msc.Shapes.Add(std::string((yyvsp[(1) - (3)].str)), MSC_POS((yylsp[(1) - (3)])).start, msc.file_url, msc.file_info, (yyvsp[(3) - (3)].shape), msc.Error);
		delete (yyvsp[(3) - (3)].shape);
	}
  #endif	
  free((yyvsp[(1) - (3)].str));
}
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 1810 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_ATTRVALUE);
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

  case 136:

/* Line 1806 of yacc.c  */
#line 1824 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.shape) = new Shape;
	if ((yyvsp[(1) - (1)].shapeelement)) {
		((yyval.shape))->Add(std::move(*((yyvsp[(1) - (1)].shapeelement))));
		free((yyvsp[(1) - (1)].shapeelement));
	}
  #endif	
}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 1834 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
	if ((yyvsp[(2) - (2)].shapeelement)) {
		((yyvsp[(1) - (2)].shape))->Add(std::move(*((yyvsp[(2) - (2)].shapeelement))));
		free((yyvsp[(2) - (2)].shapeelement));
	}
  #endif	
}
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 1844 "language.yy"
    {
    const int num_args = 0;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (1)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (1)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
	if (msg && fail)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
	else 
	    (yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (1)].shapecommand));
  #endif	
}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 1861 "language.yy"
    {
    const int num_args = 1;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (2)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (2)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yylsp[(2) - (2)]), msg);
	else if ((yyvsp[(1) - (2)].shapecommand)>=ShapeElement::SECTION_BG && (((yyvsp[(2) - (2)].str))[0]<'0' || ((yyvsp[(2) - (2)].str))[0]>'2' || ((yyvsp[(2) - (2)].str))[1]!=0))
		csh.AddCSH_Error((yylsp[(2) - (2)]), "S (section) commands require an integer between 0 and 2.");
  #else
	(yyval.shapeelement) = NULL;
	const double a = atof((yyvsp[(2) - (2)].str));
	if (msg && fail)
		msc.Error.Error(MSC_POS((yylsp[(2) - (2)])).end, msg + string(" Ignoring line."));
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

  case 140:

/* Line 1806 of yacc.c  */
#line 1886 "language.yy"
    {
    const int num_args = 2;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (3)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (3)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (3)].shapecommand), atof((yyvsp[(2) - (3)].str)), atof((yyvsp[(3) - (3)].str)));
  #endif	
  free((yyvsp[(2) - (3)].str));
  free((yyvsp[(3) - (3)].str));
}
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 1905 "language.yy"
    {
    const int num_args = 3;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (4)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (4)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (4)].shapecommand), atof((yyvsp[(2) - (4)].str)), atof((yyvsp[(3) - (4)].str)), atof((yyvsp[(4) - (4)].str)));
  #endif	
  free((yyvsp[(2) - (4)].str));
  free((yyvsp[(3) - (4)].str));
  free((yyvsp[(4) - (4)].str));
}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 1925 "language.yy"
    {
    const int num_args = 4;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (5)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (5)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (5)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
	else 
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (5)].shapecommand), atof((yyvsp[(2) - (5)].str)), atof((yyvsp[(3) - (5)].str)), atof((yyvsp[(4) - (5)].str)), atof((yyvsp[(5) - (5)].str)));
  #endif	
  free((yyvsp[(2) - (5)].str));
  free((yyvsp[(3) - (5)].str));
  free((yyvsp[(4) - (5)].str));
  free((yyvsp[(5) - (5)].str));
}
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 1946 "language.yy"
    {
    const int num_args = 5;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (6)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (6)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (6)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
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

  case 144:

/* Line 1806 of yacc.c  */
#line 1968 "language.yy"
    {
    const int num_args = 6;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (7)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (7)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (7)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yyloc), msg);
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
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

  case 145:

/* Line 1806 of yacc.c  */
#line 1991 "language.yy"
    {
    const int num_args = 7;
	const char * const msg = ShapeElement::ErrorMsg((yyvsp[(1) - (8)].shapecommand), num_args);
	const bool fail = ShapeElement::GetNumArgs((yyvsp[(1) - (8)].shapecommand)) > num_args;
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (8)]), COLOR_KEYWORD);
	if (msg)
		csh.AddCSH_ErrorAfter((yylsp[(7) - (8)]), msg);
	else
		csh.AddCSH_Error((yylsp[(8) - (8)]), "Six numbers are enough.");
  #else
	(yyval.shapeelement) = NULL;
	if (msg && fail)
		msc.Error.Error(MSC_POS((yyloc)).end, msg + string(" Ignoring line."));
	else {
	    //msc.Error.Error(MSC_POS(@8).start, "Six numbers are enough. Ignoring the rest.");
		(yyval.shapeelement) = new ShapeElement((yyvsp[(1) - (8)].shapecommand), atof((yyvsp[(2) - (8)].str)), atof((yyvsp[(3) - (8)].str)), atof((yyvsp[(4) - (8)].str)), atof((yyvsp[(5) - (8)].str)), atof((yyvsp[(6) - (8)].str)), atof((yyvsp[(7) - (8)].str)));
	}
  #endif	
  free((yyvsp[(2) - (8)].str));
  free((yyvsp[(3) - (8)].str));
  free((yyvsp[(4) - (8)].str));
  free((yyvsp[(5) - (8)].str));
  free((yyvsp[(6) - (8)].str));
  free((yyvsp[(7) - (8)].str));
}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 2020 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 2029 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor((yyvsp[(3) - (3)].str));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[(1) - (3)].str)] = color;
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 2044 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (4)]), COLOR_COLORNAME);
    csh.AddCSH((yylsp[(2) - (4)]), COLOR_EQUAL);
    csh.AddCSH((yylsp[(3) - (4)]), COLOR_COLORDEF);
    csh.AddCSH((yylsp[(4) - (4)]), COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor("++"+string((yyvsp[(4) - (4)].str)));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[(yyvsp[(1) - (4)].str)] = color;
  #else
    msc.Contexts.back().colors.AddColor((yyvsp[(1) - (4)].str), "++"+string((yyvsp[(4) - (4)].str)), msc.Error, MSC_POS((yyloc)));
  #endif
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 2062 "language.yy"
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

  case 153:

/* Line 1806 of yacc.c  */
#line 2079 "language.yy"
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

  case 154:

/* Line 1806 of yacc.c  */
#line 2099 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 2110 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
  #endif
}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 2117 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 2124 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 2134 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
  #endif
}
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 2140 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error((yylsp[(2) - (2)]), "Extra stuff after design options. Maybe missing a comma?");
  #endif
}
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 2147 "language.yy"
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

  case 164:

/* Line 1806 of yacc.c  */
#line 2166 "language.yy"
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

  case 165:

/* Line 1806 of yacc.c  */
#line 2185 "language.yy"
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

  case 166:

/* Line 1806 of yacc.c  */
#line 2204 "language.yy"
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

  case 167:

/* Line 1806 of yacc.c  */
#line 2226 "language.yy"
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

  case 168:

/* Line 1806 of yacc.c  */
#line 2250 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = new ArcParallel(&msc, (yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
  #endif
}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 2259 "language.yy"
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

  case 170:

/* Line 1806 of yacc.c  */
#line 2271 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(1) - (1)].arcbox));
  #endif
}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 2277 "language.yy"
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

  case 172:

/* Line 1806 of yacc.c  */
#line 2288 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);   
  #else
    (yyval.arcboxseries) = new ArcBoxSeries((yyvsp[(2) - (2)].arcbox));
  #endif
  free((yyvsp[(1) - (2)].str));
}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 2298 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(2) - (2)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow((yyvsp[(2) - (2)].arcbox));
    ((yyvsp[(2) - (2)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 2306 "language.yy"
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

  case 175:

/* Line 1806 of yacc.c  */
#line 2319 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(2) - (3)].arcbox))->AddArcList((yyvsp[(3) - (3)].arclist))->SetLineEnd(MSC_POS((yylsp[(2) - (3)])));
    (yyval.arcboxseries) = ((yyvsp[(1) - (3)].arcboxseries))->AddFollow((yyvsp[(2) - (3)].arcbox));
    ((yyvsp[(2) - (3)].arcbox))->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 2327 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS((yylsp[(1) - (2)])), NULL, MSC_POS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist));
    (yyval.arcboxseries) = ((yyvsp[(1) - (2)].arcboxseries))->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 2336 "language.yy"
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

  case 178:

/* Line 1806 of yacc.c  */
#line 2349 "language.yy"
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

  case 179:

/* Line 1806 of yacc.c  */
#line 2365 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (1)].arcbox))->SetLineEnd(MSC_POS((yyloc)));
    (yyval.arcbox) = (yyvsp[(1) - (1)].arcbox);
  #endif
}
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 2373 "language.yy"
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

  case 181:

/* Line 1806 of yacc.c  */
#line 2386 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (2)].arcbox))->AddAttributeList(NULL);
    ((yyvsp[(1) - (2)].arcbox))->SetLineEnd(MSC_POS((yylsp[(1) - (2)])));
    (yyval.arcbox) = ((yyvsp[(1) - (2)].arcbox))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 2394 "language.yy"
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

  case 183:

/* Line 1806 of yacc.c  */
#line 2409 "language.yy"
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

  case 184:

/* Line 1806 of yacc.c  */
#line 2424 "language.yy"
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

  case 185:

/* Line 1806 of yacc.c  */
#line 2440 "language.yy"
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

  case 186:

/* Line 1806 of yacc.c  */
#line 2451 "language.yy"
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

  case 187:

/* Line 1806 of yacc.c  */
#line 2470 "language.yy"
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

  case 188:

/* Line 1806 of yacc.c  */
#line 2480 "language.yy"
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

  case 189:

/* Line 1806 of yacc.c  */
#line 2494 "language.yy"
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

  case 191:

/* Line 1806 of yacc.c  */
#line 2515 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.arcpipeseries) = ((yyvsp[(1) - (2)].arcpipeseries))->AddArcList((yyvsp[(2) - (2)].arclist));
  #endif
}
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 2524 "language.yy"
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

  case 195:

/* Line 1806 of yacc.c  */
#line 2538 "language.yy"
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

  case 196:

/* Line 1806 of yacc.c  */
#line 2549 "language.yy"
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

  case 197:

/* Line 1806 of yacc.c  */
#line 2561 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    csh.CheckEntityHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcbox) = new ArcBox((yyvsp[(1) - (1)].arctype), NULL, MSC_POS((yylsp[(1) - (1)])), NULL, MSC_POS((yylsp[(1) - (1)])), &msc);
  #endif
}
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 2571 "language.yy"
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

  case 199:

/* Line 1806 of yacc.c  */
#line 2583 "language.yy"
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

  case 200:

/* Line 1806 of yacc.c  */
#line 2597 "language.yy"
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

  case 201:

/* Line 1806 of yacc.c  */
#line 2611 "language.yy"
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

  case 202:

/* Line 1806 of yacc.c  */
#line 2626 "language.yy"
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

  case 203:

/* Line 1806 of yacc.c  */
#line 2639 "language.yy"
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

  case 204:

/* Line 1806 of yacc.c  */
#line 2654 "language.yy"
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

  case 205:

/* Line 1806 of yacc.c  */
#line 2688 "language.yy"
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

  case 206:

/* Line 1806 of yacc.c  */
#line 2704 "language.yy"
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

  case 207:

/* Line 1806 of yacc.c  */
#line 2722 "language.yy"
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

  case 208:

/* Line 1806 of yacc.c  */
#line 2734 "language.yy"
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

  case 209:

/* Line 1806 of yacc.c  */
#line 2750 "language.yy"
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

  case 210:

/* Line 1806 of yacc.c  */
#line 2761 "language.yy"
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

  case 211:

/* Line 1806 of yacc.c  */
#line 2773 "language.yy"
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

  case 212:

/* Line 1806 of yacc.c  */
#line 2785 "language.yy"
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

  case 213:

/* Line 1806 of yacc.c  */
#line 2799 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_LEFT;
  #endif
}
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 2808 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_RIGHT;
  #endif
}
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 2817 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
	(yyval.arctypeplusdir) = 0; //dummy to supress warning
  #else
    ((yyval.arctypeplusdir)).arc = (yyvsp[(1) - (1)].arcsegdata);
	((yyval.arctypeplusdir)).dir = MSC_DIR_BIDIR;
  #endif
}
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 2827 "language.yy"
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

  case 217:

/* Line 1806 of yacc.c  */
#line 2840 "language.yy"
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

  case 218:

/* Line 1806 of yacc.c  */
#line 2850 "language.yy"
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

  case 219:

/* Line 1806 of yacc.c  */
#line 2861 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter((yylsp[(1) - (1)]), yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    (yyval.arcvertarrow) = new ArcVerticalArrow((yyvsp[(1) - (1)].arctypeplusdir), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 2869 "language.yy"
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

  case 221:

/* Line 1806 of yacc.c  */
#line 2881 "language.yy"
    {free((yyvsp[(2) - (3)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 2882 "language.yy"
    {free((yyvsp[(1) - (4)].str)); free((yyvsp[(3) - (4)].str)); (yyval.arcvertarrow)=NULL;}
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 2886 "language.yy"
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

  case 224:

/* Line 1806 of yacc.c  */
#line 2897 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter((yylsp[(1) - (1)]), "Missing the 'at' keyword.");
  #else
    (yyval.arcvertarrow) = NULL;
    msc.Error.Error(MSC_POS((yylsp[(1) - (1)])).end.NextChar(), "Missing the 'at' keyword.");
    if ((yyvsp[(1) - (1)].arcvertarrow)) delete (yyvsp[(1) - (1)].arcvertarrow);
  #endif
}
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 2908 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
  #else
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
  #endif
}
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 2916 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
  #else
    (yyval.arcbase) = ((yyvsp[(1) - (3)].arcarrow))->AddLostPos((yyvsp[(3) - (3)].vertxpos), MSC_POS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
  #endif
   free((yyvsp[(2) - (3)].str));
}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 2925 "language.yy"
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

  case 228:

/* Line 1806 of yacc.c  */
#line 2940 "language.yy"
    {
    //explicit copy here to suppress bison warning (since the two types are different)
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 2948 "language.yy"
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

  case 233:

/* Line 1806 of yacc.c  */
#line 2961 "language.yy"
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

  case 234:

/* Line 1806 of yacc.c  */
#line 2971 "language.yy"
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

  case 235:

/* Line 1806 of yacc.c  */
#line 2982 "language.yy"
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

  case 236:

/* Line 1806 of yacc.c  */
#line 2992 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(1) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 3002 "language.yy"
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

  case 238:

/* Line 1806 of yacc.c  */
#line 3015 "language.yy"
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

  case 239:

/* Line 1806 of yacc.c  */
#line 3025 "language.yy"
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

  case 240:

/* Line 1806 of yacc.c  */
#line 3036 "language.yy"
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

  case 241:

/* Line 1806 of yacc.c  */
#line 3046 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 3055 "language.yy"
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

  case 243:

/* Line 1806 of yacc.c  */
#line 3068 "language.yy"
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

  case 244:

/* Line 1806 of yacc.c  */
#line 3078 "language.yy"
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

  case 245:

/* Line 1806 of yacc.c  */
#line 3089 "language.yy"
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

  case 246:

/* Line 1806 of yacc.c  */
#line 3099 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter((yylsp[(2) - (2)]), yylloc, yychar==YYEOF);
  #else
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment((yyvsp[(2) - (2)].arcsegdata), NULL, MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])));
  #endif
}
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 3111 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 3112 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 3113 "language.yy"
    {(yyval.arctype)=MSC_ARC_UNDETERMINED_SEGMENT;}
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 3116 "language.yy"
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

  case 266:

/* Line 1806 of yacc.c  */
#line 3126 "language.yy"
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

  case 267:

/* Line 1806 of yacc.c  */
#line 3137 "language.yy"
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

  case 268:

/* Line 1806 of yacc.c  */
#line 3148 "language.yy"
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

  case 269:

/* Line 1806 of yacc.c  */
#line 3162 "language.yy"
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

  case 270:

/* Line 1806 of yacc.c  */
#line 3172 "language.yy"
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

  case 271:

/* Line 1806 of yacc.c  */
#line 3183 "language.yy"
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

  case 272:

/* Line 1806 of yacc.c  */
#line 3194 "language.yy"
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

  case 273:

/* Line 1806 of yacc.c  */
#line 3208 "language.yy"
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

  case 274:

/* Line 1806 of yacc.c  */
#line 3218 "language.yy"
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

  case 275:

/* Line 1806 of yacc.c  */
#line 3229 "language.yy"
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

  case 276:

/* Line 1806 of yacc.c  */
#line 3240 "language.yy"
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

  case 277:

/* Line 1806 of yacc.c  */
#line 3254 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 278:

/* Line 1806 of yacc.c  */
#line 3263 "language.yy"
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

  case 279:

/* Line 1806 of yacc.c  */
#line 3277 "language.yy"
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

  case 280:

/* Line 1806 of yacc.c  */
#line 3291 "language.yy"
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

  case 281:

/* Line 1806 of yacc.c  */
#line 3308 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 3317 "language.yy"
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

  case 283:

/* Line 1806 of yacc.c  */
#line 3331 "language.yy"
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

  case 284:

/* Line 1806 of yacc.c  */
#line 3345 "language.yy"
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

  case 285:

/* Line 1806 of yacc.c  */
#line 3362 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yyloc), COLOR_SYMBOL);
  #else
    ((yyval.arcsegdata)).type = (yyvsp[(1) - (1)].arctype);
    ((yyval.arcsegdata)).lost = EArrowLost::NOT;
  #endif
}
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 3371 "language.yy"
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

  case 287:

/* Line 1806 of yacc.c  */
#line 3385 "language.yy"
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

  case 288:

/* Line 1806 of yacc.c  */
#line 3399 "language.yy"
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

  case 290:

/* Line 1806 of yacc.c  */
#line 3418 "language.yy"
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

  case 291:

/* Line 1806 of yacc.c  */
#line 3441 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));

  #else
    (yyval.extvertxpos) = new ExtVertXPos((yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].vertxpos));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 3451 "language.yy"
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

  case 295:

/* Line 1806 of yacc.c  */
#line 3467 "language.yy"
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

  case 296:

/* Line 1806 of yacc.c  */
#line 3503 "language.yy"
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

  case 297:

/* Line 1806 of yacc.c  */
#line 3526 "language.yy"
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

  case 298:

/* Line 1806 of yacc.c  */
#line 3562 "language.yy"
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

  case 299:

/* Line 1806 of yacc.c  */
#line 3586 "language.yy"
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

  case 300:

/* Line 1806 of yacc.c  */
#line 3622 "language.yy"
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

  case 301:

/* Line 1806 of yacc.c  */
#line 3659 "language.yy"
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

  case 302:

/* Line 1806 of yacc.c  */
#line 3695 "language.yy"
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

  case 303:

/* Line 1806 of yacc.c  */
#line 3712 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    ((yyvsp[(1) - (1)].arcbase))->AddAttributeList(NULL);
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcbase));
  #endif
}
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 3719 "language.yy"
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

  case 305:

/* Line 1806 of yacc.c  */
#line 3732 "language.yy"
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

  case 306:

/* Line 1806 of yacc.c  */
#line 3750 "language.yy"
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

  case 307:

/* Line 1806 of yacc.c  */
#line 3764 "language.yy"
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

  case 308:

/* Line 1806 of yacc.c  */
#line 3779 "language.yy"
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

  case 309:

/* Line 1806 of yacc.c  */
#line 3799 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::LEFT;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 3806 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 3813 "language.yy"
    {
  #ifndef C_S_H_IS_COMPILED
    (yyval.eside)= ESide::END;
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 3821 "language.yy"
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

  case 313:

/* Line 1806 of yacc.c  */
#line 3834 "language.yy"
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

  case 314:

/* Line 1806 of yacc.c  */
#line 3846 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 3853 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
  #endif
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 3861 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), MSC_POS((yyloc)), MSC_POS((yyloc)).IncStartCol()));
  #endif
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 3869 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), MSC_POS((yylsp[(1) - (2)])), MSC_POS((yylsp[(1) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 318:

/* Line 1806 of yacc.c  */
#line 3877 "language.yy"
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

  case 319:

/* Line 1806 of yacc.c  */
#line 3889 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), MSC_POS((yylsp[(2) - (2)])), MSC_POS((yylsp[(2) - (2)])).IncStartCol()));
  #endif
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 321:

/* Line 1806 of yacc.c  */
#line 3900 "language.yy"
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

  case 322:

/* Line 1806 of yacc.c  */
#line 3910 "language.yy"
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

  case 323:

/* Line 1806 of yacc.c  */
#line 3920 "language.yy"
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

  case 324:

/* Line 1806 of yacc.c  */
#line 3931 "language.yy"
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

  case 325:

/* Line 1806 of yacc.c  */
#line 3942 "language.yy"
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

  case 326:

/* Line 1806 of yacc.c  */
#line 3953 "language.yy"
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

  case 327:

/* Line 1806 of yacc.c  */
#line 3964 "language.yy"
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

  case 328:

/* Line 1806 of yacc.c  */
#line 3977 "language.yy"
    {
    (yyval.attriblist) = NULL;
}
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 3981 "language.yy"
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

  case 330:

/* Line 1806 of yacc.c  */
#line 3993 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
  #else
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
  #endif
}
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 4000 "language.yy"
    {
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    csh.CheckHintBetween((yylsp[(2) - (3)]), (yylsp[(3) - (3)]), HINT_ATTR_NAME);
  #else
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
  #endif
}
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 4009 "language.yy"
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

  case 334:

/* Line 1806 of yacc.c  */
#line 4021 "language.yy"
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

  case 335:

/* Line 1806 of yacc.c  */
#line 4036 "language.yy"
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

  case 336:

/* Line 1806 of yacc.c  */
#line 4051 "language.yy"
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

  case 337:

/* Line 1806 of yacc.c  */
#line 4066 "language.yy"
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

  case 338:

/* Line 1806 of yacc.c  */
#line 4080 "language.yy"
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

  case 339:

/* Line 1806 of yacc.c  */
#line 4094 "language.yy"
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

  case 340:

/* Line 1806 of yacc.c  */
#line 4108 "language.yy"
    {
    (yyval.attrib) = NULL;
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 341:

/* Line 1806 of yacc.c  */
#line 4113 "language.yy"
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

  case 342:

/* Line 1806 of yacc.c  */
#line 4125 "language.yy"
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

  case 345:

/* Line 1806 of yacc.c  */
#line 4143 "language.yy"
    {
	(yyval.str) = (char*)malloc(2);
	((yyval.str))[0] = ShapeElement::act_code[(yyvsp[(1) - (1)].shapecommand)];
	((yyval.str))[1] = 0;
}
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 4162 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 4163 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 4164 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 4165 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 4166 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 4167 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 4168 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 4169 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 4170 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 4171 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 4172 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 4173 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 4175 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    default: _ASSERT(0);
    }
}
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 4183 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_BOX_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_BOX_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_BOX_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_BOX_DOUBLE: (yyval.str) = strdup("=="); break;
    default: _ASSERT(0);
    }
}
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 4198 "language.yy"
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

  case 398:

/* Line 1806 of yacc.c  */
#line 4214 "language.yy"
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
#line 8757 "colorsyntax.cc"
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
#line 4227 "language.yy"



/* END OF FILE */

