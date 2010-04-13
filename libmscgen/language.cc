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
    const file_line fileline(msc.current_file, yyget_lineno(yyscanner),
                             yyget_lloc(yyscanner)->first_column);

    msc.Error.Error(fileline, msg, once_msg);
};

void MscParse(Msc &msc, const char *buff, unsigned len)
{
    parse_parm  pp;

    pp.buf = const_cast<char*>(buff);
    pp.length = len;
    pp.pos = 0;
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);
    yyparse(msc, pp.yyscanner);
    yylex_destroy(pp.yyscanner);
}

inline bool string_to_bool(const char*s)
{
   return (s[0]!='n' && s[0]!='N') || (s[1]!='o' && s[1]!='O');
}




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
#line 178 "language.yy"
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
#line 357 "language.cc"
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
#line 382 "language.cc"

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
#define YYLAST   809

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  192
/* YYNRULES -- Number of states.  */
#define YYNSTATES  271

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
      24,    26,    30,    33,    39,    40,    42,    45,    48,    52,
      55,    59,    61,    63,    65,    67,    69,    72,    75,    78,
      80,    83,    85,    88,    91,    95,    97,   100,   102,   106,
     108,   112,   115,   119,   123,   127,   131,   133,   137,   140,
     143,   145,   147,   151,   156,   158,   162,   164,   168,   172,
     178,   180,   182,   186,   189,   192,   194,   196,   200,   203,
     207,   211,   215,   219,   221,   224,   229,   232,   234,   237,
     240,   242,   245,   251,   258,   260,   263,   269,   276,   278,
     281,   287,   294,   298,   301,   304,   306,   309,   313,   317,
     321,   326,   328,   330,   332,   337,   341,   345,   348,   353,
     357,   361,   364,   366,   368,   370,   372,   376,   379,   382,
     386,   389,   393,   396,   399,   403,   406,   410,   413,   416,
     420,   423,   425,   427,   429,   431,   433,   435,   437,   439,
     441,   443,   445,   447,   449,   451,   453,   455,   457,   459,
     461,   464,   467,   469,   472,   476,   478,   482,   485,   489,
     493,   497,   500,   502,   504,   506,   508,   510,   512,   514,
     516,   518,   520,   522,   524,   526,   528,   530,   532,   534,
     536,   538,   540,   542,   544,   546,   548,   550,   552,   554,
     556,   558,   560
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    -1,    46,     9,    -1,    47,    -1,    49,
      -1,    46,     9,    49,    -1,    46,     1,    -1,    47,    49,
      -1,    47,     1,    -1,    48,    -1,    46,     9,    48,    -1,
      47,    48,    -1,    50,    11,    46,     9,    12,    -1,    -1,
      76,    -1,    76,    86,    -1,    39,    76,    -1,    39,    76,
      86,    -1,    42,    75,    -1,    42,    75,    86,    -1,    86,
      -1,    53,    -1,    51,    -1,    68,    -1,    66,    -1,    36,
      58,    -1,    37,    55,    -1,    38,    60,    -1,    33,    -1,
      33,    87,    -1,    34,    -1,    34,    87,    -1,    41,    93,
      -1,    41,    93,    87,    -1,    35,    -1,    35,    87,    -1,
      15,    -1,    15,     7,    93,    -1,    52,    -1,    51,     8,
      52,    -1,    51,     1,    -1,    90,     7,    18,    -1,    90,
       7,     5,    -1,    90,     7,    93,    -1,    15,     7,    93,
      -1,    54,    -1,    53,     8,    54,    -1,    53,     1,    -1,
      90,    87,    -1,    90,    -1,    56,    -1,    55,     8,    56,
      -1,    57,    13,    88,    14,    -1,    93,    -1,    57,     8,
      93,    -1,    59,    -1,    58,     8,    59,    -1,     3,     7,
      93,    -1,     3,    61,    62,     9,    12,    -1,    11,    -1,
      63,    -1,    62,     9,    63,    -1,    36,    58,    -1,    37,
      55,    -1,    64,    -1,    65,    -1,    64,     8,    65,    -1,
      64,     1,    -1,    90,     7,    18,    -1,    90,     7,     5,
      -1,    90,     7,    93,    -1,    15,     7,    93,    -1,    67,
      -1,    66,    67,    -1,    94,    46,     9,    95,    -1,    94,
      95,    -1,    69,    -1,    40,    71,    -1,    68,    70,    -1,
      72,    -1,    72,    86,    -1,    72,    94,    46,     9,    95,
      -1,    72,    86,    94,    46,     9,    95,    -1,    72,    -1,
      72,    86,    -1,    72,    94,    46,     9,    95,    -1,    72,
      86,    94,    46,     9,    95,    -1,    72,    -1,    72,    86,
      -1,    72,    94,    46,     9,    95,    -1,    72,    86,    94,
      46,     9,    95,    -1,    90,    32,    90,    -1,    32,    90,
      -1,    90,    32,    -1,    32,    -1,    43,    90,    -1,    43,
      90,     6,    -1,    43,    90,    10,    -1,    43,    90,    32,
      -1,    43,    90,     6,    90,    -1,    32,    -1,    80,    -1,
      82,    -1,    90,    74,    90,    73,    -1,    74,    90,    73,
      -1,    90,    74,    73,    -1,    74,    73,    -1,    90,    81,
      90,    73,    -1,    81,    90,    73,    -1,    90,    81,    73,
      -1,    81,    73,    -1,    31,    -1,    77,    -1,    78,    -1,
      79,    -1,    90,    80,    90,    -1,    80,    90,    -1,    90,
      80,    -1,    77,    83,    90,    -1,    77,    83,    -1,    90,
      81,    90,    -1,    81,    90,    -1,    90,    81,    -1,    78,
      84,    90,    -1,    78,    84,    -1,    90,    82,    90,    -1,
      82,    90,    -1,    90,    82,    -1,    79,    85,    90,    -1,
      79,    85,    -1,    19,    -1,    22,    -1,    25,    -1,    28,
      -1,    20,    -1,    23,    -1,    26,    -1,    29,    -1,    21,
      -1,    24,    -1,    27,    -1,    30,    -1,    80,    -1,     6,
      -1,    81,    -1,     6,    -1,    82,    -1,     6,    -1,    16,
      -1,    16,    87,    -1,    87,    16,    -1,    87,    -1,    13,
      14,    -1,    13,    88,    14,    -1,    89,    -1,    88,     8,
      89,    -1,    88,     1,    -1,    93,     7,    93,    -1,    93,
       7,     5,    -1,    93,     7,    18,    -1,    93,     7,    -1,
      93,    -1,     4,    -1,     3,    -1,    15,    -1,    36,    -1,
      37,    -1,    38,    -1,    35,    -1,    39,    -1,    40,    -1,
      42,    -1,    19,    -1,    20,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,    -1,
      28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,    -1,
      90,    -1,    91,    -1,    92,    -1,    17,    -1,    11,    -1,
      12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   241,   241,   244,   248,   253,   260,   265,   274,   279,
     289,   293,   300,   308,   316,   319,   320,   324,   331,   340,
     345,   350,   354,   358,   367,   368,   372,   377,   382,   387,
     392,   397,   402,   407,   412,   417,   422,   429,   433,   440,
     447,   458,   468,   476,   484,   498,   506,   510,   514,   524,
     529,   536,   537,   539,   552,   558,   565,   566,   568,   575,
     587,   596,   597,   599,   600,   601,   603,   604,   605,   615,
     622,   629,   635,   643,   647,   652,   657,   662,   666,   671,
     677,   681,   685,   689,   695,   699,   703,   707,   714,   718,
     722,   726,   731,   737,   742,   747,   752,   758,   764,   770,
     792,   800,   800,   800,   802,   809,   815,   821,   827,   834,
     840,   846,   854,   858,   862,   866,   871,   877,   882,   887,
     892,   898,   904,   909,   914,   919,   924,   930,   935,   940,
     945,   950,   950,   950,   950,   951,   951,   951,   951,   952,
     952,   952,   952,   954,   954,   955,   955,   956,   956,   960,
     965,   970,   975,   978,   982,   987,   991,   995,  1005,  1011,
    1018,  1025,  1030,  1037,  1037,  1039,  1039,  1040,  1040,  1041,
    1041,  1041,  1042,  1044,  1045,  1046,  1047,  1048,  1049,  1050,
    1051,  1052,  1053,  1054,  1055,  1056,  1063,  1073,  1073,  1073,
    1073,  1075,  1080
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
  "arclist_no_semicolon", "mscenclosed", "arc", "msckey", "optlist", "opt",
  "entitylist", "entity", "styledeflist", "styledef", "tok_stringlist",
  "colordeflist", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "parallel_element", "emphasis_list", "first_emphasis",
  "follow_emphasis", "pipe_emphasis", "emphrel", "vertxpos",
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
       0,    44,    45,    45,    45,    46,    46,    46,    46,    46,
      47,    47,    47,    48,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    50,    50,    51,
      51,    51,    52,    52,    52,    52,    53,    53,    53,    54,
      54,    55,    55,    56,    57,    57,    58,    58,    59,    60,
      61,    62,    62,    63,    63,    63,    64,    64,    64,    65,
      65,    65,    65,    66,    66,    67,    67,    68,    68,    68,
      69,    69,    69,    69,    70,    70,    70,    70,    71,    71,
      71,    71,    72,    72,    72,    72,    73,    73,    73,    73,
      73,    74,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    75,    76,    76,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    78,    78,    79,    79,    79,    79,
      79,    80,    80,    80,    80,    81,    81,    81,    81,    82,
      82,    82,    82,    83,    83,    84,    84,    85,    85,    86,
      86,    86,    86,    87,    87,    88,    88,    88,    89,    89,
      89,    89,    89,    90,    90,    91,    91,    91,    91,    91,
      91,    91,    91,    92,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    93,    93,    93,
      93,    94,    95
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     3,     2,     2,     2,
       1,     3,     2,     5,     0,     1,     2,     2,     3,     2,
       3,     1,     1,     1,     1,     1,     2,     2,     2,     1,
       2,     1,     2,     2,     3,     1,     2,     1,     3,     1,
       3,     2,     3,     3,     3,     3,     1,     3,     2,     2,
       1,     1,     3,     4,     1,     3,     1,     3,     3,     5,
       1,     1,     3,     2,     2,     1,     1,     3,     2,     3,
       3,     3,     3,     1,     2,     4,     2,     1,     2,     2,
       1,     2,     5,     6,     1,     2,     5,     6,     1,     2,
       5,     6,     3,     2,     2,     1,     2,     3,     3,     3,
       4,     1,     1,     1,     4,     3,     3,     2,     4,     3,
       3,     2,     1,     1,     1,     1,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     2,     3,     1,     3,     2,     3,     3,
       3,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      14,   164,   163,   191,     0,    37,   149,   131,   135,   139,
     132,   136,   140,   133,   137,   141,   134,   138,   142,   112,
      95,    29,    31,    35,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    10,     5,     0,     0,    39,     0,
      46,    25,    73,    24,    77,    80,    15,   113,   114,   115,
       0,     0,     0,    21,   152,    50,    14,   153,   165,   190,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   169,   166,   167,   168,   170,   171,
     172,     0,   155,   187,   188,   189,   162,     0,   150,    93,
      30,    32,    36,     0,    26,    56,    27,    51,     0,    54,
       0,    28,    17,     0,    78,    88,     0,    33,   101,     0,
      19,   102,     0,   103,     0,     1,     7,    14,     9,    12,
       8,    14,    41,     0,    48,     0,    74,    79,    84,    81,
      14,    16,   144,   143,   120,   146,   145,   125,   148,   147,
     130,   117,   122,   127,   151,     0,    94,   118,   123,   128,
      49,   192,     0,     0,    76,   157,     0,   154,   161,    45,
       0,     0,     0,     0,     0,    60,     0,    18,    89,    14,
      34,     0,   107,     0,    20,   111,     0,     0,     0,    11,
       6,     0,     0,    40,     0,    47,    50,    85,    14,    14,
       0,   119,   124,   129,    43,    42,    44,    92,   116,   121,
     126,    14,   156,   159,   160,   158,    58,    57,    52,    55,
       0,     0,     0,     0,     0,    61,     0,    66,     0,    14,
       0,    96,   105,   109,   106,     0,   110,     0,    14,     0,
      14,     0,     0,    14,    75,    53,     0,    63,    64,     0,
      68,     0,     0,     0,    14,    97,    98,    99,   104,   108,
      13,    45,     0,    14,    14,    82,    72,    59,    62,    67,
      70,    69,    71,    14,    90,   100,    14,    86,    83,    91,
      87
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    31,    32,   153,    34,    35,    36,    37,    38,    39,
      40,    96,    97,    98,    94,    95,   101,   166,   214,   215,
     216,   217,    41,    42,    43,    44,   127,   104,    45,   172,
     109,   110,    46,    47,    48,    49,    50,    51,    52,   134,
     137,   140,    53,    54,    81,    82,    55,    84,    85,    86,
      56,   154
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -182
static const yytype_int16 yypact[] =
{
     297,  -182,  -182,  -182,   652,     0,    21,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
      77,    21,    21,    21,    46,   682,    53,   726,    37,   682,
     712,     9,    44,   254,  -182,  -182,    47,   101,  -182,   174,
    -182,    60,  -182,    37,  -182,   232,     2,   172,   143,    30,
      77,    77,    77,  -182,    58,   751,   422,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,    98,  -182,  -182,  -182,  -182,    86,   682,  -182,  -182,
    -182,  -182,  -182,    87,    76,  -182,    90,  -182,    62,  -182,
      89,  -182,     2,   779,  -182,   232,    85,    21,  -182,     7,
       2,  -182,     7,  -182,   765,  -182,  -182,   340,  -182,  -182,
    -182,   502,  -182,   161,  -182,    77,  -182,  -182,   232,    60,
     502,  -182,  -182,  -182,    77,  -182,  -182,    77,  -182,  -182,
      77,  -182,  -182,  -182,  -182,   542,    77,    77,    77,    77,
    -182,  -182,   102,   382,  -182,  -182,   682,  -182,   582,   109,
     682,    46,   682,   682,   682,  -182,    40,  -182,    60,   502,
    -182,    77,  -182,    91,  -182,  -182,    91,     7,     7,  -182,
    -182,   117,   125,  -182,   131,  -182,    21,    60,   502,   502,
     184,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
    -182,   422,  -182,  -182,  -182,  -182,  -182,  -182,  -182,  -182,
     147,   146,    46,   682,   148,  -182,   178,  -182,   163,   502,
     209,    27,  -182,  -182,  -182,    91,  -182,    91,   462,   682,
     502,   211,   220,   422,  -182,  -182,   682,    76,    90,    92,
    -182,   201,   622,   221,   422,    77,  -182,  -182,  -182,  -182,
    -182,  -182,   238,   422,   422,  -182,  -182,  -182,  -182,  -182,
    -182,  -182,  -182,   422,  -182,  -182,   422,  -182,  -182,  -182,
    -182
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -182,  -182,   -42,   168,   -30,    -2,  -182,  -182,    57,  -182,
      70,   -12,    34,  -182,   -10,    50,  -182,  -182,  -182,   -31,
    -182,   -26,  -182,   176,  -182,  -182,  -182,  -182,   -24,  -111,
     113,  -182,   207,  -182,  -182,  -182,   -25,   -13,   -17,  -182,
    -182,  -182,    79,     6,    64,    84,    -4,  -182,  -182,   -23,
     -37,  -181
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -66
static const yytype_int16 yytable[] =
{
      83,   175,    99,   119,   105,   111,   107,    87,   130,   115,
       1,     2,    88,   113,   152,     4,    89,   112,     6,   128,
     234,    83,   133,   103,   106,    83,   114,    90,    91,    92,
     147,   120,   139,   245,     4,   136,   138,   246,   149,   106,
       1,     2,   148,     1,     2,   116,   141,   142,   143,    93,
     171,     9,   255,   117,    12,   211,   100,    15,   121,   247,
      18,   150,   222,   264,   159,   223,   224,   226,   169,    20,
     163,     3,   267,   268,   144,   164,   212,   213,   147,   181,
       1,     2,   269,    83,   161,   270,   149,   179,   190,   111,
     148,   188,   189,   158,   160,     1,     2,   113,   162,   155,
     165,   178,   122,   116,   257,   173,   156,   211,   176,   123,
     -23,   201,   157,   170,   248,   180,   249,   146,   116,   184,
     -38,   186,   196,   119,   129,   131,   228,   220,   212,   213,
     191,   219,   229,   192,   171,   205,   193,   206,   145,    99,
     209,    83,   197,   198,   199,   200,   231,   232,   155,   135,
     230,   120,    83,   236,    83,   156,    83,   239,    83,    83,
      83,   235,   218,     8,     1,     2,    11,   221,    33,    14,
     242,   179,    17,   225,   227,   124,   182,   243,   132,   240,
     183,   167,   125,   -22,   168,   116,   241,   -65,   252,   174,
      99,     7,   150,   233,    10,   185,   208,    13,   179,   180,
      16,   238,   237,   179,     1,     2,   251,   187,   258,    83,
     116,   207,   116,   256,   179,   259,   211,   126,   244,   262,
     253,   116,   116,   179,   179,    83,   180,   177,   210,   254,
     263,   180,    83,   179,   102,   218,   179,   218,    83,   116,
     202,   265,   180,     3,     0,     4,     0,   266,     6,     0,
       0,   180,   180,     0,    -4,   118,     0,     1,     2,     0,
       0,   180,     0,   -14,   180,     3,     0,     4,     0,     5,
       6,     0,     0,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -2,     0,     0,
       1,     2,     0,     0,     0,     0,     0,     0,     3,     0,
       4,     0,     5,     6,     0,     0,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -3,     0,     0,     1,     2,     0,     0,     0,     0,     0,
       0,     3,     0,     4,     0,     5,     6,     0,     0,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,   118,     0,     1,     2,     0,     0,     0,
       0,   -14,     0,     3,     0,     4,     0,     5,     6,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     1,     2,     0,     0,     0,
       0,     0,     0,     3,   151,     4,     0,     5,     6,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     1,     2,     0,     0,     0,
       0,     0,     0,     3,   250,     4,     0,     5,     6,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     1,     2,     0,     0,     0,
       0,     0,     0,     3,     0,     4,     0,     5,     6,     0,
       0,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     1,     2,   194,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    59,
     195,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     0,     0,    74,    75,    76,
      77,    78,    79,     0,    80,     1,     2,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    59,
     204,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     0,     0,    74,    75,    76,
      77,    78,    79,     0,    80,     1,     2,   260,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    58,     0,    59,
     261,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     1,     2,    74,    75,    76,
      77,    78,    79,     0,    80,     0,    57,    58,     0,    59,
       0,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     1,     2,    74,    75,    76,
      77,    78,    79,     0,    80,     0,     0,    58,     0,    59,
       0,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,     1,     2,    74,    75,    76,
      77,    78,    79,     0,    80,     0,     0,     0,     0,     1,
       2,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,     0,   108,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,   145,     0,
       0,     0,     0,     0,     4,     0,     0,     0,     0,     0,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,     0,   146,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,     0,   108,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18
};

static const yytype_int16 yycheck[] =
{
       4,   112,    25,    33,    28,    30,    29,     7,    45,     0,
       3,     4,     6,    30,    56,    13,    20,    30,    16,    43,
     201,    25,    47,    27,    28,    29,    30,    21,    22,    23,
      55,    33,    49,     6,    13,    48,     6,    10,    55,    43,
       3,     4,    55,     3,     4,     1,    50,    51,    52,     3,
      43,    21,   233,     9,    24,    15,     3,    27,    11,    32,
      30,    55,   173,   244,    87,   176,   177,   178,   105,    32,
       8,    11,   253,   254,    16,    13,    36,    37,   103,   121,
       3,     4,   263,    87,     8,   266,   103,   117,   130,   114,
     103,   128,   129,     7,     7,     3,     4,   114,     8,     1,
      11,   114,     1,     1,    12,   109,     8,    15,   112,     8,
       9,     9,    14,   107,   225,   117,   227,    32,     1,   123,
      11,   125,   145,   153,    45,    46,     9,   169,    36,    37,
     134,   168,     7,   137,    43,   158,   140,   160,     7,   162,
     163,   145,   146,   147,   148,   149,   188,   189,     1,     6,
     187,   153,   156,     7,   158,     8,   160,     9,   162,   163,
     164,    14,   166,    20,     3,     4,    23,   171,     0,    26,
       7,   201,    29,   177,   178,     1,    15,   219,     6,     1,
     123,   102,     8,     9,   105,     1,     8,     9,   230,   110,
     213,    19,   186,     9,    22,   125,   162,    25,   228,   201,
      28,   213,   212,   233,     3,     4,   229,   128,   239,   213,
       1,   161,     1,   236,   244,   241,    15,    41,     9,   242,
       9,     1,     1,   253,   254,   229,   228,   114,   164,     9,
       9,   233,   236,   263,    27,   239,   266,   241,   242,     1,
     156,   245,   244,    11,    -1,    13,    -1,     9,    16,    -1,
      -1,   253,   254,    -1,     0,     1,    -1,     3,     4,    -1,
      -1,   263,    -1,     9,   266,    11,    -1,    13,    -1,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     0,    -1,    -1,
       3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,    -1,
      13,    -1,    15,    16,    -1,    -1,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
       0,    -1,    -1,     3,     4,    -1,    -1,    -1,    -1,    -1,
      -1,    11,    -1,    13,    -1,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     1,    -1,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    -1,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    12,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    12,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    11,    -1,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    -1,    -1,    35,    36,    37,
      38,    39,    40,    -1,    42,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,     3,     4,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    14,    15,    -1,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,     3,     4,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    15,    -1,    17,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,     3,     4,    35,    36,    37,
      38,    39,    40,    -1,    42,    -1,    -1,    -1,    -1,     3,
       4,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     7,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    -1,    32,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,    11,    13,    15,    16,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    66,    67,    68,    69,    72,    76,    77,    78,    79,
      80,    81,    82,    86,    87,    90,    94,    14,    15,    17,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    35,    36,    37,    38,    39,    40,
      42,    88,    89,    90,    91,    92,    93,     7,    87,    90,
      87,    87,    87,     3,    58,    59,    55,    56,    57,    93,
       3,    60,    76,    90,    71,    72,    90,    93,    32,    74,
      75,    80,    81,    82,    90,     0,     1,     9,     1,    48,
      49,    11,     1,     8,     1,     8,    67,    70,    72,    86,
      94,    86,     6,    80,    83,     6,    81,    84,     6,    82,
      85,    90,    90,    90,    16,     7,    32,    80,    81,    82,
      87,    12,    46,    47,    95,     1,     8,    14,     7,    93,
       7,     8,     8,     8,    13,    11,    61,    86,    86,    94,
      87,    43,    73,    90,    86,    73,    90,    74,    81,    48,
      49,    46,    15,    52,    90,    54,    90,    86,    94,    94,
      46,    90,    90,    90,     5,    18,    93,    90,    90,    90,
      90,     9,    89,     5,    18,    93,    93,    59,    56,    93,
      88,    15,    36,    37,    62,    63,    64,    65,    90,    94,
      46,    90,    73,    73,    73,    90,    73,    90,     9,     7,
      94,    46,    46,     9,    95,    14,     7,    58,    55,     9,
       1,     8,     7,    46,     9,     6,    10,    32,    73,    73,
      12,    93,    46,     9,     9,    95,    93,    12,    63,    65,
       5,    18,    93,     9,    95,    90,     9,    95,    95,    95,
      95
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
#line 232 "language.yy"
	{free((yyvaluep->str));};
#line 1692 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */
#line 232 "language.yy"
	{free((yyvaluep->str));};
#line 1697 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */
#line 236 "language.yy"
	{free((yyvaluep->str));};
#line 1702 "language.cc"
	break;
      case 15: /* "TOK_MSC" */
#line 233 "language.yy"
	{free((yyvaluep->str));};
#line 1707 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */
#line 232 "language.yy"
	{free((yyvaluep->str));};
#line 1712 "language.cc"
	break;
      case 17: /* "TOK_STYLE_NAME" */
#line 232 "language.yy"
	{free((yyvaluep->str));};
#line 1717 "language.cc"
	break;
      case 18: /* "TOK_BOOLEAN" */
#line 236 "language.yy"
	{free((yyvaluep->str));};
#line 1722 "language.cc"
	break;
      case 33: /* "TOK_COMMAND_HEADING" */
#line 235 "language.yy"
	{free((yyvaluep->str));};
#line 1727 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_NUDGE" */
#line 235 "language.yy"
	{free((yyvaluep->str));};
#line 1732 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NEWPAGE" */
#line 235 "language.yy"
	{free((yyvaluep->str));};
#line 1737 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_DEFCOLOR" */
#line 234 "language.yy"
	{free((yyvaluep->str));};
#line 1742 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFSTYLE" */
#line 234 "language.yy"
	{free((yyvaluep->str));};
#line 1747 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFDESIGN" */
#line 234 "language.yy"
	{free((yyvaluep->str));};
#line 1752 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_BIG" */
#line 233 "language.yy"
	{free((yyvaluep->str));};
#line 1757 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_PIPE" */
#line 233 "language.yy"
	{free((yyvaluep->str));};
#line 1762 "language.cc"
	break;
      case 46: /* "arclist" */
#line 228 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1767 "language.cc"
	break;
      case 47: /* "arclist_no_semicolon" */
#line 228 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1772 "language.cc"
	break;
      case 48: /* "mscenclosed" */
#line 228 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1777 "language.cc"
	break;
      case 49: /* "arc" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1782 "language.cc"
	break;
      case 51: /* "optlist" */
#line 228 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1787 "language.cc"
	break;
      case 52: /* "opt" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1792 "language.cc"
	break;
      case 53: /* "entitylist" */
#line 229 "language.yy"
	{delete (yyvaluep->entitylist);};
#line 1797 "language.cc"
	break;
      case 54: /* "entity" */
#line 229 "language.yy"
	{delete (yyvaluep->entity);};
#line 1802 "language.cc"
	break;
      case 57: /* "tok_stringlist" */
#line 230 "language.yy"
	{delete (yyvaluep->stringlist);};
#line 1807 "language.cc"
	break;
      case 66: /* "parallel" */
#line 227 "language.yy"
	{delete (yyvaluep->arcparallel);};
#line 1812 "language.cc"
	break;
      case 67: /* "parallel_element" */
#line 228 "language.yy"
	{delete (yyvaluep->arclist);};
#line 1817 "language.cc"
	break;
      case 68: /* "emphasis_list" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1822 "language.cc"
	break;
      case 69: /* "first_emphasis" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1827 "language.cc"
	break;
      case 70: /* "follow_emphasis" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1832 "language.cc"
	break;
      case 71: /* "pipe_emphasis" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1837 "language.cc"
	break;
      case 72: /* "emphrel" */
#line 227 "language.yy"
	{delete (yyvaluep->arcemph);};
#line 1842 "language.cc"
	break;
      case 73: /* "vertxpos" */
#line 224 "language.yy"
	{delete (yyvaluep->vertxpos);};
#line 1847 "language.cc"
	break;
      case 75: /* "vertrel" */
#line 223 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1852 "language.cc"
	break;
      case 76: /* "arcrel" */
#line 225 "language.yy"
	{delete (yyvaluep->arcbase);};
#line 1857 "language.cc"
	break;
      case 77: /* "arcrel_to" */
#line 226 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1862 "language.cc"
	break;
      case 78: /* "arcrel_from" */
#line 226 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1867 "language.cc"
	break;
      case 79: /* "arcrel_bidir" */
#line 226 "language.yy"
	{delete (yyvaluep->arcarrow);};
#line 1872 "language.cc"
	break;
      case 86: /* "full_arcattrlist_with_label" */
#line 230 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1877 "language.cc"
	break;
      case 87: /* "full_arcattrlist" */
#line 230 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1882 "language.cc"
	break;
      case 88: /* "arcattrlist" */
#line 230 "language.yy"
	{delete (yyvaluep->attriblist);};
#line 1887 "language.cc"
	break;
      case 89: /* "arcattr" */
#line 230 "language.yy"
	{delete (yyvaluep->attrib);};
#line 1892 "language.cc"
	break;
      case 90: /* "entity_string" */
#line 231 "language.yy"
	{free((yyvaluep->str));};
#line 1897 "language.cc"
	break;
      case 91: /* "reserved_word_string" */
#line 231 "language.yy"
	{free((yyvaluep->str));};
#line 1902 "language.cc"
	break;
      case 92: /* "symbol_string" */
#line 231 "language.yy"
	{free((yyvaluep->str));};
#line 1907 "language.cc"
	break;
      case 93: /* "string" */
#line 231 "language.yy"
	{free((yyvaluep->str));};
#line 1912 "language.cc"
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
#line 241 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:
#line 245 "language.yy"
    {
    msc.AddArcs((yyvsp[(1) - (2)].arclist));
}
    break;

  case 4:
#line 249 "language.yy"
    {
    msc.AddArcs((yyvsp[(1) - (1)].arclist));
}
    break;

  case 5:
#line 254 "language.yy"
    {
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
}
    break;

  case 6:
#line 261 "language.yy"
    {
    if ((yyvsp[(3) - (3)].arcbase))
        (yyval.arclist) = ((yyvsp[(1) - (3)].arclist))->Append((yyvsp[(3) - (3)].arcbase));     /* Add to existing list */
}
    break;

  case 7:
#line 266 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    //Skip to next semicolon
    while (yychar > YYEOF && yychar != TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 8:
#line 275 "language.yy"
    {
    if ((yyvsp[(2) - (2)].arcbase))
        (yyval.arclist) = ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
}
    break;

  case 9:
#line 280 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    //Skip to next semicolon
    while (yychar > YYEOF && yychar != TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 10:
#line 290 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
}
    break;

  case 11:
#line 294 "language.yy"
    {
    //Merge $3 into $1
    ((yyvsp[(1) - (3)].arclist))->splice(((yyvsp[(1) - (3)].arclist))->end(), *((yyvsp[(3) - (3)].arclist)));
    delete ((yyvsp[(3) - (3)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
}
    break;

  case 12:
#line 301 "language.yy"
    {
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 13:
#line 309 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(3) - (5)].arclist);
}
    break;

  case 14:
#line 316 "language.yy"
    {
    (yyval.arcbase) = NULL;
}
    break;

  case 16:
#line 321 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 17:
#line 325 "language.yy"
    {
    //Returns NULL, if BIG is before a self-pointing arrow
    (yyval.arcbase) = msc.CreateArcBigArrow((yyvsp[(2) - (2)].arcbase));
    delete (yyvsp[(2) - (2)].arcbase);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 18:
#line 332 "language.yy"
    {
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[(2) - (3)].arcbase));
    if (arrow) arrow->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    (yyval.arcbase) = arrow;
    delete (yyvsp[(2) - (3)].arcbase);
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 19:
#line 341 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 20:
#line 346 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 21:
#line 351 "language.yy"
    {
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, yyget_lineno(yyscanner), &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
}
    break;

  case 22:
#line 355 "language.yy"
    {
    (yyval.arcbase) = new CommandEntity((yyvsp[(1) - (1)].entitylist), yyget_lineno(yyscanner), &msc);
}
    break;

  case 23:
#line 359 "language.yy"
    {
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcbase) = (new ArcParallel(yyget_lineno(yyscanner), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcbase) = NULL;
}
    break;

  case 25:
#line 369 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
}
    break;

  case 26:
#line 373 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 27:
#line 378 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 28:
#line 383 "language.yy"
    {
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 29:
#line 388 "language.yy"
    {
    (yyval.arcbase) = new CommandEntity(NULL, yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 30:
#line 393 "language.yy"
    {
    (yyval.arcbase) = (new CommandEntity(NULL, yyget_lineno(yyscanner), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 31:
#line 398 "language.yy"
    {
    (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 32:
#line 403 "language.yy"
    {
    (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, yyget_lineno(yyscanner), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 33:
#line 408 "language.yy"
    {
    (yyval.arcbase) = new CommandMark((yyvsp[(2) - (2)].str), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 34:
#line 413 "language.yy"
    {
    (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), yyget_lineno(yyscanner), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 35:
#line 418 "language.yy"
    {
    (yyval.arcbase) = new CommandNewpage(yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 36:
#line 423 "language.yy"
    {
    (yyval.arcbase) = (new CommandNewpage(yyget_lineno(yyscanner), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 37:
#line 430 "language.yy"
    {
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 38:
#line 434 "language.yy"
    {
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 39:
#line 441 "language.yy"
    {
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
}
    break;

  case 40:
#line 448 "language.yy"
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

  case 41:
#line 459 "language.yy"
    {
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 42:
#line 469 "language.yy"
    {
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), file_line(msc.current_file,
                               yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 43:
#line 477 "language.yy"
    {
    msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)),
                  file_line(msc.current_file, yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
    (yyval.arcbase) = NULL;
}
    break;

  case 44:
#line 485 "language.yy"
    {
    Attribute a((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner)));
    MscFillAttr fill;
    fill.Empty();
    if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
        (yyval.arcbase) = new CommandNewBackground(a.linenum.line, &msc, fill);
    } else {
        msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 45:
#line 499 "language.yy"
    {
    msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    (yyval.arcbase) = NULL;
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 46:
#line 507 "language.yy"
    {
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
}
    break;

  case 47:
#line 511 "language.yy"
    {
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
}
    break;

  case 48:
#line 515 "language.yy"
    {
    (yyval.entitylist) = (yyvsp[(1) - (2)].entitylist);
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 49:
#line 525 "language.yy"
    {
    (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 50:
#line 530 "language.yy"
    {
    (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 53:
#line 540 "language.yy"
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

  case 54:
#line 553 "language.yy"
    {
    (yyval.stringlist) = new std::list<string>;
    ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 55:
#line 559 "language.yy"
    {
    ((yyvsp[(1) - (3)].stringlist))->push_back((yyvsp[(3) - (3)].str));
    (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 58:
#line 569 "language.yy"
    {
    msc.ColorSets.top().AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, file_line(msc.current_file, yyget_lineno(yyscanner)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 59:
#line 576 "language.yy"
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

  case 60:
#line 588 "language.yy"
    {
    //push empty color & style sets for design definition
    msc.ColorSets.push(ColorSet());
    msc.StyleSets.push(StyleSet());
    msc.SetDesign("plain", true);
    msc.saved_hscale = msc.hscale;
}
    break;

  case 68:
#line 606 "language.yy"
    {
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 69:
#line 616 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), file_line(msc.current_file,
                                     yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 70:
#line 623 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), file_line(msc.current_file,
                                     yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 71:
#line 630 "language.yy"
    {
    msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 72:
#line 636 "language.yy"
    {
    msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 73:
#line 644 "language.yy"
    {
    (yyval.arcparallel) = (new ArcParallel(yyget_lineno(yyscanner), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
}
    break;

  case 74:
#line 648 "language.yy"
    {
    (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 75:
#line 653 "language.yy"
    {
	/*MscSetArcListLinenum($2, $1);*/
    (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
}
    break;

  case 76:
#line 658 "language.yy"
    {
    (yyval.arclist) = (new ArcList)->Append(new ArcDivider(MSC_ARC_VSPACE, yyget_lineno(yyscanner), &msc));
}
    break;

  case 77:
#line 663 "language.yy"
    {
    (yyval.arcbase)=(yyvsp[(1) - (1)].arcbase);
}
    break;

  case 78:
#line 667 "language.yy"
    {
    (yyval.arcbase)=(yyvsp[(2) - (2)].arcbase);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 79:
#line 672 "language.yy"
    {
    (yyval.arcbase) = static_cast<ArcEmphasis*>((yyvsp[(1) - (2)].arcbase))->AddFollow(static_cast<ArcEmphasis*>((yyvsp[(2) - (2)].arcbase)));
}
    break;

  case 80:
#line 678 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 81:
#line 682 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 82:
#line 686 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (5)].arcemph))->AddArcList((yyvsp[(3) - (5)].arclist));
}
    break;

  case 83:
#line 690 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (6)].arcemph))->AddArcList((yyvsp[(4) - (6)].arclist))->AddAttributeList((yyvsp[(2) - (6)].attriblist));
}
    break;

  case 84:
#line 696 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcemph))->EmptyStyle();
}
    break;

  case 85:
#line 700 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcemph))->EmptyStyle()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 86:
#line 704 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (5)].arcemph))->AddArcList((yyvsp[(3) - (5)].arclist))->EmptyStyle();
}
    break;

  case 87:
#line 708 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (6)].arcemph))->AddArcList((yyvsp[(4) - (6)].arclist))->EmptyStyle()->AddAttributeList((yyvsp[(2) - (6)].attriblist));
}
    break;

  case 88:
#line 715 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
}
    break;

  case 89:
#line 719 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 90:
#line 723 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (5)].arcemph))->AddArcList((yyvsp[(3) - (5)].arclist))->SetPipe();
}
    break;

  case 91:
#line 727 "language.yy"
    {
    (yyval.arcbase) = ((yyvsp[(1) - (6)].arcemph))->AddArcList((yyvsp[(4) - (6)].arclist))->SetPipe()->AddAttributeList((yyvsp[(2) - (6)].attriblist));
}
    break;

  case 92:
#line 732 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 93:
#line 738 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, (yyvsp[(2) - (2)].str), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 94:
#line 743 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), NULL, yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 95:
#line 748 "language.yy"
    {
    (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, NULL, yyget_lineno(yyscanner), &msc);
}
    break;

  case 96:
#line 753 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (2)].str));
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 97:
#line 759 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_SIDE);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 98:
#line 765 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_SIDE);
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 99:
#line 771 "language.yy"
    {
    (yyval.vertxpos) = NULL;
    switch ((yyvsp[(3) - (3)].arctype)) {
    case MSC_EMPH_SOLID:
        (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_BY);
        break;
    case MSC_EMPH_DASHED:
        (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_EMPH_DOTTED:
        msc.Error.Error(file_line(msc.current_file, yyget_lineno(yyscanner)),
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
    case MSC_EMPH_DOUBLE:
        msc.Error.Error(file_line(msc.current_file, yyget_lineno(yyscanner)),
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 100:
#line 793 "language.yy"
    {
    (yyval.vertxpos) = new VertXPos(yyget_lineno(yyscanner), msc, (yyvsp[(2) - (4)].str), VertXPos::POS_CENTER, (yyvsp[(4) - (4)].str));
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 104:
#line 803 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
    delete (yyvsp[(4) - (4)].vertxpos);
}
    break;

  case 105:
#line 810 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(2) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 106:
#line 816 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 107:
#line 822 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos),
                              yyget_lineno(yyscanner), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 108:
#line 828 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
    delete (yyvsp[(4) - (4)].vertxpos);
}
    break;

  case 109:
#line 835 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(2) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 110:
#line 841 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), yyget_lineno(yyscanner), &msc);
    free((yyvsp[(1) - (3)].str));
    delete (yyvsp[(3) - (3)].vertxpos);
}
    break;

  case 111:
#line 847 "language.yy"
    {
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos),
                              yyget_lineno(yyscanner), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 112:
#line 855 "language.yy"
    {
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), yyget_lineno(yyscanner), &msc);
}
    break;

  case 113:
#line 859 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 114:
#line 863 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 115:
#line 867 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 116:
#line 872 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), yyget_lineno(yyscanner));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 117:
#line 878 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR , (yyvsp[(2) - (2)].str), yyget_lineno(yyscanner));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 118:
#line 883 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), RSIDE_ENT_STR , yyget_lineno(yyscanner));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 119:
#line 888 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), true, yyget_lineno(yyscanner));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 120:
#line 893 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, true, yyget_lineno(yyscanner));
}
    break;

  case 121:
#line 899 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str), yyget_lineno(yyscanner));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 122:
#line 905 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), LSIDE_ENT_STR, yyget_lineno(yyscanner));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 123:
#line 910 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, (yyvsp[(1) - (2)].str), yyget_lineno(yyscanner));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 124:
#line 915 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), false, yyget_lineno(yyscanner));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 125:
#line 920 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, false, yyget_lineno(yyscanner));
}
    break;

  case 126:
#line 925 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), yyget_lineno(yyscanner));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 127:
#line 931 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, (yyvsp[(2) - (2)].str), yyget_lineno(yyscanner));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 128:
#line 936 "language.yy"
    {
    (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), RSIDE_ENT_STR, yyget_lineno(yyscanner));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 129:
#line 941 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), true, yyget_lineno(yyscanner));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 130:
#line 946 "language.yy"
    {
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, true, yyget_lineno(yyscanner));
}
    break;

  case 149:
#line 961 "language.yy"
    {
    (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 150:
#line 966 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(1) - (2)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 151:
#line 971 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), file_line(msc.current_file, yyget_lineno(yyscanner))));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 153:
#line 979 "language.yy"
    {
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 154:
#line 983 "language.yy"
    {
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 155:
#line 988 "language.yy"
    {
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
}
    break;

  case 156:
#line 992 "language.yy"
    {
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
}
    break;

  case 157:
#line 996 "language.yy"
    {
    (yyval.attriblist) = (yyvsp[(1) - (2)].attriblist);
    //Skip to next comma or closing bracket
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_CSBRACKET) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
}
    break;

  case 158:
#line 1006 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), file_line(msc.current_file, yyget_lineno(yyscanner)));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 159:
#line 1012 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), file_line(msc.current_file,
                       yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 160:
#line 1019 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), file_line(msc.current_file,
                       yyget_lineno(yyscanner)), (yyvsp[(3) - (3)].str));
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 161:
#line 1026 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, file_line(msc.current_file, yyget_lineno(yyscanner)));
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 162:
#line 1031 "language.yy"
    {
    (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), file_line(msc.current_file, yyget_lineno(yyscanner)));
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 173:
#line 1044 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 174:
#line 1045 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 175:
#line 1046 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 176:
#line 1047 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 177:
#line 1048 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 178:
#line 1049 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 179:
#line 1050 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 180:
#line 1051 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 181:
#line 1052 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 182:
#line 1053 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 183:
#line 1054 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 184:
#line 1055 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 185:
#line 1057 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 186:
#line 1064 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 191:
#line 1076 "language.yy"
    {
    msc.PushContext();
}
    break;

  case 192:
#line 1081 "language.yy"
    {
    msc.PopContext();
}
    break;


/* Line 1267 of yacc.c.  */
#line 3430 "language.cc"
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


#line 1086 "language.yy"



/* END OF FILE */

