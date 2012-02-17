%locations
%pure-parser
%lex-param {yyscan_t *yyscanner}
%parse-param{YYMSC_RESULT_TYPE &RESULT}
%parse-param{void *yyscanner}

%{
/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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

#ifdef C_S_H_IS_COMPILED

#define YYMSC_RESULT_TYPE Csh
#define RESULT csh

#include "csh.h"

//If we scan for color syntax highlight use this location
//yyerror is defined by bison, the other is defined for flex
#define YYLTYPE_IS_DECLARED
#define YYLTYPE CshPos
#define CHAR_IF_CSH(A) char

#include "colorsyntax.h"
#include "language_misc.h"
#include "colorsyntax2.h"
#include "arcs.h"//For AttrNames and AttrValues
#include "msc.h" //For AttrNames and AttrValues
//redefine default loc action for CSH
#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#define YYLLOC_DEFAULT(Current, Rhs, N)                       \
    do                                                        \
        if (YYID (N)) {                                       \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;  \
	        (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos;	  \
        } else {                                              \
	        (Current).first_pos = (Current).last_pos =        \
	            YYRHSLOC (Rhs, 0).last_pos;                   \
        }                                                     \
    while (YYID (0))

/* yyerror
 *  Error handling function.  Do nothing for CSH
 */
void yyerror(YYLTYPE* /*loc*/, Csh & /*csh*/, void * /*yyscanner*/, const char * /*str*/)
{}

#else

#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#include "msc.h"
#define YYMSC_RESULT_TYPE Msc
#define RESULT msc

#define CHAR_IF_CSH(A) A
#include "language.h"
#include "language_misc.h"
#include "language2.h"

/* Use verbose error reporting such that the expected token names are dumped */
#define YYERROR_VERBOSE

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

%}

%token TOK_STRING TOK_QSTRING TOK_NUMBER TOK_DASH TOK_EQUAL TOK_COMMA
       TOK_SEMICOLON TOK_PLUS
       TOK_OCBRACKET TOK_CCBRACKET TOK_OSBRACKET TOK_CSBRACKET TOK_MSC
       TOK_COLON_STRING TOK_COLON_QUOTED_STRING TOK_STYLE_NAME
       TOK_BOOLEAN
       TOK_REL_SOLID_TO    TOK_REL_SOLID_FROM	TOK_REL_SOLID_BIDIR
       TOK_REL_DOUBLE_TO   TOK_REL_DOUBLE_FROM	TOK_REL_DOUBLE_BIDIR
       TOK_REL_DASHED_TO   TOK_REL_DASHED_FROM	TOK_REL_DASHED_BIDIR
       TOK_REL_DOTTED_TO   TOK_REL_DOTTED_FROM	TOK_REL_DOTTED_BIDIR
       TOK_SPECIAL_ARC     TOK_EMPH
       TOK_COMMAND_HEADING TOK_COMMAND_NUDGE TOK_COMMAND_NEWPAGE
       TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
       TOK_COMMAND_BIG TOK_COMMAND_PIPE TOK_COMMAND_MARK TOK_COMMAND_PARALLEL
       TOK_VERTICAL TOK_AT TOK_AT_POS TOK_SHOW TOK_HIDE TOK_ACTIVATE TOK_DEACTIVATE TOK_BYE
       TOK_COMMAND_VSPACE TOK_COMMAND_HSPACE TOK_COMMAND_SYMBOL TOK_COMMAND_NOTE
       TOK__NEVER__HAPPENS
%union
{
    char                          *str;
    CHAR_IF_CSH(Msc)              *msc;
    CHAR_IF_CSH(ArcBase)          *arcbase;
    CHAR_IF_CSH(ArcList)          *arclist;
    CHAR_IF_CSH(ArcArrow)         *arcarrow;
    CHAR_IF_CSH(ArcVerticalArrow) *arcvertarrow;
    CHAR_IF_CSH(ArcBox)           *arcbox;
    CHAR_IF_CSH(ArcPipe)          *arcpipe;
    CHAR_IF_CSH(ArcBoxSeries)     *arcboxseries;
    CHAR_IF_CSH(ArcPipeSeries)    *arcpipeseries;
    CHAR_IF_CSH(ArcParallel)      *arcparallel;
    CHAR_IF_CSH(MscArcType)        arctype;
    CHAR_IF_CSH(EntityDef)        *entity;
    CHAR_IF_CSH(EntityDefList)    *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    CHAR_IF_CSH(ExtVertXPos)      *extvertxpos;
    CHAR_IF_CSH(NamePair)         *namerel;
    std::list<std::string>        *stringlist;
};

%type <msc>        msc
%type <arcbase>    arcrel arc arc_with_parallel arc_with_parallel_semicolon opt vertrel scope_close
                   symbol_command symbol_command_no_attr note
%type <arcvertarrow> vertrel_no_xpos
%type <arcarrow>   arcrel_to arcrel_from arcrel_bidir
%type <arcbox>     boxrel first_box
%type <arcpipe>    first_pipe
%type <arcboxseries> box_list
%type <arcpipeseries> pipe_list_no_content pipe_list
%type <arcparallel> parallel
%type <arclist>    top_level_arclist arclist arclist_maybe_no_semicolon braced_arclist optlist
%type <entitylist> entitylist entity first_entity
%type <arctype>    relation_to relation_from relation_bidir empharcrel_straight
                   relation_to_cont relation_from_cont relation_bidir_cont
                   TOK_REL_SOLID_TO TOK_REL_DOUBLE_TO TOK_REL_DASHED_TO TOK_REL_DOTTED_TO
                   TOK_REL_SOLID_FROM TOK_REL_DOUBLE_FROM TOK_REL_DASHED_FROM
                   TOK_REL_DOTTED_FROM
                   TOK_REL_SOLID_BIDIR TOK_REL_DOUBLE_BIDIR TOK_REL_DASHED_BIDIR
                   TOK_REL_DOTTED_BIDIR
                   TOK_SPECIAL_ARC TOK_EMPH
%type <attrib>     arcattr
%type <vertxpos>   vertxpos
%type <extvertxpos> extvertxpos extvertxpos_no_string
%type <namerel>    entityrel markerrel_no_string
%type <attriblist> arcattrlist full_arcattrlist full_arcattrlist_with_label
                   full_arcattrlist_with_label_or_number
%type <str>        entity_command_prefixes
                   entity_string reserved_word_string string symbol_string colon_string symbol_type_string
                   TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_COLON_QUOTED_STRING
                   TOK_STYLE_NAME TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE
                   TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
                   TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
                   TOK_COMMAND_PARALLEL TOK_COMMAND_MARK TOK_BYE
                   TOK_NUMBER TOK_BOOLEAN TOK_VERTICAL TOK_AT TOK_AT_POS TOK_SHOW TOK_HIDE
                   TOK_ACTIVATE TOK_DEACTIVATE
                   TOK_COMMAND_VSPACE TOK_COMMAND_HSPACE TOK_COMMAND_SYMBOL TOK_COMMAND_NOTE
%type <stringlist> tok_stringlist

%destructor {if (!C_S_H) delete $$;} vertxpos
%destructor {if (!C_S_H) delete $$;} vertrel_no_xpos
%destructor {if (!C_S_H) delete $$;} arcrel arc arc_with_parallel arc_with_parallel_semicolon opt vertrel scope_close
%destructor {if (!C_S_H) delete $$;} symbol_command symbol_command_no_attr note
%destructor {if (!C_S_H) delete $$;} arcrel_to arcrel_from arcrel_bidir
%destructor {if (!C_S_H) delete $$;} boxrel first_box box_list first_pipe pipe_list pipe_list_no_content
%destructor {if (!C_S_H) delete $$;} parallel
%destructor {if (!C_S_H) delete $$;} top_level_arclist arclist arclist_maybe_no_semicolon braced_arclist optlist
%destructor {if (!C_S_H) delete $$;} entity first_entity entitylist
%destructor {if (!C_S_H) delete $$;} arcattr arcattrlist full_arcattrlist full_arcattrlist_with_label tok_stringlist
%destructor {if (!C_S_H) delete $$;} full_arcattrlist_with_label_or_number
%destructor {free($$);}  entity_string reserved_word_string string symbol_string colon_string symbol_type_string
%destructor {free($$);}  TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_COLON_QUOTED_STRING TOK_STYLE_NAME
%destructor {free($$);}  TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE
%destructor {free($$);}  TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
%destructor {free($$);}  TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
%destructor {free($$);}  TOK_COMMAND_PARALLEL TOK_COMMAND_MARK TOK_BYE
%destructor {free($$);}  TOK_NUMBER TOK_BOOLEAN TOK_VERTICAL TOK_AT TOK_AT_POS TOK_SHOW TOK_HIDE TOK_ACTIVATE TOK_DEACTIVATE
%destructor {free($$);}  TOK_COMMAND_VSPACE TOK_COMMAND_SYMBOL TOK_COMMAND_NOTE

%%

msc_with_bye: msc
              | msc TOK_BYE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_KEYWORD);
  #else
  #endif
    free($2);
    YYACCEPT; //ignore anything after bye
}

msc:
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
              | TOK_MSC braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    msc.AddArcs($2);
  #endif
    free($1);

    YYACCEPT;
}
              | msckey braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_VALUE, @1))
        csh.AddDesignsToHints();
  #else
    msc.AddArcs($2);
  #endif
    YYACCEPT;
}
           | TOK_MSC
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter(@1, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free($1);
}
           | TOK_MSC error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_Error(@2, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS(@2).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free($1);
}
           | top_level_arclist
{
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.AddArcs($1);
  #endif
}
                 | top_level_arclist error
{
  #ifdef C_S_H_IS_COMPILED
    if ((@1).last_pos >= (@2).first_pos)
        (@2).first_pos = (@1).last_pos;
    csh.AddCSH_Error(@2, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs($1);
    msc.Error.Error(MSC_POS(@2).start, "Could not recognize this as a valid line.");
  #endif
  YYACCEPT;
};

top_level_arclist: arclist_maybe_no_semicolon
                 | arclist_maybe_no_semicolon TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "Closing brace missing its opening pair.");
  #else
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).start, "Unexpected '}'.");
  #endif
}
                 | arclist_maybe_no_semicolon TOK_CCBRACKET top_level_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "Closing brace missing its opening pair.");
  #else
    //Merge $3 into $1
    ($1)->splice(($1)->end(), *($3));
    delete ($3);
    $$ = $1;
    msc.Error.Error(MSC_POS(@3).start, "Unexpected '}'.");
  #endif
};


msckey:       TOK_MSC TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_ErrorAfter(@2, "Missing a design name.");
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing design name.");
  #endif
    free($1);
}
              | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, "msc");
    csh.SetDesignTo($1);
  #else
    msc.AddAttribute(Attribute("msc", $3, MSC_POS(@1), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
};

braced_arclist: scope_open arclist_maybe_no_semicolon scope_close
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.AddCSH(@3, COLOR_BRACE);
  #else
    if ($3) {
        ($2)->Append($3); //Append any potential CommandNumbering
        ($3)->MakeMeLastNotable();
    }
    $$ = $2;
  #endif
}
            | scope_open scope_close
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.AddCSH(@2, COLOR_BRACE);
  #else
    $$ = new ArcList;
    //scope_close should not return here with a CommandNumbering
    //but just in case
    if ($2)
        delete($2);
  #endif
}
            | scope_open arclist_maybe_no_semicolon error scope_close
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.AddCSH_Error(@3, "Could not recognize this as a valid line.");
    csh.AddCSH(@4, COLOR_BRACE);
  #else
    if ($4) {
        ($2)->Append($4); //Append any potential CommandNumbering
        ($4)->MakeMeLastNotable();
    }
    $$ = $2;
    msc.Error.Error(MSC_POS(@3).start, "Syntax error.");
  #endif
    yyerrok;
}
            | scope_open arclist_maybe_no_semicolon error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.AddCSH_Error(@3, "Could not recognize this as a valid line.");
  #else
    $$ = $2;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@3).start, "Missing '}'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@3).start, "Here is the corresponding '{'.");
  #endif
}
            | scope_open arclist_maybe_no_semicolon
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.AddCSH_ErrorAfter(@2, "Missing a closing brace ('}').");
  #else
    $$ = $2;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@2).end.NextChar(), "Here is the corresponding '{'.");
  #endif
};

arclist_maybe_no_semicolon : arclist
            | arclist arc_with_parallel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@2, "Missing a semicolon (';').");
  #else
    if ($2) {
        ($1)->Append($2);
        ($2)->MakeMeLastNotable();
    }
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS(@2).start, MSC_POS(@2).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
            | arc_with_parallel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@1, "Missing a semicolon (';').");
  #else
    if ($1) {
        $$ = (new ArcList)->Append($1); /* New list */
        ($1)->MakeMeLastNotable();
    } else
        $$ = new ArcList;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing ';'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@1).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
};


arclist:    arc_with_parallel_semicolon
{
  #ifndef C_S_H_IS_COMPILED
    if ($1) {
        $$ = (new ArcList)->Append($1); /* New list */
        ($1)->MakeMeLastNotable();
    } else
        $$ = new ArcList;
  #endif
}
            | arclist arc_with_parallel_semicolon
{
  #ifndef C_S_H_IS_COMPILED
    if ($2) {
        ($1)->Append($2);     /* Add to existing list */
        ($2)->MakeMeLastNotable();
    }
    $$ = ($1);
  #endif
}

arc_with_parallel_semicolon: arc_with_parallel TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SEMICOLON);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    $$=$1;
  #endif
}
/* Dummy rule to force a lookahead, so that previous rule has a valid
 * lookahead location in yylloc */
              | arc_with_parallel TOK_SEMICOLON TOK__NEVER__HAPPENS
              | arc_with_parallel error TOK_SEMICOLON TOK__NEVER__HAPPENS
              | TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SEMICOLON);
    if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    $$=NULL;
  #endif
}
              | arc_with_parallel error TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@3, COLOR_SEMICOLON);
    csh.AddCSH_Error(@2, "I am not sure what is coming here.");
    if (csh.CheckHintAfter(@3, yylloc, yychar==YYEOF, HINT_LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    $$=$1;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Expecting a keyword, entity name, a valid arrow, box or divider symbol or an opening brace.");
  #endif
}

arc_with_parallel: arc
{
  #ifdef C_S_H_IS_COMPILED
  #else
        if ($1) ($1)->SetLineEnd(MSC_POS(@$));
        $$=$1;
  #endif
}
              | TOK_COMMAND_PARALLEL arc
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, HINT_LINE_START)) {
	    csh.AddLineBeginToHints(false);
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintBetween(@1, @2, HINT_LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintType = HINT_LINE_START;
        csh.hintsForcedOnly = true;
    }
  #else
    if ($2) {
        ($2)->SetLineEnd(MSC_POS(@$));
        ($2)->SetParallel();
    }
    $$ = $2;
  #endif
    free($1);
}

arc:           arcrel
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$=($1)->AddAttributeList(NULL);
  #endif
}
              | arcrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = ($1)->AddAttributeList($2);
  #endif
}
              | TOK_COMMAND_BIG
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@1, "Missing an arrow specification.");
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing an arrow specification.");
  #endif
    free($1);
    $$ = NULL;
}
              |TOK_COMMAND_BIG arcrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBase *arc = msc.CreateArcBigArrow($2);
    if (arc)
        arc->AddAttributeList(NULL);
    delete $2;
    $$ = arc;
  #endif
    free($1);
}
              | TOK_COMMAND_BIG arcrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @3))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @3))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    //Returns NULL, if BIG is before a self-pointing arrow
    ArcBigArrow *arrow = msc.CreateArcBigArrow($2);
    if (arrow) arrow->AddAttributeList($3);
    $$ = arrow;
    delete $2;
  #endif
    free($1);
}
              | TOK_VERTICAL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, HINT_MARKER);
    csh.AddCSH_ErrorAfter(@1, "Missing a marker.");
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a marker.");
  #endif
    free($1);
    $$ = NULL;
}
              |TOK_VERTICAL vertrel
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    if ($2) $$ = ($2)->AddAttributeList(NULL);
    else $$ = NULL;
  #endif
    free($1);
}
              | TOK_VERTICAL vertrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @3))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @3))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($2) $$ = ($2)->AddAttributeList($3);
    else $$ = NULL;
  #endif
    free($1);
}
              | full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @1))
        ArcDivider::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @1))
        ArcDivider::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList($1);
  #endif
}
              | first_entity
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = (new CommandEntity($1, &msc))->AddAttributeList(NULL);
  #endif
}
              | entity_command_prefixes
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@1, "Missing an entity.");
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing an entity.");
  #endif
    free($1);
    $$ = NULL;
}
              | entity_command_prefixes first_entity
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
  #else
    CommandEntity *ce = new CommandEntity($2, &msc);
    ce->AddAttributeList(NULL);
	$$ = ce->ApplyPrefix($1);
  #endif
    free($1);
}
            | first_entity TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@2, "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity($1, &msc);
    $$ = ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing an entity.");
  #endif
}
            | first_entity TOK_COMMA entitylist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore(@2, @3);
  #else
    CommandEntity *ce = new CommandEntity(($3)->Prepend($1), &msc);
	delete ($1);
    $$ = ce->AddAttributeList(NULL);
  #endif
}
            | entity_command_prefixes first_entity TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@3, COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@3, "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity($2, &msc);
    ce->AddAttributeList(NULL);
	$$ = ce->ApplyPrefix($1);
    msc.Error.Error(MSC_POS(@3).end.NextChar(), "Missing an entity.");
  #endif
    free($1);
}
            | entity_command_prefixes first_entity TOK_COMMA entitylist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@3, COLOR_COMMA);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.CheckEntityHintAtAndBefore(@3, @4);
  #else
    CommandEntity *ce = new CommandEntity(($4)->Prepend($2), &msc);
	delete ($2);
    ce->AddAttributeList(NULL);
    $$ = ce->ApplyPrefix($1);
  #endif
    free($1);
}
              | optlist
{
  #ifdef C_S_H_IS_COMPILED
  #else
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ($1) {
        $$ = (new ArcParallel(&msc))->AddArcList($1)->AddAttributeList(NULL);
    } else
        $$ = NULL;
  #endif
}
              | box_list
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = $1; //to remove warning for downcast
  #endif
}
              | pipe_list
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = $1; //to remove warning for downcast
  #endif
}
              | parallel
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = ($1)->AddAttributeList(NULL);
  #endif
}
              | TOK_COMMAND_DEFCOLOR colordeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFDESIGN designdef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_HEADING
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    $$ = (new CommandEntity(NULL, &msc))->AddAttributeList(NULL);
  #endif
    free($1);
}
              | TOK_COMMAND_HEADING full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandEntity(NULL, &msc))->AddAttributeList($2);
  #endif
    free($1);
}
	      | TOK_COMMAND_NUDGE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    $$ = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList(NULL);
  #endif
    free($1);
}
	      | TOK_COMMAND_NUDGE full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcDivider::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true);
  #else
    $$ = (new ArcDivider(MSC_COMMAND_NUDGE, &msc))->AddAttributeList($2);
  #endif
    free($1);
}
              | TOK_COMMAND_MARK string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.MarkerNames.insert($2);
  #else
    $$ = (new CommandMark($2, MSC_POS(@$), &msc))->AddAttributeList(NULL);
  #endif
    free($1);
    free($2);
}
              | TOK_COMMAND_MARK string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.MarkerNames.insert($2);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
        $$ = (new CommandMark($2, MSC_POS(@$), &msc))->AddAttributeList($3);
  #endif
    free($1);
    free($2);
}
              | TOK_COMMAND_NEWPAGE
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        $$ = (new CommandNewpage(&msc))->AddAttributeList(NULL);
  #endif
    free($1);
}
              | TOK_COMMAND_NEWPAGE full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandNewpage(&msc))->AddAttributeList($2);
  #endif
    free($1);
}
              | symbol_command
              | note
              | TOK_COMMAND_HSPACE entityrel full_arcattrlist_with_label_or_number
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandHSpace(&msc, $2))->AddAttributeList($3);
  #endif
    free($1);
}
              | TOK_COMMAND_HSPACE entityrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing either a number or a label.");
  #else
    $$ = new CommandHSpace(&msc, $2); //Will trigger an error: either label or space attr is needed
  #endif
    free($1);
}
              | TOK_COMMAND_HSPACE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@1, "Missing an entity.");
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_VSPACE full_arcattrlist_with_label_or_number
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandVSpace(&msc))->AddAttributeList($2);
  #endif
    free($1);
}
              | TOK_COMMAND_VSPACE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", HINT_KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", HINT_KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing either a number or a label.");
  #else
    $$ = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free($1);
};

full_arcattrlist_with_label_or_number: full_arcattrlist_with_label
            | TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("space", atof($1), MSC_POS(@1), MSC_POS(@1), $1));
    $$ = al;
  #endif
    free($1);
}
            | TOK_NUMBER full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
  #else
    ($2)->Append(new Attribute("space", atof($1), MSC_POS(@1), MSC_POS(@1), $1));
    $$ = $2;
  #endif
    free($1);
};

entityrel: entity_string TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName(@1, $1);
    csh.CheckEntityHintAt(@1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = new NamePair($1, MSC_POS(@1), NULL, file_line_range());
  #endif
    free($1);
}
           | TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@2, $2);
    csh.CheckEntityHintAt(@2);
  #else
    $$ = new NamePair(NULL, file_line_range(), $2, MSC_POS(@2));
  #endif
    free($2);
}
           | TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    $$ = NULL;
  #endif
}
           | entity_string TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName(@1, $1);
    csh.CheckEntityHintAt(@1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@3, $3);
    csh.CheckEntityHintAt(@3);
  #else
    $$ = new NamePair($1, MSC_POS(@1), $3, MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
           | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName(@1, $1);
    csh.CheckEntityHintAt(@1);
  #else
    $$ = new NamePair($1, MSC_POS(@1), NULL, file_line_range());
  #endif
    free($1);
};

//markerrel: markerrel_no_string
//        | entity_string
//{
//  #ifdef C_S_H_IS_COMPILED
//    csh.AddCSH(@1, COLOR_MARKERNAME);
//    csh.CheckHintAt(@1, HINT_MARKER);
//  #else
//    $$ = new NamePair($1, MSC_POS(@1), NULL, file_line_range());
//  #endif
//    free($1);
//};



markerrel_no_string: entity_string TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = new NamePair($1, MSC_POS(@1), NULL, file_line_range());
  #endif
    free($1);
}
           | TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.CheckHintAt(@2, HINT_MARKER);
  #else
    $$ = new NamePair(NULL, file_line_range(), $2, MSC_POS(@2));
  #endif
    free($2);
}
           | TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = NULL;
  #endif
}
           | entity_string TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH(@3, COLOR_MARKERNAME);
    csh.CheckHintAt(@3, HINT_MARKER);
  #else
    $$ = new NamePair($1, MSC_POS(@1), $3, MSC_POS(@3));
  #endif
    free($1);
    free($3);
};

entity_command_prefixes: TOK_HIDE | TOK_SHOW | TOK_ACTIVATE | TOK_DEACTIVATE;

optlist:     opt
{
  #ifndef C_S_H_IS_COMPILED
    if ($1) {
        $$ = (new ArcList)->Append($1); /* New list */
        ($1)->MakeMeLastNotable();
    } else
        $$ = NULL;
  #endif
}
           | optlist TOK_COMMA opt
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintBetween(@2, @3, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    if ($3) {
        if ($1)
            $$ = ($1)->Append($3);     /* Add to existing list */
        else
            $$ = (new ArcList)->Append($3); /* New list */
        ($3)->MakeMeLastNotable();
    } else
        $$ = $1;
  #endif
}
           | optlist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Expecting an option here.");
  #endif
}
           | optlist TOK_COMMA TOK__NEVER__HAPPENS
           | optlist TOK_COMMA error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.AddCSH_Error(@3, "An option expected here.");
  #else
    $$ = $1;
  #endif
}


opt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddAttribute(Attribute($1, str2bool($3), MSC_POS(@1), MSC_POS(@3), $3));
    $$ = NULL;
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddAttribute(Attribute($1, atof($3), MSC_POS(@1), MSC_POS(@3), $3));
    $$ = NULL;
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_AttrValue(@3, $3, $1);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    Attribute a($1, $3, MSC_POS(@1), MSC_POS(@3));
    MscFillAttr fill;
    fill.Empty();
    if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
        $$ = (new CommandNewBackground(&msc, fill))->AddAttributeList(NULL);
    } else {
        msc.AddAttribute(a);
        $$ = NULL;
    }
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_ErrorAfter(@2, "Missing option value.");
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing option value.");
    $$ = NULL;
  #endif
    free($1);
}
            | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_DESIGNNAME);
        if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints();
            csh.hintStatus = HINT_READY;
        }
        csh.SetDesignTo($3);
  #else
        msc.AddAttribute(Attribute("msc", $3, MSC_POS(@$), MSC_POS(@3)));
        $$ = NULL;
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_ErrorAfter(@2, "Missing option value.");
        if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_VALUE, "msc")) {
            csh.AddDesignsToHints();
            csh.hintStatus = HINT_READY;
        }
        if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
            csh.AddOptionsToHints();
            csh.hintStatus = HINT_READY;
        }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing option value.");
    $$ = NULL;
  #endif
    free($1);
};

entitylist:   entity
{
  #ifndef C_S_H_IS_COMPILED
    $$ = (EntityDefList*)($1);
  #endif
}
            | entitylist TOK_COMMA entity
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintAtAndBefore(@2, @3);
  #else
    $$ = (EntityDefList*)(($1)->Append($3));
	delete ($3);
  #endif
}
            | entitylist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = (EntityDefList*)($1);
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Expecting an entity here.");
  #endif
}
/* Dummy rule to force a lookahead, so that previous rule has a valid
 * lookahead location in yylloc */
              | entitylist TOK_COMMA TOK__NEVER__HAPPENS;


entity:       entity_string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, NULL, file_line());
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, NULL, file_line());
  #endif
    free($1);
}
             | entity_string full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, $3, MSC_POS(@3).start);
  #endif
    free($1);
}
            | entity_string braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, $2, MSC_POS(@2).start);
  #endif
    free($1);
};

first_entity:  entity_string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity(@1, $1);  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, NULL, file_line());
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt(@1, HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity(@1, $1);   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, NULL, file_line());
  #endif
    free($1);
}
                   | entity_string full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	} else if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        EntityDef::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        EntityDef::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity(@1, $1);  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, $3, MSC_POS(@3).start);
  #endif
    free($1);
}
            | entity_string braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
  	if (csh.CheckHintAt(@1, HINT_LINE_START)) {
	    csh.AddLineBeginToHints();
	    csh.hintStatus = HINT_READY;
	}
    csh.AddCSH_KeywordOrEntity(@1, $1);   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityDef *ed = new EntityDef($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, $2, MSC_POS(@2).start);
  #endif
    free($1);
};

styledeflist: styledef
             | styledeflist TOK_COMMA styledef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #endif
};

styledef : tok_stringlist full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    for (std::list<std::string>::iterator i = ($1)->begin(); i!=($1)->end(); i++)
        if (csh.ForbiddenStyles.find(*i) != csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(string(*i));
	if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (std::list<std::string>::iterator i = ($1)->begin(); i!=($1)->end(); i++) {
        MscStyle style = msc.Contexts.back().styles.GetStyle(*i);
        AttributeList::iterator j=($2)->begin();
        while (j!=($2)->end()) {
           if (!style.AddAttribute(**j, &msc))
               msc.Error.Error(**j, false, "Attribute '" + (*j)->name + "' is not applicable to styles. Ignoring it.");
           j++;
        }
        msc.Contexts.back().styles[*i] = style;
    }
    delete($1);
    delete($2);
  #endif
};

tok_stringlist : string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_STYLENAME);
        $$ = new std::list<string>;
        if (strcmp($1, "emphasis")==0)
            ($$)->push_back("box");
        else if (strcmp($1, "emptyemphasis")==0)
            ($$)->push_back("emptybox");
        else ($$)->push_back($1);
  #else
        $$ = new std::list<string>;
        ($$)->push_back(ConvertEmphasisToBox($1, &@1, msc));
  #endif
    free($1);
}
             | tok_stringlist TOK_COMMA string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_COMMA);
        csh.AddCSH(@3, COLOR_STYLENAME);
        $$ = $1;
        if (strcmp($3, "emphasis")==0)
            ($$)->push_back("box");
        else if (strcmp($3, "emptyemphasis")==0)
            ($$)->push_back("emptybox");
        else ($$)->push_back($3);
  #else
        ($1)->push_back(ConvertEmphasisToBox($3, &@3, msc));
        $$ = $1;
  #endif
    free($3);
};

colordeflist: colordef
             | colordeflist TOK_COMMA colordef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #endif
};

colordef : TOK_STRING TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_COLORNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_COLORDEF);
    MscColorType color = csh.Contexts.back().Colors.GetColor($3);
    if (color.valid)
        csh.Contexts.back().Colors[$1] = color;
  #else
    msc.Contexts.back().colors.AddColor($1, $3, msc.Error, MSC_POS(@$));
  #endif
    free($1);
    free($3);
};

designdef : TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_DESIGNNAME);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH(@4, COLOR_SEMICOLON);
    csh.AddCSH(@5, COLOR_BRACE);
    csh.Designs[$1] = csh.Contexts.back();
    csh.PopContext();
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    static_cast<Context&>(design) = msc.Contexts.back();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
    free($1);
}
           |TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON error TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_DESIGNNAME);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH(@4, COLOR_SEMICOLON);
    csh.AddCSH_Error(@5, "Could not recognize this as part of a design definition.");
    csh.AddCSH(@6, COLOR_BRACE);
    csh.Designs[$1] = csh.Contexts.back();
    csh.PopContext();
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    static_cast<Context&>(design) = msc.Contexts.back();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
};


scope_open_empty: TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
    msc.saved_hscale = msc.hscale;
  #endif
};

designelementlist: designelement
                   | designelementlist TOK_SEMICOLON designelement
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SEMICOLON);
  #endif
};

designelement: TOK_COMMAND_DEFCOLOR colordeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #endif
    free($1);
}
              | designoptlist;

designoptlist: designopt
               | designoptlist TOK_COMMA designopt
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #endif
}
               | designoptlist error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "Extra stuff after design options. Maybe missing a comma?");
  #endif
};

designopt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute($1, str2bool($3), MSC_POS(@$), MSC_POS(@3), $3));
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute($1, atof($3), MSC_POS(@$), MSC_POS(@3), $3));
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute($1, $3, MSC_POS(@1), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    if (csh.CheckHintAt(@1, HINT_ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1)) {
        Msc::AttributeValues("msc", csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute("msc", $3, MSC_POS(@$), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
};


parallel:    braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        $$ = (new ArcParallel(&msc))->AddArcList($1);
    else
        $$ = NULL;
  #endif
}
	     | parallel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    if ($2==NULL)
        $$ = $1;
    else if ($1)
        $$ = ($1)->AddArcList($2);
    else
        $$ = (new ArcParallel(&msc))->AddArcList($2);
  #endif
};

box_list: first_box
{
  #ifndef C_S_H_IS_COMPILED
    $$ = new ArcBoxSeries($1);
  #endif
}
/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
           | box_list boxrel
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow($2);
    ($2)->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
           | box_list boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @3))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @3))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($2)->SetLineEnd(MSC_POS2(@2, @3));
    $$ = ($1)->AddFollow($2);
    ($2)->AddAttributeList($3); //should come after AddFollow

  #endif
}
           | box_list boxrel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->AddArcList($3)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow($2);
    ($2)->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
           | box_list braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ArcBox *temp = new ArcBox(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
    temp->AddArcList($2);
    $$ = ($1)->AddFollow(temp);
    temp->AddAttributeList(NULL); //should come after AddFollow
  #endif
}
           | box_list boxrel full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($2)->AddArcList($4)->SetLineEnd(MSC_POS2(@2, @3));
    $$ = ($1)->AddFollow($2);
    ($2)->AddAttributeList($3); //should come after AddFollow
  #endif
}
           | box_list full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcBox *temp = new ArcBox(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
    temp->AddArcList($3)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow(temp);
    temp->AddAttributeList($2); //should come after AddFollow
  #endif
};

/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
first_box:   boxrel
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->AddAttributeList(NULL);
    ($1)->SetLineEnd(MSC_POS(@$));
    $$ = $1;
  #endif
}
           | boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($1)->AddAttributeList($2);
    ($1)->SetLineEnd(MSC_POS(@$));
    $$ = ($1);
  #endif
}
           | boxrel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->AddAttributeList(NULL);
    ($1)->SetLineEnd(MSC_POS(@1));
    $$ = ($1)->AddArcList($2);
  #endif
}
           | boxrel full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($1)->SetLineEnd(MSC_POS2(@1, @2));
    ($1)->AddArcList($3)->AddAttributeList($2);
    $$ = ($1);
  #endif
};


first_pipe: TOK_COMMAND_PIPE boxrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne(@1, @2, HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
  #else
    $$ = new ArcPipe($2);
    ($$)->AddAttributeList(NULL)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
}
             | TOK_COMMAND_PIPE error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.CheckHintAtAndBeforePlusOne(@1, @2, HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    }
    csh.AddCSH_ErrorAfter(@2, "Missing a box symbol.");
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing a box symbol. Ignoring pipe.");
    $$ = NULL;
  #endif
    free($1);
}
             | TOK_COMMAND_PIPE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "The keyword '" + string($1) +"' should be followed by an entity, or '--', '..', '++' or '=='.");
  #endif
    free($1);
}
             | TOK_COMMAND_PIPE boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintBetweenPlusOne(@1, @2, HINT_ENTITY)) {
        csh.hintStatus = HINT_READY;
        csh.AddEntitiesToHints();
    } else if (csh.CheckHintLocated(HINT_ATTR_NAME, @3))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @3))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = new ArcPipe($2);
    ($$)->AddAttributeList($3)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
};

pipe_list_no_content: first_pipe
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = new ArcPipeSeries($1);
  #endif
}
             | pipe_list_no_content boxrel
{
  #ifndef C_S_H_IS_COMPILED
    ArcPipe *ap = new ArcPipe($2);
    ap->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollowWithAttributes(ap, NULL);
  #endif
}
             | pipe_list_no_content boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @3))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @3))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    ArcPipe *ap = new ArcPipe($2);
    ap->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollowWithAttributes(ap, $3);
  #endif
};

pipe_list: pipe_list_no_content
         | pipe_list_no_content braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    $$ = ($1)->AddArcList($2);
  #endif
};

boxrel:   entity_string TOK_EMPH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = new ArcBox($2, $1, MSC_POS(@1), $3, MSC_POS(@3), &msc);
  #endif
    free($1);
    free($3);
}
           | TOK_EMPH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
  #else
    $$ = new ArcBox($1, NULL, MSC_POS(@1), $2, MSC_POS(@2), &msc);
  #endif
    free($2);
}
           | entity_string TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = new ArcBox($2, $1, MSC_POS(@1), NULL, MSC_POS(@2), &msc);
  #endif
    free($1);
}
           | TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    $$ = new ArcBox($1, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
  #endif
};

vertxpos: TOK_AT entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2));
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_AT, atof($3));
  #endif
    free($1);
    free($2);
    free($3);
}
         | TOK_AT entity_string TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_SIDE);
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH(@4, COLOR_ATTRVALUE);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_SIDE, -atof($4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_PLUS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_SIDE);
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_PLUS TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH(@4, COLOR_ATTRVALUE);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_SIDE, atof($4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
  #else
    switch ($3) {
    default:
        _ASSERT(0);
    case MSC_EMPH_SOLID:
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_BY);
        break;
    case MSC_EMPH_DASHED:
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_BY);
        break;
    case MSC_EMPH_DOTTED:
        msc.Error.Error(MSC_POS(@3).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        $$ = NULL;
        break;
    case MSC_EMPH_DOUBLE:
        msc.Error.Error(MSC_POS(@3).start,
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); break;
        $$ = NULL;
        break;
    }
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@3, @4);
    csh.AddCSH_EntityName(@4, $4);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4), VertXPos::POS_CENTER);
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_DASH entity_string TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@3, @4);
    csh.AddCSH_EntityName(@4, $4);
    csh.AddCSH(@5, COLOR_ATTRVALUE);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4), VertXPos::POS_CENTER, atof($5));
  #endif
    free($1);
    free($2);
    free($4);
    free($5);
}
         | TOK_AT
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@1, "Missing an entity name.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing an entity name.");
  #endif
    free($1);
}
         | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveBeginsWith("at", $1) && csh.cursor_pos == (@1).last_pos)
        csh.AddCSH(@1, COLOR_KEYWORD_PARTIAL);
    if (csh.CheckHintAt(@1, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
};


empharcrel_straight: TOK_EMPH | relation_to | relation_bidir;

vertrel_no_xpos: entity_string empharcrel_straight entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH(@3, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAtAndBefore(@2, @3, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($2, $1, $3, &msc);
  #endif
    free($1);
    free($3);
}
       | empharcrel_straight entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore(@1, @2, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, $2, &msc);
  #endif
    free($2);
}
       | entity_string empharcrel_straight
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($2, $1, MARKER_HERE_STR, &msc);
  #endif
    free($1);
}
       | empharcrel_straight
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, &msc);
  #endif
}
       | entity_string relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH(@3, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAtAndBefore(@2, @3, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($2, $3, $1, &msc);
  #endif
    free($1);
    free($3);
}
       | relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore(@1, @2, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($1, $2, MARKER_HERE_STR, &msc);
  #endif
    free($2);
}
       | entity_string relation_from
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($2, MARKER_HERE_STR, $1, &msc);
  #endif
    free($1);
}
       | relation_from
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_MARKER);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, &msc);
  #endif
}
       | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, HINT_MARKER);
    csh.AddCSH_ErrorAfter(@1, "Missing a box or arrow symbol.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free($1);
}
       /* These are to ensure we do have a valid lookahead above */
       | empharcrel_straight entity_string TOK__NEVER__HAPPENS {free($2); $$=NULL;}
       | entity_string empharcrel_straight entity_string TOK__NEVER__HAPPENS {free($1); free($3); $$=NULL;}
       | relation_from entity_string TOK__NEVER__HAPPENS {free($2); $$=NULL;}
       | entity_string relation_from entity_string TOK__NEVER__HAPPENS {free($1); free($3); $$=NULL;}


vertrel: vertrel_no_xpos vertxpos
{
  #ifdef C_S_H_IS_COMPILED
  #else
    if ($1)
        $$ = ($1)->AddXpos($2);
    else
        $$ = NULL;
    delete $2;
  #endif
}
         | vertrel_no_xpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@1, "Missing the 'at' keyword.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing the 'at' keyword.");
    if ($1) delete $1;
  #endif
}

arcrel:       TOK_SPECIAL_ARC
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
  #else
    $$ = new ArcDivider($1, &msc);
  #endif
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
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), $3, true, MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
            | relation_to entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
  #else
    $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, MSC_POS(@1), $2, true, MSC_POS(@2));
  #endif
    free($2);
}
            | entity_string relation_to
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, true, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_to relation_to_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAt(@3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = ($1)->AddSegment($2, $3, MSC_POS(@3), MSC_POS2(@2, @3));
  #endif
    free($3);
}
            | arcrel_to relation_to_cont
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    $$ = ($1)->AddSegment($2, NULL, MSC_POS(@2), MSC_POS(@2));
  #endif
};


arcrel_from:    entity_string relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = msc.CreateArcArrow($2, $3, MSC_POS(@3), $1, false, MSC_POS(@1));
  #endif
    free($1);
    free($3);
}
             | relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
  #else
    $$ = msc.CreateArcArrow($1, $2, MSC_POS(@2), LSIDE_ENT_STR, false, MSC_POS(@1));
  #endif
    free($2);
}
             | entity_string relation_from
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = msc.CreateArcArrow($2, RSIDE_ENT_STR, MSC_POS(@2), $1, false, MSC_POS(@1));
  #endif
    free($1);
}
             | arcrel_from relation_from_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = ($1)->AddSegment($2, $3, MSC_POS(@3), MSC_POS2(@2, @3));
  #endif
    free($3);
}
             | arcrel_from relation_from_cont
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = ($1)->AddSegment($2, NULL, MSC_POS(@2), MSC_POS(@2));
  #endif
};

arcrel_bidir:    entity_string relation_bidir entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), $3, true, MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
            | relation_bidir entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@1, @2);
    csh.AddCSH_EntityName(@2, $2);
  #else
    $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, MSC_POS(@1), $2, true, MSC_POS(@2));
  #endif
    free($2);
}
            | entity_string relation_bidir
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==EOF);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, true, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_bidir relation_bidir_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
  #else
    $$ = ($1)->AddSegment($2, $3, MSC_POS(@3), MSC_POS2(@2, @3));
  #endif
    free($3);
}
            | arcrel_bidir relation_bidir_cont
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = ($1)->AddSegment($2, NULL, MSC_POS(@2), MSC_POS(@2));
  #endif
};

relation_to:   TOK_REL_SOLID_TO | TOK_REL_DOUBLE_TO | TOK_REL_DASHED_TO | TOK_REL_DOTTED_TO;
relation_from: TOK_REL_SOLID_FROM | TOK_REL_DOUBLE_FROM | TOK_REL_DASHED_FROM | TOK_REL_DOTTED_FROM;
relation_bidir: TOK_REL_SOLID_BIDIR | TOK_REL_DOUBLE_BIDIR | TOK_REL_DASHED_BIDIR | TOK_REL_DOTTED_BIDIR;

relation_to_cont : relation_to | TOK_DASH {$$=MSC_ARC_UNDETERMINED_SEGMENT;};
relation_from_cont : relation_from | TOK_DASH {$$=MSC_ARC_UNDETERMINED_SEGMENT;};
relation_bidir_cont : relation_bidir | TOK_DASH {$$=MSC_ARC_UNDETERMINED_SEGMENT;};

extvertxpos: extvertxpos_no_string
             | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName(@1, $1);
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAt(@1, HINT_KEYWORD)) {
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
    $$ = NULL;
  #endif
    free($1);
};

extvertxpos_no_string: TOK_AT_POS vertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName(@1, $1);

  #else
    $$ = new ExtVertXPos($1, MSC_POS(@1), $2);
  #endif
    free($1);
}
             | TOK_AT_POS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName(@1, $1);
    if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
};

symbol_type_string: entity_string | symbol_string;

symbol_command_no_attr: TOK_COMMAND_SYMBOL symbol_type_string markerrel_no_string extvertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne(@4, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    $$ = new CommandSymbol(&msc, $2, $3, $4, NULL);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string markerrel_no_string extvertxpos extvertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new CommandSymbol(&msc, $2, $3, $4, $5);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string extvertxpos_no_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfterPlusOne(@3, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    $$ = new CommandSymbol(&msc, $2, NULL, $3, NULL);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string extvertxpos_no_string extvertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    csh.CheckHintBetween(@2, @3, HINT_MARKER);
  #else
    $$ = new CommandSymbol(&msc, $2, NULL, $3, $4);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string markerrel_no_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAfter(@3, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
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
    $$ = new CommandSymbol(&msc, $2, $3, NULL, NULL);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.hintStatus != HINT_READY &&
        csh.CheckHintAtAndBefore(@2, @3, HINT_MARKER)) {
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
    $$ = new CommandSymbol(&msc, $2, NULL, NULL, NULL);
  #endif
    free($1);
    free($2);
    free($3);
}
                | TOK_COMMAND_SYMBOL symbol_type_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (CaseInsensitiveEqual($2, "arc") || CaseInsensitiveEqual($2, "rectangle") ||
        CaseInsensitiveEqual($2, "..."))
        csh.AddCSH(@2, COLOR_KEYWORD);
    else if (CaseInsensitiveBeginsWith("arc", $2)==0 &&
             CaseInsensitiveBeginsWith("rectangle", $2)==0 &&
             CaseInsensitiveBeginsWith("...", $2)==0)
        csh.AddCSH_Error(@2, "Bad symbol name. Use 'arc', '...' or 'rectangle'.");
    if (csh.CheckHintAtAndBefore(@1, @2, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
    else if (csh.CheckHintAfterPlusOne(@2, yylloc, yychar==YYEOF, HINT_MARKER)) {
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
    $$ = new CommandSymbol(&msc, $2, NULL, NULL, NULL);
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, HINT_KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "arc", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "rectangle", HINT_KEYWORD, true));
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "...", HINT_KEYWORD, true));
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@1).end, "Missing a symbol type.", "Use 'arc', '...' or 'rectangle'.");
    $$ = NULL;
  #endif
    free($1);
};

symbol_command: symbol_command_no_attr
                | symbol_command_no_attr full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(HINT_ATTR_NAME, @2))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(HINT_ATTR_VALUE, @2))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = ($1)->AddAttributeList($2);
  #endif
};

note:            TOK_COMMAND_NOTE extvertxpos_no_string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    ArcBase *a = new CommandNote(&msc, $2, $3); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    $$ = NULL; //no need to add to arclist
  #endif
    free($1);
}
               | TOK_COMMAND_NOTE vertxpos full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    ArcBase *a;
    if ($2) {
        const ExtVertXPos extvertxpos($2);
        a = new CommandNote(&msc, &extvertxpos, $3); //This attaches itself to the target of the note
    } else 
        a = new CommandNote(&msc, NULL, $3);
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    $$ = NULL; //no need to add to arclist
  #endif
    free($1);
}
               | TOK_COMMAND_NOTE full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    ArcBase *a = new CommandNote(&msc, NULL, $2); //This attaches itself to the target of the note
    if (!a->IsValid()) delete a; //if attachment not successful, drop it
    $$ = NULL; //no need to add to arclist
  #endif
    free($1);
};


colon_string: TOK_COLON_QUOTED_STRING
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString(@1, $1, false);
  #endif
    $$ = $1;
}
             | TOK_COLON_STRING
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_ColonString(@1, $1, true);
  #endif
    $$ = $1;
};

full_arcattrlist_with_label: colon_string
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$ = (new AttributeList)->Append(new Attribute("label", $1, MSC_POS(@$), MSC_POS(@$).IncStartCol()));
  #endif
    free($1);
}
              | colon_string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$ = ($2)->Prepend(new Attribute("label", $1, MSC_POS(@1), MSC_POS(@1).IncStartCol()));
  #endif
    free($1);
}
              | full_arcattrlist colon_string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$ = ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol()));
        //Merge $3 at the end of $1
        ($1)->splice(($1)->end(), *($3));
        delete ($3); //empty list now
        $$ = $1;
  #endif
    free($2);
}
              | full_arcattrlist colon_string
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$ = ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol()));
  #endif
    free($2);
}
              | full_arcattrlist;


full_arcattrlist: TOK_OSBRACKET TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@2, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = new AttributeList;
  #endif
}
                   | TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@3, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = $2;
  #endif
}
                   | TOK_OSBRACKET arcattrlist error TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_Error(@3, "Extra stuff after an attribute list. Maybe missing a comma?");
        csh.AddCSH(@4, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = $2;
  #endif
}
                   | TOK_OSBRACKET error TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_Error(@2, "Could not recognize this as an attribute.");
        csh.AddCSH(@3, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = new AttributeList;
  #endif
}
                   | TOK_OSBRACKET arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_ErrorAfter(@2, "Missing a square bracket (']').");
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = $2;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing ']'.");
  #endif
}
                   | TOK_OSBRACKET arcattrlist error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_Error(@3, "Missing a ']'.");
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = $2;
    msc.Error.Error(MSC_POS(@3).start, "Missing ']'.");
  #endif
}
                   | TOK_OSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_ErrorAfter(@1, "Missing a square bracket (']').");
        csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    $$ = new AttributeList;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing ']'.");
  #endif
}
/* Dummy rule to force a lookahead, so that previous rule has a valid
 * lookahead location in yylloc */
                   | TOK_OSBRACKET TOK__NEVER__HAPPENS
{
    $$ = NULL;
}
                   | TOK_OSBRACKET error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_Error(@2, "Missing a ']'.");
        csh.CheckHintBetween(@1, @2, HINT_ATTR_NAME);
  #else
    $$ = new AttributeList;
    msc.Error.Error(MSC_POS(@2).start, "Missing ']'.");
  #endif
}

arcattrlist:    arcattr
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = (new AttributeList)->Append($1);
  #endif
}
              | arcattrlist TOK_COMMA arcattr
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckHintBetween(@2, @3, HINT_ATTR_NAME);
  #else
    $$ = ($1)->Append($3);
  #endif
}
              | arcattrlist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_NAME);
  #else
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Expecting an entity here.");
  #endif
}
              | arcattrlist TOK_COMMA TOK__NEVER__HAPPENS;

arcattr:         string TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_AttrValue(@3, $3, $1);
        csh.CheckHintAt(@1, HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, $3, MSC_POS(@1), MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
        csh.CheckHintAt(@1, HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, atof($3), MSC_POS(@$), MSC_POS(@3), $3);
  #endif
    free($1);
    free($3);
}
	    | string TOK_EQUAL TOK_BOOLEAN
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
        csh.CheckHintAt(@1, HINT_ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, HINT_ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, str2bool($3), MSC_POS(@$), MSC_POS(@3), $3);
  #endif
    free($1);
    free($3);
}
	    | string TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.CheckHintAt(@1, HINT_ATTR_NAME);
        csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, HINT_ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, (char*)NULL, MSC_POS(@$), MSC_POS(@$));
  #endif
    free($1);
}
/* Dummy rule to force a lookahead, so that previous rule has a valid
 * lookahead location in yylloc */
            | string TOK_EQUAL TOK__NEVER__HAPPENS
{
    $$ = NULL;
    free($1);
}
	    | string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName(@1, $1);
        csh.CheckHintAt(@1, HINT_ATTR_NAME);
  #else
        $$ = new Attribute($1, MSC_POS(@$));
  #endif
    free($1);
};

//cannot be a reserved word, symbol or style name
entity_string: TOK_QSTRING | TOK_STRING;

reserved_word_string : TOK_MSC | TOK_COMMAND_DEFCOLOR |
                       TOK_COMMAND_DEFSTYLE | TOK_COMMAND_DEFDESIGN |
                       TOK_COMMAND_NEWPAGE | TOK_COMMAND_BIG | TOK_COMMAND_PIPE |
                       TOK_VERTICAL | TOK_COMMAND_PARALLEL |
                       TOK_COMMAND_HEADING | TOK_COMMAND_NUDGE |
                       TOK_COMMAND_MARK | TOK_AT | TOK_AT_POS | TOK_SHOW | TOK_HIDE |
                       TOK_ACTIVATE | TOK_DEACTIVATE | TOK_BYE |
                       TOK_COMMAND_HSPACE | TOK_COMMAND_VSPACE | TOK_COMMAND_SYMBOL |
                       TOK_COMMAND_NOTE;

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
    default: _ASSERT(0);
    }
}
       | TOK_EMPH
{
    switch ($1) {
    case MSC_EMPH_SOLID:  $$ = strdup("--"); break;
    case MSC_EMPH_DASHED: $$ = strdup("++"); break;
    case MSC_EMPH_DOTTED: $$ = strdup(".."); break;
    case MSC_EMPH_DOUBLE: $$ = strdup("=="); break;
    default: _ASSERT(0);
    }
};

string: entity_string | reserved_word_string | symbol_string | TOK_STYLE_NAME;

scope_open: TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.PushContext();
    if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, HINT_LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.PushContext();
  #endif
}
/* Dummy rule to force a lookahead, so that previous rule has a valid
 * lookahead location in yylloc */
              | TOK_OCBRACKET TOK__NEVER__HAPPENS;

scope_close: TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    $$ = NULL;
    csh.PopContext();
  #else
    $$ = msc.PopContext();
  #endif
};


%%


/* END OF FILE */
