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
#define YYMSC_GETPOS(A) file_line_range()
#define YYMSC_GETPOS2(A, B) file_line_range()
#define ADDCSH(A, B) msc.AddCSH(A, B)
#define ADDCSH_ATTRVALUE(A, B, C) msc.AddCSH_AttrValue(A, B, C)
#define ADDCSH_ATTRNAME(A, B, C) msc.AddCSH_AttrName(A, B, C)
#define ADDCSH_ENTITYNAME(A, B) msc.AddCSH_EntityName(A, B)
#define ADDCSH_COLON_STRING(A, B, C) msc.AddCSH_ColonString(A, B, C)
#define SETLINEEND(ARC, F, L)
string ConvertEmphasisToBox(const string &style, const YYLTYPE *loc, Msc &msc) {return string();}
#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif
#else
#define YYMSC_GETPOS(A) YYMSC_GETPOS2(A,A)
#define YYMSC_GETPOS2(A, B) file_line_range(                                   \
            file_line(msc.current_file, (A).first_line, (A).first_column), \
            file_line(msc.current_file, (B).last_line, (B).last_column))
#define ADDCSH(A, B)
#define ADDCSH_ATTRVALUE(A, B, C)
#define ADDCSH_ATTRNAME(A, B, C)
#define ADDCSH_ENTITYNAME(A, B)
#define ADDCSH_COLON_STRING(A, B, C)
#define SETLINEEND(ARC, F, L) do {(ARC)->SetLineEnd(YYMSC_GETPOS2(F,L));} while(0)
string ConvertEmphasisToBox(const string &style, const YYLTYPE *loc, Msc &msc)
{
    const char *newname = style == "emphasis"?"box":"emptybox";
    if (style == "emphasis" || style == "emptyemphasis") {

        msc.Error.Warning(YYMSC_GETPOS(*loc).start,
            "Stylename '" + style + "' is deprecated, using " + newname + " instead.");
        return newname;
    }
    return style;
}
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
      std::pair<string,string>("TOK_COMMAND_PARALLEL", "'parallel'"),
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
    msc.Error.Error(YYMSC_GETPOS(*loc).start, msg, once_msg);
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
	}						        \
      else							\
	{							\
	  (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
	}							\
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
#line 253 "language.yy"
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
#line 434 "language.cc"
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
#line 459 "language.cc"

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
#define YYFINAL  132
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   877

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  215
/* YYNRULES -- Number of states.  */
#define YYNSTATES  278

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
       0,     0,     3,     4,     6,     8,    11,    14,    18,    22,
      25,    30,    34,    36,    39,    41,    44,    49,    53,    59,
      64,    66,    70,    72,    75,    78,    82,    85,    87,    90,
      92,    95,    98,   102,   105,   109,   111,   113,   115,   117,
     119,   122,   125,   128,   130,   133,   135,   138,   141,   145,
     147,   150,   152,   156,   160,   164,   168,   172,   174,   178,
     181,   183,   185,   189,   192,   194,   198,   200,   204,   208,
     214,   221,   223,   225,   229,   232,   235,   237,   239,   243,
     246,   250,   254,   258,   262,   264,   267,   269,   271,   274,
     278,   282,   285,   290,   294,   296,   299,   302,   306,   308,
     311,   314,   317,   321,   323,   326,   330,   333,   336,   338,
     341,   345,   349,   353,   358,   360,   362,   364,   369,   373,
     377,   380,   385,   389,   393,   396,   398,   400,   402,   404,
     408,   411,   414,   418,   421,   425,   428,   431,   435,   438,
     442,   445,   448,   452,   455,   457,   459,   461,   463,   465,
     467,   469,   471,   473,   475,   477,   479,   481,   483,   485,
     487,   489,   491,   493,   495,   497,   500,   503,   507,   509,
     512,   516,   521,   525,   529,   531,   535,   539,   543,   547,
     550,   552,   554,   556,   558,   560,   562,   564,   566,   568,
     570,   572,   574,   576,   578,   580,   582,   584,   586,   588,
     590,   592,   594,   596,   598,   600,   602,   604,   606,   608,
     610,   612,   614,   616,   618,   620
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,    -1,    48,    -1,    50,    -1,    50,     1,
      -1,    50,    12,    -1,    50,    12,    48,    -1,   100,    50,
     101,    -1,   100,   101,    -1,   100,    50,     1,   101,    -1,
     100,    50,     1,    -1,    53,    -1,    50,    53,    -1,    51,
      -1,    50,    51,    -1,    52,    11,    50,    12,    -1,    52,
      11,    12,    -1,    52,    11,    50,     1,    12,    -1,    52,
      11,    50,     1,    -1,    15,    -1,    15,     7,    99,    -1,
       9,    -1,     1,     9,    -1,    54,     9,    -1,    54,     1,
       9,    -1,    54,     1,    -1,    55,    -1,    43,    55,    -1,
      81,    -1,    81,    92,    -1,    40,    81,    -1,    40,    81,
      92,    -1,    44,    80,    -1,    44,    80,    92,    -1,    92,
      -1,    58,    -1,    56,    -1,    72,    -1,    71,    -1,    37,
      63,    -1,    38,    60,    -1,    39,    65,    -1,    34,    -1,
      34,    93,    -1,    35,    -1,    35,    93,    -1,    42,    99,
      -1,    42,    99,    93,    -1,    36,    -1,    36,    93,    -1,
      57,    -1,    56,     8,    57,    -1,    96,     7,    19,    -1,
      96,     7,     5,    -1,    96,     7,    99,    -1,    15,     7,
      99,    -1,    59,    -1,    58,     8,    59,    -1,    96,    93,
      -1,    96,    -1,    61,    -1,    60,     8,    61,    -1,    62,
      93,    -1,    99,    -1,    62,     8,    99,    -1,    64,    -1,
      63,     8,    64,    -1,     3,     7,    99,    -1,     3,    66,
      67,     9,    12,    -1,     3,    66,    67,     9,     1,    12,
      -1,    11,    -1,    68,    -1,    67,     9,    68,    -1,    37,
      63,    -1,    38,    60,    -1,    69,    -1,    70,    -1,    69,
       8,    70,    -1,    69,     1,    -1,    96,     7,    19,    -1,
      96,     7,     5,    -1,    96,     7,    99,    -1,    15,     7,
      99,    -1,    49,    -1,    71,    49,    -1,    73,    -1,    76,
      -1,    72,    77,    -1,    72,    77,    92,    -1,    72,    77,
      49,    -1,    72,    49,    -1,    72,    77,    92,    49,    -1,
      72,    92,    49,    -1,    77,    -1,    77,    92,    -1,    77,
      49,    -1,    77,    92,    49,    -1,    77,    -1,    77,    92,
      -1,    41,    74,    -1,    75,    74,    -1,    75,    41,    74,
      -1,    75,    -1,    75,    49,    -1,    96,    33,    96,    -1,
      33,    96,    -1,    96,    33,    -1,    33,    -1,    45,    96,
      -1,    45,    96,     6,    -1,    45,    96,    10,    -1,    45,
      96,    33,    -1,    45,    96,     6,    96,    -1,    33,    -1,
      85,    -1,    87,    -1,    96,    79,    96,    78,    -1,    79,
      96,    78,    -1,    96,    79,    78,    -1,    79,    78,    -1,
      96,    86,    96,    78,    -1,    86,    96,    78,    -1,    96,
      86,    78,    -1,    86,    78,    -1,    32,    -1,    82,    -1,
      83,    -1,    84,    -1,    96,    85,    96,    -1,    85,    96,
      -1,    96,    85,    -1,    82,    88,    96,    -1,    82,    88,
      -1,    96,    86,    96,    -1,    86,    96,    -1,    96,    86,
      -1,    83,    89,    96,    -1,    83,    89,    -1,    96,    87,
      96,    -1,    87,    96,    -1,    96,    87,    -1,    84,    90,
      96,    -1,    84,    90,    -1,    20,    -1,    23,    -1,    26,
      -1,    29,    -1,    21,    -1,    24,    -1,    27,    -1,    30,
      -1,    22,    -1,    25,    -1,    28,    -1,    31,    -1,    85,
      -1,     6,    -1,    86,    -1,     6,    -1,    87,    -1,     6,
      -1,    17,    -1,    16,    -1,    91,    -1,    91,    93,    -1,
      93,    91,    -1,    93,    91,    93,    -1,    93,    -1,    13,
      14,    -1,    13,    94,    14,    -1,    13,    94,     1,    14,
      -1,    13,     1,    14,    -1,    13,    94,     1,    -1,    95,
      -1,    94,     8,    95,    -1,    99,     7,    99,    -1,    99,
       7,     5,    -1,    99,     7,    19,    -1,    99,     7,    -1,
      99,    -1,     4,    -1,     3,    -1,    15,    -1,    37,    -1,
      38,    -1,    39,    -1,    36,    -1,    40,    -1,    41,    -1,
      44,    -1,    43,    -1,    34,    -1,    35,    -1,    42,    -1,
      45,    -1,    20,    -1,    21,    -1,    22,    -1,    23,    -1,
      24,    -1,    25,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    31,    -1,    32,    -1,    33,    -1,
      96,    -1,    97,    -1,    98,    -1,    18,    -1,    11,    -1,
      12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   316,   316,   319,   327,   328,   336,   344,   358,   367,
     376,   387,   399,   407,   414,   419,   431,   440,   449,   460,
     470,   478,   492,   500,   509,   518,   529,   539,   540,   551,
     552,   557,   568,   581,   590,   599,   604,   609,   619,   624,
     629,   638,   647,   656,   665,   674,   683,   692,   703,   714,
     723,   733,   741,   757,   771,   785,   805,   819,   824,   833,
     843,   855,   856,   863,   879,   889,   901,   902,   909,   922,
     940,   962,   969,   970,   977,   984,   991,   993,   994,  1000,
    1007,  1019,  1031,  1043,  1057,  1065,  1076,  1081,  1086,  1092,
    1099,  1105,  1112,  1119,  1129,  1135,  1142,  1148,  1156,  1161,
    1168,  1178,  1184,  1196,  1197,  1203,  1215,  1225,  1235,  1244,
    1255,  1267,  1279,  1310,  1325,  1325,  1325,  1327,  1340,  1351,
    1362,  1371,  1384,  1395,  1406,  1417,  1425,  1429,  1433,  1438,
    1450,  1460,  1470,  1480,  1490,  1502,  1512,  1522,  1532,  1541,
    1553,  1563,  1573,  1583,  1592,  1592,  1592,  1592,  1593,  1593,
    1593,  1593,  1594,  1594,  1594,  1594,  1596,  1596,  1597,  1597,
    1598,  1598,  1601,  1608,  1616,  1624,  1632,  1640,  1652,  1655,
    1664,  1673,  1683,  1693,  1703,  1708,  1717,  1729,  1741,  1753,
    1763,  1774,  1774,  1776,  1776,  1777,  1777,  1778,  1778,  1778,
    1779,  1779,  1780,  1780,  1781,  1781,  1783,  1784,  1785,  1786,
    1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,  1795,  1802,
    1812,  1812,  1812,  1812,  1814,  1819
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
  "braced_arclist", "arclist", "mscenclosed", "msckey", "complete_arc",
  "arc_with_parallel", "arc", "optlist", "opt", "entitylist", "entity",
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
     295,   296,   297,   298,   299,   300
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    49,    49,
      49,    49,    50,    50,    50,    50,    51,    51,    51,    51,
      52,    52,    53,    53,    53,    53,    53,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    56,    56,    57,    57,    57,    57,    58,    58,    59,
      59,    60,    60,    61,    62,    62,    63,    63,    64,    65,
      65,    66,    67,    67,    68,    68,    68,    69,    69,    69,
      70,    70,    70,    70,    71,    71,    72,    72,    72,    72,
      72,    72,    72,    72,    73,    73,    73,    73,    74,    74,
      75,    75,    75,    76,    76,    77,    77,    77,    77,    78,
      78,    78,    78,    78,    79,    79,    79,    80,    80,    80,
      80,    80,    80,    80,    80,    81,    81,    81,    81,    82,
      82,    82,    82,    82,    83,    83,    83,    83,    83,    84,
      84,    84,    84,    84,    85,    85,    85,    85,    86,    86,
      86,    86,    87,    87,    87,    87,    88,    88,    89,    89,
      90,    90,    91,    91,    92,    92,    92,    92,    92,    93,
      93,    93,    93,    93,    94,    94,    95,    95,    95,    95,
      95,    96,    96,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      99,    99,    99,    99,   100,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     3,     3,     2,
       4,     3,     1,     2,     1,     2,     4,     3,     5,     4,
       1,     3,     1,     2,     2,     3,     2,     1,     2,     1,
       2,     2,     3,     2,     3,     1,     1,     1,     1,     1,
       2,     2,     2,     1,     2,     1,     2,     2,     3,     1,
       2,     1,     3,     3,     3,     3,     3,     1,     3,     2,
       1,     1,     3,     2,     1,     3,     1,     3,     3,     5,
       6,     1,     1,     3,     2,     2,     1,     1,     3,     2,
       3,     3,     3,     3,     1,     2,     1,     1,     2,     3,
       3,     2,     4,     3,     1,     2,     2,     3,     1,     2,
       2,     2,     3,     1,     2,     3,     2,     2,     1,     2,
       3,     3,     3,     4,     1,     1,     1,     4,     3,     3,
       2,     4,     3,     3,     2,     1,     1,     1,     1,     3,
       2,     2,     3,     2,     3,     2,     2,     3,     2,     3,
       2,     2,     3,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     3,     1,     2,
       3,     4,     3,     3,     1,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   182,   181,    22,   214,     0,    20,   163,   162,
     144,   148,   152,   145,   149,   153,   146,   150,   154,   147,
     151,   155,   125,   108,    43,    45,    49,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,    84,     0,    14,
       0,    12,     0,    27,    37,    51,    36,    57,    39,    38,
      86,   103,    87,    94,    29,   126,   127,   128,     0,     0,
       0,   164,    35,   168,    60,     0,    23,     0,   169,   183,
     213,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   192,   193,   187,   184,   185,
     186,   188,   189,   194,   191,   190,   195,     0,   174,   210,
     211,   212,   180,     0,   106,    44,    46,    50,     0,    40,
      66,    41,    61,     0,    64,     0,    42,    31,     0,   100,
      98,     0,    47,     0,    28,   114,     0,    33,   115,     0,
     116,     0,     1,     5,     0,    15,    13,     0,    26,    24,
       0,     0,    85,    91,    88,     0,     0,   104,   101,    96,
      95,    30,   157,   156,   133,   159,   158,   138,   161,   160,
     143,   130,   135,   140,   165,   166,     0,   107,   131,   136,
     141,    59,   215,     0,     9,   172,   173,     0,   170,   179,
      56,     0,     0,     0,     0,    63,    71,     0,    32,    99,
      48,     0,     0,   120,     0,    34,   124,     0,     0,     0,
       7,    17,     0,    25,    52,     0,    58,    60,    90,    89,
      93,   102,    97,   132,   137,   142,   167,    54,    53,    55,
     105,   129,   134,   139,    11,     8,   171,   175,   177,   178,
     176,    68,    67,    62,    65,     0,     0,     0,     0,    72,
       0,    77,     0,    56,   109,   118,   122,   119,     0,   123,
       0,    19,    16,    92,    10,     0,    74,    75,     0,    79,
       0,     0,   110,   111,   112,   117,   121,    18,    83,     0,
      69,    73,    78,    81,    80,    82,   113,    70
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,   111,   112,   113,   109,   110,   116,
     187,   238,   239,   240,   241,    48,    49,    50,   119,    51,
      52,    53,   193,   126,   127,    54,    55,    56,    57,    58,
      59,    60,   154,   157,   160,    61,    62,    63,    97,    98,
      99,   100,   101,   114,    65,   174
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -156
static const yytype_int16 yypact[] =
{
     248,     5,  -156,  -156,  -156,  -156,   337,    17,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,   100,    53,    53,    53,    79,   685,    91,
     806,    43,   685,   728,   772,    96,  -156,  -156,   200,  -156,
     101,  -156,    44,  -156,   110,  -156,   113,  -156,   114,   177,
    -156,    64,  -156,   331,    72,   133,    93,   818,   100,   100,
     100,    53,  -156,   119,   832,   382,  -156,   117,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,    42,  -156,  -156,
    -156,  -156,   126,   685,  -156,  -156,  -156,  -156,   131,   136,
    -156,   137,  -156,    13,  -156,   140,  -156,    72,   846,  -156,
      72,   122,    53,   145,  -156,  -156,    25,    72,  -156,    25,
    -156,   792,  -156,     5,   293,  -156,  -156,   426,   138,  -156,
      58,   100,  -156,  -156,   331,   114,    43,  -156,  -156,  -156,
     114,  -156,  -156,  -156,   100,  -156,  -156,   100,  -156,  -156,
     100,  -156,  -156,  -156,  -156,    53,   556,   100,   100,   100,
     100,  -156,  -156,   470,  -156,  -156,   144,   685,  -156,   599,
     152,   685,    79,   685,   685,  -156,  -156,   770,  -156,  -156,
    -156,   685,   100,  -156,   120,  -156,  -156,   120,    25,    25,
    -156,  -156,   514,  -156,  -156,   157,  -156,    53,  -156,   114,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,    45,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,   168,    79,   685,   169,  -156,
      63,  -156,   175,  -156,    30,  -156,  -156,  -156,   120,  -156,
     120,    78,  -156,  -156,  -156,   685,   136,   137,   112,  -156,
      77,   642,   100,  -156,  -156,  -156,  -156,  -156,  -156,   171,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -156,  -156,    50,   -43,   -53,   -31,  -156,   -25,  -156,   156,
    -156,    51,  -156,    54,   -41,    14,  -156,   -30,    23,  -156,
    -156,  -156,   -51,  -156,   -52,  -156,  -156,  -156,   -35,  -156,
    -156,   -14,  -120,    83,  -156,   188,  -156,  -156,  -156,   -23,
     -12,    -9,  -156,  -156,  -156,   182,   -34,   -22,  -156,    69,
       0,  -156,  -156,    -5,  -156,  -155
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -77
static const yytype_int16 yytable[] =
{
      64,   102,   105,   106,   107,   142,   143,   135,   147,   196,
     149,   128,   173,   136,    66,   145,   148,   120,   225,   150,
     151,   184,   129,   104,   103,   130,     6,   122,     2,     3,
     118,   121,   153,    64,   131,   144,   262,   120,    64,   164,
     263,   168,   171,   176,   156,   138,     2,     3,   159,   121,
     177,   121,   169,   139,    66,   170,   178,   172,   161,   162,
     163,     2,     3,   264,   259,    64,     6,     2,     3,   254,
     192,   260,   -76,   123,   245,     5,    23,   246,   247,   249,
       2,     3,   108,   188,   202,     6,   189,    66,     8,     9,
     267,   185,   235,   195,   115,   168,   132,    23,   180,   155,
     190,   208,   210,     2,     3,   146,   169,   212,   128,   170,
     209,   211,   137,   269,    11,     2,     3,    14,   140,   199,
      17,   141,   130,    20,   270,     5,   194,   235,   265,   197,
     266,   175,   120,   179,    64,     8,     9,    64,   181,   152,
     205,   207,   135,   216,   182,   183,   121,   203,   136,   236,
     237,   186,   191,    10,   213,   167,    13,   214,   226,    16,
     215,   219,    19,   -21,   166,   192,   253,   220,   221,   222,
     223,   135,   102,    64,   230,   255,   231,   136,   258,   234,
       2,     3,   261,   277,   200,   171,   243,   242,     5,   124,
       6,   204,   244,     8,     9,   206,   257,   233,   248,   250,
      -4,   133,    64,     2,     3,   232,   256,   271,   272,     4,
      23,     5,   134,     6,   198,     7,     8,     9,   117,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,   165,   227,     0,    -2,     1,
     268,     2,     3,     0,     0,     0,   275,     4,   242,     5,
     242,     6,   276,     7,     8,     9,     0,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    -6,     1,     0,     2,     3,     0,     0,
       0,     0,     4,     0,     5,     0,     6,     0,     7,     8,
       9,     0,     0,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    67,     0,
       2,     3,     5,     0,     6,     0,     0,     8,     9,     0,
       0,    68,    69,     0,     0,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,     1,     0,     2,     3,     0,     0,     0,
       0,     4,     0,     5,   172,     6,     0,     7,     8,     9,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,     1,     0,     2,
       3,     0,     0,     0,     0,     4,     0,     5,   201,     6,
       0,     7,     8,     9,     0,     0,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,   224,     0,     2,     3,     0,     0,     0,     0,     4,
       0,     5,   172,     6,     0,     7,     8,     9,     0,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,   251,     0,     2,     3,     0,
       0,     0,     0,     4,     0,     5,   252,     6,     0,     7,
       8,     9,     0,     0,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,     2,
       3,   217,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,    70,   218,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,     2,     3,   228,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,    70,   229,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,     2,     3,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
      70,   274,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,     2,     3,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,     2,     3,     0,     0,     0,     0,     0,     0,     5,
       0,     6,     0,   123,     8,     9,     0,     0,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,     0,    34,     2,     3,     2,     3,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     0,   125,     0,   236,   237,     2,
       3,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   158,   125,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   166,
      12,     0,     0,    15,     0,     6,    18,     0,     0,    21,
       0,     0,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     0,   167,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     6,    24,    25,    26,    48,    49,    38,    51,   129,
      53,    34,    65,    38,     9,    49,    51,    31,   173,    53,
      54,     8,    34,    23,     7,    34,    13,    32,     3,     4,
      30,    31,    55,    33,    34,    49,     6,    51,    38,    61,
      10,    64,    64,     1,    56,     1,     3,     4,    57,    49,
       8,    51,    64,     9,     9,    64,    14,    12,    58,    59,
      60,     3,     4,    33,     1,    65,    13,     3,     4,   224,
      45,     8,     9,    15,   194,    11,    33,   197,   198,   199,
       3,     4,     3,   117,   137,    13,   120,     9,    16,    17,
      12,   113,    15,   127,     3,   118,     0,    33,   103,     6,
     122,   144,   145,     3,     4,    41,   118,   150,   131,   118,
     144,   146,    11,     1,    21,     3,     4,    24,     8,   131,
      27,     8,   131,    30,    12,    11,   126,    15,   248,   129,
     250,    14,   146,     7,   134,    16,    17,   137,     7,     6,
     140,   141,   173,   165,     8,     8,   146,     9,   173,    37,
      38,    11,     7,    20,   154,    33,    23,   157,    14,    26,
     160,   166,    29,    11,     7,    45,   209,   167,   168,   169,
     170,   202,   177,   173,   179,     7,   181,   202,     9,   184,
       3,     4,     7,    12,   134,   207,   191,   187,    11,    33,
      13,   140,   192,    16,    17,   141,   237,   183,   198,   199,
       0,     1,   202,     3,     4,   182,   236,   258,   260,     9,
      33,    11,    12,    13,   131,    15,    16,    17,    30,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    63,   177,    -1,     0,     1,
     255,     3,     4,    -1,    -1,    -1,   261,     9,   258,    11,
     260,    13,   262,    15,    16,    17,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,     0,     1,    -1,     3,     4,    -1,    -1,
      -1,    -1,     9,    -1,    11,    -1,    13,    -1,    15,    16,
      17,    -1,    -1,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,     1,    -1,
       3,     4,    11,    -1,    13,    -1,    -1,    16,    17,    -1,
      -1,    14,    15,    -1,    -1,    18,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     1,    -1,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    17,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,     1,    -1,     3,
       4,    -1,    -1,    -1,    -1,     9,    -1,    11,    12,    13,
      -1,    15,    16,    17,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,     1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    11,    12,    13,    -1,    15,    16,    17,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,     1,    -1,     3,     4,    -1,
      -1,    -1,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    17,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    15,    -1,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,     3,     4,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,     3,     4,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      15,    -1,    -1,    18,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     3,     4,    -1,    -1,    -1,    -1,    -1,    -1,    11,
      -1,    13,    -1,    15,    16,    17,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    -1,    44,     3,     4,     3,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    -1,    37,    38,     3,
       4,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     6,    33,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     7,
      22,    -1,    -1,    25,    -1,    13,    28,    -1,    -1,    31,
      -1,    -1,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     9,    11,    13,    15,    16,    17,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    71,    72,
      73,    75,    76,    77,    81,    82,    83,    84,    85,    86,
      87,    91,    92,    93,    96,   100,     9,     1,    14,    15,
      18,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    94,    95,    96,
      97,    98,    99,     7,    96,    93,    93,    93,     3,    63,
      64,    60,    61,    62,    99,     3,    65,    81,    96,    74,
      77,    96,    99,    15,    55,    33,    79,    80,    85,    86,
      87,    96,     0,     1,    12,    51,    53,    11,     1,     9,
       8,     8,    49,    49,    77,    92,    41,    49,    74,    49,
      92,    92,     6,    85,    88,     6,    86,    89,     6,    87,
      90,    96,    96,    96,    93,    91,     7,    33,    85,    86,
      87,    93,    12,    50,   101,    14,     1,     8,    14,     7,
      99,     7,     8,     8,     8,    93,    11,    66,    92,    92,
      93,     7,    45,    78,    96,    92,    78,    96,    79,    86,
      48,    12,    50,     9,    57,    96,    59,    96,    49,    92,
      49,    74,    49,    96,    96,    96,    93,     5,    19,    99,
      96,    96,    96,    96,     1,   101,    14,    95,     5,    19,
      99,    99,    64,    61,    99,    15,    37,    38,    67,    68,
      69,    70,    96,    99,    96,    78,    78,    78,    96,    78,
      96,     1,    12,    49,   101,     7,    63,    60,     9,     1,
       8,     7,     6,    10,    33,    78,    78,    12,    99,     1,
      12,    68,    70,     5,    19,    99,    96,    12
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
#line 307 "language.yy"
	{free((yyvaluep->str));};
#line 1800 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */
#line 307 "language.yy"
	{free((yyvaluep->str));};
#line 1805 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */
#line 311 "language.yy"
	{free((yyvaluep->str));};
#line 1810 "language.cc"
	break;
      case 15: /* "TOK_MSC" */
#line 308 "language.yy"
	{free((yyvaluep->str));};
#line 1815 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */
#line 307 "language.yy"
	{free((yyvaluep->str));};
#line 1820 "language.cc"
	break;
      case 17: /* "TOK_COLON_QUOTED_STRING" */
#line 307 "language.yy"
	{free((yyvaluep->str));};
#line 1825 "language.cc"
	break;
      case 18: /* "TOK_STYLE_NAME" */
#line 307 "language.yy"
	{free((yyvaluep->str));};
#line 1830 "language.cc"
	break;
      case 19: /* "TOK_BOOLEAN" */
#line 311 "language.yy"
	{free((yyvaluep->str));};
#line 1835 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_HEADING" */
#line 310 "language.yy"
	{free((yyvaluep->str));};
#line 1840 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NUDGE" */
#line 310 "language.yy"
	{free((yyvaluep->str));};
#line 1845 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_NEWPAGE" */
#line 310 "language.yy"
	{free((yyvaluep->str));};
#line 1850 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFCOLOR" */
#line 309 "language.yy"
	{free((yyvaluep->str));};
#line 1855 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFSTYLE" */
#line 309 "language.yy"
	{free((yyvaluep->str));};
#line 1860 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_DEFDESIGN" */
#line 309 "language.yy"
	{free((yyvaluep->str));};
#line 1865 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_BIG" */
#line 308 "language.yy"
	{free((yyvaluep->str));};
#line 1870 "language.cc"
	break;
      case 41: /* "TOK_COMMAND_PIPE" */
#line 308 "language.yy"
	{free((yyvaluep->str));};
#line 1875 "language.cc"
	break;
      case 42: /* "TOK_COMMAND_MARK" */
#line 311 "language.yy"
	{free((yyvaluep->str));};
#line 1880 "language.cc"
	break;
      case 43: /* "TOK_COMMAND_PARALLEL" */
#line 308 "language.yy"
	{free((yyvaluep->str));};
#line 1885 "language.cc"
	break;
      case 48: /* "top_level_arclist" */
#line 303 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1890 "language.cc"
	break;
      case 49: /* "braced_arclist" */
#line 303 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1895 "language.cc"
	break;
      case 50: /* "arclist" */
#line 303 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1900 "language.cc"
	break;
      case 51: /* "mscenclosed" */
#line 303 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1905 "language.cc"
	break;
      case 53: /* "complete_arc" */
#line 299 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1910 "language.cc"
	break;
      case 54: /* "arc_with_parallel" */
#line 299 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1915 "language.cc"
	break;
      case 55: /* "arc" */
#line 299 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1920 "language.cc"
	break;
      case 56: /* "optlist" */
#line 303 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1925 "language.cc"
	break;
      case 57: /* "opt" */
#line 299 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1930 "language.cc"
	break;
      case 58: /* "entitylist" */
#line 304 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};
#line 1935 "language.cc"
	break;
      case 59: /* "entity" */
#line 304 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};
#line 1940 "language.cc"
	break;
      case 62: /* "tok_stringlist" */
#line 305 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};
#line 1945 "language.cc"
	break;
      case 71: /* "parallel" */
#line 302 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};
#line 1950 "language.cc"
	break;
      case 72: /* "emphasis_list" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1955 "language.cc"
	break;
      case 73: /* "first_emphasis" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1960 "language.cc"
	break;
      case 74: /* "pipe_def" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1965 "language.cc"
	break;
      case 75: /* "pipe_def_list" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1970 "language.cc"
	break;
      case 76: /* "pipe_emphasis" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1975 "language.cc"
	break;
      case 77: /* "emphrel" */
#line 301 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1980 "language.cc"
	break;
      case 78: /* "vertxpos" */
#line 298 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};
#line 1985 "language.cc"
	break;
      case 80: /* "vertrel" */
#line 297 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1990 "language.cc"
	break;
      case 81: /* "arcrel" */
#line 299 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1995 "language.cc"
	break;
      case 82: /* "arcrel_to" */
#line 300 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 2000 "language.cc"
	break;
      case 83: /* "arcrel_from" */
#line 300 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 2005 "language.cc"
	break;
      case 84: /* "arcrel_bidir" */
#line 300 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 2010 "language.cc"
	break;
      case 92: /* "full_arcattrlist_with_label" */
#line 305 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 2015 "language.cc"
	break;
      case 93: /* "full_arcattrlist" */
#line 305 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 2020 "language.cc"
	break;
      case 94: /* "arcattrlist" */
#line 305 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 2025 "language.cc"
	break;
      case 95: /* "arcattr" */
#line 305 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};
#line 2030 "language.cc"
	break;
      case 96: /* "entity_string" */
#line 306 "language.yy"
	{free((yyvaluep->str));};
#line 2035 "language.cc"
	break;
      case 97: /* "reserved_word_string" */
#line 306 "language.yy"
	{free((yyvaluep->str));};
#line 2040 "language.cc"
	break;
      case 98: /* "symbol_string" */
#line 306 "language.yy"
	{free((yyvaluep->str));};
#line 2045 "language.cc"
	break;
      case 99: /* "string" */
#line 306 "language.yy"
	{free((yyvaluep->str));};
#line 2050 "language.cc"
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
#line 316 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:
#line 320 "language.yy"
    {
    if (C_S_H) {
    } else {
        msc.AddArcs((yyvsp[(1) - (1)].arclist));
    }
}
    break;

  case 5:
#line 329 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    }
}
    break;

  case 6:
#line 337 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
    }
}
    break;

  case 7:
#line 345 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_ERROR);
    } else {
        //Merge $3 into $1
        ((yyvsp[(1) - (3)].arclist))->splice(((yyvsp[(1) - (3)].arclist))->end(), *((yyvsp[(3) - (3)].arclist)));
        delete ((yyvsp[(3) - (3)].arclist));
        (yyval.arclist) = (yyvsp[(1) - (3)].arclist);
        msc.Error.Error(YYMSC_GETPOS((yylsp[(3) - (3)])).start, "Unexpected '}'.");
    }
}
    break;

  case 8:
#line 359 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(2) - (3)].arclist);
    }
}
    break;

  case 9:
#line 368 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_BRACE);
        ADDCSH((yylsp[(2) - (2)]), COLOR_BRACE);
    } else {
         (yyval.arclist) = new ArcList;
    }
}
    break;

  case 10:
#line 377 "language.yy"
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

  case 11:
#line 388 "language.yy"
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

  case 12:
#line 400 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
}
    break;

  case 13:
#line 408 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
}
    break;

  case 14:
#line 415 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
}
    break;

  case 15:
#line 420 "language.yy"
    {
    if (C_S_H) break;
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 16:
#line 432 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (4)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
    }
}
    break;

  case 17:
#line 441 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_BRACE);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACE);
    } else {
        (yyval.arclist) = new ArcList;
    }
}
    break;

  case 18:
#line 450 "language.yy"
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

  case 19:
#line 461 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (4)]), COLOR_BRACE);
        ADDCSH((yylsp[(4) - (4)]), COLOR_ERROR);
    } else {
        (yyval.arclist) = (yyvsp[(3) - (4)].arclist);
    }
}
    break;

  case 20:
#line 471 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 21:
#line 479 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_DESIGNNAME);
    } else {
        msc.AddDesignAttribute(Attribute("msc", (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])),
                                                    YYMSC_GETPOS((yylsp[(3) - (3)]))));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 22:
#line 493 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=NULL;
    }
}
    break;

  case 23:
#line 501 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_ERROR);
        ADDCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=NULL;
    }
}
    break;

  case 24:
#line 510 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    } else {
        if ((yyvsp[(1) - (2)].arcbase)) SETLINEEND((yyvsp[(1) - (2)].arcbase), (yyloc), (yyloc));
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 25:
#line 519 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        ADDCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
    } else {
        if ((yyvsp[(1) - (3)].arcbase)) SETLINEEND((yyvsp[(1) - (3)].arcbase), (yyloc), (yyloc));
        (yyval.arcbase)=(yyvsp[(1) - (3)].arcbase);
    }
    yyerrok;
}
    break;

  case 26:
#line 530 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        if ((yyvsp[(1) - (2)].arcbase)) SETLINEEND((yyvsp[(1) - (2)].arcbase), (yyloc), (yyloc));
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 28:
#line 541 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        ((yyvsp[(2) - (2)].arcbase))->SetParallel();
        (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 30:
#line 553 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 31:
#line 558 "language.yy"
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

  case 32:
#line 569 "language.yy"
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

  case 33:
#line 582 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (yyvsp[(2) - (2)].arcbase);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 34:
#line 591 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = ((yyvsp[(2) - (3)].arcbase))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 35:
#line 600 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
}
    break;

  case 36:
#line 605 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = new CommandEntity((yyvsp[(1) - (1)].entitylist), &msc);
}
    break;

  case 37:
#line 610 "language.yy"
    {
    if (C_S_H) break;
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ((yyvsp[(1) - (1)].arclist)) {
        (yyval.arcbase) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    } else
        (yyval.arcbase) = NULL;
}
    break;

  case 38:
#line 620 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
}
    break;

  case 39:
#line 625 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
}
    break;

  case 40:
#line 630 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 41:
#line 639 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 42:
#line 648 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 43:
#line 657 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new CommandEntity(NULL, &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 44:
#line 666 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new CommandEntity(NULL, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 45:
#line 675 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new ArcDivider(MSC_COMMAND_NUDGE, &msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 46:
#line 684 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 47:
#line 693 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(2) - (2)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new CommandMark((yyvsp[(2) - (2)].str), YYMSC_GETPOS((yyloc)), &msc);
    }
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 48:
#line 704 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 49:
#line 715 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = new CommandNewpage(&msc);
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 50:
#line 724 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcbase) = (new CommandNewpage(&msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 51:
#line 734 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
}
    break;

  case 52:
#line 742 "language.yy"
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

  case 53:
#line 758 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
        msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), string_to_bool((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yylsp[(1) - (3)])),
                                                           YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
        (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 54:
#line 772 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ATTRNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str), COLOR_OPTIONNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_EQUAL);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ATTRVALUE);
    } else {
       msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), atof((yyvsp[(3) - (3)].str)), YYMSC_GETPOS((yylsp[(1) - (3)])),
                                                YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(3) - (3)].str)));
       (yyval.arcbase) = NULL;
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 55:
#line 786 "language.yy"
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
            (yyval.arcbase) = new CommandNewBackground(&msc, fill);
        } else {
            msc.AddAttribute(Attribute((yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yylsp[(3) - (3)]))));
            (yyval.arcbase) = NULL;
        }
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 56:
#line 806 "language.yy"
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

  case 57:
#line 820 "language.yy"
    {
    if (C_S_H) break;
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
}
    break;

  case 58:
#line 825 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
}
    break;

  case 59:
#line 834 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    } else {
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (2)].str), &msc))->AddAttributeList((yyvsp[(2) - (2)].attriblist), &msc);
        SETLINEEND((yyval.entity), (yyloc), (yyloc));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 60:
#line 844 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    } else {
        (yyval.entity) = (new EntityDef((yyvsp[(1) - (1)].str), &msc))->AddAttributeList(NULL, &msc);
        SETLINEEND((yyval.entity), (yyloc), (yyloc));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 62:
#line 857 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 63:
#line 864 "language.yy"
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

  case 64:
#line 880 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    } else {
        (yyval.stringlist) = new std::list<string>;
        ((yyval.stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(1) - (1)].str), &(yylsp[(1) - (1)]), msc));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 65:
#line 890 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        ADDCSH((yylsp[(3) - (3)]), COLOR_STYLENAME);
    } else {
       ((yyvsp[(1) - (3)].stringlist))->push_back(ConvertEmphasisToBox((yyvsp[(3) - (3)].str), &(yylsp[(3) - (3)]), msc));
       (yyval.stringlist) = (yyvsp[(1) - (3)].stringlist);
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 67:
#line 903 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 68:
#line 910 "language.yy"
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

  case 69:
#line 923 "language.yy"
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

  case 70:
#line 941 "language.yy"
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

  case 71:
#line 963 "language.yy"
    {
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
}
    break;

  case 73:
#line 971 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
    }
}
    break;

  case 74:
#line 978 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 75:
#line 985 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 78:
#line 995 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 79:
#line 1001 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    }
}
    break;

  case 80:
#line 1008 "language.yy"
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

  case 81:
#line 1020 "language.yy"
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

  case 82:
#line 1032 "language.yy"
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

  case 83:
#line 1044 "language.yy"
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

  case 84:
#line 1058 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
}
    break;

  case 85:
#line 1066 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(2) - (2)].arclist)==NULL)
        (yyval.arcparallel) = (yyvsp[(1) - (2)].arcparallel);
    else if ((yyvsp[(1) - (2)].arcparallel))
        (yyval.arcparallel) = ((yyvsp[(1) - (2)].arcparallel))->AddArcList((yyvsp[(2) - (2)].arclist));
    else
        (yyval.arcparallel) = (new ArcParallel(&msc))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 86:
#line 1077 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 87:
#line 1082 "language.yy"
    {
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 88:
#line 1087 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (2)].arcemph), (yylsp[(2) - (2)]), (yylsp[(2) - (2)]));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
}
    break;

  case 89:
#line 1093 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    SETLINEEND((yyvsp[(2) - (3)].arcemph), (yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
}
    break;

  case 90:
#line 1100 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (3)].arcemph), (yylsp[(2) - (3)]), (yylsp[(2) - (3)]));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
}
    break;

  case 91:
#line 1106 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
}
    break;

  case 92:
#line 1113 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (4)].arcemph), (yylsp[(2) - (4)]), (yylsp[(3) - (4)]));
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
}
    break;

  case 93:
#line 1120 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), &msc);
    SETLINEEND(temp, (yylsp[(2) - (3)]), (yylsp[(2) - (3)]));
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
}
    break;

  case 94:
#line 1130 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (1)].arcemph), (yyloc), (yyloc));
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 95:
#line 1136 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    SETLINEEND((yyvsp[(1) - (2)].arcemph), (yyloc), (yyloc));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 96:
#line 1143 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (2)].arcemph), (yylsp[(1) - (2)]), (yylsp[(1) - (2)]));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 97:
#line 1149 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(1) - (3)].arcemph), (yylsp[(1) - (3)]), (yylsp[(2) - (3)]));
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 98:
#line 1157 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
}
    break;

  case 99:
#line 1162 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 100:
#line 1169 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        SETLINEEND((yyvsp[(2) - (2)].arcemph), (yyloc), (yyloc));
        (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 101:
#line 1179 "language.yy"
    {
    if (C_S_H) break;
    SETLINEEND((yyvsp[(2) - (2)].arcemph), (yylsp[(2) - (2)]), (yylsp[(2) - (2)]));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow((yyvsp[(2) - (2)].arcemph));
}
    break;

  case 102:
#line 1185 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_KEYWORD);
    } else {
        SETLINEEND((yyvsp[(3) - (3)].arcemph), (yylsp[(2) - (3)]), (yylsp[(3) - (3)]));
        (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(3) - (3)].arcemph));
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 104:
#line 1198 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 105:
#line 1204 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), &msc);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 106:
#line 1216 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (2)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), &msc);
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 107:
#line 1226 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), &msc);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 108:
#line 1236 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), &msc);
    }
}
    break;

  case 109:
#line 1245 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])));
    }
    free((yyvsp[(1) - (2)].str));
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 110:
#line 1256 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_SIDE);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 111:
#line 1268 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_SIDE);
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 112:
#line 1280 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
        ADDCSH((yylsp[(3) - (3)]), COLOR_SYMBOL);
    } else {
        switch ((yyvsp[(3) - (3)].arctype)) {
        case MSC_EMPH_SOLID:
            (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), VertXPos::POS_LEFT_BY);
            break;
        case MSC_EMPH_DASHED:
            (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), VertXPos::POS_RIGHT_BY);
            break;
        case MSC_EMPH_DOTTED:
            msc.Error.Error(YYMSC_GETPOS((yylsp[(3) - (3)])).start,
                            "unexpected '..', expected '-', '--', '+' or '++'."
                            " Ignoring vertical."); break;
            (yyval.vertxpos) = NULL;
            break;
        case MSC_EMPH_DOUBLE:
            msc.Error.Error(YYMSC_GETPOS((yylsp[(3) - (3)])).start,
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

  case 113:
#line 1311 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_KEYWORD);
        ADDCSH_ENTITYNAME((yylsp[(2) - (4)]), (yyvsp[(2) - (4)].str));
        ADDCSH((yylsp[(3) - (4)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(4) - (4)]), (yyvsp[(4) - (4)].str));
    } else {
        (yyval.vertxpos) = new VertXPos(msc, (yyvsp[(2) - (4)].str), YYMSC_GETPOS((yylsp[(2) - (4)])), (yyvsp[(4) - (4)].str), YYMSC_GETPOS((yylsp[(4) - (4)])), VertXPos::POS_CENTER);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(2) - (4)].str));
    free((yyvsp[(4) - (4)].str));
}
    break;

  case 117:
#line 1328 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 118:
#line 1341 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 119:
#line 1352 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 120:
#line 1363 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
    }
}
    break;

  case 121:
#line 1372 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (4)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(3) - (4)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 122:
#line 1385 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH((yylsp[(2) - (3)]), COLOR_MARKERNAME);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 123:
#line 1396 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_MARKERNAME);
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 124:
#line 1407 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 125:
#line 1418 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), &msc);
}
    break;

  case 126:
#line 1426 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 127:
#line 1430 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 128:
#line 1434 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 129:
#line 1439 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 130:
#line 1451 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 131:
#line 1461 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 132:
#line 1471 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 133:
#line 1481 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 134:
#line 1491 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 135:
#line 1503 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 136:
#line 1513 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 137:
#line 1523 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), false, YYMSC_GETPOS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 138:
#line 1533 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), false, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 139:
#line 1542 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), YYMSC_GETPOS((yylsp[(1) - (3)])), (yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])));
    }
    free((yyvsp[(1) - (3)].str));
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 140:
#line 1554 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(1) - (2)].arctype), LSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(1) - (2)])), (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 141:
#line 1564 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcarrow) = msc.CreateArcArrow((yyvsp[(2) - (2)].arctype), (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), RSIDE_ENT_STR, YYMSC_GETPOS((yylsp[(2) - (2)])));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 142:
#line 1574 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (3)]), (yyvsp[(3) - (3)].str));
    } else {
        (yyval.arcarrow) = ((yyvsp[(1) - (3)].arcarrow))->AddSegment((yyvsp[(3) - (3)].str), YYMSC_GETPOS((yylsp[(3) - (3)])), true, YYMSC_GETPOS2((yylsp[(2) - (3)]), (yylsp[(3) - (3)])));
    }
    free((yyvsp[(3) - (3)].str));
}
    break;

  case 143:
#line 1584 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 162:
#line 1602 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), false);
    };
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 163:
#line 1609 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str), true);
    };
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 164:
#line 1617 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)).IncStartCol()));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 165:
#line 1625 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yylsp[(1) - (2)])).IncStartCol()));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 166:
#line 1633 "language.yy"
    {
    if (C_S_H) {
    } else {
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yylsp[(2) - (2)])).IncStartCol()));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 167:
#line 1641 "language.yy"
    {
    if (C_S_H) {
    } else {
        ((yyvsp[(1) - (3)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (3)].str), YYMSC_GETPOS((yylsp[(2) - (3)])), YYMSC_GETPOS((yylsp[(2) - (3)])).IncStartCol()));
        //Merge $3 at the end of $1
        ((yyvsp[(1) - (3)].attriblist))->splice(((yyvsp[(1) - (3)].attriblist))->end(), *((yyvsp[(3) - (3)].attriblist)));
        delete ((yyvsp[(3) - (3)].attriblist)); //empty list now
        (yyval.attriblist) = (yyvsp[(1) - (3)].attriblist);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 169:
#line 1656 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        ADDCSH((yylsp[(2) - (2)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 170:
#line 1665 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 171:
#line 1674 "language.yy"
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

  case 172:
#line 1684 "language.yy"
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

  case 173:
#line 1694 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ERROR);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 174:
#line 1704 "language.yy"
    {
    if (C_S_H) break;
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
}
    break;

  case 175:
#line 1709 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
}
    break;

  case 176:
#line 1718 "language.yy"
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

  case 177:
#line 1730 "language.yy"
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

  case 178:
#line 1742 "language.yy"
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

  case 179:
#line 1754 "language.yy"
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

  case 180:
#line 1764 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 196:
#line 1783 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 197:
#line 1784 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 198:
#line 1785 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 199:
#line 1786 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 200:
#line 1787 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 201:
#line 1788 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 202:
#line 1789 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 203:
#line 1790 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 204:
#line 1791 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 205:
#line 1792 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 206:
#line 1793 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 207:
#line 1794 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 208:
#line 1796 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 209:
#line 1803 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 214:
#line 1815 "language.yy"
    {
    msc.PushContext();
}
    break;

  case 215:
#line 1820 "language.yy"
    {
    msc.PopContext();
}
    break;


/* Line 1267 of yacc.c.  */
#line 4296 "language.cc"
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


#line 1825 "language.yy"



/* END OF FILE */

