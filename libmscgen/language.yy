%locations
%pure-parser
%lex-param {yyscan_t *yyscanner}
%parse-param{Msc &msc}
%parse-param{void *yyscanner}

%{
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
#define ADDCSH_ATTRVALUE(A, C, B) msc.AddCSH_AttrValue(A, C, B)
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
#define ADDCSH_ATTRVALUE(A, C, B)
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



%}

%token TOK_STRING TOK_QSTRING TOK_NUMBER TOK_DASH TOK_EQUAL TOK_COMMA
       TOK_SEMICOLON TOK_PLUS
       TOK_OCBRACKET TOK_CCBRACKET TOK_OSBRACKET TOK_CSBRACKET TOK_MSC
       TOK_COLON_STRING TOK_STYLE_NAME
       TOK_BOOLEAN
       TOK_REL_SOLID_TO    TOK_REL_SOLID_FROM	TOK_REL_SOLID_BIDIR
       TOK_REL_DOUBLE_TO   TOK_REL_DOUBLE_FROM	TOK_REL_DOUBLE_BIDIR
       TOK_REL_DASHED_TO   TOK_REL_DASHED_FROM	TOK_REL_DASHED_BIDIR
       TOK_REL_DOTTED_TO   TOK_REL_DOTTED_FROM	TOK_REL_DOTTED_BIDIR
       TOK_SPECIAL_ARC     TOK_EMPH
       TOK_COMMAND_HEADING TOK_COMMAND_NUDGE TOK_COMMAND_NEWPAGE
       TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
       TOK_COMMAND_BIG TOK_COMMAND_PIPE TOK_COMMAND_MARK TOK_VERTICAL TOK_AT
%union
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
};

%type <msc>        msc
%type <arcbase>    arcrel arc complete_arc opt vertrel
%type <arcarrow>   arcrel_to arcrel_from arcrel_bidir
%type <arcemph>    emphrel emphasis_list pipe_emphasis first_emphasis
%type <arcparallel>parallel
%type <arclist>    arclist braced_arclist mscenclosed optlist
%type <entity>     entity
%type <entitylist> entitylist
%type <arctype>    relation_to relation_from relation_bidir empharcrel_straight
                   TOK_REL_SOLID_TO TOK_REL_DOUBLE_TO TOK_REL_DASHED_TO TOK_REL_DOTTED_TO
                   TOK_REL_SOLID_FROM TOK_REL_DOUBLE_FROM TOK_REL_DASHED_FROM
                   TOK_REL_DOTTED_FROM
                   TOK_REL_SOLID_BIDIR TOK_REL_DOUBLE_BIDIR TOK_REL_DASHED_BIDIR
                   TOK_REL_DOTTED_BIDIR
                   TOK_SPECIAL_ARC TOK_EMPH
%type <attrib>     arcattr
%type <vertxpos>   vertxpos
%type <attriblist> arcattrlist full_arcattrlist full_arcattrlist_with_label
%type <str>        entity_string reserved_word_string string symbol_string
                   TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_STYLE_NAME
                   TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE
                   TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
                   TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
                   TOK_NUMBER TOK_BOOLEAN TOK_VERTICAL TOK_AT
%type <stringlist> tok_stringlist

%destructor {if (!C_S_H) delete $$;} vertrel
%destructor {if (!C_S_H) delete $$;} vertxpos
%destructor {if (!C_S_H) delete $$;} arcrel arc complete_arc opt
%destructor {if (!C_S_H) delete $$;} arcrel_to arcrel_from arcrel_bidir
%destructor {if (!C_S_H) delete $$;} emphrel first_emphasis emphasis_list pipe_emphasis
%destructor {if (!C_S_H) delete $$;} parallel
%destructor {if (!C_S_H) delete $$;} arclist braced_arclist mscenclosed optlist
%destructor {if (!C_S_H) delete $$;} entity entitylist
%destructor {if (!C_S_H) delete $$;} arcattr arcattrlist full_arcattrlist full_arcattrlist_with_label tok_stringlist
%destructor {free($$);}  entity_string reserved_word_string string symbol_string
%destructor {free($$);}  TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_STYLE_NAME
%destructor {free($$);}  TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE
%destructor {free($$);}  TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
%destructor {free($$);}  TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
%destructor {free($$);}  TOK_NUMBER TOK_BOOLEAN

%%

msc:
{
    //no action for empty file
}
             | arclist
{
    if (C_S_H) {
    } else {
        msc.AddArcs($1);
    }
}
             | arclist error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
    } else {
        msc.AddArcs($1);
    }
};

braced_arclist: scope_open arclist scope_close
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_BRACE);
        ADDCSH(@3, COLOR_BRACE);
    } else {
        $$ = $2;
    }
}
            | scope_open arclist error scope_close
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_BRACE);
        ADDCSH(@3, COLOR_ERROR);
        ADDCSH(@4, COLOR_BRACE);
    } else {
        $$ = $2;
    }
    yyerrok;
}
            | scope_open scope_close
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_BRACE);
        ADDCSH(@2, COLOR_BRACE);
    } else {
         $$ = new ArcList;
    }
};


arclist:    complete_arc
{
    if (C_S_H) break;
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = new ArcList;
}
            | arclist complete_arc
{
    if (C_S_H) break;
    if ($2)
        ($1)->Append($2);     /* Add to existing list */
    $$ = ($1);
}
            | mscenclosed
{
    if (C_S_H) break;
    $$ = $1;
}
            | arclist mscenclosed
{
    if (C_S_H) break;
    //Merge $2 into $1
    ($1)->splice(($1)->end(), *($2));
    delete ($2);
    $$ = $1;
};

//here we do not use open_scope, but only TOK_OCBRACKET, because no new
//scope is opened for an msc {...} arclist: anything you do there spills over
//to later sections
mscenclosed: msckey TOK_OCBRACKET arclist TOK_CCBRACKET
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_BRACE);
        ADDCSH(@4, COLOR_BRACE);
    } else {
        $$ = $3;
    }
}
              |msckey TOK_OCBRACKET arclist error TOK_CCBRACKET
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_BRACE);
        ADDCSH(@4, COLOR_ERROR);
        ADDCSH(@5, COLOR_BRACE);
    } else {
        $$ = $3;
    }
    yyerrok;
};

msckey:       TOK_MSC
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
    }
    free($1);
}
              | TOK_MSC TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_DESIGNNAME);
    } else {
        msc.AddDesignAttribute(Attribute("msc", $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3)));
    }
    free($1);
    free($3);
};

complete_arc: TOK_SEMICOLON
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SEMICOLON);
    } else {
        $$=NULL;
    }
}
              |arc TOK_SEMICOLON
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SEMICOLON);
    } else {
        $$=$1;
    }
}
              |arc error TOK_SEMICOLON
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
        ADDCSH(@3, COLOR_SEMICOLON);
    } else {
        $$=$1;
    }
    yyerrok;
}
              |arc error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
    } else {
        $$=$1;
    }
};

arc:            arcrel
              | arcrel full_arcattrlist_with_label
{
    if (C_S_H) break;
    $$ = ($1)->AddAttributeList($2);
}
              |TOK_COMMAND_BIG arcrel
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        //Returns NULL, if BIG is before a self-pointing arrow
        $$ = msc.CreateArcBigArrow($2);
        delete $2;
    }
    free($1);
}
              | TOK_COMMAND_BIG arcrel full_arcattrlist_with_label
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        //Returns NULL, if BIG is before a self-pointing arrow
        ArcBigArrow *arrow = msc.CreateArcBigArrow($2);
        if (arrow) arrow->AddAttributeList($3);
        $$ = arrow;
        delete $2;
    }
    free($1);
}
              |TOK_VERTICAL vertrel
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = $2;
    }
    free($1);
}
              | TOK_VERTICAL vertrel full_arcattrlist_with_label
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = ($2)->AddAttributeList($3);
    }
    free($1);
}
              | full_arcattrlist_with_label
{
    if (C_S_H) break;
    $$ = (new ArcDivider(MSC_ARC_VSPACE, YYMSC_GETPOS(@$), &msc))->AddAttributeList($1);
}
              | entitylist
{
    if (C_S_H) break;
    $$ = new CommandEntity($1, YYMSC_GETPOS(@$), &msc);
}
              | optlist
{
    if (C_S_H) break;
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ($1) {
        $$ = (new ArcParallel(YYMSC_GETPOS(@$), &msc))->AddArcList($1);
    } else
        $$ = NULL;
}
              | emphasis_list
{
    if (C_S_H) break;
    $$ = $1; //to remove warning for downcast
}
              | parallel
{
    if (C_S_H) break;
    $$ = $1;
}
              | TOK_COMMAND_DEFCOLOR colordeflist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = NULL;
    }
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = NULL;
    }
    free($1);
}
              | TOK_COMMAND_DEFDESIGN designdef
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = NULL;
    }
    free($1);
}
              | TOK_COMMAND_HEADING
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = new CommandEntity(NULL, YYMSC_GETPOS(@$), &msc);
    }
    free($1);
}
              | TOK_COMMAND_HEADING full_arcattrlist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = (new CommandEntity(NULL, YYMSC_GETPOS(@$), &msc))->AddAttributeList($2);
    }
    free($1);
}
	      | TOK_COMMAND_NUDGE
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS(@$), &msc);
    }
    free($1);
}
	      | TOK_COMMAND_NUDGE  full_arcattrlist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = (new ArcDivider(MSC_COMMAND_NUDGE, YYMSC_GETPOS(@$), &msc))->AddAttributeList($2);
    }
    free($1);
}
              | TOK_COMMAND_MARK string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = new CommandMark($2, YYMSC_GETPOS(@$), &msc);
    }
    free($2);
}
              | TOK_COMMAND_MARK string full_arcattrlist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = (new CommandMark($2, YYMSC_GETPOS(@$), &msc))->AddAttributeList($3);
    }
    free($2);
}
              | TOK_COMMAND_NEWPAGE
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = new CommandNewpage(YYMSC_GETPOS(@$), &msc);
    }
    free($1);
}
              | TOK_COMMAND_NEWPAGE full_arcattrlist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = (new CommandNewpage(YYMSC_GETPOS(@$), &msc))->AddAttributeList($2);
    }
    free($1);
};

optlist:     opt
{
    if (C_S_H) break;
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = NULL;
}
           | optlist TOK_COMMA opt
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
        break;
    }
    if ($3) {
        if ($1)
            $$ = ($1)->Append($3);     /* Add to existing list */
        else
            $$ = (new ArcList)->Append($3); /* New list */
    } else {
        $$ = $1;
    }
}
            | optlist error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
        break;
    }
    $$ = $1;
};

opt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddAttribute(Attribute($1, string_to_bool($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3));
        $$ = NULL;
    }
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
       msc.AddAttribute(Attribute($1, atof($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3));
       $$ = NULL;
    }
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH_ATTRVALUE(@3, $3, $1);
    } else {
        Attribute a($1, $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3));
        MscFillAttr fill;
        fill.Empty();
        if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
            $$ = new CommandNewBackground(a.linenum_attr, &msc, fill);
        } else {
            msc.AddAttribute(Attribute($1, $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3)));
            $$ = NULL;
        }
    }
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddAttribute(Attribute("msc", $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3)));
        $$ = NULL;
    }
    free($1);
    free($3);
};

entitylist:   entity
{
    if (C_S_H) break;
    $$ = (EntityDefList*)((new EntityDefList)->Append($1));
}
            | entitylist TOK_COMMA entity
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
        break;
    }
    $$ = (EntityDefList*)(($1)->Append($3));
}
            | entitylist error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
        break;
    }
    $$ = $1;
};

entity:       entity_string full_arcattrlist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
    } else {
        $$ = (new EntityDef($1, &msc))->AddAttributeList($2, &msc);
    }
    free($1);
}
            | entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
    } else {
        $$ = (new EntityDef($1, &msc))->AddAttributeList(NULL, &msc);
    }
    free($1);
};


styledeflist: styledef
             | styledeflist TOK_COMMA styledef
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
    }
};

styledef : tok_stringlist TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_BRACKET);
        ADDCSH(@4, COLOR_BRACKET);
        break;
    }
    for (std::list<string>::iterator i = ($1)->begin(); i!=($1)->end(); i++) {
        MscStyle style = msc.StyleSets.top().GetStyle(*i);
        AttributeList::iterator j=($3)->begin();
        while (j!=($3)->end())
           style.AddAttribute(**(j++), &msc);
        msc.StyleSets.top()[*i] = style;
    }
    delete($1);
    delete($3);
};

tok_stringlist : string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_STYLENAME);
    } else {
        $$ = new std::list<string>;
        ($$)->push_back($1);
    }
    free($1);
}
             | tok_stringlist TOK_COMMA string
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
        ADDCSH(@3, COLOR_STYLENAME);
    } else {
       ($1)->push_back($3);
       $$ = $1;
    }
    free($3);
};

colordeflist: colordef
             | colordeflist TOK_COMMA colordef
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
    }
};

colordef : TOK_STRING TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_COLORNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_COLORDEF);
    } else {
        msc.ColorSets.top().AddColor($1, $3, msc.Error, YYMSC_GETPOS(@$));
    }
    free($1);
    free($3);
};

designdef : TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON TOK_CCBRACKET
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_DESIGNNAME);
        ADDCSH(@2, COLOR_BRACE);
        ADDCSH(@4, COLOR_SEMICOLON);
        ADDCSH(@5, COLOR_BRACE);
        break;
    }
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
}
           |TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON error
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_DESIGNNAME);
        ADDCSH(@2, COLOR_BRACE);
        ADDCSH(@4, COLOR_SEMICOLON);
        ADDCSH(@5, COLOR_ERROR);
        break;
    }
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
}


scope_open_empty: TOK_OCBRACKET
{
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
};

designelementlist: designelement
                   | designelementlist TOK_SEMICOLON designelement
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SEMICOLON);
    }
};

designelement: TOK_COMMAND_DEFCOLOR colordeflist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    }
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    }
    free($1);
}
              | designoptlist;

designoptlist: designopt
               | designoptlist TOK_COMMA designopt
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
    }
}
               | designoptlist error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
    }
};

designopt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute($1, string_to_bool($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3));
    }
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute($1, atof($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3));
    }
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_OPTIONNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute($1, $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3)));
    }
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        msc.AddDesignAttribute(Attribute("msc", $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3)));
    }
    free($1);
    free($3);
};


parallel:    braced_arclist
{
    if (C_S_H) break;
    if ($1)
        $$ = (new ArcParallel(YYMSC_GETPOS(@$), &msc))->AddArcList($1);
    else
        $$ = NULL;
}
	     | parallel braced_arclist
{
    if (C_S_H) break;
    if ($2==NULL)
        $$ = $1;
    else if ($1)
        $$ = ($1)->AddArcList($2);
    else
        $$ = (new ArcParallel(YYMSC_GETPOS(@$), &msc))->AddArcList($2);
};

emphasis_list: first_emphasis
{
    if (C_S_H) break;
    $$ = $1;
}
           | TOK_COMMAND_PIPE pipe_emphasis
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
    } else {
        $$ = $2;
    }
    free($1);
}
/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
           | emphasis_list emphrel
{
    if (C_S_H) break;
    $$ = ($1)->AddFollow(($2)->ChangeStyleForFollow());
}
           | emphasis_list emphrel full_arcattrlist_with_label
{
    if (C_S_H) break;
    ($2)->ChangeStyleForFollow()->AddAttributeList($3);
    $$ = ($1)->AddFollow($2);
}
           | emphasis_list emphrel braced_arclist
{
    if (C_S_H) break;
    $$ = ($1)->AddFollow(($2)->AddArcList($3)->ChangeStyleForFollow());
}
           | emphasis_list braced_arclist
{
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS(@1), NULL, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$), &msc);
    temp->AddArcList($2)->ChangeStyleForFollow($1);
    $$ = ($1)->AddFollow(temp);
}
           | emphasis_list emphrel full_arcattrlist_with_label braced_arclist
{
    if (C_S_H) break;
    ($2)->AddArcList($4)->ChangeStyleForFollow()->AddAttributeList($3);
    $$ = ($1)->AddFollow($2);
}
           | emphasis_list full_arcattrlist_with_label braced_arclist
{
    if (C_S_H) break;
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, YYMSC_GETPOS(@1), NULL, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$), &msc);
    temp->AddArcList($3)->ChangeStyleForFollow($1)->AddAttributeList($2);
    $$ = ($1)->AddFollow(temp);
};

/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
first_emphasis:   emphrel
{
    if (C_S_H) break;
    $$ = $1;
}
           | emphrel full_arcattrlist_with_label
{
    if (C_S_H) break;
    ($1)->AddAttributeList($2);
    $$ = ($1);
}
           | emphrel braced_arclist
{
    if (C_S_H) break;
    $$ = ($1)->AddArcList($2);
}
           | emphrel full_arcattrlist_with_label braced_arclist
{
    if (C_S_H) break;
    ($1)->AddArcList($3)->AddAttributeList($2);
    $$ = ($1);
};

/* ALWAYS Add Arclist before Attributes or setpipe.
** AddArcList changes default attributes to style "emphasis" */
pipe_emphasis:   emphrel
{
    if (C_S_H) break;
    $$ = ($1)->SetPipe();
}
           | emphrel full_arcattrlist_with_label
{
    if (C_S_H) break;
    ($1)->SetPipe()->AddAttributeList($2);
    $$ = ($1);
}
           | emphrel braced_arclist
{
    if (C_S_H) break;
    $$ = ($1)->AddArcList($2)->SetPipe();
}
           | emphrel full_arcattrlist_with_label braced_arclist
{
    if (C_S_H) break;
    ($1)->AddArcList($3)->SetPipe()->AddAttributeList($2);
    $$ = ($1);
};

emphrel:   entity_string TOK_EMPH entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcEmphasis($2, $1, YYMSC_GETPOS(@1), $3, YYMSC_GETPOS(@3), YYMSC_GETPOS(@$), &msc);
    }
    free($1);
    free($3);
}
           | TOK_EMPH entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcEmphasis($1, NULL, YYMSC_GETPOS(@1), $2, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$), &msc);
    }
    free($2);
}
           | entity_string TOK_EMPH
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = new ArcEmphasis($2, $1, YYMSC_GETPOS(@1), NULL, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$), &msc);
    }
    free($1);
}
           | TOK_EMPH
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
    } else {
        $$ = new ArcEmphasis($1, NULL, YYMSC_GETPOS(@1), NULL, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$), &msc);
    }
};

vertxpos: TOK_AT entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2);
    }
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_ENTITYNAME);
        ADDCSH(@3, COLOR_SYMBOL);
    } else {
        $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2, VertXPos::POS_LEFT_SIDE);
    }
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_PLUS
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_ENTITYNAME);
        ADDCSH(@3, COLOR_SYMBOL);
    } else {
        $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2, VertXPos::POS_RIGHT_SIDE);
    }
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_EMPH
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_ENTITYNAME);
        ADDCSH(@3, COLOR_SYMBOL);
    } else {
        switch ($3) {
        case MSC_EMPH_SOLID:
            $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2, VertXPos::POS_LEFT_BY);
            break;
        case MSC_EMPH_DASHED:
            $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2, VertXPos::POS_RIGHT_BY);
            break;
        case MSC_EMPH_DOTTED:
            msc.Error.Error(YYMSC_GETPOS(@3),
                            "unexpected '..', expected '-', '--', '+' or '++'."
                            " Ignoring vertical."); break;
            $$ = NULL;
            break;
        case MSC_EMPH_DOUBLE:
            msc.Error.Error(YYMSC_GETPOS(@3),
                            "unexpected '==', expected '-', '--', '+' or '++'."
                            " Ignoring vertical."); break;
            $$ = NULL;
            break;
        }
    }
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_KEYWORD);
        ADDCSH(@2, COLOR_ENTITYNAME);
        ADDCSH(@3, COLOR_SYMBOL);
        ADDCSH(@4, COLOR_ENTITYNAME);
    } else {
        $$ = new VertXPos(YYMSC_GETPOS(@2), msc, $2, VertXPos::POS_CENTER, $4);
    }
    free($1);
    free($2);
    free($4);
};

empharcrel_straight: TOK_EMPH | relation_to | relation_bidir;

vertrel: entity_string empharcrel_straight entity_string vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcVerticalArrow($2, $1, $3, $4, YYMSC_GETPOS(@$), &msc);
        delete $4;
    }
    free($1);
    free($3);
}
       | empharcrel_straight entity_string vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, $2, $3, YYMSC_GETPOS(@$), &msc);
        delete $3;
    }
    free($2);
}
       | entity_string empharcrel_straight vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = new ArcVerticalArrow($2, $1, MARKER_HERE_STR, $3, YYMSC_GETPOS(@$), &msc);
        delete $3;
    }
    free($1);
}
       | empharcrel_straight vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
    } else {
        $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, $2, YYMSC_GETPOS(@$), &msc);
        delete $2;
    }
}
       | entity_string relation_from entity_string vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcVerticalArrow($2, $3, $1, $4, YYMSC_GETPOS(@$), &msc);
        delete $4;
    }
    free($1);
    free($3);
}
       | relation_from entity_string vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = new ArcVerticalArrow($1, $2, MARKER_HERE_STR, $3, YYMSC_GETPOS(@$), &msc);
        delete $3;
    }
    free($2);
}
       | entity_string relation_from vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = new ArcVerticalArrow($2, MARKER_HERE_STR, $1, $3, YYMSC_GETPOS(@$), &msc);
        delete $3;
    }
    free($1);
}
       | relation_from vertxpos
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        break;
    }
    $$ = new ArcVerticalArrow($1, MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, $2, YYMSC_GETPOS(@$), &msc);
    delete $2;
};


arcrel:       TOK_SPECIAL_ARC
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        break;
    }
    $$ = new ArcDivider($1, YYMSC_GETPOS(@$), &msc);
}
            | arcrel_to
{
    $$ = $1;
}
            | arcrel_from
{
    $$ = $1;
}
            | arcrel_bidir
{
    $$ = $1;
};

arcrel_to:    entity_string relation_to entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($2, $1, YYMSC_GETPOS(@1), $3, YYMSC_GETPOS(@3), YYMSC_GETPOS(@$));
    }
    free($1);
    free($3);
}
            | relation_to entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, YYMSC_GETPOS(@1), $2, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$));
    }
    free($2);
}
            | entity_string relation_to
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = msc.CreateArcArrow($2, $1, YYMSC_GETPOS(@1), RSIDE_ENT_STR, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$));
    }
    free($1);
}
            | arcrel_to relation_to_cont entity_string
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = ($1)->AddSegment($3, YYMSC_GETPOS(@3), true, YYMSC_GETPOS(@2));
    }
    free($3);
}
            | arcrel_to relation_to_cont
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        break;
    }
    $$ = ($1)->AddSegment(NULL, YYMSC_GETPOS(@2), true, YYMSC_GETPOS(@2));
};


arcrel_from:    entity_string relation_from entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($2, $3, YYMSC_GETPOS(@3), $1, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$));
    }
    free($1);
    free($3);
}
             | relation_from entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($1, $2, YYMSC_GETPOS(@2), LSIDE_ENT_STR, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$));
    }
    free($2);
}
             | entity_string relation_from
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = msc.CreateArcArrow($2, RSIDE_ENT_STR, YYMSC_GETPOS(@2), $1, YYMSC_GETPOS(@1), YYMSC_GETPOS(@$));
    }
    free($1);
}
             | arcrel_from relation_from_cont entity_string
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = ($1)->AddSegment($3, YYMSC_GETPOS(@3), false, YYMSC_GETPOS(@2));
    }
    free($3);
}
             | arcrel_from relation_from_cont
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        break;
    }
    $$ = ($1)->AddSegment(NULL, YYMSC_GETPOS(@2), false, YYMSC_GETPOS(@2));
};

arcrel_bidir:    entity_string relation_bidir entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($2, $1, YYMSC_GETPOS(@1), $3, YYMSC_GETPOS(@3), YYMSC_GETPOS(@$));
    }
    free($1);
    free($3);
}
            | relation_bidir entity_string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_SYMBOL);
        ADDCSH(@2, COLOR_ENTITYNAME);
    } else {
        $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, YYMSC_GETPOS(@1), $2, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$));
    }
    free($2);
}
            | entity_string relation_bidir
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ENTITYNAME);
        ADDCSH(@2, COLOR_SYMBOL);
    } else {
        $$ = msc.CreateArcArrow($2, $1, YYMSC_GETPOS(@1), RSIDE_ENT_STR, YYMSC_GETPOS(@2), YYMSC_GETPOS(@$));
    }
    free($1);
}
            | arcrel_bidir relation_bidir_cont entity_string
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        ADDCSH(@3, COLOR_ENTITYNAME);
    } else {
        $$ = ($1)->AddSegment($3, YYMSC_GETPOS(@3), true, YYMSC_GETPOS(@2));
    }
    free($3);
}
            | arcrel_bidir relation_bidir_cont
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_SYMBOL);
        break;
    }
    $$ = ($1)->AddSegment(NULL, YYMSC_GETPOS(@2), true, YYMSC_GETPOS(@2));
};

relation_to:   TOK_REL_SOLID_TO | TOK_REL_DOUBLE_TO | TOK_REL_DASHED_TO | TOK_REL_DOTTED_TO;
relation_from: TOK_REL_SOLID_FROM | TOK_REL_DOUBLE_FROM | TOK_REL_DASHED_FROM | TOK_REL_DOTTED_FROM;
relation_bidir: TOK_REL_SOLID_BIDIR | TOK_REL_DOUBLE_BIDIR | TOK_REL_DASHED_BIDIR | TOK_REL_DOTTED_BIDIR;

relation_to_cont : relation_to | TOK_DASH;
relation_from_cont : relation_from | TOK_DASH;
relation_bidir_cont : relation_bidir | TOK_DASH;



full_arcattrlist_with_label: TOK_COLON_STRING
{
    if (C_S_H) {
        ADDCSH_COLON_STRING(@1, $1);
    } else {
        $$ = (new AttributeList)->Append(new Attribute("label", $1, YYMSC_GETPOS(@$), YYMSC_GETPOS(@$).IncCol()));
    }
    free($1);
}
              | TOK_COLON_STRING full_arcattrlist
{
    if (C_S_H) {
        ADDCSH_COLON_STRING(@1, $1);
    } else {
        $$ = ($2)->Prepend(new Attribute("label", $1, YYMSC_GETPOS(@1), YYMSC_GETPOS(@1).IncCol()));
    }
    free($1);
}
              | full_arcattrlist TOK_COLON_STRING
{
    if (C_S_H) {
        ADDCSH_COLON_STRING(@1, $2);
    } else {
        $$ = ($1)->Append(new Attribute("label", $2, YYMSC_GETPOS(@2), YYMSC_GETPOS(@2).IncCol()));
    }
    free($2);
}
              | full_arcattrlist;


full_arcattrlist: TOK_OSBRACKET TOK_CSBRACKET
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_BRACKET);
        ADDCSH(@2, COLOR_BRACKET);
        break;
    }
    $$ = new AttributeList;
}
                   | TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_BRACKET);
        ADDCSH(@3, COLOR_BRACKET);
        break;
    }
    $$ = $2;
};

arcattrlist:    arcattr
{
    if (C_S_H) break;
    $$ = (new AttributeList)->Append($1);
}
              | arcattrlist TOK_COMMA arcattr
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_COMMA);
        break;
    }
    $$ = ($1)->Append($3);
}
              | arcattrlist error
{
    if (C_S_H) {
        ADDCSH(@2, COLOR_ERROR);
        break;
    }
    $$ = $1;
};

arcattr:         string TOK_EQUAL string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ATTRNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH_ATTRVALUE(@3, $3, $1);
    } else {
        $$ = new Attribute($1, $3, YYMSC_GETPOS(@$), YYMSC_GETPOS(@3));
    }
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_NUMBER
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ATTRNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        $$ = new Attribute($1, atof($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3);
    }
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_BOOLEAN
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ATTRNAME);
        ADDCSH(@2, COLOR_EQUAL);
        ADDCSH(@3, COLOR_ATTRVALUE);
    } else {
        $$ = new Attribute($1, string_to_bool($3), YYMSC_GETPOS(@$), YYMSC_GETPOS(@3), $3);
    }
    free($1);
    free($3);
}
	    | string TOK_EQUAL
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_ATTRNAME);
        ADDCSH(@2, COLOR_EQUAL);
    } else {
        $$ = new Attribute($1, (char*)NULL, YYMSC_GETPOS(@$), YYMSC_GETPOS(@$));
    }
    free($1);
}
	    | string
{
    if (C_S_H) {
        ADDCSH(@1, COLOR_STYLENAME);
    } else {
        $$ = new Attribute($1, YYMSC_GETPOS(@$));
    }
    free($1);
};

//cannot be a reserved word, symbol or style name
entity_string: TOK_QSTRING | TOK_STRING;

reserved_word_string : TOK_MSC | TOK_COMMAND_DEFCOLOR |
                       TOK_COMMAND_DEFSTYLE | TOK_COMMAND_DEFDESIGN |
                       TOK_COMMAND_NEWPAGE | TOK_COMMAND_BIG | TOK_COMMAND_PIPE |
                       TOK_VERTICAL

symbol_string : TOK_REL_SOLID_TO  {$$ = strdup("->");}
       | TOK_REL_SOLID_FROM	  {$$ = strdup("<-");}
       | TOK_REL_SOLID_BIDIR      {$$ = strdup("<->");}
       | TOK_REL_DOUBLE_TO        {$$ = strdup("=>");}
       | TOK_REL_DOUBLE_FROM	  {$$ = strdup("<=");}
       | TOK_REL_DOUBLE_BIDIR     {$$ = strdup("<=>");}
       | TOK_REL_DASHED_TO        {$$ = strdup(">>");}
       | TOK_REL_DASHED_FROM	  {$$ = strdup("<<");}
       | TOK_REL_DASHED_BIDIR     {$$ = strdup("<<>>");}
       | TOK_REL_DOTTED_TO        {$$ = strdup(">");}
       | TOK_REL_DOTTED_FROM	  {$$ = strdup("<");}
       | TOK_REL_DOTTED_BIDIR     {$$ = strdup("<>");}
       | TOK_SPECIAL_ARC
{
    switch ($1) {
    case MSC_ARC_DIVIDER:  $$ = strdup("---"); break;
    case MSC_ARC_DISCO:    $$ = strdup("..."); break;
    }
}
       | TOK_EMPH
{
    switch ($1) {
    case MSC_EMPH_SOLID:  $$ = strdup("--"); break;
    case MSC_EMPH_DASHED: $$ = strdup("++"); break;
    case MSC_EMPH_DOTTED: $$ = strdup(".."); break;
    case MSC_EMPH_DOUBLE: $$ = strdup("=="); break;
    }
};

string: entity_string | reserved_word_string | symbol_string | TOK_STYLE_NAME;

scope_open: TOK_OCBRACKET
{
    msc.PushContext();
};

scope_close: TOK_CCBRACKET
{
    msc.PopContext();
};


%%


/* END OF FILE */
