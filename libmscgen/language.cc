/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1



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
     TOK_STYLE_NAME = 272,
     TOK_BOOLEAN = 273,
     TOK_REL_SOLID_TO = 274,
     TOK_REL_SOLID_FROM = 275,
     TOK_REL_SOLID_BIDIR = 276,
     TOK_REL_DOUBLE_TO = 277,
     TOK_REL_DOUBLE_FROM = 278,
     TOK_REL_DOUBLE_BIDIR = 279,
     TOK_REL_DASHED_TO = 280,
     TOK_REL_DASHED_FROM = 281,
     TOK_REL_DASHED_BIDIR = 282,
     TOK_REL_DOTTED_TO = 283,
     TOK_REL_DOTTED_FROM = 284,
     TOK_REL_DOTTED_BIDIR = 285,
     TOK_SPECIAL_ARC = 286,
     TOK_EMPH = 287,
     TOK_COMMAND_HEADING = 288,
     TOK_COMMAND_NUDGE = 289,
     TOK_COMMAND_NEWPAGE = 290,
     TOK_COMMAND_DEFCOLOR = 291,
     TOK_COMMAND_DEFSTYLE = 292,
     TOK_COMMAND_DEFDESIGN = 293,
     TOK_COMMAND_BIG = 294,
     TOK_COMMAND_PIPE = 295,
     TOK_COMMAND_MARK = 296,
     TOK_VERTICAL = 297,
     TOK_AT = 298
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
#define TOK_STYLE_NAME 272
#define TOK_BOOLEAN 273
#define TOK_REL_SOLID_TO 274
#define TOK_REL_SOLID_FROM 275
#define TOK_REL_SOLID_BIDIR 276
#define TOK_REL_DOUBLE_TO 277
#define TOK_REL_DOUBLE_FROM 278
#define TOK_REL_DOUBLE_BIDIR 279
#define TOK_REL_DASHED_TO 280
#define TOK_REL_DASHED_FROM 281
#define TOK_REL_DASHED_BIDIR 282
#define TOK_REL_DOTTED_TO 283
#define TOK_REL_DOTTED_FROM 284
#define TOK_REL_DOTTED_BIDIR 285
#define TOK_SPECIAL_ARC 286
#define TOK_EMPH 287
#define TOK_COMMAND_HEADING 288
#define TOK_COMMAND_NUDGE 289
#define TOK_COMMAND_NEWPAGE 290
#define TOK_COMMAND_DEFCOLOR 291
#define TOK_COMMAND_DEFSTYLE 292
#define TOK_COMMAND_DEFDESIGN 293
#define TOK_COMMAND_BIG 294
#define TOK_COMMAND_PIPE 295
#define TOK_COMMAND_MARK 296
#define TOK_VERTICAL 297
#define TOK_AT 298




/* Copy the first part of user declarations.  */
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <iostream>
#include "msc.h"
#include "language.h"
#include "language2.h"

/* Use verbose error reporting such that the expected token names are dumped */
#define YYERROR_VERBOSE

#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif


/* yyerror
 *  Error handling function.  The TOK_XXX names are substituted for more
 *  understandable values that make more sense to the user.
 */
void yyerror(YYLTYPE*loc, Msc &msc, void *yyscanner, const char *str)
{
    static const std::pair<string, string> tokens[] = {
      std::pair<string,string>("TOK_REL_SOLID_TO", "'->'"),
      std::pair<string,string>("TOK_REL_SOLID_FROM", "'<-'"),
      std::pair<string,string>("TOK_REL_SOLID_BIDIR", "'<->'"),
      std::pair<string,string>("TOK_REL_DOUBLE_TO", "'=>'"),
      std::pair<string,string>("TOK_REL_DOUBLE_FROM", "'<='"),
      std::pair<string,string>("TOK_REL_DOUBLE_BIDIR", "'<=>'"),
      std::pair<string,string>("TOK_REL_DASHED_TO", "'>>'"),
      std::pair<string,string>("TOK_REL_DASHED_FROM", "'<<'"),
      std::pair<string,string>("TOK_REL_DASHED_BIDIR", "'<<>>'"),
      std::pair<string,string>("TOK_REL_DOTTED_TO", "'>'"),
      std::pair<string,string>("TOK_REL_DOTTED_FROM", "'<'"),
      std::pair<string,string>("TOK_REL_DOTTED_BIDIR", "'<>'"),
      std::pair<string,string>("TOK_EMPH", "'..', '--', '=='"),
      std::pair<string,string>("TOK_SPECIAL_ARC", "'...', '---'"),
      std::pair<string,string>("syntax error, ", ""),
      std::pair<string,string>(", expecting $end", ""),
      std::pair<string,string>("$end", "end of input"),
      std::pair<string,string>("TOK_OCBRACKET", "'{'"),
      std::pair<string,string>("TOK_CCBRACKET", "'}'"),
      std::pair<string,string>("TOK_OSBRACKET", "'['"),
      std::pair<string,string>("TOK_CSBRACKET", "']'"),
      std::pair<string,string>("TOK_EQUAL", "'='"),
      std::pair<string,string>("TOK_DASH", "'-'"),
      std::pair<string,string>("TOK_PLUS", "'+'"),
      std::pair<string,string>("TOK_COMMA", "','"),
      std::pair<string,string>("TOK_SEMICOLON", "';'"),
      std::pair<string,string>("TOK_MSC", "'msc'"),
      std::pair<string,string>("TOK_BOOLEAN", "'yes', 'no'"),
      std::pair<string,string>("TOK_COMMAND_HEADING", "'heading'"),
      std::pair<string,string>("TOK_COMMAND_NUDGE", "'nudge'"),
      std::pair<string,string>("TOK_COMMAND_NEWPAGE", "'newpage'"),
      std::pair<string,string>("TOK_COMMAND_DEFCOLOR", "'defcolor'"),
      std::pair<string,string>("TOK_COMMAND_DEFSTYLE", "'defstyle'"),
      std::pair<string,string>("TOK_COMMAND_DEFDESIGN", "'defdesign'"),
      std::pair<string,string>("TOK_COMMAND_BIG", "'block'"),
      std::pair<string,string>("TOK_COMMAND_PIPE", "'pipe'"),
      std::pair<string,string>("TOK_COMMAND_MARK", "'mark'"),
      std::pair<string,string>("TOK_VERTICAL", "'vertical'"),
      std::pair<string,string>("TOK_AT", "'at'"),
      std::pair<string,string>("TOK_COLON_STRING", "':'"),  //just say colon to the user
      std::pair<string,string>("TOK_NUMBER", "number"),
      std::pair<string,string>("TOK_STRING", "string"),
      std::pair<string,string>("TOK_STYLE_NAME", "style name"),
      std::pair<string,string>("TOK_QSTRING", "quoted string")};

    #define tokArrayLen (sizeof(tokens) / sizeof(std::pair<string,string>))
    string msg(str);

    string once_msg;

    for (int i=0; i<tokArrayLen; i++) {
        int pos = 0;
        while (1) {
            pos = msg.find(tokens[i].first, pos);
            if (pos == string::npos) break;
            msg.erase(pos, tokens[i].first.length());
            string ins = tokens[i].second;
            //special comment for unexpected symbols
            //special handling for numbers and strings
            if (i>tokArrayLen-5) {
                int exppos = msg.find("expecting");
                //If we replace what was unexpected, use actual token text
                if (pos < exppos) {
                    if (i==tokArrayLen-1)
                        ins += ": " + string(yyget_text(yyscanner));
                    else
                        ins += ": '" + string(yyget_text(yyscanner)) + "'";
                    if (i==tokArrayLen-2) {
                        string hint(yyget_text(yyscanner));
                        int pos = hint.find_first_not_of("abcdefghijklmnopqrstuvwxyz");
                        hint.insert(pos," ");
                        once_msg = "Try splitting it with a space: '"+hint+"'.";
                    }
                } else {
                    ins = "a <" + ins + ">";
                }
            }
            msg.insert(pos, ins);
        }
    }
    int pos = msg.rfind("', '");
    if (pos != string::npos) {
        msg.erase(pos+1, 1);
        msg.insert(pos+1, " or");
    }
    msg.append(".");
    //If no error location was set when error was detected
    //use the location of the lookahead
    if (msc.current_pos.line == 0) {
        msc.current_pos.line = loc->first_line;
        msc.current_pos.col = loc->first_column;
    }
    msc.Error.Error(msc.current_pos, msg, once_msg);
    msc.current_pos.line = 0;
    msc.current_pos.col = 0;
};

void MscParse(Msc &msc, const char *buff, unsigned len)
{
    parse_parm  pp;
    pp.buf = const_cast<char*>(buff);
    pp.length = len;
    pp.pos = 0;
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);
	msc.current_pos.line = 0;
	msc.current_pos.col = 0;
    yyparse(msc, pp.yyscanner);
    yylex_destroy(pp.yyscanner);
}

inline bool string_to_bool(const char*s)
{
   return (s[0]!='n' && s[0]!='N') || (s[1]!='o' && s[1]!='O');
}

#define YYMSC_GETPOS(A) file_line(msc.current_pos.file, A.first_line, A.first_column)



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

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 185 "language.yy"
{
    char             *str;
    Msc              *msc;
    ArcBase          *arcbase;
    ArcList          *arclist;
    ArcArrow         *arcarrow;
    ArcEmphasis      *arcemph;
    ArcParallel      *arcparallel;
    MscArcType        arctype;
    EntityDef        *entity;
    EntityDefList    *entitylist;
    Attribute        *attrib;
    AttributeList    *attriblist;
    VertXPos         *vertxpos;
    std::list<string>*stringlist;
    unsigned long     linenum;
}
/* Line 187 of yacc.c.  */
#line 364 "language.cc"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
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


/* Line 216 of yacc.c.  */
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
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
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  115
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   783

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  195
/* YYNRULES -- Number of states.  */
#define YYNSTATES  257

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   298

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
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    15,    18,    21,
      23,    27,    30,    36,    42,    47,    52,    55,    56,    58,
      61,    64,    68,    71,    75,    77,    79,    81,    83,    85,
      88,    91,    94,    96,    99,   101,   104,   107,   111,   113,
     116,   118,   122,   124,   128,   131,   135,   139,   143,   147,
     149,   153,   156,   159,   161,   163,   167,   172,   174,   178,
     180,   184,   188,   194,   200,   202,   204,   208,   211,   214,
     216,   218,   222,   225,   229,   233,   237,   241,   243,   246,
     248,   251,   254,   258,   262,   265,   270,   274,   276,   279,
     282,   286,   288,   291,   294,   298,   302,   305,   308,   310,
     313,   317,   321,   325,   330,   332,   334,   336,   341,   345,
     349,   352,   357,   361,   365,   368,   370,   372,   374,   376,
     380,   383,   386,   390,   393,   397,   400,   403,   407,   410,
     414,   417,   420,   424,   427,   429,   431,   433,   435,   437,
     439,   441,   443,   445,   447,   449,   451,   453,   455,   457,
     459,   461,   463,   465,   468,   471,   473,   476,   480,   482,
     486,   489,   493,   497,   501,   504,   506,   508,   510,   512,
     514,   516,   518,   520,   522,   524,   526,   528,   530,   532,
     534,   536,   538,   540,   542,   544,   546,   548,   550,   552,
     554,   556,   558,   560,   562,   564
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    -1,    46,     9,    -1,    47,    -1,    50,
      -1,    46,     9,    50,    -1,    46,     1,    -1,    47,    50,
      -1,    48,    -1,    46,     9,    48,    -1,    47,    48,    -1,
      51,    11,    46,     9,    12,    -1,    51,    11,    46,     9,
       1,    -1,    93,    46,     9,    94,    -1,    93,    46,     9,
       1,    -1,    93,    94,    -1,    -1,    75,    -1,    75,    85,
      -1,    39,    75,    -1,    39,    75,    85,    -1,    42,    74,
      -1,    42,    74,    85,    -1,    85,    -1,    54,    -1,    52,
      -1,    68,    -1,    67,    -1,    36,    59,    -1,    37,    56,
      -1,    38,    61,    -1,    33,    -1,    33,    86,    -1,    34,
      -1,    34,    86,    -1,    41,    92,    -1,    41,    92,    86,
      -1,    35,    -1,    35,    86,    -1,    15,    -1,    15,     7,
      92,    -1,    53,    -1,    52,     8,    53,    -1,    52,     1,
      -1,    89,     7,    18,    -1,    89,     7,     5,    -1,    89,
       7,    92,    -1,    15,     7,    92,    -1,    55,    -1,    54,
       8,    55,    -1,    54,     1,    -1,    89,    86,    -1,    89,
      -1,    57,    -1,    56,     8,    57,    -1,    58,    13,    87,
      14,    -1,    92,    -1,    58,     8,    92,    -1,    60,    -1,
      59,     8,    60,    -1,     3,     7,    92,    -1,     3,    62,
      63,     9,    12,    -1,     3,    62,    63,     9,     1,    -1,
      11,    -1,    64,    -1,    63,     9,    64,    -1,    36,    59,
      -1,    37,    56,    -1,    65,    -1,    66,    -1,    65,     8,
      66,    -1,    65,     1,    -1,    89,     7,    18,    -1,    89,
       7,     5,    -1,    89,     7,    92,    -1,    15,     7,    92,
      -1,    49,    -1,    67,    49,    -1,    69,    -1,    40,    70,
      -1,    68,    71,    -1,    68,    71,    85,    -1,    68,    71,
      49,    -1,    68,    49,    -1,    68,    71,    85,    49,    -1,
      68,    85,    49,    -1,    71,    -1,    71,    85,    -1,    71,
      49,    -1,    71,    85,    49,    -1,    71,    -1,    71,    85,
      -1,    71,    49,    -1,    71,    85,    49,    -1,    89,    32,
      89,    -1,    32,    89,    -1,    89,    32,    -1,    32,    -1,
      43,    89,    -1,    43,    89,     6,    -1,    43,    89,    10,
      -1,    43,    89,    32,    -1,    43,    89,     6,    89,    -1,
      32,    -1,    79,    -1,    81,    -1,    89,    73,    89,    72,
      -1,    73,    89,    72,    -1,    89,    73,    72,    -1,    73,
      72,    -1,    89,    80,    89,    72,    -1,    80,    89,    72,
      -1,    89,    80,    72,    -1,    80,    72,    -1,    31,    -1,
      76,    -1,    77,    -1,    78,    -1,    89,    79,    89,    -1,
      79,    89,    -1,    89,    79,    -1,    76,    82,    89,    -1,
      76,    82,    -1,    89,    80,    89,    -1,    80,    89,    -1,
      89,    80,    -1,    77,    83,    89,    -1,    77,    83,    -1,
      89,    81,    89,    -1,    81,    89,    -1,    89,    81,    -1,
      78,    84,    89,    -1,    78,    84,    -1,    19,    -1,    22,
      -1,    25,    -1,    28,    -1,    20,    -1,    23,    -1,    26,
      -1,    29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,
      -1,    79,    -1,     6,    -1,    80,    -1,     6,    -1,    81,
      -1,     6,    -1,    16,    -1,    16,    86,    -1,    86,    16,
      -1,    86,    -1,    13,    14,    -1,    13,    87,    14,    -1,
      88,    -1,    87,     8,    88,    -1,    87,     1,    -1,    92,
       7,    92,    -1,    92,     7,     5,    -1,    92,     7,    18,
      -1,    92,     7,    -1,    92,    -1,     4,    -1,     3,    -1,
      15,    -1,    36,    -1,    37,    -1,    38,    -1,    35,    -1,
      39,    -1,    40,    -1,    42,    -1,    19,    -1,    20,    -1,
      21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    89,    -1,    90,    -1,    91,    -1,
      17,    -1,    11,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   249,   249,   252,   256,   261,   268,   274,   278,   285,
     289,   296,   307,   311,   316,   320,   325,   332,   335,   336,
     340,   347,   356,   361,   366,   370,   374,   383,   387,   391,
     396,   401,   406,   411,   416,   421,   426,   431,   436,   441,
     448,   452,   459,   466,   477,   482,   489,   496,   510,   518,
     522,   526,   531,   536,   543,   544,   546,   559,   565,   572,
     573,   575,   582,   593,   607,   614,   615,   617,   618,   619,
     621,   622,   623,   625,   631,   637,   643,   651,   655,   660,
     664,   670,   674,   679,   683,   689,   694,   702,   706,   711,
     715,   723,   727,   732,   736,   742,   748,   753,   758,   763,
     769,   775,   781,   806,   814,   814,   814,   816,   823,   829,
     835,   840,   847,   853,   859,   866,   870,   874,   878,   883,
     889,   894,   899,   904,   910,   916,   921,   926,   931,   936,
     942,   947,   952,   957,   962,   962,   962,   962,   963,   963,
     963,   963,   964,   964,   964,   964,   966,   966,   967,   967,
     968,   968,   972,   977,   982,   987,   990,   994,   999,  1003,
    1007,  1012,  1018,  1024,  1030,  1035,  1042,  1042,  1044,  1044,
    1045,  1045,  1046,  1046,  1046,  1047,  1049,  1050,  1051,  1052,
    1053,  1054,  1055,  1056,  1057,  1058,  1059,  1060,  1061,  1068,
    1078,  1078,  1078,  1078,  1080,  1085
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
  "TOK_CSBRACKET", "TOK_MSC", "TOK_COLON_STRING", "TOK_STYLE_NAME",
  "TOK_BOOLEAN", "TOK_REL_SOLID_TO", "TOK_REL_SOLID_FROM",
  "TOK_REL_SOLID_BIDIR", "TOK_REL_DOUBLE_TO", "TOK_REL_DOUBLE_FROM",
  "TOK_REL_DOUBLE_BIDIR", "TOK_REL_DASHED_TO", "TOK_REL_DASHED_FROM",
  "TOK_REL_DASHED_BIDIR", "TOK_REL_DOTTED_TO", "TOK_REL_DOTTED_FROM",
  "TOK_REL_DOTTED_BIDIR", "TOK_SPECIAL_ARC", "TOK_EMPH",
  "TOK_COMMAND_HEADING", "TOK_COMMAND_NUDGE", "TOK_COMMAND_NEWPAGE",
  "TOK_COMMAND_DEFCOLOR", "TOK_COMMAND_DEFSTYLE", "TOK_COMMAND_DEFDESIGN",
  "TOK_COMMAND_BIG", "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK",
  "TOK_VERTICAL", "TOK_AT", "$accept", "msc", "arclist",
  "arclist_ending_with_mscenclosed", "mscenclosed", "braced_arclist",
  "arc", "msckey", "optlist", "opt", "entitylist", "entity",
  "styledeflist", "styledef", "tok_stringlist", "colordeflist", "colordef",
  "designdef", "scope_open_empty", "designelementlist", "designelement",
  "designoptlist", "designopt", "parallel", "emphasis_list",
  "first_emphasis", "pipe_emphasis", "emphrel", "vertxpos",
  "empharcrel_straight", "vertrel", "arcrel", "arcrel_to", "arcrel_from",
  "arcrel_bidir", "relation_to", "relation_from", "relation_bidir",
  "relation_to_cont", "relation_from_cont", "relation_bidir_cont",
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
     295,   296,   297,   298
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    45,    46,    46,    46,    46,    47,
      47,    47,    48,    48,    49,    49,    49,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      51,    51,    52,    52,    52,    53,    53,    53,    53,    54,
      54,    54,    55,    55,    56,    56,    57,    58,    58,    59,
      59,    60,    61,    61,    62,    63,    63,    64,    64,    64,
      65,    65,    65,    66,    66,    66,    66,    67,    67,    68,
      68,    68,    68,    68,    68,    68,    68,    69,    69,    69,
      69,    70,    70,    70,    70,    71,    71,    71,    71,    72,
      72,    72,    72,    72,    73,    73,    73,    74,    74,    74,
      74,    74,    74,    74,    74,    75,    75,    75,    75,    76,
      76,    76,    76,    76,    77,    77,    77,    77,    77,    78,
      78,    78,    78,    78,    79,    79,    79,    79,    80,    80,
      80,    80,    81,    81,    81,    81,    82,    82,    83,    83,
      84,    84,    85,    85,    85,    85,    86,    86,    87,    87,
      87,    88,    88,    88,    88,    88,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      92,    92,    92,    92,    93,    94
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     3,     2,     2,     1,
       3,     2,     5,     5,     4,     4,     2,     0,     1,     2,
       2,     3,     2,     3,     1,     1,     1,     1,     1,     2,
       2,     2,     1,     2,     1,     2,     2,     3,     1,     2,
       1,     3,     1,     3,     2,     3,     3,     3,     3,     1,
       3,     2,     2,     1,     1,     3,     4,     1,     3,     1,
       3,     3,     5,     5,     1,     1,     3,     2,     2,     1,
       1,     3,     2,     3,     3,     3,     3,     1,     2,     1,
       2,     2,     3,     3,     2,     4,     3,     1,     2,     2,
       3,     1,     2,     2,     3,     3,     2,     2,     1,     2,
       3,     3,     3,     4,     1,     1,     1,     4,     3,     3,
       2,     4,     3,     3,     2,     1,     1,     1,     1,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     2,     3,     1,     3,
       2,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      17,   167,   166,   194,     0,    40,   152,   134,   138,   142,
     135,   139,   143,   136,   140,   144,   137,   141,   145,   115,
      98,    32,    34,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     9,    77,     5,     0,     0,    42,
       0,    49,    28,    27,    79,    87,    18,   116,   117,   118,
       0,     0,     0,    24,   155,    53,    17,   156,   168,   193,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   172,   169,   170,   171,   173,   174,
     175,     0,   158,   190,   191,   192,   165,     0,   153,    96,
      33,    35,    39,     0,    29,    59,    30,    54,     0,    57,
       0,    31,    20,     0,    80,    91,     0,    36,   104,     0,
      22,   105,     0,   106,     0,     1,     7,    17,    11,     8,
      17,    44,     0,    51,     0,    78,    84,    81,     0,    89,
      88,    19,   147,   146,   123,   149,   148,   128,   151,   150,
     133,   120,   125,   130,   154,     0,    97,   121,   126,   131,
      52,   195,     0,    17,    16,   160,     0,   157,   164,    48,
       0,     0,     0,     0,     0,    64,     0,    21,    93,    92,
      37,     0,   110,     0,    23,   114,     0,     0,     0,    10,
       6,     0,     0,    43,     0,    50,    53,    83,    82,    86,
      90,   122,   127,   132,    46,    45,    47,    95,   119,   124,
     129,     0,   159,   162,   163,   161,    61,    60,    55,    58,
       0,     0,     0,     0,     0,    65,     0,    70,     0,    94,
      99,   108,   112,   109,     0,   113,     0,     0,     0,    85,
      15,    14,    56,     0,    67,    68,     0,    72,     0,     0,
     100,   101,   102,   107,   111,    13,    12,    48,    76,    63,
      62,    66,    71,    74,    73,    75,   103
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    31,    32,   153,    34,    35,    36,    37,    38,    39,
      40,    41,    96,    97,    98,    94,    95,   101,   166,   214,
     215,   216,   217,    42,    43,    44,   104,    45,   172,   109,
     110,    46,    47,    48,    49,    50,    51,    52,   134,   137,
     140,    53,    54,    81,    82,    83,    84,    85,    86,    56,
     154
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -98
static const yytype_int16 yypact[] =
{
     226,   -98,   -98,   -98,   626,    -4,    -6,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     188,    -6,    -6,    -6,    10,   656,    37,   700,    35,   656,
     686,    44,    48,   269,   -98,   -98,   -98,    62,    54,   -98,
      83,   -98,    71,   100,   -98,   159,   117,   162,   136,   155,
     188,   188,   188,   -98,    30,   725,   436,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   150,   -98,   -98,   -98,   -98,    91,   656,   -98,   -98,
     -98,   -98,   -98,    95,   110,   -98,   123,   -98,    51,   -98,
      97,   -98,   117,   753,   -98,   159,   103,    -6,   -98,    28,
     117,   -98,    28,   -98,   739,   -98,   -98,   312,   -98,   -98,
     476,   -98,    57,   -98,   188,   -98,   -98,   159,    71,   -98,
      71,   -98,   -98,   -98,   188,   -98,   -98,   188,   -98,   -98,
     188,   -98,   -98,   -98,   -98,   516,   188,   188,   188,   188,
     -98,   -98,   127,   476,   -98,   -98,   656,   -98,   556,   139,
     656,    10,   656,   656,   656,   -98,    50,   -98,   -98,    71,
     -98,   188,   -98,   111,   -98,   -98,   111,    28,    28,   -98,
     -98,   151,   148,   -98,   156,   -98,    -6,   -98,    71,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   354,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     166,   176,    10,   656,   177,   -98,   106,   -98,   181,   -98,
     113,   -98,   -98,   -98,   111,   -98,   111,   396,   656,   -98,
     -98,   -98,   -98,   656,   110,   123,    33,   -98,    90,   596,
     188,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   -98,   -54,   193,   -32,   -31,   -28,   -98,   -98,    74,
     -98,    73,   -15,    38,   -98,   -10,    42,   -98,   -98,   -98,
     -30,   -98,   -34,   -98,   -98,   -98,   -98,   -20,   -97,    93,
     -98,   178,   -98,   -98,   -98,   -26,   -13,    -8,   -98,   -98,
     -98,   -27,     3,    46,    52,     0,   -98,   -98,   -19,   -98,
      11
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -70
static const yytype_int16 yytable[] =
{
      55,   118,   152,    87,   111,   119,    99,     4,   105,    88,
     107,   125,   126,    93,   129,   175,   128,   112,   130,   131,
      89,   133,   113,   127,    90,    91,    92,   103,   106,   147,
     114,     1,     2,    55,   249,   136,     1,     2,     1,     2,
     100,   139,   148,   106,   115,   250,   144,   149,   211,   116,
     141,   142,   143,     1,     2,   121,    55,   117,   150,   163,
       1,     2,   122,   -26,   164,   211,   181,    20,   159,   212,
     213,   171,   182,   120,   168,   167,   221,   147,   169,   222,
     223,   225,     3,   174,   123,   179,   212,   213,   111,   180,
     148,   124,   -25,     1,     2,   149,   187,   189,   158,   190,
     188,   178,   160,     1,     2,   211,   113,   237,   165,   173,
     170,     3,   176,     4,   238,   -69,     6,    55,   161,   240,
      55,   118,   184,   241,   186,   119,   196,   243,   116,   244,
       4,   162,    20,     6,   191,   146,   201,   192,   219,   205,
     193,   206,   135,    99,   209,   242,   197,   198,   199,   200,
     -41,   155,   116,    55,   171,   228,     8,   229,   156,    11,
     227,   138,    14,   145,   157,    17,   218,   155,   132,   179,
       3,   220,     4,   180,   156,     6,     9,   224,   226,    12,
     232,     7,    15,   233,    10,    18,   236,    13,   239,   150,
      16,     1,     2,    33,    99,   179,   183,   185,   235,   180,
     208,    55,   234,   207,   252,   102,   251,   177,   202,   247,
     210,     0,   231,     0,   248,     0,     0,     0,     0,     0,
     255,     0,     0,     0,     0,     0,    -2,    55,     0,     1,
       2,     0,     0,     0,     0,     0,   218,     3,   218,     4,
     256,     5,     6,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    -4,
       0,     0,     1,     2,     0,     0,     0,     0,     0,     0,
       3,     0,     4,     0,     5,     6,     0,     0,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -3,     0,     0,     1,     2,     0,     0,     0,
       0,     0,     0,     3,     0,     4,     0,     5,     6,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   230,     0,     1,     2,     0,
       0,     0,     0,   -17,     0,     3,   151,     4,     0,     5,
       6,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,   245,     0,     1,
       2,     0,     0,     0,     0,   -17,     0,     3,   246,     4,
       0,     5,     6,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     1,
       2,     0,     0,     0,     0,     0,     0,     3,   151,     4,
       0,     5,     6,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     1,
       2,     0,     0,     0,     0,     0,     0,     3,     0,     4,
       0,     5,     6,     0,     0,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,     1,
       2,   194,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    59,   195,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,    74,    75,    76,    77,    78,    79,     0,    80,     1,
       2,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    59,   204,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     0,
       0,    74,    75,    76,    77,    78,    79,     0,    80,     1,
       2,   253,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    59,   254,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     1,
       2,    74,    75,    76,    77,    78,    79,     0,    80,     0,
      57,    58,     0,    59,     0,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     1,
       2,    74,    75,    76,    77,    78,    79,     0,    80,     0,
       0,    58,     0,    59,     0,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,     1,
       2,    74,    75,    76,    77,    78,    79,     0,    80,     0,
       0,     0,     0,     1,     2,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,     0,   108,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,   145,     0,     0,     0,     0,     0,     4,     0,
       0,     0,     0,     0,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,     0,   146,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
       0,   108,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
};

static const yytype_int16 yycheck[] =
{
       0,    33,    56,     7,    30,    33,    25,    13,    28,     6,
      29,    42,    43,     3,    45,   112,    43,    30,    45,    46,
      20,    47,    30,    43,    21,    22,    23,    27,    28,    55,
      30,     3,     4,    33,     1,    48,     3,     4,     3,     4,
       3,    49,    55,    43,     0,    12,    16,    55,    15,     1,
      50,    51,    52,     3,     4,     1,    56,     9,    55,     8,
       3,     4,     8,     9,    13,    15,   120,    32,    87,    36,
      37,    43,    15,    11,   105,   102,   173,   103,   105,   176,
     177,   178,    11,   110,     1,   117,    36,    37,   114,   117,
     103,     8,     9,     3,     4,   103,   127,   128,     7,   130,
     127,   114,     7,     3,     4,    15,   114,     1,    11,   109,
     107,    11,   112,    13,     8,     9,    16,   117,     8,     6,
     120,   153,   122,    10,   124,   153,   145,   224,     1,   226,
      13,     8,    32,    16,   134,    32,     9,   137,   169,   158,
     140,   160,     6,   162,   163,    32,   146,   147,   148,   149,
      11,     1,     1,   153,    43,     7,    20,   188,     8,    23,
       9,     6,    26,     7,    14,    29,   166,     1,     6,   201,
      11,   171,    13,   201,     8,    16,    21,   177,   178,    24,
      14,    19,    27,     7,    22,    30,     9,    25,     7,   186,
      28,     3,     4,     0,   213,   227,   122,   124,   213,   227,
     162,   201,   212,   161,   238,    27,   236,   114,   156,   228,
     164,    -1,   201,    -1,   233,    -1,    -1,    -1,    -1,    -1,
     239,    -1,    -1,    -1,    -1,    -1,     0,   227,    -1,     3,
       4,    -1,    -1,    -1,    -1,    -1,   236,    11,   238,    13,
     240,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     0,
      -1,    -1,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      11,    -1,    13,    -1,    15,    16,    -1,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,     0,    -1,    -1,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    -1,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,     3,     4,    -1,
      -1,    -1,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,     3,
       4,    -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    12,    13,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,    13,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     3,
       4,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      14,    15,    -1,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     3,
       4,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    15,    -1,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     3,
       4,    35,    36,    37,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,    -1,     3,     4,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     7,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,    11,    13,    15,    16,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    67,    68,    69,    71,    75,    76,    77,    78,
      79,    80,    81,    85,    86,    89,    93,    14,    15,    17,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    35,    36,    37,    38,    39,    40,
      42,    87,    88,    89,    90,    91,    92,     7,    86,    89,
      86,    86,    86,     3,    59,    60,    56,    57,    58,    92,
       3,    61,    75,    89,    70,    71,    89,    92,    32,    73,
      74,    79,    80,    81,    89,     0,     1,     9,    48,    50,
      11,     1,     8,     1,     8,    49,    49,    71,    85,    49,
      85,    85,     6,    79,    82,     6,    80,    83,     6,    81,
      84,    89,    89,    89,    16,     7,    32,    79,    80,    81,
      86,    12,    46,    47,    94,     1,     8,    14,     7,    92,
       7,     8,     8,     8,    13,    11,    62,    85,    49,    85,
      86,    43,    72,    89,    85,    72,    89,    73,    80,    48,
      50,    46,    15,    53,    89,    55,    89,    49,    85,    49,
      49,    89,    89,    89,     5,    18,    92,    89,    89,    89,
      89,     9,    88,     5,    18,    92,    92,    60,    57,    92,
      87,    15,    36,    37,    63,    64,    65,    66,    89,    49,
      89,    72,    72,    72,    89,    72,    89,     9,     7,    49,
       1,    94,    14,     7,    59,    56,     9,     1,     8,     7,
       6,    10,    32,    72,    72,     1,    12,    92,    92,     1,
      12,    64,    66,     5,    18,    92,    89
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
      yyerror (&yylloc, msc, yyscanner, YY_("syntax error: cannot back up")); \
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
		  Type, Value, Location, msc, yyscanner); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Msc &msc, void *yyscanner)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, msc, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Msc &msc;
    void *yyscanner;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (msc);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Msc &msc, void *yyscanner)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, msc, yyscanner)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    Msc &msc;
    void *yyscanner;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, msc, yyscanner);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, Msc &msc, void *yyscanner)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, msc, yyscanner)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    Msc &msc;
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
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , msc, yyscanner);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, msc, yyscanner); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Msc &msc, void *yyscanner)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, msc, yyscanner)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    Msc &msc;
    void *yyscanner;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (msc);
  YYUSE (yyscanner);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 3: /* "TOK_STRING" */
#line 240 "language.yy"
	{free((yyvaluep->str));};
#line 1689 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */
#line 240 "language.yy"
	{free((yyvaluep->str));};
#line 1694 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */
#line 244 "language.yy"
	{free((yyvaluep->str));};
#line 1699 "language.cc"
	break;
      case 15: /* "TOK_MSC" */
#line 241 "language.yy"
	{free((yyvaluep->str));};
#line 1704 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */
#line 240 "language.yy"
	{free((yyvaluep->str));};
#line 1709 "language.cc"
	break;
      case 17: /* "TOK_STYLE_NAME" */
#line 240 "language.yy"
	{free((yyvaluep->str));};
#line 1714 "language.cc"
	break;
      case 18: /* "TOK_BOOLEAN" */
#line 244 "language.yy"
	{free((yyvaluep->str));};
#line 1719 "language.cc"
	break;
      case 33: /* "TOK_COMMAND_HEADING" */
#line 243 "language.yy"
	{free((yyvaluep->str));};
#line 1724 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_NUDGE" */
#line 243 "language.yy"
	{free((yyvaluep->str));};
#line 1729 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NEWPAGE" */
#line 243 "language.yy"
	{free((yyvaluep->str));};
#line 1734 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_DEFCOLOR" */
#line 242 "language.yy"
	{free((yyvaluep->str));};
#line 1739 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFSTYLE" */
#line 242 "language.yy"
	{free((yyvaluep->str));};
#line 1744 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFDESIGN" */
#line 242 "language.yy"
	{free((yyvaluep->str));};
#line 1749 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_BIG" */
#line 241 "language.yy"
	{free((yyvaluep->str));};
#line 1754 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_PIPE" */
#line 241 "language.yy"
	{free((yyvaluep->str));};
#line 1759 "language.cc"
	break;
      case 46: /* "arclist" */
#line 236 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1764 "language.cc"
	break;
      case 47: /* "arclist_ending_with_mscenclosed" */
#line 236 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1769 "language.cc"
	break;
      case 48: /* "mscenclosed" */
#line 236 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1774 "language.cc"
	break;
      case 49: /* "braced_arclist" */
#line 236 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1779 "language.cc"
	break;
      case 50: /* "arc" */
#line 232 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1784 "language.cc"
	break;
      case 52: /* "optlist" */
#line 236 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1789 "language.cc"
	break;
      case 53: /* "opt" */
#line 232 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1794 "language.cc"
	break;
      case 54: /* "entitylist" */
#line 237 "language.yy"
	{delete (yyvaluep->entitylist);};
#line 1799 "language.cc"
	break;
      case 55: /* "entity" */
#line 237 "language.yy"
	{delete (yyvaluep->entity);};
#line 1804 "language.cc"
	break;
      case 58: /* "tok_stringlist" */
#line 238 "language.yy"
	{delete (yyvaluep->stringlist);};
#line 1809 "language.cc"
	break;
      case 67: /* "parallel" */
#line 235 "language.yy"
	{delete (yyvaluep->arcparallel);};
#line 1814 "language.cc"
	break;
      case 68: /* "emphasis_list" */
#line 234 "language.yy"
	{delete (yyvaluep->arcemph);};
#line 1819 "language.cc"
	break;
      case 69: /* "first_emphasis" */
#line 234 "language.yy"
	{delete (yyvaluep->arcemph);};
#line 1824 "language.cc"
	break;
      case 70: /* "pipe_emphasis" */
#line 234 "language.yy"
	{delete (yyvaluep->arcemph);};
#line 1829 "language.cc"
	break;
      case 71: /* "emphrel" */
#line 234 "language.yy"
	{delete (yyvaluep->arcemph);};
#line 1834 "language.cc"
	break;
      case 72: /* "vertxpos" */
#line 231 "language.yy"
	{delete (yyvaluep->vertxpos);};
#line 1839 "language.cc"
	break;
      case 74: /* "vertrel" */
#line 230 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1844 "language.cc"
	break;
      case 75: /* "arcrel" */
#line 232 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1849 "language.cc"
	break;
      case 76: /* "arcrel_to" */
#line 233 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1854 "language.cc"
	break;
      case 77: /* "arcrel_from" */
#line 233 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1859 "language.cc"
	break;
      case 78: /* "arcrel_bidir" */
#line 233 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1864 "language.cc"
	break;
      case 85: /* "full_arcattrlist_with_label" */
#line 238 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1869 "language.cc"
	break;
      case 86: /* "full_arcattrlist" */
#line 238 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1874 "language.cc"
	break;
      case 87: /* "arcattrlist" */
#line 238 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1879 "language.cc"
	break;
      case 88: /* "arcattr" */
#line 238 "language.yy"
	{delete (yyvaluep->attrib);};
#line 1884 "language.cc"
	break;
      case 89: /* "entity_string" */
#line 239 "language.yy"
	{free((yyvaluep->str));};
#line 1889 "language.cc"
	break;
      case 90: /* "reserved_word_string" */
#line 239 "language.yy"
	{free((yyvaluep->str));};
#line 1894 "language.cc"
	break;
      case 91: /* "symbol_string" */
#line 239 "language.yy"
	{free((yyvaluep->str));};
#line 1899 "language.cc"
	break;
      case 92: /* "string" */
#line 239 "language.yy"
	{free((yyvaluep->str));};
#line 1904 "language.cc"
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
int yyparse (Msc &msc, void *yyscanner);
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
yyparse (Msc &msc, void *yyscanner)
#else
int
yyparse (msc, yyscanner)
    Msc &msc;
    void *yyscanner;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
  yylloc.first_column = yylloc.last_column = 0;
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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
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
#line 249 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:
#line 253 "language.yy"
    {
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
}
    break;

  case 4:
#line 257 "language.yy"
    {
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
}
    break;

  case 5:
#line 262 "language.yy"
    {
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
}
    break;

  case 6:
#line 269 "language.yy"
    {
    if ((yyvsp[(3) - (3)].arcbase))
        ((yyvsp[(1) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (3)].arclist));
}
    break;

  case 7:
#line 275 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 8:
#line 279 "language.yy"
    {
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
}
    break;

  case 9:
#line 286 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
}
    break;

  case 10:
#line 290 "language.yy"
    {
    //Merge $3 into $1
    ((yyvsp[(1) - (3)].arclist))->splice(((yyvsp[(1) - (3)].arclist))->end(), *((yyvsp[(3) - (3)].arclist)));
    delete ((yyvsp[(3) - (3)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
}
    break;

  case 11:
#line 297 "language.yy"
    {
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 12:
#line 308 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(3) - (5)].arclist);
}
    break;

  case 13:
#line 312 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(3) - (5)].arclist);
}
    break;

  case 14:
#line 317 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
}
    break;

  case 15:
#line 321 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    msc.PopContext();
}
    break;

  case 16:
#line 326 "language.yy"
    {
    (yyval.arclist) = new ArcList;
}
    break;

  case 17:
#line 332 "language.yy"
    {
    (yyval.arcbase) = NULL;
}
    break;

  case 19:
#line 337 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 20:
#line 341 "language.yy"
    {
    //Returns NULL, if BIG is before a self-pointing arrow
    (yyval.arcbase) = msc.CreateArcBigArrow((yyvsp[(2) - (2)].arcbase));
    delete (yyvsp[(2) - (2)].arcbase);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 21:
#line 348 "language.yy"
    {
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[(2) - (3)].arcbase));
    if (arrow) arrow->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    (yyval.arcbase) = arrow;
    delete (yyvsp[(2) - (3)].arcbase);
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 22:
#line 357 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 23:
#line 362 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 24:
#line 367 "language.yy"
    {
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
}
    break;

  case 25:
#line 371 "language.yy"
    {
    (yyval.arcbase) = new CommandEntity((yyvsp[(1) - (1)].entitylist), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 26:
#line 375 "language.yy"
    {
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
}
    break;

  case 27:
#line 384 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
}
    break;

  case 28:
#line 388 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
}
    break;

  case 29:
#line 392 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 30:
#line 397 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 31:
#line 402 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 32:
#line 407 "language.yy"
    {
    (yyval.arcbase) = new CommandEntity(NULL, YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 33:
#line 412 "language.yy"
    {
    (yyval.arcbase) = (new CommandEntity(NULL, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 34:
#line 417 "language.yy"
    {
    (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 35:
#line 422 "language.yy"
    {
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 36:
#line 427 "language.yy"
    {
    (yyval.arcbase) = new CommandMark((yyvsp[(2) - (2)].str), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 37:
#line 432 "language.yy"
    {
    (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 38:
#line 437 "language.yy"
    {
    (yyval.arcbase) = new CommandNewpage(YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 39:
#line 442 "language.yy"
    {
    (yyval.arcbase) = (new CommandNewpage(YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 40:
#line 449 "language.yy"
    {
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 41:
#line 453 "language.yy"
    {
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 42:
#line 460 "language.yy"
    {
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
}
    break;

  case 43:
#line 467 "language.yy"
    {
    if ((yyvsp[(3) - (3)].arcbase)) {
        if ((yyvsp[(1) - (3)].arclist))
            (yyval.arclist) = ((yyvsp[(1) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase));     /* Add to existing list */
        else
            (yyval.arclist) = (new ArcList)->Append((yyvsp[(3) - (3)].arcbase)); /* New list */
    } else {
        (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
    }
}
    break;

  case 44:
#line 478 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 45:
#line 483 "language.yy"
    {
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 46:
#line 490 "language.yy"
    {
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 47:
#line 497 "language.yy"
    {
    Attribute a((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])));
    MscFillAttr fill;
    fill.Empty();
    if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
        (yyval.arcbase) = new CommandNewBackground(a.linenum_attr, &msc, fill);
    } else {
        msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 48:
#line 511 "language.yy"
    {
    msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 49:
#line 519 "language.yy"
    {
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
}
    break;

  case 50:
#line 523 "language.yy"
    {
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
}
    break;

  case 51:
#line 527 "language.yy"
    {
    (yyval.entitylist) = (yyvsp[(1) - (2)].entitylist);
}
    break;

  case 52:
#line 532 "language.yy"
    {
    (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 53:
#line 537 "language.yy"
    {
    (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 56:
#line 547 "language.yy"
    {
    for (std::list<string>::iterator i = ((yyvsp[(1) - (4)].stringlist))->begin(); i!=((yyvsp[(1) - (4)].stringlist))->end(); i++) {
        MscStyle style = msc.StyleSets.top().GetStyle(*i);
        AttributeList::iterator j=((yyvsp[(3) - (4)].attriblist))->begin();
        while (j!=((yyvsp[(3) - (4)].attriblist))->end())
           style.AddAttribute(**(j++), &msc);
        msc.StyleSets.top()[*i] = style;
    }
    delete((yyvsp[(1) - (4)].stringlist));
    delete((yyvsp[(3) - (4)].attriblist));
}
    break;

  case 57:
#line 560 "language.yy"
    {
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 58:
#line 566 "language.yy"
    {
    ((yyvsp[(1) - (3)].stringlist))->push_back((yyvsp[(3) - (3)].str));
    (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 61:
#line 576 "language.yy"
    {
    msc.ColorSets.top().AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 62:
#line 583 "language.yy"
    {
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[(yyvsp[(1) - (5)].str)];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
}
    break;

  case 63:
#line 594 "language.yy"
    {
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[(yyvsp[(1) - (5)].str)];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
}
    break;

  case 64:
#line 608 "language.yy"
    {
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
}
    break;

  case 73:
#line 626 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 74:
#line 632 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 75:
#line 638 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 76:
#line 644 "language.yy"
    {
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 77:
#line 652 "language.yy"
    {
    (yyval.arcparallel) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
}
    break;

  case 78:
#line 656 "language.yy"
    {
    (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 79:
#line 661 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 80:
#line 665 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 81:
#line 671 "language.yy"
    {
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
}
    break;

  case 82:
#line 675 "language.yy"
    {
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
}
    break;

  case 83:
#line 680 "language.yy"
    {
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
}
    break;

  case 84:
#line 684 "language.yy"
    {
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
}
    break;

  case 85:
#line 690 "language.yy"
    {
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
}
    break;

  case 86:
#line 695 "language.yy"
    {
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), YYMSC_GETPOS((yyloc)), &msc);
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
}
    break;

  case 87:
#line 703 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 88:
#line 707 "language.yy"
    {
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 89:
#line 712 "language.yy"
    {
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 90:
#line 716 "language.yy"
    {
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 91:
#line 724 "language.yy"
    {
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
}
    break;

  case 92:
#line 728 "language.yy"
    {
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 93:
#line 733 "language.yy"
    {
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist))->SetPipe();
}
    break;

  case 94:
#line 737 "language.yy"
    {
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->SetPipe()->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 95:
#line 743 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 96:
#line 749 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 97:
#line 754 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 98:
#line 759 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 99:
#line 764 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (2)].str));
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 100:
#line 770 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_SIDE);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 101:
#line 776 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_SIDE);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 102:
#line 782 "language.yy"
    {
    switch ((yyvsp[(3) - (3)].arctype)) {
    case MSC_EMPH_SOLID:
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_BY);
        break;
    case MSC_EMPH_DASHED:
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_EMPH_DOTTED:
        msc.Error.Error(YYMSC_GETPOS((yylsp[(3) - (3)])),
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        (yyval.vertxpos) = NULL;
        break;
    case MSC_EMPH_DOUBLE:
        msc.Error.Error(YYMSC_GETPOS((yylsp[(3) - (3)])),
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        (yyval.vertxpos) = NULL;
        break;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 103:
#line 807 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yyloc)), msc, (yyvsp[(2) - (4)].str), VertXPos::POS_CENTER, (yyvsp[(4) - (4)].str));
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 107:
#line 817 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
    delete (yyvsp[(4) - (4)].vertxpos);
}
    break;

  case 108:
#line 824 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(2) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 109:
#line 830 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 110:
#line 836 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 111:
#line 841 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
    delete (yyvsp[(4) - (4)].vertxpos);
}
    break;

  case 112:
#line 848 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(2) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 113:
#line 854 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    free((yyvsp[(1) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 114:
#line 860 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 115:
#line 867 "language.yy"
    {
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 116:
#line 871 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 117:
#line 875 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 118:
#line 879 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 119:
#line 884 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 120:
#line 890 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 121:
#line 895 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 122:
#line 900 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS((yylsp[(2) - (3)])));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 123:
#line 905 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 124:
#line 911 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 125:
#line 917 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 126:
#line 922 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 127:
#line 927 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), false, YYMSC_GETPOS((yylsp[(2) - (3)])));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 128:
#line 932 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), false, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 129:
#line 937 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 130:
#line 943 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 131:
#line 948 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 132:
#line 953 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS((yylsp[(2) - (3)])));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 133:
#line 958 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 152:
#line 973 "language.yy"
    {
    (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)).IncCol()));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 153:
#line 978 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yylsp[(1) - (2)])).IncCol()));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 154:
#line 983 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yylsp[(2) - (2)])).IncCol()));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 156:
#line 991 "language.yy"
    {
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 157:
#line 995 "language.yy"
    {
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 158:
#line 1000 "language.yy"
    {
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
}
    break;

  case 159:
#line 1004 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
}
    break;

  case 160:
#line 1008 "language.yy"
    {
    (yyval.attriblist) = (yyvsp[(1) - (2)].attriblist);
}
    break;

  case 161:
#line 1013 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 162:
#line 1019 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 163:
#line 1025 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 164:
#line 1031 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 165:
#line 1036 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 176:
#line 1049 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 177:
#line 1050 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 178:
#line 1051 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 179:
#line 1052 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 180:
#line 1053 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 181:
#line 1054 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 182:
#line 1055 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 183:
#line 1056 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 184:
#line 1057 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 185:
#line 1058 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 186:
#line 1059 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 187:
#line 1060 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 188:
#line 1062 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 189:
#line 1069 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 194:
#line 1081 "language.yy"
    {
    msc.PushContext();
}
    break;

  case 195:
#line 1086 "language.yy"
    {
    msc.PopContext();
}
    break;


/* Line 1267 of yacc.c.  */
#line 3427 "language.cc"
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
      yyerror (&yylloc, msc, yyscanner, YY_("syntax error"));
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
	    yyerror (&yylloc, msc, yyscanner, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, msc, yyscanner, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval, &yylloc, msc, yyscanner);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
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
		  yystos[yystate], yyvsp, yylsp, msc, yyscanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, msc, yyscanner, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, msc, yyscanner);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, msc, yyscanner);
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


#line 1091 "language.yy"



/* END OF FILE */

