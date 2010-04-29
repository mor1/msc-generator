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
#define ADDCSH_COLON_STRING(A, B)           \
    do {                                    \
    CshPos colon = (A);                     \
    colon.last_pos = colon.first_pos;       \
    msc.AddCSH(colon, COLOR_COLON);         \
    (A).first_pos++;                        \
    msc.AddCSH_AttrValue(A, (B)+1, NULL);   \
    } while (0)

#ifndef HAVE_UNISTD_H
int isatty (int) {return 0;}
#endif
#else
#define YYMSC_GETPOS(A) file_line(msc.current_file, (A).first_line, (A).first_column)
#define ADDCSH(A, B)
#define ADDCSH_ATTRVALUE(A, B, C)
#define ADDCSH_ATTRNAME(A, B, C)
#define ADDCSH_ENTITYNAME(A, B)
#define ADDCSH_COLON_STRING(A, B)
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
#line 240 "language.yy"
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
#line 418 "language.cc"
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
#line 443 "language.cc"

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
#define YYFINAL  119
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   719

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  199
/* YYNRULES -- Number of states.  */
#define YYNSTATES  260

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
       0,     0,     3,     4,     6,     9,    13,    16,    21,    25,
      27,    30,    32,    35,    40,    44,    50,    55,    57,    61,
      63,    66,    69,    73,    76,    78,    81,    84,    88,    91,
      95,    97,    99,   101,   103,   105,   108,   111,   114,   116,
     119,   121,   124,   127,   131,   133,   136,   138,   142,   146,
     150,   154,   158,   160,   164,   167,   169,   171,   175,   178,
     180,   184,   186,   190,   194,   200,   207,   209,   211,   215,
     218,   221,   223,   225,   229,   232,   236,   240,   244,   248,
     250,   253,   255,   258,   261,   265,   269,   272,   277,   281,
     283,   286,   289,   293,   295,   298,   301,   305,   309,   312,
     315,   317,   320,   324,   328,   332,   337,   339,   341,   343,
     348,   352,   356,   359,   364,   368,   372,   375,   377,   379,
     381,   383,   387,   390,   393,   397,   400,   404,   407,   410,
     414,   417,   421,   424,   427,   431,   434,   436,   438,   440,
     442,   444,   446,   448,   450,   452,   454,   456,   458,   460,
     462,   464,   466,   468,   470,   472,   475,   478,   480,   483,
     487,   492,   496,   500,   502,   506,   510,   514,   518,   521,
     523,   525,   527,   529,   531,   533,   535,   537,   539,   541,
     543,   545,   547,   549,   551,   553,   555,   557,   559,   561,
     563,   565,   567,   569,   571,   573,   575,   577,   579,   581
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    -1,    47,    -1,    47,     1,    -1,    93,
      47,    94,    -1,    93,    94,    -1,    93,    47,     1,    94,
      -1,    93,    47,     1,    -1,    50,    -1,    47,    50,    -1,
      48,    -1,    47,    48,    -1,    49,    11,    47,    12,    -1,
      49,    11,    12,    -1,    49,    11,    47,     1,    12,    -1,
      49,    11,    47,     1,    -1,    15,    -1,    15,     7,    92,
      -1,     9,    -1,     1,     9,    -1,    51,     9,    -1,    51,
       1,     9,    -1,    51,     1,    -1,    75,    -1,    75,    85,
      -1,    39,    75,    -1,    39,    75,    85,    -1,    42,    74,
      -1,    42,    74,    85,    -1,    85,    -1,    54,    -1,    52,
      -1,    68,    -1,    67,    -1,    36,    59,    -1,    37,    56,
      -1,    38,    61,    -1,    33,    -1,    33,    86,    -1,    34,
      -1,    34,    86,    -1,    41,    92,    -1,    41,    92,    86,
      -1,    35,    -1,    35,    86,    -1,    53,    -1,    52,     8,
      53,    -1,    89,     7,    18,    -1,    89,     7,     5,    -1,
      89,     7,    92,    -1,    15,     7,    92,    -1,    55,    -1,
      54,     8,    55,    -1,    89,    86,    -1,    89,    -1,    57,
      -1,    56,     8,    57,    -1,    58,    86,    -1,    92,    -1,
      58,     8,    92,    -1,    60,    -1,    59,     8,    60,    -1,
       3,     7,    92,    -1,     3,    62,    63,     9,    12,    -1,
       3,    62,    63,     9,     1,    12,    -1,    11,    -1,    64,
      -1,    63,     9,    64,    -1,    36,    59,    -1,    37,    56,
      -1,    65,    -1,    66,    -1,    65,     8,    66,    -1,    65,
       1,    -1,    89,     7,    18,    -1,    89,     7,     5,    -1,
      89,     7,    92,    -1,    15,     7,    92,    -1,    46,    -1,
      67,    46,    -1,    69,    -1,    40,    70,    -1,    68,    71,
      -1,    68,    71,    85,    -1,    68,    71,    46,    -1,    68,
      46,    -1,    68,    71,    85,    46,    -1,    68,    85,    46,
      -1,    71,    -1,    71,    85,    -1,    71,    46,    -1,    71,
      85,    46,    -1,    71,    -1,    71,    85,    -1,    71,    46,
      -1,    71,    85,    46,    -1,    89,    32,    89,    -1,    32,
      89,    -1,    89,    32,    -1,    32,    -1,    43,    89,    -1,
      43,    89,     6,    -1,    43,    89,    10,    -1,    43,    89,
      32,    -1,    43,    89,     6,    89,    -1,    32,    -1,    79,
      -1,    81,    -1,    89,    73,    89,    72,    -1,    73,    89,
      72,    -1,    89,    73,    72,    -1,    73,    72,    -1,    89,
      80,    89,    72,    -1,    80,    89,    72,    -1,    89,    80,
      72,    -1,    80,    72,    -1,    31,    -1,    76,    -1,    77,
      -1,    78,    -1,    89,    79,    89,    -1,    79,    89,    -1,
      89,    79,    -1,    76,    82,    89,    -1,    76,    82,    -1,
      89,    80,    89,    -1,    80,    89,    -1,    89,    80,    -1,
      77,    83,    89,    -1,    77,    83,    -1,    89,    81,    89,
      -1,    81,    89,    -1,    89,    81,    -1,    78,    84,    89,
      -1,    78,    84,    -1,    19,    -1,    22,    -1,    25,    -1,
      28,    -1,    20,    -1,    23,    -1,    26,    -1,    29,    -1,
      21,    -1,    24,    -1,    27,    -1,    30,    -1,    79,    -1,
       6,    -1,    80,    -1,     6,    -1,    81,    -1,     6,    -1,
      16,    -1,    16,    86,    -1,    86,    16,    -1,    86,    -1,
      13,    14,    -1,    13,    87,    14,    -1,    13,    87,     1,
      14,    -1,    13,     1,    14,    -1,    13,    87,     1,    -1,
      88,    -1,    87,     8,    88,    -1,    92,     7,    92,    -1,
      92,     7,     5,    -1,    92,     7,    18,    -1,    92,     7,
      -1,    92,    -1,     4,    -1,     3,    -1,    15,    -1,    36,
      -1,    37,    -1,    38,    -1,    35,    -1,    39,    -1,    40,
      -1,    42,    -1,    19,    -1,    20,    -1,    21,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1,    31,    -1,    32,
      -1,    89,    -1,    90,    -1,    91,    -1,    17,    -1,    11,
      -1,    12,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   302,   302,   305,   312,   321,   330,   339,   350,   362,
     370,   377,   382,   394,   403,   412,   423,   433,   441,   454,
     462,   471,   479,   489,   498,   499,   504,   515,   528,   537,
     546,   551,   556,   566,   571,   576,   585,   594,   603,   612,
     621,   630,   639,   648,   657,   666,   676,   684,   700,   713,
     726,   746,   760,   765,   774,   783,   794,   795,   802,   818,
     828,   840,   841,   848,   861,   879,   901,   908,   909,   916,
     923,   930,   932,   933,   939,   946,   958,   970,   982,   996,
    1004,  1015,  1020,  1030,  1035,  1041,  1046,  1053,  1059,  1068,
    1073,  1079,  1084,  1093,  1098,  1104,  1109,  1116,  1128,  1138,
    1148,  1157,  1168,  1180,  1192,  1223,  1238,  1238,  1238,  1240,
    1253,  1264,  1275,  1284,  1297,  1308,  1319,  1330,  1338,  1342,
    1346,  1351,  1363,  1373,  1383,  1393,  1403,  1415,  1425,  1435,
    1445,  1454,  1466,  1476,  1486,  1496,  1505,  1505,  1505,  1505,
    1506,  1506,  1506,  1506,  1507,  1507,  1507,  1507,  1509,  1509,
    1510,  1510,  1511,  1511,  1515,  1524,  1533,  1542,  1545,  1554,
    1563,  1573,  1583,  1593,  1598,  1607,  1619,  1631,  1643,  1653,
    1664,  1664,  1666,  1666,  1667,  1667,  1668,  1668,  1668,  1669,
    1671,  1672,  1673,  1674,  1675,  1676,  1677,  1678,  1679,  1680,
    1681,  1682,  1683,  1690,  1700,  1700,  1700,  1700,  1702,  1707
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
  "TOK_VERTICAL", "TOK_AT", "$accept", "msc", "braced_arclist", "arclist",
  "mscenclosed", "msckey", "complete_arc", "arc", "optlist", "opt",
  "entitylist", "entity", "styledeflist", "styledef", "tok_stringlist",
  "colordeflist", "colordef", "designdef", "scope_open_empty",
  "designelementlist", "designelement", "designoptlist", "designopt",
  "parallel", "emphasis_list", "first_emphasis", "pipe_emphasis",
  "emphrel", "vertxpos", "empharcrel_straight", "vertrel", "arcrel",
  "arcrel_to", "arcrel_from", "arcrel_bidir", "relation_to",
  "relation_from", "relation_bidir", "relation_to_cont",
  "relation_from_cont", "relation_bidir_cont",
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
      47,    47,    47,    48,    48,    48,    48,    49,    49,    50,
      50,    50,    50,    50,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    52,    52,    53,    53,
      53,    53,    54,    54,    55,    55,    56,    56,    57,    58,
      58,    59,    59,    60,    61,    61,    62,    63,    63,    64,
      64,    64,    65,    65,    65,    66,    66,    66,    66,    67,
      67,    68,    68,    68,    68,    68,    68,    68,    68,    69,
      69,    69,    69,    70,    70,    70,    70,    71,    71,    71,
      71,    72,    72,    72,    72,    72,    73,    73,    73,    74,
      74,    74,    74,    74,    74,    74,    74,    75,    75,    75,
      75,    76,    76,    76,    76,    76,    77,    77,    77,    77,
      77,    78,    78,    78,    78,    78,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    85,    85,    85,    86,    86,
      86,    86,    86,    87,    87,    88,    88,    88,    88,    88,
      89,    89,    90,    90,    90,    90,    90,    90,    90,    90,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    92,    92,    92,    92,    93,    94
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
       2,     1,     2,     2,     3,     3,     2,     4,     3,     1,
       2,     2,     3,     1,     2,     2,     3,     3,     2,     2,
       1,     2,     3,     3,     3,     4,     1,     1,     1,     4,
       3,     3,     2,     4,     3,     3,     2,     1,     1,     1,
       1,     3,     2,     2,     3,     2,     3,     2,     2,     3,
       2,     3,     2,     2,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     2,     3,
       4,     3,     3,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,   171,   170,    19,   198,     0,    17,   154,   136,
     140,   144,   137,   141,   145,   138,   142,   146,   139,   143,
     147,   117,   100,    38,    40,    44,     0,     0,     0,     0,
       0,     0,     0,     0,    79,     0,    11,     0,     9,     0,
      32,    46,    31,    52,    34,    33,    81,    89,    24,   118,
     119,   120,     0,     0,     0,    30,   157,    55,     0,    20,
       0,   158,   172,   197,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   176,   173,
     174,   175,   177,   178,   179,     0,   163,   194,   195,   196,
     169,     0,   155,    98,    39,    41,    45,     0,    35,    61,
      36,    56,     0,    59,     0,    37,    26,     0,    82,    93,
       0,    42,   106,     0,    28,   107,     0,   108,     0,     1,
       4,    12,    10,     0,    23,    21,     0,     0,    80,    86,
      83,     0,    91,    90,    25,   149,   148,   125,   151,   150,
     130,   153,   152,   135,   122,   127,   132,   156,     0,    99,
     123,   128,   133,    54,   199,     0,     6,   161,   162,     0,
     159,   168,    51,     0,     0,     0,     0,    58,    66,     0,
      27,    95,    94,    43,     0,   112,     0,    29,   116,     0,
       0,     0,    14,     0,    22,     0,    47,     0,    53,    55,
      85,    84,    88,    92,   124,   129,   134,    49,    48,    50,
      97,   121,   126,   131,     8,     5,   160,   164,   166,   167,
     165,    63,    62,    57,    60,     0,     0,     0,     0,    67,
       0,    72,     0,    96,   101,   110,   114,   111,     0,   115,
       0,    16,    13,     0,    87,     7,     0,    69,    70,     0,
      74,     0,     0,   102,   103,   104,   109,   113,    15,    51,
      78,     0,    64,    68,    73,    76,    75,    77,   105,    65
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,   100,   101,   102,    98,    99,   105,   169,   218,
     219,   220,   221,    44,    45,    46,   108,    47,   175,   113,
     114,    48,    49,    50,    51,    52,    53,    54,   137,   140,
     143,    55,    56,    85,    86,    87,    88,    89,   103,    58,
     156
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -154
static const yytype_int16 yypact[] =
{
     184,     1,  -154,  -154,  -154,  -154,   456,    42,    44,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,    80,    44,    44,    44,    34,   606,    86,   650,
      10,   606,   636,    93,  -154,   246,  -154,    85,  -154,    38,
     100,  -154,   110,  -154,    90,    30,  -154,    64,    -9,   524,
     483,   115,    80,    80,    80,  -154,   103,   675,   288,  -154,
     106,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,    47,  -154,  -154,  -154,  -154,
     117,   606,  -154,  -154,  -154,  -154,  -154,   122,   114,  -154,
     133,  -154,    51,  -154,   136,  -154,    -9,   689,  -154,    64,
     112,    44,  -154,    22,    -9,  -154,    22,  -154,   208,  -154,
       1,  -154,  -154,   330,   150,  -154,    99,    80,  -154,  -154,
      64,    90,  -154,    90,  -154,  -154,  -154,    80,  -154,  -154,
      80,  -154,  -154,    80,  -154,  -154,  -154,  -154,   496,    80,
      80,    80,    80,  -154,  -154,   372,  -154,  -154,   146,   606,
    -154,   536,   151,   606,    34,   606,   606,  -154,  -154,    95,
    -154,  -154,    90,  -154,    80,  -154,   118,  -154,  -154,   118,
      22,    22,  -154,   414,  -154,   156,  -154,   164,  -154,    44,
    -154,    90,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,    61,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,  -154,  -154,  -154,   166,    34,   606,   155,  -154,
     157,  -154,   169,  -154,    50,  -154,  -154,  -154,   118,  -154,
     118,    79,  -154,   606,  -154,  -154,   606,   114,   133,    75,
    -154,   131,   576,    80,  -154,  -154,  -154,  -154,  -154,  -154,
    -154,   165,  -154,  -154,  -154,  -154,  -154,  -154,  -154,  -154
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,   -24,   -49,   -30,  -154,   -27,  -154,  -154,    52,
    -154,    59,   -38,    25,  -154,   -25,    28,  -154,  -154,  -154,
     -50,  -154,   -47,  -154,  -154,  -154,  -154,   -18,  -113,    78,
    -154,   172,  -154,  -154,  -154,   -21,   -26,   -13,  -154,  -154,
    -154,    24,    -7,  -154,    39,     0,  -154,  -154,     9,  -154,
    -153
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -72
static const yytype_int16 yytable[] =
{
      57,    92,   205,   178,     6,   121,   116,     8,   122,   155,
      59,   115,   109,     2,     3,    90,    94,    95,    96,   117,
     128,   129,    93,   132,   139,     2,     3,   130,   136,   107,
     110,   151,   118,     2,     3,    57,   150,    97,   142,   124,
     111,     5,    22,     6,   152,   110,     8,   125,   158,    91,
     153,   235,   144,   145,   146,   159,   243,     6,    57,   166,
     244,   160,    22,   225,     6,   174,   226,   227,   229,   131,
      59,   133,   134,   154,   183,     5,   251,     6,     2,     3,
       8,   151,   245,     2,     3,   171,   150,   252,    59,   104,
     215,   248,   181,   119,   152,   167,   123,   115,     2,     3,
     162,     5,     2,     3,   173,   117,   190,   192,   126,   193,
     215,   216,   217,   176,   185,   246,   179,   247,   127,   147,
     157,   141,   164,    57,   161,   121,   187,   189,   122,   163,
     170,   216,   217,   172,     2,     3,    11,   194,   177,    14,
     195,   165,    17,   196,   149,    20,   215,   168,   223,   200,
     201,   202,   203,   121,   191,    57,   122,   199,   240,   184,
     206,   174,   -18,   233,   239,   241,   -71,   234,    90,   222,
     210,   148,   211,   236,   224,   214,   242,   259,   186,   238,
     228,   230,   153,    57,    -2,     1,   188,     2,     3,   253,
     213,   237,   212,     4,   254,     5,   180,     6,   207,     7,
       8,   106,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   222,
     112,   222,   249,   258,     0,   250,    -3,   120,     0,     2,
       3,   257,     0,     0,     0,     4,     0,     5,     0,     6,
       0,     7,     8,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     1,
       0,     2,     3,     0,     0,     0,     0,     4,     0,     5,
     154,     6,     0,     7,     8,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,     1,     0,     2,     3,     0,     0,     0,     0,     4,
       0,     5,   182,     6,     0,     7,     8,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,   204,     0,     2,     3,     0,     0,     0,
       0,     4,     0,     5,   154,     6,     0,     7,     8,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,   231,     0,     2,     3,     0,
       0,     0,     0,     4,     0,     5,   232,     6,     0,     7,
       8,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    60,     0,     2,
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    62,     0,    63,     0,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,   138,
       0,    78,    79,    80,    81,    82,    83,     0,    84,     2,
       3,   197,     0,    10,     0,     0,    13,     0,     0,    16,
       0,    62,    19,    63,   198,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
     135,    78,    79,    80,    81,    82,    83,     0,    84,     2,
       3,   208,     0,     9,     0,     0,    12,     0,     0,    15,
       0,    62,    18,    63,   209,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     0,
       0,    78,    79,    80,    81,    82,    83,     0,    84,     2,
       3,   255,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    63,   256,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     2,
       3,    78,    79,    80,    81,    82,    83,     0,    84,     0,
       0,    62,     0,    63,     0,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,     2,
       3,    78,    79,    80,    81,    82,    83,     0,    84,     0,
       0,     0,     0,     2,     3,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,     0,   112,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   148,     0,     0,     0,     0,     0,     6,     0,
       0,     0,     0,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     0,   149,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20
};

static const yytype_int16 yycheck[] =
{
       0,     8,   155,   116,    13,    35,    32,    16,    35,    58,
       9,    32,    30,     3,     4,     6,    23,    24,    25,    32,
      44,    45,    22,    47,    50,     3,     4,    45,    49,    29,
      30,    57,    32,     3,     4,    35,    57,     3,    51,     1,
      31,    11,    32,    13,    57,    45,    16,     9,     1,     7,
      57,   204,    52,    53,    54,     8,     6,    13,    58,     8,
      10,    14,    32,   176,    13,    43,   179,   180,   181,    45,
       9,    47,    48,    12,   123,    11,     1,    13,     3,     4,
      16,   107,    32,     3,     4,   109,   107,    12,     9,     3,
      15,    12,   118,     0,   107,   102,    11,   118,     3,     4,
      91,    11,     3,     4,   111,   118,   130,   131,     8,   133,
      15,    36,    37,   113,    15,   228,   116,   230,     8,    16,
      14,     6,     8,   123,     7,   155,   126,   127,   155,     7,
     106,    36,    37,   109,     3,     4,    21,   137,   114,    24,
     140,     8,    27,   143,    32,    30,    15,    11,   172,   149,
     150,   151,   152,   183,   130,   155,   183,   148,     1,     9,
      14,    43,    11,     7,     9,     8,     9,   191,   159,   169,
     161,     7,   163,     7,   174,   166,     7,    12,   126,   217,
     180,   181,   189,   183,     0,     1,   127,     3,     4,   239,
     165,   216,   164,     9,   241,    11,   118,    13,   159,    15,
      16,    29,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   239,
      32,   241,   233,   243,    -1,   236,     0,     1,    -1,     3,
       4,   242,    -1,    -1,    -1,     9,    -1,    11,    -1,    13,
      -1,    15,    16,    -1,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,     1,
      -1,     3,     4,    -1,    -1,    -1,    -1,     9,    -1,    11,
      12,    13,    -1,    15,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,     1,    -1,     3,     4,    -1,    -1,    -1,    -1,     9,
      -1,    11,    12,    13,    -1,    15,    16,    -1,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     1,    -1,     3,     4,    -1,    -1,    -1,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,     1,    -1,     3,     4,    -1,
      -1,    -1,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    -1,    -1,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,     1,    -1,     3,
       4,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    15,    -1,    17,    -1,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     6,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,     3,
       4,     5,    -1,    20,    -1,    -1,    23,    -1,    -1,    26,
      -1,    15,    29,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
       6,    35,    36,    37,    38,    39,    40,    -1,    42,     3,
       4,     5,    -1,    19,    -1,    -1,    22,    -1,    -1,    25,
      -1,    15,    28,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    -1,
      -1,    35,    36,    37,    38,    39,    40,    -1,    42,     3,
       4,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    15,    -1,    17,    18,    19,    20,    21,    22,    23,
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
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     4,     9,    11,    13,    15,    16,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    67,    68,    69,    71,    75,    76,
      77,    78,    79,    80,    81,    85,    86,    89,    93,     9,
       1,    14,    15,    17,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    35,    36,
      37,    38,    39,    40,    42,    87,    88,    89,    90,    91,
      92,     7,    86,    89,    86,    86,    86,     3,    59,    60,
      56,    57,    58,    92,     3,    61,    75,    89,    70,    71,
      89,    92,    32,    73,    74,    79,    80,    81,    89,     0,
       1,    48,    50,    11,     1,     9,     8,     8,    46,    46,
      71,    85,    46,    85,    85,     6,    79,    82,     6,    80,
      83,     6,    81,    84,    89,    89,    89,    16,     7,    32,
      79,    80,    81,    86,    12,    47,    94,    14,     1,     8,
      14,     7,    92,     7,     8,     8,     8,    86,    11,    62,
      85,    46,    85,    86,    43,    72,    89,    85,    72,    89,
      73,    80,    12,    47,     9,    15,    53,    89,    55,    89,
      46,    85,    46,    46,    89,    89,    89,     5,    18,    92,
      89,    89,    89,    89,     1,    94,    14,    88,     5,    18,
      92,    92,    60,    57,    92,    15,    36,    37,    63,    64,
      65,    66,    89,    46,    89,    72,    72,    72,    89,    72,
      89,     1,    12,     7,    46,    94,     7,    59,    56,     9,
       1,     8,     7,     6,    10,    32,    72,    72,    12,    92,
      92,     1,    12,    64,    66,     5,    18,    92,    89,    12
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
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1731 "language.cc"
	break;
      case 4: /* "TOK_QSTRING" */
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1736 "language.cc"
	break;
      case 5: /* "TOK_NUMBER" */
#line 297 "language.yy"
	{free((yyvaluep->str));};
#line 1741 "language.cc"
	break;
      case 15: /* "TOK_MSC" */
#line 294 "language.yy"
	{free((yyvaluep->str));};
#line 1746 "language.cc"
	break;
      case 16: /* "TOK_COLON_STRING" */
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1751 "language.cc"
	break;
      case 17: /* "TOK_STYLE_NAME" */
#line 293 "language.yy"
	{free((yyvaluep->str));};
#line 1756 "language.cc"
	break;
      case 18: /* "TOK_BOOLEAN" */
#line 297 "language.yy"
	{free((yyvaluep->str));};
#line 1761 "language.cc"
	break;
      case 33: /* "TOK_COMMAND_HEADING" */
#line 296 "language.yy"
	{free((yyvaluep->str));};
#line 1766 "language.cc"
	break;
      case 34: /* "TOK_COMMAND_NUDGE" */
#line 296 "language.yy"
	{free((yyvaluep->str));};
#line 1771 "language.cc"
	break;
      case 35: /* "TOK_COMMAND_NEWPAGE" */
#line 296 "language.yy"
	{free((yyvaluep->str));};
#line 1776 "language.cc"
	break;
      case 36: /* "TOK_COMMAND_DEFCOLOR" */
#line 295 "language.yy"
	{free((yyvaluep->str));};
#line 1781 "language.cc"
	break;
      case 37: /* "TOK_COMMAND_DEFSTYLE" */
#line 295 "language.yy"
	{free((yyvaluep->str));};
#line 1786 "language.cc"
	break;
      case 38: /* "TOK_COMMAND_DEFDESIGN" */
#line 295 "language.yy"
	{free((yyvaluep->str));};
#line 1791 "language.cc"
	break;
      case 39: /* "TOK_COMMAND_BIG" */
#line 294 "language.yy"
	{free((yyvaluep->str));};
#line 1796 "language.cc"
	break;
      case 40: /* "TOK_COMMAND_PIPE" */
#line 294 "language.yy"
	{free((yyvaluep->str));};
#line 1801 "language.cc"
	break;
      case 46: /* "braced_arclist" */
#line 289 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1806 "language.cc"
	break;
      case 47: /* "arclist" */
#line 289 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1811 "language.cc"
	break;
      case 48: /* "mscenclosed" */
#line 289 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1816 "language.cc"
	break;
      case 50: /* "complete_arc" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1821 "language.cc"
	break;
      case 51: /* "arc" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1826 "language.cc"
	break;
      case 52: /* "optlist" */
#line 289 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arclist);};
#line 1831 "language.cc"
	break;
      case 53: /* "opt" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1836 "language.cc"
	break;
      case 54: /* "entitylist" */
#line 290 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entitylist);};
#line 1841 "language.cc"
	break;
      case 55: /* "entity" */
#line 290 "language.yy"
	{if (!C_S_H) delete (yyvaluep->entity);};
#line 1846 "language.cc"
	break;
      case 58: /* "tok_stringlist" */
#line 291 "language.yy"
	{if (!C_S_H) delete (yyvaluep->stringlist);};
#line 1851 "language.cc"
	break;
      case 67: /* "parallel" */
#line 288 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcparallel);};
#line 1856 "language.cc"
	break;
      case 68: /* "emphasis_list" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1861 "language.cc"
	break;
      case 69: /* "first_emphasis" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1866 "language.cc"
	break;
      case 70: /* "pipe_emphasis" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1871 "language.cc"
	break;
      case 71: /* "emphrel" */
#line 287 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcemph);};
#line 1876 "language.cc"
	break;
      case 72: /* "vertxpos" */
#line 284 "language.yy"
	{if (!C_S_H) delete (yyvaluep->vertxpos);};
#line 1881 "language.cc"
	break;
      case 74: /* "vertrel" */
#line 283 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1886 "language.cc"
	break;
      case 75: /* "arcrel" */
#line 285 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcbase);};
#line 1891 "language.cc"
	break;
      case 76: /* "arcrel_to" */
#line 286 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1896 "language.cc"
	break;
      case 77: /* "arcrel_from" */
#line 286 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1901 "language.cc"
	break;
      case 78: /* "arcrel_bidir" */
#line 286 "language.yy"
	{if (!C_S_H) delete (yyvaluep->arcarrow);};
#line 1906 "language.cc"
	break;
      case 85: /* "full_arcattrlist_with_label" */
#line 291 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1911 "language.cc"
	break;
      case 86: /* "full_arcattrlist" */
#line 291 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1916 "language.cc"
	break;
      case 87: /* "arcattrlist" */
#line 291 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attriblist);};
#line 1921 "language.cc"
	break;
      case 88: /* "arcattr" */
#line 291 "language.yy"
	{if (!C_S_H) delete (yyvaluep->attrib);};
#line 1926 "language.cc"
	break;
      case 89: /* "entity_string" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1931 "language.cc"
	break;
      case 90: /* "reserved_word_string" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1936 "language.cc"
	break;
      case 91: /* "symbol_string" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1941 "language.cc"
	break;
      case 92: /* "string" */
#line 292 "language.yy"
	{free((yyvaluep->str));};
#line 1946 "language.cc"
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
#line 302 "language.yy"
    {
    //no action for empty file
}
    break;

  case 3:
#line 306 "language.yy"
    {
    if (C_S_H) {
    } else {
        msc.AddArcs((yyvsp[(1) - (1)].arclist));
    }
}
    break;

  case 4:
#line 313 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        msc.AddArcs((yyvsp[(1) - (2)].arclist));
    }
}
    break;

  case 5:
#line 322 "language.yy"
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
#line 331 "language.yy"
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
#line 340 "language.yy"
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
#line 351 "language.yy"
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
#line 363 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = new ArcList;
}
    break;

  case 10:
#line 371 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(2) - (2)].arcbase))
        ((yyvsp[(1) - (2)].arclist))->Append((yyvsp[(2) - (2)].arcbase));     /* Add to existing list */
    (yyval.arclist) = ((yyvsp[(1) - (2)].arclist));
}
    break;

  case 11:
#line 378 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arclist) = (yyvsp[(1) - (1)].arclist);
}
    break;

  case 12:
#line 383 "language.yy"
    {
    if (C_S_H) break;
    //Merge $2 into $1
    ((yyvsp[(1) - (2)].arclist))->splice(((yyvsp[(1) - (2)].arclist))->end(), *((yyvsp[(2) - (2)].arclist)));
    delete ((yyvsp[(2) - (2)].arclist));
    (yyval.arclist) = (yyvsp[(1) - (2)].arclist);
}
    break;

  case 13:
#line 395 "language.yy"
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
#line 404 "language.yy"
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
#line 413 "language.yy"
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
#line 424 "language.yy"
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
#line 434 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_KEYWORD);
    } else {
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 18:
#line 442 "language.yy"
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
#line 455 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=NULL;
    }
}
    break;

  case 20:
#line 463 "language.yy"
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
#line 472 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 22:
#line 480 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_ERROR);
        ADDCSH((yylsp[(3) - (3)]), COLOR_SEMICOLON);
    } else {
        (yyval.arcbase)=(yyvsp[(1) - (3)].arcbase);
    }
    yyerrok;
}
    break;

  case 23:
#line 490 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    } else {
        (yyval.arcbase)=(yyvsp[(1) - (2)].arcbase);
    }
}
    break;

  case 25:
#line 500 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = ((yyvsp[(1) - (2)].arcbase))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
}
    break;

  case 26:
#line 505 "language.yy"
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
#line 516 "language.yy"
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
#line 529 "language.yy"
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
#line 538 "language.yy"
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
#line 547 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (new ArcDivider(MSC_ARC_VSPACE, YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(1) - (1)].attriblist));
}
    break;

  case 31:
#line 552 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = new CommandEntity((yyvsp[(1) - (1)].entitylist), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 32:
#line 557 "language.yy"
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
#line 567 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcemph); //to remove warning for downcast
}
    break;

  case 34:
#line 572 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcparallel);
}
    break;

  case 35:
#line 577 "language.yy"
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
#line 586 "language.yy"
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
#line 595 "language.yy"
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
#line 604 "language.yy"
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
#line 613 "language.yy"
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
#line 622 "language.yy"
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
#line 631 "language.yy"
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
#line 640 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
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
    } else {
        (yyval.arcbase) = (new CommandMark((yyvsp[(2) - (3)].str), YYMSC_GETPOS((yyloc)), &msc))->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 44:
#line 658 "language.yy"
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
#line 667 "language.yy"
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
#line 677 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arcbase))
        (yyval.arclist) = (new ArcList)->Append((yyvsp[(1) - (1)].arcbase)); /* New list */
    else
        (yyval.arclist) = NULL;
}
    break;

  case 47:
#line 685 "language.yy"
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
#line 701 "language.yy"
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
#line 714 "language.yy"
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
#line 727 "language.yy"
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
#line 747 "language.yy"
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
#line 761 "language.yy"
    {
    if (C_S_H) break;
    (yyval.entitylist) = (EntityDefList*)((new EntityDefList)->Append((yyvsp[(1) - (1)].entity)));
}
    break;

  case 53:
#line 766 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.entitylist) = (EntityDefList*)(((yyvsp[(1) - (3)].entitylist))->Append((yyvsp[(3) - (3)].entity)));
}
    break;

  case 54:
#line 775 "language.yy"
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
#line 784 "language.yy"
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
#line 796 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 58:
#line 803 "language.yy"
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
#line 819 "language.yy"
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
#line 829 "language.yy"
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
#line 842 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 63:
#line 849 "language.yy"
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
#line 862 "language.yy"
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
#line 880 "language.yy"
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
#line 902 "language.yy"
    {
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
}
    break;

  case 68:
#line 910 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_SEMICOLON);
    }
}
    break;

  case 69:
#line 917 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 70:
#line 924 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 73:
#line 934 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
    }
}
    break;

  case 74:
#line 940 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_ERROR);
    }
}
    break;

  case 75:
#line 947 "language.yy"
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
#line 959 "language.yy"
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
#line 971 "language.yy"
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
#line 983 "language.yy"
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
#line 997 "language.yy"
    {
    if (C_S_H) break;
    if ((yyvsp[(1) - (1)].arclist))
        (yyval.arcparallel) = (new ArcParallel(YYMSC_GETPOS((yyloc)), &msc))->AddArcList((yyvsp[(1) - (1)].arclist));
    else
        (yyval.arcparallel) = NULL;
}
    break;

  case 80:
#line 1005 "language.yy"
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
#line 1016 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 82:
#line 1021 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_KEYWORD);
    } else {
        (yyval.arcemph) = (yyvsp[(2) - (2)].arcemph);
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 83:
#line 1031 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(((yyvsp[(2) - (2)].arcemph))->ChangeStyleForFollow());
}
    break;

  case 84:
#line 1036 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(2) - (3)].arcemph))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow((yyvsp[(2) - (3)].arcemph));
}
    break;

  case 85:
#line 1042 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(((yyvsp[(2) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow());
}
    break;

  case 86:
#line 1047 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), NULL, YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yyloc)), &msc);
    temp->AddArcList((yyvsp[(2) - (2)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (2)].arcemph));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddFollow(temp);
}
    break;

  case 87:
#line 1054 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(2) - (4)].arcemph))->AddArcList((yyvsp[(4) - (4)].arclist))->ChangeStyleForFollow()->AddAttributeList((yyvsp[(3) - (4)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (4)].arcemph))->AddFollow((yyvsp[(2) - (4)].arcemph));
}
    break;

  case 88:
#line 1060 "language.yy"
    {
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), NULL, YYMSC_GETPOS((yylsp[(1) - (3)])), YYMSC_GETPOS((yyloc)), &msc);
    temp->AddArcList((yyvsp[(3) - (3)].arclist))->ChangeStyleForFollow((yyvsp[(1) - (3)].arcemph))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph))->AddFollow(temp);
}
    break;

  case 89:
#line 1069 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = (yyvsp[(1) - (1)].arcemph);
}
    break;

  case 90:
#line 1074 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 91:
#line 1080 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist));
}
    break;

  case 92:
#line 1085 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 93:
#line 1094 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (1)].arcemph))->SetPipe();
}
    break;

  case 94:
#line 1099 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (2)].arcemph))->SetPipe()->AddAttributeList((yyvsp[(2) - (2)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph));
}
    break;

  case 95:
#line 1105 "language.yy"
    {
    if (C_S_H) break;
    (yyval.arcemph) = ((yyvsp[(1) - (2)].arcemph))->AddArcList((yyvsp[(2) - (2)].arclist))->SetPipe();
}
    break;

  case 96:
#line 1110 "language.yy"
    {
    if (C_S_H) break;
    ((yyvsp[(1) - (3)].arcemph))->AddArcList((yyvsp[(3) - (3)].arclist))->SetPipe()->AddAttributeList((yyvsp[(2) - (3)].attriblist));
    (yyval.arcemph) = ((yyvsp[(1) - (3)].arcemph));
}
    break;

  case 97:
#line 1117 "language.yy"
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

  case 98:
#line 1129 "language.yy"
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

  case 99:
#line 1139 "language.yy"
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

  case 100:
#line 1149 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
    } else {
        (yyval.arcemph) = new ArcEmphasis((yyvsp[(1) - (1)].arctype), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), NULL, YYMSC_GETPOS((yylsp[(1) - (1)])), YYMSC_GETPOS((yyloc)), &msc);
    }
}
    break;

  case 101:
#line 1158 "language.yy"
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

  case 102:
#line 1169 "language.yy"
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

  case 103:
#line 1181 "language.yy"
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

  case 104:
#line 1193 "language.yy"
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

  case 105:
#line 1224 "language.yy"
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

  case 109:
#line 1241 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (4)]), (yyvsp[(1) - (4)].str));
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (4)]), (yyvsp[(3) - (4)].str));
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 110:
#line 1254 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(2) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 111:
#line 1265 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), (yyvsp[(1) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 112:
#line 1276 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(2) - (2)].vertxpos);
    }
}
    break;

  case 113:
#line 1285 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (4)]), (yyvsp[(1) - (4)].str));
        ADDCSH((yylsp[(2) - (4)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(3) - (4)]), (yyvsp[(3) - (4)].str));
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (4)].arctype), (yyvsp[(3) - (4)].str), (yyvsp[(1) - (4)].str), (yyvsp[(4) - (4)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(4) - (4)].vertxpos);
    }
    free((yyvsp[(1) - (4)].str));
    free((yyvsp[(3) - (4)].str));
}
    break;

  case 114:
#line 1298 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_SYMBOL);
        ADDCSH_ENTITYNAME((yylsp[(2) - (3)]), (yyvsp[(2) - (3)].str));
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (3)].arctype), (yyvsp[(2) - (3)].str), MARKER_HERE_STR, (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(2) - (3)].str));
}
    break;

  case 115:
#line 1309 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_ENTITYNAME((yylsp[(1) - (3)]), (yyvsp[(1) - (3)].str));
        ADDCSH((yylsp[(2) - (3)]), COLOR_SYMBOL);
    } else {
        (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(2) - (3)].arctype), MARKER_HERE_STR, (yyvsp[(1) - (3)].str), (yyvsp[(3) - (3)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
        delete (yyvsp[(3) - (3)].vertxpos);
    }
    free((yyvsp[(1) - (3)].str));
}
    break;

  case 116:
#line 1320 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcVerticalArrow((yyvsp[(1) - (2)].arctype), MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, (yyvsp[(2) - (2)].vertxpos), YYMSC_GETPOS((yyloc)), &msc);
    delete (yyvsp[(2) - (2)].vertxpos);
}
    break;

  case 117:
#line 1331 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcbase) = new ArcDivider((yyvsp[(1) - (1)].arctype), YYMSC_GETPOS((yyloc)), &msc);
}
    break;

  case 118:
#line 1339 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 119:
#line 1343 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 120:
#line 1347 "language.yy"
    {
    (yyval.arcbase) = (yyvsp[(1) - (1)].arcarrow);
}
    break;

  case 121:
#line 1352 "language.yy"
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

  case 122:
#line 1364 "language.yy"
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

  case 123:
#line 1374 "language.yy"
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

  case 124:
#line 1384 "language.yy"
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

  case 125:
#line 1394 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 126:
#line 1404 "language.yy"
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

  case 127:
#line 1416 "language.yy"
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

  case 128:
#line 1426 "language.yy"
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

  case 129:
#line 1436 "language.yy"
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

  case 130:
#line 1446 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), false, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 131:
#line 1455 "language.yy"
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

  case 132:
#line 1467 "language.yy"
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

  case 133:
#line 1477 "language.yy"
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

  case 134:
#line 1487 "language.yy"
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

  case 135:
#line 1497 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (2)]), COLOR_SYMBOL);
        break;
    }
    (yyval.arcarrow) = ((yyvsp[(1) - (2)].arcarrow))->AddSegment(NULL, YYMSC_GETPOS((yylsp[(2) - (2)])), true, YYMSC_GETPOS((yylsp[(2) - (2)])));
}
    break;

  case 154:
#line 1516 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (1)]), (yyvsp[(1) - (1)].str));
    } else {
        (yyval.attriblist) = (new AttributeList)->Append(new Attribute("label", (yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)), YYMSC_GETPOS((yyloc)).IncCol()));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 155:
#line 1525 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(1) - (2)]), (yyvsp[(1) - (2)].str));
    } else {
        (yyval.attriblist) = ((yyvsp[(2) - (2)].attriblist))->Prepend(new Attribute("label", (yyvsp[(1) - (2)].str), YYMSC_GETPOS((yylsp[(1) - (2)])), YYMSC_GETPOS((yylsp[(1) - (2)])).IncCol()));
    }
    free((yyvsp[(1) - (2)].str));
}
    break;

  case 156:
#line 1534 "language.yy"
    {
    if (C_S_H) {
        ADDCSH_COLON_STRING((yylsp[(2) - (2)]), (yyvsp[(2) - (2)].str));
    } else {
        (yyval.attriblist) = ((yyvsp[(1) - (2)].attriblist))->Append(new Attribute("label", (yyvsp[(2) - (2)].str), YYMSC_GETPOS((yylsp[(2) - (2)])), YYMSC_GETPOS((yylsp[(2) - (2)])).IncCol()));
    }
    free((yyvsp[(2) - (2)].str));
}
    break;

  case 158:
#line 1546 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (2)]), COLOR_BRACKET);
        ADDCSH((yylsp[(2) - (2)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = new AttributeList;
}
    break;

  case 159:
#line 1555 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_BRACKET);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 160:
#line 1564 "language.yy"
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

  case 161:
#line 1574 "language.yy"
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

  case 162:
#line 1584 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (3)]), COLOR_BRACKET);
        ADDCSH((yylsp[(3) - (3)]), COLOR_ERROR);
        break;
    }
    (yyval.attriblist) = (yyvsp[(2) - (3)].attriblist);
}
    break;

  case 163:
#line 1594 "language.yy"
    {
    if (C_S_H) break;
    (yyval.attriblist) = (new AttributeList)->Append((yyvsp[(1) - (1)].attrib));
}
    break;

  case 164:
#line 1599 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(2) - (3)]), COLOR_COMMA);
        break;
    }
    (yyval.attriblist) = ((yyvsp[(1) - (3)].attriblist))->Append((yyvsp[(3) - (3)].attrib));
}
    break;

  case 165:
#line 1608 "language.yy"
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

  case 166:
#line 1620 "language.yy"
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

  case 167:
#line 1632 "language.yy"
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

  case 168:
#line 1644 "language.yy"
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

  case 169:
#line 1654 "language.yy"
    {
    if (C_S_H) {
        ADDCSH((yylsp[(1) - (1)]), COLOR_STYLENAME);
    } else {
        (yyval.attrib) = new Attribute((yyvsp[(1) - (1)].str), YYMSC_GETPOS((yyloc)));
    }
    free((yyvsp[(1) - (1)].str));
}
    break;

  case 180:
#line 1671 "language.yy"
    {(yyval.str) = strdup("->");}
    break;

  case 181:
#line 1672 "language.yy"
    {(yyval.str) = strdup("<-");}
    break;

  case 182:
#line 1673 "language.yy"
    {(yyval.str) = strdup("<->");}
    break;

  case 183:
#line 1674 "language.yy"
    {(yyval.str) = strdup("=>");}
    break;

  case 184:
#line 1675 "language.yy"
    {(yyval.str) = strdup("<=");}
    break;

  case 185:
#line 1676 "language.yy"
    {(yyval.str) = strdup("<=>");}
    break;

  case 186:
#line 1677 "language.yy"
    {(yyval.str) = strdup(">>");}
    break;

  case 187:
#line 1678 "language.yy"
    {(yyval.str) = strdup("<<");}
    break;

  case 188:
#line 1679 "language.yy"
    {(yyval.str) = strdup("<<>>");}
    break;

  case 189:
#line 1680 "language.yy"
    {(yyval.str) = strdup(">");}
    break;

  case 190:
#line 1681 "language.yy"
    {(yyval.str) = strdup("<");}
    break;

  case 191:
#line 1682 "language.yy"
    {(yyval.str) = strdup("<>");}
    break;

  case 192:
#line 1684 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_ARC_DIVIDER:  (yyval.str) = strdup("---"); break;
    case MSC_ARC_DISCO:    (yyval.str) = strdup("..."); break;
    }
}
    break;

  case 193:
#line 1691 "language.yy"
    {
    switch ((yyvsp[(1) - (1)].arctype)) {
    case MSC_EMPH_SOLID:  (yyval.str) = strdup("--"); break;
    case MSC_EMPH_DASHED: (yyval.str) = strdup("++"); break;
    case MSC_EMPH_DOTTED: (yyval.str) = strdup(".."); break;
    case MSC_EMPH_DOUBLE: (yyval.str) = strdup("=="); break;
    }
}
    break;

  case 198:
#line 1703 "language.yy"
    {
    msc.PushContext();
}
    break;

  case 199:
#line 1708 "language.yy"
    {
    msc.PopContext();
}
    break;


/* Line 1267 of yacc.c.  */
#line 4079 "language.cc"
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


#line 1713 "language.yy"



/* END OF FILE */

