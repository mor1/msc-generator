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
	if (msc.current_pos.line == 0) {
		msc.current_pos.line = yyget_lineno(yyscanner);
        msc.current_pos.col = 0;
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

//locations somehow do not work, so we hide them with macros
//define this, if locations actually work
#ifdef LOCATIONS_ACTUALLY_WORK
#define YYMSC_GETLINENO(A) A.first_line
#define YYSMC_GETCOLNO(A) A.first_column
#else
#define YYMSC_GETLINENO(A) yyget_lineno(yyscanner)
#define YYMSC_GETCOLNO(A) 0
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
    unsigned long     linenum;
};

%type <msc>        msc
%type <arcbase>    arcrel arc opt first_emphasis follow_emphasis emphasis_list pipe_emphasis vertrel
%type <arcarrow>   arcrel_to arcrel_from arcrel_bidir
%type <arcemph>    emphrel
%type <arcparallel>parallel
%type <arclist>    arclist arclist_no_semicolon mscenclosed parallel_element optlist
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
%type <linenum>    TOK_OCBRACKET TOK_OSBRACKET

%destructor {delete $$;} vertrel
%destructor {delete $$;} vertxpos
%destructor {delete $$;} arcrel arc opt first_emphasis follow_emphasis emphasis_list pipe_emphasis
%destructor {delete $$;} arcrel_to arcrel_from arcrel_bidir
%destructor {delete $$;} emphrel parallel
%destructor {delete $$;} arclist arclist_no_semicolon mscenclosed parallel_element optlist
%destructor {delete $$;} entity entitylist
%destructor {delete $$;} arcattr arcattrlist full_arcattrlist full_arcattrlist_with_label tok_stringlist
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
             | arclist TOK_SEMICOLON
{
    msc.AddArcs($1);
}
             | arclist_no_semicolon
{
    msc.AddArcs($1);
};

arclist:    arc
{
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = new ArcList;
}
            | arclist TOK_SEMICOLON arc
{
    if ($3)
        $$ = ($1)->Append($3);     /* Add to existing list */
}
            | arclist error
{
    $$ = $1;
    //Skip to next semicolon
    while (yychar > YYEOF && yychar != TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
}
            | arclist_no_semicolon arc
{
    if ($2)
        $$ = ($1)->Append($2);     /* Add to existing list */
}
            | arclist_no_semicolon error
{
    $$ = $1;
    //Skip to next semicolon
    while (yychar > YYEOF && yychar != TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
};

arclist_no_semicolon: mscenclosed
{
    $$ = $1;
}
           | arclist TOK_SEMICOLON mscenclosed
{
    //Merge $3 into $1
    ($1)->splice(($1)->end(), *($3));
    delete ($3);
    $$ = $1;
}
           | arclist_no_semicolon mscenclosed
{
    //Merge $2 into $1
    ($1)->splice(($1)->end(), *($2));
    delete ($2);
    $$ = $1;
};

mscenclosed: msckey TOK_OCBRACKET arclist TOK_SEMICOLON TOK_CCBRACKET
{
    $$ = $3;
};



arc:
{
    $$ = NULL;
}
              |arcrel
              | arcrel full_arcattrlist_with_label
{
    $$ = ($1)->AddAttributeList($2);
}
              |TOK_COMMAND_BIG arcrel
{
    //Returns NULL, if BIG is before a self-pointing arrow
    $$ = msc.CreateArcBigArrow($2);
    delete $2;
    free($1);
}
              | TOK_COMMAND_BIG arcrel full_arcattrlist_with_label
{
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow($2);
    if (arrow) arrow->AddAttributeList($3);
    $$ = arrow;
    delete $2;
    free($1);
}
              |TOK_VERTICAL vertrel
{
    $$ = $2;
    free($1);
}
              | TOK_VERTICAL vertrel full_arcattrlist_with_label
{
    $$ = ($2)->AddAttributeList($3);
    free($1);
}
              | full_arcattrlist_with_label
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new ArcDivider(MSC_ARC_VSPACE, l, &msc))->AddAttributeList($1);
}
              | entitylist
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new CommandEntity($1, l, &msc);
}
              | optlist
{
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ($1) {
		file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
        $$ = (new ArcParallel(l, &msc))->AddArcList($1);
    } else
        $$ = NULL;
}
              | emphasis_list
              | parallel
{
    $$ = $1;
}
              | TOK_COMMAND_DEFCOLOR colordeflist
{
    $$ = NULL;
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
    $$ = NULL;
    free($1);
}
              | TOK_COMMAND_DEFDESIGN designdef
{
    $$ = NULL;
    free($1);
}
              | TOK_COMMAND_HEADING
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new CommandEntity(NULL, l, &msc);
    free($1);
}
              | TOK_COMMAND_HEADING full_arcattrlist
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new CommandEntity(NULL, l, &msc))->AddAttributeList($2);
    free($1);
}
	      | TOK_COMMAND_NUDGE
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcDivider(MSC_COMMAND_NUDGE, l, &msc);
    free($1);
}
	      | TOK_COMMAND_NUDGE  full_arcattrlist
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new ArcDivider(MSC_COMMAND_NUDGE, l, &msc))->AddAttributeList($2);
    free($1);
}
              | TOK_COMMAND_MARK string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new CommandMark($2, l, &msc);
    free($2);
}
              | TOK_COMMAND_MARK string full_arcattrlist
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new CommandMark($2, l, &msc))->AddAttributeList($3);
    free($2);
}
              | TOK_COMMAND_NEWPAGE
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new CommandNewpage(l, &msc);
    free($1);
}
              | TOK_COMMAND_NEWPAGE full_arcattrlist
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new CommandNewpage(l, &msc))->AddAttributeList($2);
    free($1);
};


msckey:       TOK_MSC
{
    free($1);
}
              | TOK_MSC TOK_EQUAL string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    msc.AddDesignAttribute(Attribute("msc", $3, l));
    free($1);
    free($3);
};

optlist:     opt
{
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = NULL;
}
           | optlist TOK_COMMA opt
{
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
    $$ = $1;
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
};

opt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    msc.AddAttribute(Attribute($1, string_to_bool($3), l, $3));
    free($1);
    free($3);
    $$ = NULL;
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    msc.AddAttribute(Attribute($1, atof($3), l, $3));
    free($1);
    free($3);
    $$ = NULL;
}
            | entity_string TOK_EQUAL string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    Attribute a($1, $3, l);
    MscFillAttr fill;
    fill.Empty();
    if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
        $$ = new CommandNewBackground(a.linenum, &msc, fill);
    } else {
    	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
        msc.AddAttribute(Attribute($1, $3, l));
        $$ = NULL;
    }
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
    msc.AddAttribute(Attribute("msc", $3, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$))));
    $$ = NULL;
    free($1);
    free($3);
};

entitylist:   entity
{
    $$ = (EntityDefList*)((new EntityDefList)->Append($1));
}
            | entitylist TOK_COMMA entity
{
    $$ = (EntityDefList*)(($1)->Append($3));
}
            | entitylist error
{
    $$ = $1;
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
};

entity:       entity_string full_arcattrlist
{
    $$ = (new EntityDef($1, &msc))->AddAttributeList($2, &msc);
    free($1);
}
            | entity_string
{
    $$ = (new EntityDef($1, &msc))->AddAttributeList(NULL, &msc);
    free($1);
};


styledeflist: styledef
             | styledeflist TOK_COMMA styledef;

styledef : tok_stringlist TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
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
    $$ = new std::list<string>;
    ($$)->push_back($1);
    free($1);
}
             | tok_stringlist TOK_COMMA string
{
    ($1)->push_back($3);
    $$ = $1;
    free($3);
};

colordeflist: colordef
             | colordeflist TOK_COMMA colordef;

colordef : TOK_STRING TOK_EQUAL string
{
    msc.ColorSets.top().AddColor($1, $3, msc.Error, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)));
    free($1);
    free($3);
};

designdef : TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON TOK_CCBRACKET
{
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
};

scope_open_empty: TOK_OCBRACKET
{
    //push empty color & style sets for design definition
    msc.ColorSets.push(ColorSet());
    msc.StyleSets.push(StyleSet());
    msc.SetDesign("plain", true);
    msc.saved_hscale = msc.hscale;
};

designelementlist: designelement
                   | designelementlist TOK_SEMICOLON designelement;

designelement: TOK_COMMAND_DEFCOLOR colordeflist
              | TOK_COMMAND_DEFSTYLE styledeflist
              | designoptlist;

designoptlist: designopt
               | designoptlist TOK_COMMA designopt
               | designoptlist error
{
    //Skip to next comma or semicolon
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_SEMICOLON) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
};


designopt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
    msc.AddDesignAttribute(Attribute($1, string_to_bool($3), 
	                                 file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)), 
									 $3));
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
    msc.AddDesignAttribute(Attribute($1, atof($3), 
                           file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)), 
						   $3));
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
    msc.AddDesignAttribute(Attribute($1, $3, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$))));
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
    msc.AddDesignAttribute(Attribute("msc", $3, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$))));
    free($1);
    free($3);
};


parallel:    parallel_element
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new ArcParallel(l, &msc))->AddArcList($1);
}
	     | parallel parallel_element
{
    $$ = ($1)->AddArcList($2);
};

parallel_element: scope_open arclist TOK_SEMICOLON scope_close
{
	/*MscSetArcListLinenum($2, $1);*/
    $$ = $2;
};
	      | scope_open scope_close
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = (new ArcList)->Append(new ArcDivider(MSC_ARC_VSPACE, l, &msc));
};

emphasis_list: first_emphasis
{
    $$=$1;
}
           | TOK_COMMAND_PIPE pipe_emphasis
{
    $$=$2;
    free($1);
}
           | emphasis_list follow_emphasis
{
    $$ = static_cast<ArcEmphasis*>($1)->AddFollow(static_cast<ArcEmphasis*>($2));
};

/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
first_emphasis:   emphrel
{
    $$ = $1;
}
           | emphrel full_arcattrlist_with_label
{
    $$ = ($1)->AddAttributeList($2);
}
           | emphrel scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($3);
}
           | emphrel full_arcattrlist_with_label scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($4)->AddAttributeList($2);
};

/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
follow_emphasis:   emphrel
{
    $$ = ($1)->EmptyStyle();
}
           | emphrel full_arcattrlist_with_label
{
    $$ = ($1)->EmptyStyle()->AddAttributeList($2);
}
           | emphrel scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($3)->EmptyStyle();
}
           | emphrel full_arcattrlist_with_label scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($4)->EmptyStyle()->AddAttributeList($2);
};

/* ALWAYS Add Arclist before Attributes or setpipe.
** AddArcList changes default attributes to style "emphasis" */
pipe_emphasis:   emphrel
{
    $$ = ($1)->SetPipe();
}
           | emphrel full_arcattrlist_with_label
{
    $$ = ($1)->SetPipe()->AddAttributeList($2);
}
           | emphrel scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($3)->SetPipe();
}
           | emphrel full_arcattrlist_with_label scope_open arclist TOK_SEMICOLON scope_close
{
    $$ = ($1)->AddArcList($4)->SetPipe()->AddAttributeList($2);
};

emphrel:   entity_string TOK_EMPH entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcEmphasis($2, $1, $3, l, &msc);
    free($1);
    free($3);
}
           | TOK_EMPH entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcEmphasis($1, NULL, $2, l, &msc);
    free($2);
}
           | entity_string TOK_EMPH
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcEmphasis($2, $1, NULL, l, &msc);
    free($1);
}
           | TOK_EMPH
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcEmphasis($1, NULL, NULL, l, &msc);
};

vertxpos: TOK_AT entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new VertXPos(l, msc, $2);
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new VertXPos(l, msc, $2, VertXPos::POS_LEFT_SIDE);
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_PLUS
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new VertXPos(l, msc, $2, VertXPos::POS_RIGHT_SIDE);
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_EMPH
{
    $$ = NULL;
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    switch ($3) {
    case MSC_EMPH_SOLID:
        $$ = new VertXPos(l, msc, $2, VertXPos::POS_LEFT_BY);
        break;
    case MSC_EMPH_DASHED:
        $$ = new VertXPos(l, msc, $2, VertXPos::POS_RIGHT_BY);
        break;
    case MSC_EMPH_DOTTED:
        msc.Error.Error(file_line(msc.current_pos.file, YYMSC_GETLINENO(@3), YYMSC_GETCOLNO(@3)),
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
    case MSC_EMPH_DOUBLE:
        msc.Error.Error(file_line(msc.current_pos.file, YYMSC_GETLINENO(@3), YYMSC_GETCOLNO(@3)),
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
    }
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new VertXPos(l, msc, $2, VertXPos::POS_CENTER, $4);
    free($1);
    free($2);
    free($4);
};

empharcrel_straight: TOK_EMPH | relation_to | relation_bidir;

vertrel: entity_string empharcrel_straight entity_string vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($2, $1, $3, $4, l, &msc);
    free($1);
    free($3);
    delete $4;
}
       | empharcrel_straight entity_string vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, $2, $3, l, &msc);
    free($2);
    delete $3;
}
       | entity_string empharcrel_straight vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($2, $1, MARKER_HERE_STR, $3, l, &msc);
    free($1);
    delete $3;
}
       | empharcrel_straight vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, $2, l, &msc);
    delete $2;
}
       | entity_string relation_from entity_string vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($2, $3, $1, $4, l, &msc);
    free($1);
    free($3);
    delete $4;
}
       | relation_from entity_string vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($1, $2, MARKER_HERE_STR, $3, l, &msc);
    free($2);
    delete $3;
}
       | entity_string relation_from vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($2, MARKER_HERE_STR, $1, $3, l, &msc);
    free($1);
    delete $3;
}
       | relation_from vertxpos
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcVerticalArrow($1, MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, $2, l, &msc);
    delete $2;
};


arcrel:       TOK_SPECIAL_ARC
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = new ArcDivider($1, l, &msc);
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
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, $1, $3, l);
    free($1);
    free($3);
}
            | relation_to entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR , $2, l);
    free($2);
}
            | entity_string relation_to
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, $1, RSIDE_ENT_STR , l);
    free($1);
}
            | arcrel_to relation_to_cont entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@3), YYMSC_GETCOLNO(@3));
    $$ = ($1)->AddSegment($3, true, l);
    free($3);
}
            | arcrel_to relation_to_cont
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@2), YYMSC_GETCOLNO(@2));
    $$ = ($1)->AddSegment(NULL, true, l);
};


arcrel_from:    entity_string relation_from entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, $3, $1, l);
    free($1);
    free($3);
}
             | relation_from entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($1, $2, LSIDE_ENT_STR, l);
    free($2);
}
             | entity_string relation_from
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, RSIDE_ENT_STR, $1, l);
    free($1);
}
             | arcrel_from relation_from_cont entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@3), YYMSC_GETCOLNO(@3));
    $$ = ($1)->AddSegment($3, false, l);
    free($3);
}
             | arcrel_from relation_from_cont
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@2), YYMSC_GETCOLNO(@2));
    $$ = ($1)->AddSegment(NULL, false, l);
};

arcrel_bidir:    entity_string relation_bidir entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, $1, $3, l);
    free($1);
    free($3);
}
            | relation_bidir entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, $2, l);
    free($2);
}
            | entity_string relation_bidir
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$));
    $$ = msc.CreateArcArrow($2, $1, RSIDE_ENT_STR, l);
    free($1);
}
            | arcrel_bidir relation_bidir_cont entity_string
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@3), YYMSC_GETCOLNO(@3));
    $$ = ($1)->AddSegment($3, true, l);
    free($3);
}
            | arcrel_bidir relation_bidir_cont
{
	file_line l(msc.current_pos.file, YYMSC_GETLINENO(@2), YYMSC_GETCOLNO(@2));
    $$ = ($1)->AddSegment(NULL, true, l);
};

relation_to:   TOK_REL_SOLID_TO | TOK_REL_DOUBLE_TO | TOK_REL_DASHED_TO | TOK_REL_DOTTED_TO;
relation_from: TOK_REL_SOLID_FROM | TOK_REL_DOUBLE_FROM | TOK_REL_DASHED_FROM | TOK_REL_DOTTED_FROM;
relation_bidir: TOK_REL_SOLID_BIDIR | TOK_REL_DOUBLE_BIDIR | TOK_REL_DASHED_BIDIR | TOK_REL_DOTTED_BIDIR;

relation_to_cont : relation_to | TOK_DASH;
relation_from_cont : relation_from | TOK_DASH;
relation_bidir_cont : relation_bidir | TOK_DASH;



full_arcattrlist_with_label: TOK_COLON_STRING
{
    $$ = (new AttributeList)->Append(new Attribute("label", $1, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$))));
    free($1);
}
              | TOK_COLON_STRING full_arcattrlist
{
    $$ = ($2)->Append(new Attribute("label", $1, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$))));
    free($1);
}
              | full_arcattrlist TOK_COLON_STRING
{
    $$ = ($1)->Append(new Attribute("label", $2, file_line(msc.current_pos.file, YYMSC_GETLINENO(@2), YYMSC_GETCOLNO(@2))));
    free($2);
}
              | full_arcattrlist;


full_arcattrlist: TOK_OSBRACKET TOK_CSBRACKET
{
    $$ = new AttributeList;
}
                   | TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
    $$ = $2;
}

arcattrlist:    arcattr
{
    $$ = (new AttributeList)->Append($1);
}
              | arcattrlist TOK_COMMA arcattr
{
    $$ = ($1)->Append($3);
}
              | arcattrlist error
{
    $$ = $1;
    //Skip to next comma or closing bracket
    while (yychar > YYEOF && yychar != TOK_COMMA && yychar !=TOK_CSBRACKET) {
        yychar = YYLEX;
        yytoken = YYTRANSLATE (yychar);
    }
	msc.current_pos.line = YYMSC_GETLINENO(@2);
	msc.current_pos.col  = YYMSC_GETCOLNO(@2);
};

arcattr:         string TOK_EQUAL string
{
    $$ = new Attribute($1, $3, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)));
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_NUMBER
{
    $$ = new Attribute($1, atof($3), 
	                   file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)), 
					   $3);
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_BOOLEAN
{
    $$ = new Attribute($1, string_to_bool($3), 
	                   file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)), 
					   $3);
    free($1);
    free($3);
}
	    | string TOK_EQUAL
{
    $$ = new Attribute($1, (char*)NULL, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)));
    free($1);
}
	    | string
{
    $$ = new Attribute($1, file_line(msc.current_pos.file, YYMSC_GETLINENO(@$), YYMSC_GETCOLNO(@$)));
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
