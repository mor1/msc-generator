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

/* Substitute the variable and function names.  */
#define yyparse csh_parse
#define yylex   csh_lex
#define yyerror csh_error
#define yylval  csh_lval
#define yychar  csh_char
#define yydebug csh_debug
#define yynerrs csh_nerrs
#define yylloc csh_lloc

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
     TOK_VERTICAL = 298,
     TOK_AT = 299
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
#define TOK_VERTICAL 298
#define TOK_AT 299




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
#include "msc.h"
#ifdef C_S_H_IS_COMPILED
#include "colorsyntax.h"
#include "colorsyntax2.h"
#else
#include "language.h"
#include "language2.h"
#endif

/* Use verbose error reporting such that the expected token names are dumped */
#define YYERROR_VERBOSE

#ifdef C_S_H_IS_COMPILED
#define YYMSC_GETPOS(A) file_line()
#define ADDCSH(A, B) msc.AddCSH(A, B)
#define ADDCSH_ATTRVALUE(A, B, C) msc.AddCSH_AttrValue(A, B, C)
#define ADDCSH_ATTRNAME(A, B, C) msc.AddCSH_AttrName(A, B, C)
#define ADDCSH_ENTITYNAME(A, B) msc.AddCSH_EntityName(A, B)
#define ADDCSH_COLON_STRING(A, B, C) msc.AddCSH_ColonString(A, B, C)
#define SETLINEEND(ARC, FL, FC, LL, LC)

#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif
#else
#define YYMSC_GETPOS(A) file_line(msc.current_file, (A).first_line, (A).first_column)
#define ADDCSH(A, B)
#define ADDCSH_ATTRVALUE(A, B, C)
#define ADDCSH_ATTRNAME(A, B, C)
#define ADDCSH_ENTITYNAME(A, B)
#define ADDCSH_COLON_STRING(A, B, C)
#define SETLINEEND(ARC, FL, FC, LL, LC) do {(ARC)->SetLineEnd(FL, FC, LL, LC);} while(0)
#ifndef HAVE_UNISTD_H
extern int isatty (int);
#endif

#endif

/* yyerror
 *  Error handling function.  The TOK_XXX names are substituted for more
 *  understandable values that make more sense to the user.
 */
void yyerror(YYLTYPE*loc, Msc &msc, void *yyscanner, const char *str)
{
//only for language parser
#ifndef C_S_H_IS_COMPILED
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
      std::pair<string,string>("TOK_COLON_QUOTED_STRING", "':'"),  //just say colon to the user
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
    msc.Error.Error(YYMSC_GETPOS(*loc), msg, once_msg);
#endif
};

#ifdef C_S_H_IS_COMPILED
void CshParse(Msc &msc, const char *buff, unsigned len)
#else
void MscParse(Msc &msc, const char *buff, unsigned len)
#endif
{
    parse_parm  pp;
    pp.buf = const_cast<char*>(buff);
    pp.length = len;
    pp.pos = 0;
    pp.msc = &msc;
    yylex_init(&pp.yyscanner);
    yyset_extra(&pp, pp.yyscanner);
    yyparse(msc, pp.yyscanner);
    yylex_destroy(pp.yyscanner);
}

inline bool string_to_bool(const char*s)
{
   return (s[0]!='n' && s[0]!='N') || (s[1]!='o' && s[1]!='O');
}

#ifdef C_S_H_IS_COMPILED
#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_pos = YYRHSLOC (Rhs, 1).first_pos;	\
	  (Current).last_pos  = YYRHSLOC (Rhs, N).last_pos;	\
	}								\
      else								\
	{								\
	  (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
	}								\
    while (YYID (0))
#endif





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
#line 236 "language.yy"
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
}
/* Line 187 of yacc.c.  */
#line 424 "colorsyntax.cc"
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
#line 449 "colorsyntax.cc"

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
#define YYFINAL  122
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   766

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  45
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  205
/* YYNRULES -- Number of states.  */
#define YYNSTATES  267

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     6,     9,    13,    16,    21,    25,
      27,    30,    32,    35,    40,    44,    50,    55,    57,    61,
      63,    66,    69,    73,    76,    78,    81,    84,    88,    91,
      95,    97,    99,   101,   103,   105,   108,   111,   114,   116,
     119,   121,   124,   127,   131,   133,   136,   138,   142,   146,
     150,   154,   158,   160,   164,   167,   169,   171,   175,   178,
     180,   184,   186,   190,   194,   200,   207,   209,   211,   215,
     218,   221,   223,   225,   229,   232,   236,   240,   244,   248,
     250,   253,   255,   257,   260,   264,   268,   271,   276,   280,
     282,   285,   288,   292,   294,   297,   300,   303,   307,   309,
     312,   316,   319,   322,   324,   327,   331,   335,   339,   344,
     346,   348,   350,   355,   359,   363,   366,   371,   375,   379,
     382,   384,   386,   388,   390,   394,   397,   400,   404,   407,
     411,   414,   417,   421,   424,   428,   431,   434,   438,   441,
     443,   445,   447,   449,   451,   453,   455,   457,   459,   461,
     463,   465,   467,   469,   471,   473,   475,   477,   479,   481,
     483,   486,   489,   493,   495,   498,   502,   507,   511,   515,
     517,   521,   525,   529,   533,   536,   538,   540,   542,   544,
     546,   548,   550,   552,   554,   556,   558,   560,   562,   564,
     566,   568,   570,   572,   574,   576,   578,   580,   582,   584,
     586,   588,   590,   592,   594,   596
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      46,     0,    -1,    -1,    48,    -1,    48,     1,    -1,    97,
      48,    98,    -1,    97,    98,    -1,    97,    48,     1,    98,
      -1,    97,    48,     1,    -1,    51,    -1,    48,    51,    -1,
      49,    -1,    48,    49,    -1,    50,    11,    48,    12,    -1,
      50,    11,    12,    -1,    50,    11,    48,     1,    12,    -1,
      50,    11,    48,     1,    -1,    15,    -1,    15,     7,    96,
      -1,     9,    -1,     1,     9,    -1,    52,     9,    -1,    52,
       1,     9,    -1,    52,     1,    -1,    78,    -1,    78,    89,
      -1,    40,    78,    -1,    40,    78,    89,    -1,    43,    77,
      -1,    43,    77,    89,    -1,    89,    -1,    55,    -1,    53,
      -1,    69,    -1,    68,    -1,    37,    60,    -1,    38,    57,
      -1,    39,    62,    -1,    34,    -1,    34,    90,    -1,    35,
      -1,    35,    90,    -1,    42,    96,    -1,    42,    96,    90,
      -1,    36,    -1,    36,    90,    -1,    54,    -1,    53,     8,
      54,    -1,    93,     7,    19,    -1,    93,     7,     5,    -1,
      93,     7,    96,    -1,    15,     7,    96,    -1,    56,    -1,
      55,     8,    56,    -1,    93,    90,    -1,    93,    -1,    58,
      -1,    57,     8,    58,    -1,    59,    90,    -1,    96,    -1,
      59,     8,    96,    -1,    61,    -1,    60,     8,    61,    -1,
       3,     7,    96,    -1,     3,    63,    64,     9,    12,    -1,
       3,    63,    64,     9,     1,    12,    -1,    11,    -1,    65,
      -1,    64,     9,    65,    -1,    37,    60,    -1,    38,    57,
      -1,    66,    -1,    67,    -1,    66,     8,    67,    -1,    66,
       1,    -1,    93,     7,    19,    -1,    93,     7,     5,    -1,
      93,     7,    96,    -1,    15,     7,    96,    -1,    47,    -1,
      68,    47,    -1,    70,    -1,    73,    -1,    69,    74,    -1,
      69,    74,    89,    -1,    69,    74,    47,    -1,    69,    47,
      -1,    69,    74,    89,    47,    -1,    69,    89,    47,    -1,
      74,    -1,    74,    89,    -1,    74,    47,    -1,    74,    89,
      47,    -1,    74,    -1,    74,    89,    -1,    41,    71,    -1,
      72,    71,    -1,    72,    41,    71,    -1,    72,    -1,    72,
      47,    -1,    93,    33,    93,    -1,    33,    93,    -1,    93,
      33,    -1,    33,    -1,    44,    93,    -1,    44,    93,     6,
      -1,    44,    93,    10,    -1,    44,    93,    33,    -1,    44,
      93,     6,    93,    -1,    33,    -1,    82,    -1,    84,    -1,
      93,    76,    93,    75,    -1,    76,    93,    75,    -1,    93,
      76,    75,    -1,    76,    75,    -1,    93,    83,    93,    75,
      -1,    83,    93,    75,    -1,    93,    83,    75,    -1,    83,
      75,    -1,    32,    -1,    79,    -1,    80,    -1,    81,    -1,
      93,    82,    93,    -1,    82,    93,    -1,    93,    82,    -1,
      79,    85,    93,    -1,    79,    85,    -1,    93,    83,    93,
      -1,    83,    93,    -1,    93,    83,    -1,    80,    86,    93,
      -1,    80,    86,    -1,    93,    84,    93,    -1,    84,    93,
      -1,    93,    84,    -1,    81,    87,    93,    -1,    81,    87,
      -1,    20,    -1,    23,    -1,    26,    -1,    29,    -1,    21,
      -1,    24,    -1,    27,    -1,    30,    -1,    22,    -1,    25,
      -1,    28,    -1,    31,    -1,    82,    -1,     6,    -1,    83,
      -1,     6,    -1,    84,    -1,     6,    -1,    17,    -1,    16,
      -1,    88,    -1,    88,    90,    -1,    90,    88,    -1,    90,
      88,    90,    -1,    90,    -1,    13,    14,    -1,    13,    91,
      14,    -1,    13,    91,     1,    14,    -1,    13,     1,    14,
      -1,    13,    91,     1,    -1,    92,    -1,    91,     8,    92,
      -1,    96,     7,    96,    -1,    96,     7,     5,    -1,    96,
       7,    19,    -1,    96,     7,    -1,    96,    -1,     4,    -1,
       3,    -1,    15,    -1,    37,    -1,    38,    -1,    39,    -1,
      36,    -1,    40,    -1,    41,    -1,    43,    -1,    20,    -1,
      21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,    32,    -1,    33,    -1,    93,    -1,    94,    -1,
      95,    -1,    18,    -1,    11,    -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   298,   298,   301,   308,   317,   326,   335,   346,   358,
     366,   373,   378,   390,   399,   408,   419,   429,   437,   450,
     458,   467,   476,   487,   497,   498,   503,   514,   527,   536,
     545,   550,   555,   565,   570,   575,   584,   593,   602,   611,
     620,   629,   638,   648,   658,   667,   677,   685,   701,   714,
     727,   747,   761,   766,   775,   784,   795,   796,   803,   819,
     829,   841,   842,   849,   862,   880,   902,   909,   910,   917,
     924,   931,   933,   934,   940,   947,   959,   971,   983,   997,
    1005,  1016,  1021,  1026,  1032,  1039,  1045,  1052,  1059,  1069,
    1075,  1082,  1088,  1096,  1101,  1109,  1119,  1125,  1137,  1138,
    1144,  1156,  1166,  1176,  1185,  1196,  1208,  1220,  1251,  1266,
    1266,  1266,  1268,  1281,  1292,  1303,  1312,  1325,  1336,  1347,
    1358,  1366,  1370,  1374,  1379,  1391,  1401,  1411,  1421,  1431,
    1443,  1453,  1463,  1473,  1482,  1494,  1504,  1514,  1524,  1533,
    1533,  1533,  1533,  1534,  1534,  1534,  1534,  1535,  1535,  1535,
    1535,  1537,  1537,  1538,  1538,  1539,  1539,  1542,  1549,  1557,
    1565,  1573,  1581,  1593,  1596,  1605,  1614,  1624,  1634,  1644,
    1649,  1658,  1670,  1682,  1694,  1704,  1715,  1715,  1717,  1717,
    1718,  1718,  1719,  1719,  1719,  1720,  1722,  1723,  1724,  1725,
    1726,  1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1741,
    1751,  1751,  1751,  1751,  1753,  1758
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
  "TOK_COMMAND_PIPE", "TOK_COMMAND_MARK", "TOK_VERTICAL", "TOK_AT",
  "$accept", "msc", "braced_arclist", "arclist", "mscenclosed", "msckey",
  "complete_arc", "arc", "optlist", "opt", "entitylist", "entity",
  "styledeflist", "styledef", "tok_stringlist", "colordeflist", "colordef",
  "designdef", "scope_open_empty", "designelementlist", "designelement",
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
     295,   296,   297,   298,   299
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    45,    46,    46,    46,    47,    47,    47,    47,    48,
      48,    48,    48,    49,    49,    49,    49,    50,    50,    51,
      51,    51,    51,    51,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    54,    54,
      54,    54,    55,    55,    56,    56,    57,    57,    58,    59,
      59,    60,    60,    61,    62,    62,    63,    64,    64,    65,
      65,    65,    66,    66,    66,    67,    67,    67,    67,    68,
      68,    69,    69,    69,    69,    69,    69,    69,    69,    70,
      70,    70,    70,    71,    71,    72,    72,    72,    73,    73,
      74,    74,    74,    74,    75,    75,    75,    75,    75,    76,
      76,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      78,    78,    78,    78,    79,    79,    79,    79,    79,    80,
      80,    80,    80,    80,    81,    81,    81,    81,    81,    82,
      82,    82,    82,    83,    83,    83,    83,    84,    84,    84,
      84,    85,    85,    86,    86,    87,    87,    88,    88,    89,
      89,    89,    89,    89,    90,    90,    90,    90,    90,    91,
      91,    92,    92,    92,    92,    92,    93,    93,    94,    94,
      94,    94,    94,    94,    94,    94,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      96,    96,    96,    96,    97,    98
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     2,     3,     2,     4,     3,     1,
       2,     1,     2,     4,     3,     5,     4,     1,     3,     1,
       2,     2,     3,     2,     1,     2,     2,     3,     2,     3,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     2,
       1,     2,     2,     3,     1,     2,     1,     3,     3,     3,
       3,     3,     1,     3,     2,     1,     1,     3,     2,     1,
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
       2,     2,     3,     1,     2,     3,     4,     3,     3,     1,
       3,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   177,   176,    19,   204,     0,    17,   158,   157,
     139,   143,   147,   140,   144,   148,   141,   145,   149,   142,
     146,   150,   120,   103,    38,    40,    44,     0,     0,     0,
       0,     0,     0,     0,     0,    79,     0,    11,     0,     9,
       0,    32,    46,    31,    52,    34,    33,    81,    98,    82,
      89,    24,   121,   122,   123,     0,     0,     0,   159,    30,
     163,    55,     0,    20,     0,   164,   178,   203,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   182,   179,   180,   181,   183,   184,   185,     0,
     169,   200,   201,   202,   175,     0,   101,    39,    41,    45,
       0,    35,    61,    36,    56,     0,    59,     0,    37,    26,
       0,    95,    93,     0,    42,   109,     0,    28,   110,     0,
     111,     0,     1,     4,    12,    10,     0,    23,    21,     0,
       0,    80,    86,    83,     0,     0,    99,    96,    91,    90,
      25,   152,   151,   128,   154,   153,   133,   156,   155,   138,
     125,   130,   135,   160,   161,     0,   102,   126,   131,   136,
      54,   205,     0,     6,   167,   168,     0,   165,   174,    51,
       0,     0,     0,     0,    58,    66,     0,    27,    94,    43,
       0,   115,     0,    29,   119,     0,     0,     0,    14,     0,
      22,     0,    47,     0,    53,    55,    85,    84,    88,    97,
      92,   127,   132,   137,   162,    49,    48,    50,   100,   124,
     129,   134,     8,     5,   166,   170,   172,   173,   171,    63,
      62,    57,    60,     0,     0,     0,     0,    67,     0,    72,
       0,   104,   113,   117,   114,     0,   118,     0,    16,    13,
       0,    87,     7,     0,    69,    70,     0,    74,     0,     0,
     105,   106,   107,   112,   116,    15,    51,    78,     0,    64,
      68,    73,    76,    75,    77,   108,    65
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,   103,   104,   105,   101,   102,   108,   176,   226,
     227,   228,   229,    45,    46,    47,   111,    48,    49,    50,
     181,   116,   117,    51,    52,    53,    54,    55,    56,    57,
     143,   146,   149,    58,    59,    60,    89,    90,    91,    92,
      93,   106,    62,   163
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -154
static const yytype_int16 yypact[] =
{
     236,     9,  -154,  -154,  -154,  -154,   495,    21,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,   114,    36,    36,    36,    48,   188,    55,
     695,    31,   188,   661,    75,  -154,   280,  -154,    66,  -154,
      12,    86,  -154,    92,  -154,    94,   168,  -154,    50,  -154,
      28,   180,   119,    80,    57,   114,   114,   114,    36,  -154,
     115,   721,   323,  -154,    99,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,    98,
    -154,  -154,  -154,  -154,   116,   188,  -154,  -154,  -154,  -154,
     127,   113,  -154,   136,  -154,    34,  -154,   140,  -154,   180,
     735,  -154,   180,   120,    36,  -154,    22,   180,  -154,    22,
    -154,   680,  -154,     9,  -154,  -154,   366,   138,  -154,   179,
     114,  -154,  -154,    28,    94,    31,  -154,  -154,  -154,    94,
    -154,  -154,  -154,   114,  -154,  -154,   114,  -154,  -154,   114,
    -154,  -154,  -154,  -154,    36,   536,   114,   114,   114,   114,
    -154,  -154,   409,  -154,  -154,   146,   188,  -154,   577,   141,
     188,    48,   188,   188,  -154,  -154,    77,  -154,  -154,  -154,
     114,  -154,   110,  -154,  -154,   110,    22,    22,  -154,   452,
    -154,   160,  -154,   162,  -154,    36,  -154,    94,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,    56,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,   163,    48,   188,   164,  -154,    88,  -154,
     170,    10,  -154,  -154,  -154,   110,  -154,   110,    58,  -154,
     188,  -154,  -154,   188,   113,   136,   659,  -154,   239,   618,
     114,  -154,  -154,  -154,  -154,  -154,  -154,  -154,   166,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,   -31,   -50,   -34,  -154,   -25,  -154,  -154,    46,
    -154,    60,   -37,    23,  -154,   -26,    29,  -154,  -154,  -154,
     -42,  -154,   -43,  -154,  -154,  -154,   -40,  -154,  -154,   -24,
    -113,   101,  -154,   200,  -154,  -154,  -154,   -23,    -1,    17,
    -154,  -154,  -154,   172,    90,   -21,  -154,    67,     0,  -154,
    -154,    -5,  -154,  -153
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -72
static const yytype_int16 yytable[] =
{
      61,    94,   124,    97,    98,    99,   184,   112,   137,   213,
     118,   125,   162,   127,   131,   132,   250,   136,    63,   138,
     251,   128,   133,    96,   112,     2,     3,   114,    95,   142,
     110,   113,   119,   121,     2,     3,    61,   153,   157,     5,
     160,     6,   173,   252,     8,     9,   113,     6,   113,     6,
     120,   100,   145,     2,     3,   150,   151,   152,   107,   242,
     158,     5,    61,   147,    23,    63,   180,    63,   161,   232,
     255,   148,   233,   234,   236,   122,   189,   126,   159,    12,
       2,     3,    15,    23,   174,    18,   144,   157,    21,   247,
     169,   135,   223,   179,   129,   199,   248,   -71,   118,   165,
     130,    11,   196,   198,    14,     5,   166,    17,   200,   158,
      20,   112,   167,   164,   224,   225,   182,     2,     3,   185,
     187,   171,   253,   168,   254,   141,    61,   159,   124,   193,
     195,     8,     9,   204,   170,   113,   134,   125,   120,    10,
     139,   140,    13,   201,   172,    16,   202,   190,    19,   203,
     207,   175,   -18,   156,   180,   124,   208,   209,   210,   211,
     214,    94,    61,   218,   125,   219,   241,   240,   222,   155,
     243,     2,     3,   246,   160,   192,   230,   249,   266,     5,
     231,     6,     2,     3,     8,     9,   235,   237,   245,    61,
     194,     2,     3,     6,   191,   221,     8,     9,   244,   177,
     220,    23,   178,    66,   260,   261,    67,   183,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   186,   197,    82,    83,    84,    85,    86,    87,
     109,    88,   154,   215,     0,   256,    -2,     1,   257,     2,
       3,     0,     2,     3,   264,     4,   230,     5,   230,     6,
     265,     7,     8,     9,   223,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -3,   123,     0,     2,     3,     0,     0,     0,     0,     4,
       0,     5,     0,     6,     0,     7,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,     1,     0,     2,     3,     0,     0,
       0,     0,     4,     0,     5,   161,     6,     0,     7,     8,
       9,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     1,     0,     2,
       3,     0,     0,     0,     0,     4,     0,     5,   188,     6,
       0,     7,     8,     9,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
     212,     0,     2,     3,     0,     0,     0,     0,     4,     0,
       5,   161,     6,     0,     7,     8,     9,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,   238,     0,     2,     3,     0,     0,     0,
       0,     4,     0,     5,   239,     6,     0,     7,     8,     9,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    64,     0,     2,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    65,
      66,     0,     0,    67,     0,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,     0,
       0,    82,    83,    84,    85,    86,    87,     0,    88,     2,
       3,   205,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,    67,   206,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
       0,     0,    82,    83,    84,    85,    86,    87,     0,    88,
       2,     3,   216,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,    67,   217,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,     0,     0,    82,    83,    84,    85,    86,    87,     0,
      88,     2,     3,   262,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,    67,   263,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     0,     0,    82,    83,    84,    85,    86,    87,
     258,    88,     2,     3,     2,     3,     0,     0,     0,     0,
       0,   259,     0,     0,   223,     0,     0,     0,     0,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,   115,     0,   224,   225,     2,     3,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,   115,     0,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   155,     0,
       0,     0,     0,     0,     6,     0,     0,     0,     0,     0,
       0,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,   156,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     6,    36,    24,    25,    26,   119,    31,    48,   162,
      33,    36,    62,     1,    45,    46,     6,    48,     9,    50,
      10,     9,    46,    23,    48,     3,     4,    32,     7,    52,
      30,    31,    33,    33,     3,     4,    36,    58,    61,    11,
      61,    13,     8,    33,    16,    17,    46,    13,    48,    13,
      33,     3,    53,     3,     4,    55,    56,    57,     3,   212,
      61,    11,    62,     6,    33,     9,    44,     9,    12,   182,
      12,    54,   185,   186,   187,     0,   126,    11,    61,    22,
       3,     4,    25,    33,   105,    28,     6,   110,    31,     1,
      95,    41,    15,   114,     8,   135,     8,     9,   121,     1,
       8,    21,   133,   134,    24,    11,     8,    27,   139,   110,
      30,   135,    14,    14,    37,    38,   116,     3,     4,   119,
     121,     8,   235,     7,   237,     6,   126,   110,   162,   129,
     130,    16,    17,   154,     7,   135,    46,   162,   121,    20,
      50,    51,    23,   143,     8,    26,   146,     9,    29,   149,
     155,    11,    11,    33,    44,   189,   156,   157,   158,   159,
      14,   166,   162,   168,   189,   170,   197,     7,   173,     7,
       7,     3,     4,     9,   195,   129,   176,     7,    12,    11,
     180,    13,     3,     4,    16,    17,   186,   187,   225,   189,
     130,     3,     4,    13,    15,   172,    16,    17,   224,   109,
     171,    33,   112,    15,   246,   248,    18,   117,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,   121,   133,    36,    37,    38,    39,    40,    41,
      30,    43,    60,   166,    -1,   240,     0,     1,   243,     3,
       4,    -1,     3,     4,   249,     9,   246,    11,   248,    13,
     250,    15,    16,    17,    15,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
       0,     1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    11,    -1,    13,    -1,    15,    16,    17,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     1,    -1,     3,     4,    -1,    -1,
      -1,    -1,     9,    -1,    11,    12,    13,    -1,    15,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,     1,    -1,     3,
       4,    -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,
      -1,    15,    16,    17,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
       1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,
      11,    12,    13,    -1,    15,    16,    17,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     1,    -1,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,     1,    -1,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    14,
      15,    -1,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      -1,    36,    37,    38,    39,    40,    41,    -1,    43,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    -1,    36,    37,    38,    39,    40,    41,    -1,    43,
       3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    -1,    36,    37,    38,    39,    40,    41,    -1,
      43,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    -1,    36,    37,    38,    39,    40,    41,
       1,    43,     3,     4,     3,     4,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    -1,    37,    38,     3,     4,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,     7,    -1,
      -1,    -1,    -1,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     9,    11,    13,    15,    16,    17,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    68,    69,    70,    72,    73,
      74,    78,    79,    80,    81,    82,    83,    84,    88,    89,
      90,    93,    97,     9,     1,    14,    15,    18,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    36,    37,    38,    39,    40,    41,    43,    91,
      92,    93,    94,    95,    96,     7,    93,    90,    90,    90,
       3,    60,    61,    57,    58,    59,    96,     3,    62,    78,
      93,    71,    74,    93,    96,    33,    76,    77,    82,    83,
      84,    93,     0,     1,    49,    51,    11,     1,     9,     8,
       8,    47,    47,    74,    89,    41,    47,    71,    47,    89,
      89,     6,    82,    85,     6,    83,    86,     6,    84,    87,
      93,    93,    93,    90,    88,     7,    33,    82,    83,    84,
      90,    12,    48,    98,    14,     1,     8,    14,     7,    96,
       7,     8,     8,     8,    90,    11,    63,    89,    89,    90,
      44,    75,    93,    89,    75,    93,    76,    83,    12,    48,
       9,    15,    54,    93,    56,    93,    47,    89,    47,    71,
      47,    93,    93,    93,    90,     5,    19,    96,    93,    93,
      93,    93,     1,    98,    14,    92,     5,    19,    96,    96,
      61,    58,    96,    15,    37,    38,    64,    65,    66,    67,
      93,    93,    75,    75,    75,    93,    75,    93,     1,    12,
       7,    47,    98,     7,    60,    57,     9,     1,     8,     7,
       6,    10,    33,    75,    75,    12,    96,    96,     1,    12,
      65,    67,     5,    19,    96,    93,    12
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
#line 289 "language.yy"
	{free((yyvaluep->str));};
#line 1756 "colorsyntax.cc"
	break;
      case 4: /* "TOK_QSTRING" */
#line 289 "language.yy"
	{free((yyvaluep->str));};
#line 1761 "colorsyntax.cc"
	break;
      case 5: /* "TOK_NUMBER" */
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1766 "colorsyntax.cc"
	break;
      case 15: /* "TOK_MSC" */
#line 290 "language.yy"
	{free((yyvaluep->str));};
#line 1771 "colorsyntax.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */
#line 289 "language.yy"
	{free((yyvaluep->str));};
#line 1776 "colorsyntax.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */
#line 289 "language.yy"
	{free((yyvaluep->str));};
#line 1781 "colorsyntax.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */
#line 289 "language.yy"
	{free((yyvaluep->str));};
#line 1786 "colorsyntax.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1791 "colorsyntax.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1796 "colorsyntax.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1801 "colorsyntax.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1806 "colorsyntax.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */
#line 291 "language.yy"
	{free((yyvaluep->str));};
#line 1811 "colorsyntax.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */
#line 291 "language.yy"
	{free((yyvaluep->str));};
#line 1816 "colorsyntax.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */
#line 291 "language.yy"
	{free((yyvaluep->str));};
#line 1821 "colorsyntax.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */
#line 290 "language.yy"
	{free((yyvaluep->str));};
#line 1826 "colorsyntax.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */
#line 290 "language.yy"
	{free((yyvaluep->str));};
#line 1831 "colorsyntax.cc"
	break;
      case 47: /* "braced_arclist" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1836 "colorsyntax.cc"
	break;
      case 48: /* "arclist" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1841 "colorsyntax.cc"
	break;
      case 49: /* "mscenclosed" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1846 "colorsyntax.cc"
	break;
      case 51: /* "complete_arc" */
#line 281 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1851 "colorsyntax.cc"
	break;
      case 52: /* "arc" */
#line 281 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1856 "colorsyntax.cc"
	break;
      case 53: /* "optlist" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1861 "colorsyntax.cc"
	break;
      case 54: /* "opt" */
#line 281 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1866 "colorsyntax.cc"
	break;
      case 55: /* "entitylist" */
#line 286 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};
#line 1871 "colorsyntax.cc"
	break;
      case 56: /* "entity" */
#line 286 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};
#line 1876 "colorsyntax.cc"
	break;
      case 59: /* "tok_stringlist" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};
#line 1881 "colorsyntax.cc"
	break;
      case 68: /* "parallel" */
#line 284 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};
#line 1886 "colorsyntax.cc"
	break;
      case 69: /* "emphasis_list" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1891 "colorsyntax.cc"
	break;
      case 70: /* "first_emphasis" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1896 "colorsyntax.cc"
	break;
      case 71: /* "pipe_def" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1901 "colorsyntax.cc"
	break;
      case 72: /* "pipe_def_list" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1906 "colorsyntax.cc"
	break;
      case 73: /* "pipe_emphasis" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1911 "colorsyntax.cc"
	break;
      case 74: /* "emphrel" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1916 "colorsyntax.cc"
	break;
      case 75: /* "vertxpos" */
#line 280 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};
#line 1921 "colorsyntax.cc"
	break;
      case 77: /* "vertrel" */
#line 279 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1926 "colorsyntax.cc"
	break;
      case 78: /* "arcrel" */
#line 281 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1931 "colorsyntax.cc"
	break;
      case 79: /* "arcrel_to" */
#line 282 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1936 "colorsyntax.cc"
	break;
      case 80: /* "arcrel_from" */
#line 282 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1941 "colorsyntax.cc"
	break;
      case 81: /* "arcrel_bidir" */
#line 282 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1946 "colorsyntax.cc"
	break;
      case 89: /* "full_arcattrlist_with_label" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1951 "colorsyntax.cc"
	break;
      case 90: /* "full_arcattrlist" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1956 "colorsyntax.cc"
	break;
      case 91: /* "arcattrlist" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1961 "colorsyntax.cc"
	break;
      case 92: /* "arcattr" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};
#line 1966 "colorsyntax.cc"
	break;
      case 93: /* "entity_string" */
#line 288 "language.yy"
	{free((yyvaluep->str));};
#line 1971 "colorsyntax.cc"
	break;
      case 94: /* "reserved_word_string" */
#line 288 "language.yy"
	{free((yyvaluep->str));};
#line 1976 "colorsyntax.cc"
	break;
      case 95: /* "symbol_string" */
#line 288 "language.yy"
	{free((yyvaluep->str));};
#line 1981 "colorsyntax.cc"
	break;
      case 96: /* "string" */
#line 288 "language.yy"
	{free((yyvaluep->str));};
#line 1986 "colorsyntax.cc"
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
#line 298 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:
#line 302 "language.yy"
    {
    if (C_S_H) {
    } else {
        msc.AddArcs((yyvsp[(1) - (1)].arclist));
    }
}
    break;

  case 4:
#line 309 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        msc.AddArcs((yyvsp[(1) - (2)].arclist));
    }
}
    break;

  case 5:
#line 318 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
    }
}
    break;

  case 6:
#line 327 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_BRACE);
        ADDCSH((yylsp[(2) - (2)]), COLOR_BRACE);
    } else {
         (yyval.arclist) = new ArcList;
    }
}
    break;

  case 7:
#line 336 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (4)]), COLOR_ERROR);
        ADDCSH((yylsp[(4) - (4)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(2) - (4)].arclist);
    }
    yyerrok;
}
    break;

  case 8:
#line 347 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ERROR);
    } else {
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
        //Do not pop context, as the missing scope_close would have done
    }
}
    break;

  case 9:
#line 359 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
}
    break;

  case 10:
#line 367 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
}
    break;

  case 11:
#line 374 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
}
    break;

  case 12:
#line 379 "language.yy"
    {
    if (C_S_H) break;
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 13:
#line 391 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (4)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
    }
}
    break;

  case 14:
#line 400 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = new ArcList;
    }
}
    break;

  case 15:
#line 409 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (5)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (5)]), COLOR_ERROR);
        ADDCSH((yylsp[(5) - (5)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(3) - (5)].arclist);
    }
    yyerrok;
}
    break;

  case 16:
#line 420 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (4)]), COLOR_ERROR);
    } else {
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
    }
}
    break;

  case 17:
#line 430 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 18:
#line 438 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    } else {
        msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 19:
#line 451 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=NULL;
    }
}
    break;

  case 20:
#line 459 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_ERROR);
        ADDCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=NULL;
    }
}
    break;

  case 21:
#line 468 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    } else {
        if ((yyvsp[(1) - (2)].arcbase)) SETLINEEND((yyvsp[(1) - (2)].arcbase), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 22:
#line 477 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        ADDCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
    } else {
        if ((yyvsp[(1) - (3)].arcbase)) SETLINEEND((yyvsp[(1) - (3)].arcbase), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
        (yyval.arcbase)=(yyvsp[(1) - (3)].arcbase);
    }
    yyerrok;
}
    break;

  case 23:
#line 488 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        if ((yyvsp[(1) - (2)].arcbase)) SETLINEEND((yyvsp[(1) - (2)].arcbase), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 25:
#line 499 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 26:
#line 504 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        //Returns NULL, if BIG is before a self-pointing arrow
        (yyval.arcbase) = msc.CreateArcBigArrow((yyvsp[(2) - (2)].arcbase));
        delete (yyvsp[(2) - (2)].arcbase);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 27:
#line 515 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    } else {
        //Returns NULL, if BIG is before a self-pointing arrow
        ArcBigArrow *arrow = msc.CreateArcBigArrow((yyvsp[(2) - (3)].arcbase));
        if (arrow) arrow->AddAttributeList((yyvsp[(3) - (3)].attriblist));
        (yyval.arcbase) = arrow;
        delete (yyvsp[(2) - (3)].arcbase);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 28:
#line 528 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 29:
#line 537 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 30:
#line 546 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
}
    break;

  case 31:
#line 551 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = new CommandEntity((yyvsp[(1) - (1)].entitylist), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 32:
#line 556 "language.yy"
    {
    if (C_S_H) break;
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
}
    break;

  case 33:
#line 566 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
}
    break;

  case 34:
#line 571 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
}
    break;

  case 35:
#line 576 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 36:
#line 585 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 37:
#line 594 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 38:
#line 603 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new CommandEntity(NULL, YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 39:
#line 612 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new CommandEntity(NULL, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 40:
#line 621 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 41:
#line 630 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 42:
#line 639 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new CommandMark((yyvsp[(2) - (2)].str), YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 43:
#line 649 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 44:
#line 659 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new CommandNewpage(YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 45:
#line 668 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new CommandNewpage(YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 46:
#line 678 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
}
    break;

  case 47:
#line 686 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
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

  case 48:
#line 702 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 49:
#line 715 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
       msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
       (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 50:
#line 728 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH_ATTRVALUE((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
    } else {
        Attribute a((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])));
        MscFillAttr fill;
        fill.Empty();
        if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
            (yyval.arcbase) = new CommandNewBackground(a.linenum_attr, &msc, fill);
        } else {
            msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
            (yyval.arcbase) = NULL;
        }
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 51:
#line 748 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 52:
#line 762 "language.yy"
    {
    if (C_S_H) break;
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
}
    break;

  case 53:
#line 767 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
}
    break;

  case 54:
#line 776 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    } else {
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 55:
#line 785 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    } else {
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 57:
#line 797 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 58:
#line 804 "language.yy"
    {
    if (C_S_H) {
        break;
    }
    for (std::list<string>::iterator i = ((yyvsp[(1) - (2)].stringlist))->begin(); i!=((yyvsp[(1) - (2)].stringlist))->end(); i++) {
        MscStyle style = msc.StyleSets.top().GetStyle(*i);
        AttributeList::iterator j=((yyvsp[(2) - (2)].attriblist))->begin();
        while (j!=((yyvsp[(2) - (2)].attriblist))->end())
           style.AddAttribute(**(j++), &msc);
        msc.StyleSets.top()[*i] = style;
    }
    delete((yyvsp[(1) - (2)].stringlist));
    delete((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 59:
#line 820 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    } else {
        (yyval.stringlist) = new std::list<string>;
        ((yyval.stringlist))->push_back((yyvsp[(1) - (1)].str));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 60:
#line 830 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        ADDCSH((yylsp[(3) - (3)]), COLOR_STYLENAME);
    } else {
       ((yyvsp[(1) - (3)].stringlist))->push_back((yyvsp[(3) - (3)].str));
       (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 62:
#line 843 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 63:
#line 850 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_COLORNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_COLORDEF);
    } else {
        msc.ColorSets.top().AddColor((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), msc.Error, YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 64:
#line 863 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (5)]), COLOR_DESIGNNAME);
        ADDCSH((yylsp[(2) - (5)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (5)]), COLOR_SEMICOLON);
        ADDCSH((yylsp[(5) - (5)]), COLOR_BRACE);
        break;
    }
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

  case 65:
#line 881 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (6)]), COLOR_DESIGNNAME);
        ADDCSH((yylsp[(2) - (6)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (6)]), COLOR_SEMICOLON);
        ADDCSH((yylsp[(5) - (6)]), COLOR_ERROR);
        ADDCSH((yylsp[(6) - (6)]), COLOR_BRACE);
        break;
    }
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[(yyvsp[(1) - (6)].str)];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
}
    break;

  case 66:
#line 903 "language.yy"
    {
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
}
    break;

  case 68:
#line 911 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
    }
}
    break;

  case 69:
#line 918 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 70:
#line 925 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 73:
#line 935 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 74:
#line 941 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    }
}
    break;

  case 75:
#line 948 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 76:
#line 960 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 77:
#line 972 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 78:
#line 984 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 79:
#line 998 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
}
    break;

  case 80:
#line 1006 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(2) - (2)].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[(1) - (2)].arcparallel);
    else if ((yyvsp[(1) - (2)].arcparallel))
        (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
    else
        (yyval.arcparallel) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 81:
#line 1017 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 82:
#line 1022 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 83:
#line 1027 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (2)].arcemph), (yylsp[(2) - (2)]).first_line, (yylsp[(2) - (2)]).first_column, (yylsp[(2) - (2)]).last_line, (yylsp[(2) - (2)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
}
    break;

  case 84:
#line 1033 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    SETLINEEND((yyvsp[(2) - (3)].arcemph), (yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, (yylsp[(3) - (3)]).last_line, (yylsp[(3) - (3)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
}
    break;

  case 85:
#line 1040 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (3)].arcemph), (yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, (yylsp[(2) - (3)]).last_line, (yylsp[(2) - (3)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
}
    break;

  case 86:
#line 1046 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
}
    break;

  case 87:
#line 1053 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (4)].arcemph), (yylsp[(2) - (4)]).first_line, (yylsp[(2) - (4)]).first_column, (yylsp[(3) - (4)]).last_line, (yylsp[(3) - (4)]).last_column);
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
}
    break;

  case 88:
#line 1060 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), YYMSC_GETPOS((yyloc)), &msc);
    SETLINEEND(temp, (yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, (yylsp[(2) - (3)]).last_line, (yylsp[(2) - (3)]).last_column);
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
}
    break;

  case 89:
#line 1070 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (1)].arcemph), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 90:
#line 1076 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    SETLINEEND((yyvsp[(1) - (2)].arcemph), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 91:
#line 1083 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (2)].arcemph), (yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yylsp[(1) - (2)]).last_line, (yylsp[(1) - (2)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 92:
#line 1089 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (3)].arcemph), (yylsp[(1) - (3)]).first_line, (yylsp[(1) - (3)]).first_column, (yylsp[(2) - (3)]).last_line, (yylsp[(2) - (3)]).last_column);
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 93:
#line 1097 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
}
    break;

  case 94:
#line 1102 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    SETLINEEND((yyvsp[(1) - (2)].arcemph), (yylsp[(1) - (2)]).first_line, (yylsp[(1) - (2)]).first_column, (yylsp[(2) - (2)]).last_line, (yylsp[(2) - (2)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 95:
#line 1110 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        SETLINEEND((yyvsp[(2) - (2)].arcemph), (yyloc).first_line, (yyloc).first_column, (yyloc).last_line, (yyloc).last_column);
        (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 96:
#line 1120 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (2)].arcemph), (yylsp[(2) - (2)]).first_line, (yylsp[(2) - (2)]).first_column, (yylsp[(2) - (2)]).last_line, (yylsp[(2) - (2)]).last_column);
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
}
    break;

  case 97:
#line 1126 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    } else {
        SETLINEEND((yyvsp[(3) - (3)].arcemph), (yylsp[(2) - (3)]).first_line, (yylsp[(2) - (3)]).first_column, (yylsp[(3) - (3)]).last_line, (yylsp[(3) - (3)]).last_column);
        (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(3) - (3)].arcemph));
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 99:
#line 1139 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 100:
#line 1145 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 101:
#line 1157 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 102:
#line 1167 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 103:
#line 1177 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), YYMSC_GETPOS((yyloc)), &msc);
    }
}
    break;

  case 104:
#line 1186 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (2)])), msc, (yyvsp[(2) - (2)].str));
    }
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 105:
#line 1197 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (3)])), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_SIDE);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 106:
#line 1209 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (3)])), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_SIDE);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 107:
#line 1221 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        switch ((yyvsp[(3) - (3)].arctype)) {
        case MSC_EMPH_SOLID:
            (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (3)])), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_LEFT_BY);
            break;
        case MSC_EMPH_DASHED:
            (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (3)])), msc, (yyvsp[(2) - (3)].str), VertXPos::POS_RIGHT_BY);
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
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 108:
#line 1252 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
        ADDCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(4) - (4)]), (yyvsp[(4) - (4)].str));
    } else {
        (yyval.vertxpos) = new VertXPos(YYMSC_GETPOS((yylsp[(2) - (4)])), msc, (yyvsp[(2) - (4)].str), VertXPos::POS_CENTER, (yyvsp[(4) - (4)].str));
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 112:
#line 1269 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 113:
#line 1282 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 114:
#line 1293 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 115:
#line 1304 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
    }
}
    break;

  case 116:
#line 1313 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 117:
#line 1326 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 118:
#line 1337 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 119:
#line 1348 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 120:
#line 1359 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 121:
#line 1367 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 122:
#line 1371 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 123:
#line 1375 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 124:
#line 1380 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 125:
#line 1392 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 126:
#line 1402 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 127:
#line 1412 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS((yylsp[(2) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 128:
#line 1422 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 129:
#line 1432 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 130:
#line 1444 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 131:
#line 1454 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 132:
#line 1464 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), false, YYMSC_GETPOS((yylsp[(2) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 133:
#line 1474 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), false, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 134:
#line 1483 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 135:
#line 1495 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 136:
#line 1505 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 137:
#line 1515 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS((yylsp[(2) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 138:
#line 1525 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 157:
#line 1543 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
    };
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 158:
#line 1550 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
    };
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 159:
#line 1558 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)).IncCol()));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 160:
#line 1566 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yylsp[(1) - (2)])).IncCol()));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 161:
#line 1574 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yylsp[(2) - (2)])).IncCol()));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 162:
#line 1582 "language.yy"
    {
    if (C_S_H) {
    } else {
        ((yyvsp[(1) - (3)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), YYMSC_GETPOS((yylsp[(2) - (3)])).IncCol()));
        //Merge $3 at the end of $1
        ((yyvsp[(1) - (3)].attriblist))->splice(((yyvsp[(1) - (3)].attriblist))->end(), *((yyvsp[(3) - (3)].attriblist)));
        delete ((yyvsp[(3) - (3)].attriblist)); //empty list now
        (yyval.attriblist) = (yyvsp[(1) - (3)].attriblist);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 164:
#line 1597 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        ADDCSH((yylsp[(2) - (2)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 165:
#line 1606 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 166:
#line 1615 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (4)]), COLOR_ERROR);
        ADDCSH((yylsp[(4) - (4)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (4)].attriblist);
}
    break;

  case 167:
#line 1625 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 168:
#line 1635 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ERROR);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 169:
#line 1645 "language.yy"
    {
    if (C_S_H) break;
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
}
    break;

  case 170:
#line 1650 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
}
    break;

  case 171:
#line 1659 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH_ATTRVALUE((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str), (yyvsp[(1) - (3)].str));
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 172:
#line 1671 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 173:
#line 1683 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_ATTRNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 174:
#line 1695 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str), COLOR_ATTRNAME);
        ADDCSH((yylsp[(2) - (2)]), COLOR_EQUAL);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (2)].str), (char*)NULL, YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 175:
#line 1705 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 186:
#line 1722 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 187:
#line 1723 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 188:
#line 1724 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 189:
#line 1725 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 190:
#line 1726 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 191:
#line 1727 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 192:
#line 1728 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 193:
#line 1729 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 194:
#line 1730 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 195:
#line 1731 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 196:
#line 1732 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 197:
#line 1733 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 198:
#line 1735 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 199:
#line 1742 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 204:
#line 1754 "language.yy"
    {
    msc.PushContext();
}
    break;

  case 205:
#line 1759 "language.yy"
    {
    msc.PopContext();
}
    break;


/* Line 1267 of yacc.c.  */
#line 4187 "colorsyntax.cc"
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


#line 1764 "language.yy"



/* END OF FILE */

