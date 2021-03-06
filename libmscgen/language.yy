%locations
%pure-parser
%lex-param {yyscan_t *yyscanner}
%parse-param{YYMSC_RESULT_TYPE &RESULT}
%parse-param{void *yyscanner}
%initial-action
{
  #ifdef C_S_H_IS_COMPILED
    @$.first_pos = 0;
    @$.last_pos = 0;
  #endif
};

%{
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
%}

%token TOK_STRING TOK_QSTRING TOK_NUMBER TOK_DASH TOK_EQUAL TOK_COMMA
       TOK_SEMICOLON TOK_PLUS TOK_PLUS_EQUAL
       TOK_OCBRACKET TOK_CCBRACKET TOK_OSBRACKET TOK_CSBRACKET
       TOK_ASTERISK
       TOK_MSC TOK_COLON_STRING TOK_COLON_QUOTED_STRING TOK_STYLE_NAME TOK_COLORDEF
       TOK_REL_TO TOK_REL_FROM TOK_REL_BIDIR TOK_REL_X TOK_REL_MSCGEN
       TOK_SPECIAL_ARC     TOK_EMPH TOK_EMPH_PLUS_PLUS
       TOK_COMMAND_HEADING TOK_COMMAND_NUDGE TOK_COMMAND_NEWPAGE
       TOK_COMMAND_DEFSHAPE TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
       TOK_COMMAND_BIG TOK_COMMAND_BOX TOK_COMMAND_PIPE TOK_COMMAND_MARK TOK_COMMAND_PARALLEL TOK_COMMAND_OVERLAP
       TOK_VERTICAL TOK_VERTICAL_SHAPE TOK_AT TOK_LOST TOK_AT_POS 
       TOK_SHOW TOK_HIDE TOK_ACTIVATE TOK_DEACTIVATE TOK_BYE
       TOK_COMMAND_VSPACE TOK_COMMAND_HSPACE TOK_COMMAND_SYMBOL TOK_COMMAND_NOTE
       TOK_COMMAND_COMMENT TOK_COMMAND_ENDNOTE TOK_COMMAND_FOOTNOTE
       TOK_COMMAND_TITLE TOK_COMMAND_SUBTITLE TOK_COMMAND_TEXT
       TOK_SHAPE_COMMAND
       TOK_MSCGEN_RBOX TOK_MSCGEN_ABOX
       TOK__NEVER__HAPPENS
       TOK_EOF 0
%union
{
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
    EArcSymbol                     arcsymbol;
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
};

%type <arcbase>    arcrel arc arc_with_parallel arc_with_parallel_semicolon opt scope_close
                   symbol_command symbol_command_no_attr note comment
%type <arcvertarrow> vertrel_no_xpos vertrel
%type <arcarrow>   arcrel_to arcrel_from arcrel_bidir arcrel_arrow
%type <arcbox>     boxrel first_box mscgen_boxrel 
%type <arcpipe>    first_pipe
%type <arcboxseries> box_list mscgen_box
%type <arcpipeseries> pipe_list_no_content pipe_list
%type <arcparallel> parallel
%type <arclist>    top_level_arclist arclist arclist_maybe_no_semicolon braced_arclist optlist mscgen_boxlist
%type <entitylist> entitylist entity first_entity
%type <arcsymbol>  TOK_REL_TO TOK_REL_FROM TOK_REL_BIDIR TOK_REL_MSCGEN
                   relation_to_cont_no_loss relation_from_cont_no_loss relation_bidir_cont_no_loss
                   TOK_EMPH TOK_EMPH_PLUS_PLUS emphrel
                   TOK_SPECIAL_ARC
%type <arcsegdata> relation_to relation_from relation_bidir
                   relation_to_cont relation_from_cont relation_bidir_cont
%type <eside>      comment_command
%type <attrib>     arcattr
%type <vertxpos>   vertxpos
%type <extvertxpos> extvertxpos extvertxpos_no_string
%type <namerel>    entityrel markerrel_no_string hspace_location
%type <attriblist> arcattrlist full_arcattrlist full_arcattrlist_with_label
                   full_arcattrlist_with_label_or_number
%type <str>        entity_command_prefixes titlecommandtoken
                   entity_string reserved_word_string string  
                   symbol_string colon_string symbol_type_string alpha_string color_string
                   overlap_or_parallel mscgen_emphrel
                   TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_COLON_QUOTED_STRING TOK_COLORDEF
                   TOK_STYLE_NAME TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_BOX TOK_COMMAND_PIPE
                   TOK_COMMAND_DEFSHAPE TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
                   TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
                   TOK_COMMAND_PARALLEL TOK_COMMAND_OVERLAP TOK_COMMAND_MARK TOK_BYE
                   TOK_NUMBER  
                   TOK_VERTICAL  TOK_AT TOK_LOST TOK_AT_POS
                   TOK_SHOW TOK_HIDE TOK_ACTIVATE TOK_DEACTIVATE
                   TOK_COMMAND_VSPACE TOK_COMMAND_HSPACE TOK_COMMAND_SYMBOL TOK_COMMAND_NOTE
                   TOK_COMMAND_COMMENT TOK_COMMAND_ENDNOTE TOK_COMMAND_FOOTNOTE
                   TOK_COMMAND_TITLE TOK_COMMAND_SUBTITLE TOK_COMMAND_TEXT TOK_VERTICAL_SHAPE
                   TOK_MSCGEN_RBOX TOK_MSCGEN_ABOX TOK_REL_X
%type <stringlist> stylenamelist
%type <vshape>     vertical_shape
%type<arctypeplusdir> empharcrel_straight
%type <shapecommand> TOK_SHAPE_COMMAND
%type <shapeelement> shapeline
%type <shape> shapedeflist

%destructor {if (!C_S_H) delete $$;} <arcbase> <arclist> <arcarrow> <arcvertarrow> 
%destructor {if (!C_S_H) delete $$;} <arcbox> <arcpipe> <arcboxseries> <arcpipeseries> <arcparallel>
%destructor {if (!C_S_H) delete $$;} <entitylist> <attrib> <attriblist> 
%destructor {if (!C_S_H) delete $$;} <vertxpos> <extvertxpos> <namerel> <stringlist>
%destructor {free($$);} <str> 

%{
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
        if (N) {                                            \
            (Current).first_pos = YYRHSLOC(Rhs,1).first_pos;\
            (Current).last_pos  = YYRHSLOC(Rhs,N).last_pos; \
        } else {                                            \
            (Current).first_pos = (Current).last_pos =      \
                YYRHSLOC (Rhs, 0).last_pos;                 \
        }                                                   \
    while (0)

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

%}

%%

msc_with_bye: msc eof
{
	YYACCEPT;
}

eof:   TOK_EOF 
     | TOK_BYE 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	csh.AddCSH_AllCommentBeyond(@1);
  #else
  #endif
    free($1);
} 
     | TOK_BYE TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_SEMICOLON);
	csh.AddCSH_AllCommentBeyond(@2);
  #else
  #endif
    free($1);
}

msc:
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddLineBeginToHints();
    csh.hintStatus = HINT_READY;
    csh.hintSource = EHintSourceType::LINE_START;
    csh.hintsForcedOnly = true;
  #else
    //no action for empty file
  #endif
}
              | msckey braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore(@1)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @1))
        csh.AddDesignsToHints(true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @1))
        csh.AddOptionsToHints();
    else if (csh.CheckHintLocated(EHintSourceType::LINE_START, @1)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddArcs($2);
  #endif
}
           | TOK_MSC error eof
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore(@1) || csh.CheckLineStartHintAt(@1)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_Error(@2, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #else
    msc.Error.Error(MSC_POS(@2).start, "Missing an equal sign or a list of elements between braces ('{' and '}').");
  #endif
    free($1);
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
           | top_level_arclist eof
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore(@1)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    msc.AddArcs($1);
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
}
                 | top_level_arclist error eof
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckLineStartHintBefore(@1)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    CshPos pos = @2;
    if ((@1).last_pos >= (@2).first_pos)
        pos.first_pos = (@1).last_pos;
    csh.AddCSH_Error(pos, "Could not recognize this as a valid line.");
  #else
    msc.AddArcs($1);
    msc.Error.Error(MSC_POS(@2).start, "Could not recognize this as a valid line.");
  #endif
	YYACCEPT; //We should noty parse further in msc_with_bye as we may have something beyond BYE (in eof)
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


msckey:       TOK_MSC 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.SwitchToMscgenCompatMode();
  #else
    msc.SwitchToMscgenCompatMode();
  #endif
    free($1);
}
               | TOK_MSC TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_ErrorAfter(@2, "Missing a design name.");
    csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc");
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing design name.");
  #endif
    free($1);
}
              | TOK_MSC TOK_EQUAL alpha_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
    csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, "msc");
    std::string msg = csh.SetDesignTo($3, true);
    if (msg.length())
        csh.AddCSH_Error(@3, msg.c_str());
  #else
    ArcBase *dummy = msc.AddAttribute(Attribute("msc", $3, MSC_POS(@1), MSC_POS(@3)));
    if (dummy) delete dummy;
  #endif
    free($1);
    free($3);
};

braced_arclist: scope_open arclist_maybe_no_semicolon scope_close
{
  #ifdef C_S_H_IS_COMPILED
  #else
    if ($3) ($2)->Append($3); //Append any potential CommandNumbering
    $$ = $2;
  #endif
}
            | scope_open scope_close
{
  #ifdef C_S_H_IS_COMPILED
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
    csh.AddCSH_Error(@3, "Could not recognize this as a valid line.");
  #else
    if ($4) ($2)->Append($4); //Append any potential CommandNumbering
    $$ = $2;
    msc.Error.Error(MSC_POS(@3).start, "Syntax error.");
  #endif
    yyerrok;
}
            | scope_open arclist_maybe_no_semicolon error TOK_EOF
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@3, "Could not recognize this as a valid line.");
  #else
    $$ = $2;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@3).start, "Missing '}'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@3).start, "Here is the corresponding '{'.");
  #endif
}
            | scope_open arclist_maybe_no_semicolon TOK_EOF
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@2, "Missing a closing brace ('}').");
  #else
    $$ = $2;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@2).end.NextChar(), "Here is the corresponding '{'.");
  #endif
}
            | scope_open TOK_EOF
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@1, "Missing a closing brace ('}').");
  #else
    $$ = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a corresponding '}'.");
  #endif
}
            | scope_open arclist_maybe_no_semicolon TOK_BYE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@3, "The command 'bye' can only be used at the top level.");
  #else
    $$ = $2;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@3).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@3).start, "Here is the opening '{'.");
  #endif
  free($3);
}
            | scope_open TOK_BYE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "The command 'bye' can only be used at the top level and not inside curly braces '{' and '}'.");
  #else
    $$ = NULL;
    //Do not pop context, as the missing scope_close would have done
    msc.Error.Error(MSC_POS(@2).start, "The command 'bye' can not be used between curly braces '{' and '}'.");
  #endif
  free($2);
};



arclist_maybe_no_semicolon : arclist
            | arclist arc_with_parallel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@2, "Missing a semicolon (';').");
  #else
    if ($2) ($1)->Append($2);
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS(@2).start, MSC_POS(@2).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
}
            | arc_with_parallel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ErrorAfter(@1, "Missing a semicolon (';').");
  #else
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = new ArcList;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@1).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
};


arclist:    arc_with_parallel_semicolon
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = new ArcList;
  #endif
}
            | arclist arc_with_parallel_semicolon
{
  #ifndef C_S_H_IS_COMPILED
    if ($2) ($1)->Append($2);     /* Add to existing list */
    $$ = ($1);
  #endif
};

arc_with_parallel_semicolon: arc_with_parallel TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SEMICOLON);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
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
    if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
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
    if (csh.CheckHintAfter(@3, yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
       csh.AddLineBeginToHints();
       csh.hintStatus = HINT_READY;
    }
  #else
    $$=$1;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a semicolon ';'.");
    msc.Error.Error(MSC_POS(@1).start, MSC_POS(@1).end.NextChar(), "Here is the beginning of the command as I understood it.");
  #endif
};

overlap_or_parallel: TOK_COMMAND_PARALLEL | TOK_COMMAND_OVERLAP;

arc_with_parallel: arc
{
  #ifdef C_S_H_IS_COMPILED
  #else
    if ($1) 
		($1)->SetLineEnd(MSC_POS(@$));
    $$=$1;
  #endif
}
              | overlap_or_parallel arc
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
  #else
    if ($2) {
        ($2)->SetLineEnd(MSC_POS(@$));
        if (CaseInsensitiveEqual($1, "parallel"))
            ($2)->SetParallel();
        else 
            ($2)->SetOverlap();
    }
    $$ = $2;
  #endif
    free($1);
}
              | overlap_or_parallel 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
};

arc:           arcrel
{
  #ifdef C_S_H_IS_COMPILED
  #else
    if ($1)
        ($1)->AddAttributeList(NULL);
    $$=($1);
  #endif
}
              | arcrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($1)
        ($1)->AddAttributeList($2);
    $$ = ($1);
  #endif
}
              | TOK_COMMAND_BIG
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcBigArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcBigArrow::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a marker or one of 'brace', 'bracket', 'range', 'box' or an arrow or box symbol, such as '->' or '--'.");
  #endif
    free($1);
    $$ = NULL;
}
              |TOK_VERTICAL vertrel
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH(@1, COLOR_KEYWORD);
  #else
    if ($2) {
      ($2)->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ($2)->AddAttributeList(NULL);
      $$ = ($2);
    } else $$ = NULL;
  #endif
    free($1);
}
              |TOK_VERTICAL vertical_shape vertrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt(@2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    }
#else
    if ($3) {
      ($3)->SetVerticalShape($2);
      ($3)->AddAttributeList(NULL);
      $$ = ($3);
    } else $$ = NULL;
  #endif
    free($1);
}
              |TOK_VERTICAL vertical_shape 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt(@2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAfterPlusOne(@2, yylloc, yychar==YYEOF, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_FILLING;
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
    }
#else
	ArcTypePlusDir typeplusdir;
	typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = EDirType::RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape($2);
    ava->AddAttributeList(NULL);
	$$ = ava;
  #endif
    free($1);
}
              | TOK_VERTICAL vertrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
    } if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($2) {
      ($2)->SetVerticalShape(ArcVerticalArrow::ARROW_OR_BOX);
      ($2)->AddAttributeList($3);
      $$ = ($2);
    } else $$ = NULL;
  #endif
    free($1);
}
              | TOK_VERTICAL vertical_shape vertrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @4))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @4))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($3) {
      ($3)->SetVerticalShape($2);
      ($3)->AddAttributeList($4);
      $$ = ($3);
    } else $$ = NULL;
  #endif
    free($1);
}
              | TOK_VERTICAL vertical_shape full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints(false);
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintAt(@2, EHintSourceType::KEYWORD)) {
        csh.AddVerticalTypesToHints();
        csh.hintStatus = HINT_READY;
        csh.hintsForcedOnly = true;
    } else if (csh.CheckHintBetween(@2, @3, EHintSourceType::MARKER)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcVerticalArrow::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcVerticalArrow::AttributeValues(csh.hintAttrName, csh);
  #else
	ArcTypePlusDir typeplusdir;
    typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
	typeplusdir.arc.lost = EArrowLost::NOT;
	typeplusdir.dir = EDirType::RIGHT;
	ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
	VertXPos vxp(msc);
	ava->AddXpos(&vxp);
    ava->SetVerticalShape($2);
    ava->AddAttributeList($3);
	$$ = ava;
  #endif
    free($1);
}
              | full_arcattrlist  
{
    //Here we have no label and may continue as a parallel block
    //->offer parallel attributes, as well...
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @1)) {
        ArcDivider::AttributeNames(csh, false, false);
        ArcParallel::AttributeNames(csh, true);
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @1)) {
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
        ArcParallel::AttributeValues(csh.hintAttrName, csh, true);
    }
  #else
    //... but due to the lack of curly brace we are a divider
    $$ = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ($$)->AddAttributeList($1);
  #endif
}
              | colon_string
{
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
  #else
    AttributeList *al = new AttributeList;
    al->Append(new Attribute("label", $1, MSC_POS(@$), MSC_POS(@$).IncStartCol()));
    $$ = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ($$)->AddAttributeList(al);
  #endif
    free($1);
}
              | colon_string full_arcattrlist
{
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2)) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2)) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ($2)->Prepend(new Attribute("label", $1, MSC_POS(@1), MSC_POS(@1).IncStartCol()));
    $$ = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ($$)->AddAttributeList($2);
#endif
    free($1);
}
              | full_arcattrlist colon_string full_arcattrlist
{
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @1) || 
        csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @1) || 
             csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3)) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol()));
    //Merge $3 at the end of $1 (after the colon label, so ordering is kept)
    ($1)->splice(($1)->end(), *($3));
    delete ($3); //empty list now
    $$ = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ($$)->AddAttributeList($1);
#endif
    free($2);
}
              | full_arcattrlist colon_string
{
    //Here we have a colon label added: this is a divider
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @1)) 
        ArcDivider::AttributeNames(csh, false, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @1)) 
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, false);
  #else
    ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol())); 
    $$ = new ArcDivider(EArcSymbol::DIV_VSPACE, &msc);
    ($$)->AddAttributeList($1);    
  #endif
    free($2);
}
              | first_entity
{
  #ifdef C_S_H_IS_COMPILED
  #else
    $$ = (new CommandEntity($1, &msc, false));
    ($$)->AddAttributeList(NULL);
  #endif
}
              | entity_command_prefixes
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
  #else
    CommandEntity *ce = new CommandEntity($2, &msc, false);
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
    CommandEntity *ce = new CommandEntity($1, &msc, false);
    ce->AddAttributeList(NULL);
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing an entity.");
    $$ = ce;
  #endif
}
            | first_entity TOK_COMMA entitylist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintBetween(@2, @3);
  #else
    ($3)->Prepend($1);
    CommandEntity *ce = new CommandEntity($3, &msc, false);
    delete ($1);
    ce->AddAttributeList(NULL);
    $$ = ce;
  #endif
}
            | entity_command_prefixes first_entity TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@3, COLOR_COMMA);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBeforePlusOne(@1, @2);
    csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
    csh.AddCSH_ErrorAfter(@3, "Missing an entity.");
  #else
    CommandEntity *ce = new CommandEntity($2, &msc, false);
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else 
        csh.CheckEntityHintBetween(@3, @4);
#else
    ($4)->Prepend($2);
    CommandEntity *ce = new CommandEntity($4, &msc, false);
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
     * enclose them in an "ArcParallel" element used only for this. 
     * This will be an internally defined ArcParallel that will
     * get unrolled in Msc::PostParseArcList()*/
    $$ = ($1) ? new ArcParallel(&msc, $1, NULL, true, true) : NULL;
  #endif
}
              | mscgen_boxlist
{
  #ifdef C_S_H_IS_COMPILED
  #else
    /* This may be a list of ArcBoxSeries (each with one emptybox), we
     * enclose them in an "ArcParallel" element used only for this. 
     * This will be an internally defined ArcParallel that will
     * get unrolled in Msc::PostParseArcList()*/
    $$ = ($1) ? new ArcParallel(&msc, $1, NULL, false, true) : NULL;
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
    $$ = ($1);
	if ($1)
        ($$)->AddAttributeList(NULL);
  #endif
}
              | TOK_COMMAND_DEFSHAPE shapedef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSHAPE 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter(@$, "Missing shape name and definition.");
#else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFCOLOR colordeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFCOLOR 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a color name to (re-)define.");
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSTYLE styledeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSTYLE 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a style name to (re-)define.");
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFDESIGN designdef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_DEFDESIGN 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter(@$, "Missing design name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a design name to (re-)define.");
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_HEADING
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    $$ = (new CommandEntity(NULL, &msc, false));
    ($$)->AddAttributeList(NULL);
  #endif
    free($1);
}
              | TOK_COMMAND_HEADING full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandEntity::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandEntity::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandEntity(NULL, &msc, false));
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
          | TOK_COMMAND_NUDGE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    $$ = (new ArcDivider(EArcSymbol::DIV_VSPACE, &msc));
    ($$)->AddAttributeList(NULL);
  #endif
    free($1);
}
          | TOK_COMMAND_NUDGE full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcDivider::AttributeNames(csh, true, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, true, false);
  #else
    $$ = (new ArcDivider(EArcSymbol::DIV_VSPACE, &msc));
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
          | titlecommandtoken full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcDivider::AttributeNames(csh, false, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcDivider::AttributeValues(csh.hintAttrName, csh, false, true);
  #else
    const EArcSymbol t = CaseInsensitiveEqual("title", $1) ? EArcSymbol::DIV_TITLE :
                         CaseInsensitiveEqual("subtitle", $1) ? EArcSymbol::DIV_SUBTITLE :
                         EArcSymbol::INVALID;
    $$ = (new ArcDivider(t, &msc));
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
          | titlecommandtoken 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter(@$, "Missing label.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing label. Ignoring (sub)title.", "Titles and subtitles must have a label.");
    $$ = NULL;
  #endif
    free($1);
}
              | TOK_COMMAND_TEXT
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing 'at' keyword.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing 'at' clause.");
#endif
    free($1);
}
              | TOK_COMMAND_TEXT full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing 'at' keyword.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing 'at' clause. Ignoring this.");
    if ($2)
        delete $2;
  #endif
    free($1);
}
              | TOK_COMMAND_TEXT vertxpos full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
#else
    CommandSymbol *s = new CommandSymbol(&msc, $2, MSC_POS(@2));
    s->AddAttributeList($3);
    $$ = s;
    if ($2)
        delete $2;
  #endif
    free($1);
}
              | TOK_COMMAND_TEXT vertxpos 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter(@2, "Missing label.");
  #else
    if ($2)
        delete $2;
    msc.Error.Error(MSC_POS(@2).end, "Missing actual text - specify a label. Ignoring this.");
    $$ = NULL;
#endif
    free($1);
}
              | TOK_COMMAND_MARK 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing marker name.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing marker name. Ignoring this.", "You need to supply a name which then can be used to refer to this vertical position you are marking here.");
  #endif
    free($1);
}
              | TOK_COMMAND_MARK alpha_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.MarkerNames.insert($2);
  #else
    $$ = (new CommandMark($2, MSC_POS(@$), &msc));
    ($$)->AddAttributeList(NULL);
  #endif
    free($1);
    free($2);
}
              | TOK_COMMAND_MARK alpha_string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.MarkerNames.insert($2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandMark::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandMark::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandMark($2, MSC_POS(@$), &msc));
    ($$)->AddAttributeList($3);
  #endif
    free($1);
    free($2);
}
              | TOK_COMMAND_MARK full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing marker name.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing marker name. Ignoring this.", "You need to supply a name which then can be used to refer to this vertical position you are marking here.");
    if ($2)
        delete $2;
#endif
    free($1);
}
              | TOK_COMMAND_NEWPAGE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    $$ = (new CommandNewpage(&msc, true));
    ($$)->AddAttributeList(NULL);
  #endif
    free($1);
}
              | TOK_COMMAND_NEWPAGE full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandNewpage::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandNewpage::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandNewpage(&msc, true));
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
              | symbol_command
              | note
              | comment
              | TOK_COMMAND_HSPACE hspace_location full_arcattrlist_with_label_or_number
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandHSpace(&msc, $2));
    ($$)->AddAttributeList($3);
  #endif
    free($1);
}
              | TOK_COMMAND_HSPACE hspace_location
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "Enter a number in pixels to set horizontal spacing.", 
            EHintType::KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", 
            "Enter some text the width of which will be used as horizontal spacing.", 
            EHintType::KEYWORD, false));
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing an entity, 'left comment' or 'right comment'.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing an entity, 'left comment' or 'right comment'. Ignoring this command.");
  #endif
    free($1);
}
              | TOK_COMMAND_HSPACE full_arcattrlist_with_label_or_number
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end, "Missing an entity, 'left comment' or 'right comment'. Ignoring this command.");
    delete $2;
  #endif
    free($1);
}
              | TOK_COMMAND_VSPACE full_arcattrlist_with_label_or_number
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandHSpace::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandHSpace::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = (new CommandVSpace(&msc));
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
              | TOK_COMMAND_VSPACE
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<number>", 
            "Enter a number in pixels to add that much empty vertical spacing.", 
            EHintType::KEYWORD, false));
        csh.AddToHints(CshHint(csh.HintPrefixNonSelectable() + "<label>", 
            "Enter some text the height of which will be added as vertical empty space.", 
            EHintType::KEYWORD, false));
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@1, "Missing either a number or a label.");
  #else
    $$ = new CommandVSpace(&msc); //will result in an error, since label or space attribute is needed
  #endif
    free($1);
};

titlecommandtoken: TOK_COMMAND_TITLE | TOK_COMMAND_SUBTITLE;

hspace_location: entityrel
                 | TOK_AT_POS TOK_COMMAND_COMMENT
{
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual($1, "left") || CaseInsensitiveEqual($1, "right"))
        csh.AddCSH(@1, COLOR_KEYWORD);
    else
        csh.AddCSH_Error(@1, "Use either `left comment` or `right comment` to specify which comment column to size.");
    csh.AddCSH(@2, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    if (CaseInsensitiveEqual($1, "left"))
        $$ = new NamePair(LNOTE_ENT_STR, MSC_POS(@1), NULL, MSC_POS(@1));
    else if (CaseInsensitiveEqual($1, "right"))
        $$ = new NamePair(RNOTE_ENT_STR, MSC_POS(@1), NULL, MSC_POS(@1));
    else {
        msc.Error.Error(MSC_POS(@1).start, "Use either `left` or `right` to specify which comment column to size. Ignoring command.");
        $$ = NULL;
    }
  #endif
    free($1);
    free($2);
}
                 | TOK_AT_POS
{
  #ifdef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual($1, "left") || CaseInsensitiveEqual($1, "right")) {
        csh.AddCSH(@1, COLOR_KEYWORD);
        if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
            csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "comment", NULL, EHintType::KEYWORD, true));
            csh.hintStatus = HINT_READY;
        }
    }  else
        csh.AddCSH_Error(@1, "Use either `left comment` or `right comment` to specify which comment column to size.");
    if (csh.CheckHintAt(@1, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS(@1).start, "Use <entity>-<entity>, `left comment`, `right comment` to specify horizontal spacing. Ignoring command.");
    $$ = NULL;
  #endif
    free($1);
}
                 | TOK_COMMAND_COMMENT
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.AddLeftRightHSpaceToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS(@1).start, "Use `left comment` or `right comment` to specify horizontal spacing for comments. Ignoring command.");
    $$ = NULL;
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

dash_or_dashdash: TOK_DASH | TOK_EMPH;

entityrel: entity_string dash_or_dashdash
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_EntityName(@1, $1);
    csh.CheckEntityHintAt(@1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = new NamePair($1, MSC_POS(@1), NULL, FileLineColRange());
  #endif
    free($1);
}
           | dash_or_dashdash entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@2, $2);
    csh.CheckEntityHintAt(@2);
  #else
    $$ = new NamePair(NULL, FileLineColRange(), $2, MSC_POS(@2));
  #endif
    free($2);
}
           | dash_or_dashdash
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    $$ = NULL;
  #endif
}
           | entity_string dash_or_dashdash entity_string
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
    $$ = new NamePair($1, MSC_POS(@1), NULL, FileLineColRange());
  #endif
    free($1);
};


markerrel_no_string: entity_string dash_or_dashdash
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    $$ = new NamePair($1, MSC_POS(@1), NULL, FileLineColRange());
  #endif
    free($1);
}
           | dash_or_dashdash entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.CheckHintAt(@2, EHintSourceType::MARKER);
  #else
    $$ = new NamePair(NULL, FileLineColRange(), $2, MSC_POS(@2));
  #endif
    free($2);
}
           | dash_or_dashdash
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    $$ = NULL;
  #endif
}
           | entity_string dash_or_dashdash entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH(@3, COLOR_MARKERNAME);
    csh.CheckHintAt(@3, EHintSourceType::MARKER);
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
        //($1)->MakeMeLastNotable(); Do not make chart options notable
    } else
        $$ = NULL;
  #endif
}
           | optlist TOK_COMMA opt
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    if ($3) {
        if ($1)
            $$ = ($1)->Append($3);     /* Add to existing list */
        else
            $$ = (new ArcList)->Append($3); /* New list */
        //($3)->MakeMeLastNotable(); Do not make chart options notable
    } else
        $$ = $1;
  #endif
}
           | optlist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
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
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_Error(@3, "An option expected here.");
  #else
    $$ = $1;
  #endif
};


opt:         entity_string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = msc.AddAttribute(Attribute($1, atof($3), MSC_POS(@1), MSC_POS(@3), $3));
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_AttrValue_CheckAndAddEscapeHint(@3, $3, $1);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = msc.AddAttribute(Attribute($1, $3, MSC_POS(@1), MSC_POS(@3)));
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
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
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
    if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, "msc")) {
        csh.AddDesignsToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo($3, true);
    if (msg.length())
        csh.AddCSH_Error(@3, msg.c_str());
  #else
    $$ = msc.AddAttribute(Attribute("msc", $3, MSC_POS(@$), MSC_POS(@3)));
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
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc")) {
        csh.AddDesignsToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing option value.");
    $$ = NULL;
  #endif
    free($1);
}
            | TOK_MSC TOK_PLUS_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, "msc+")) {
        csh.AddDesignsToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddOptionsToHints();
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo($3, false);
    if (msg.length())
        csh.AddCSH_Error(@3, msg.c_str());
  #else
        $$ = msc.AddAttribute(Attribute("msc+", $3, MSC_POS(@$), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_PLUS_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH_ErrorAfter(@2, "Missing option value.");
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc+")) {
        csh.AddDesignsToHints(false);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
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
    $$ = ($1);
  #endif
}
            | entitylist TOK_COMMA entity
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintBetween(@2, @3);
#else
    ($3)->Prepend($1);
    $$ = $3;
    delete ($1);
  #endif
}
            | entitylist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = ($1);
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
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, NULL, FileLineCol());
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free($1);
}
             | entity_string full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS2(@1, @2));
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
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@1));
    $$ = ed->AddAttributeList(NULL, $2, MSC_POS(@2).start);
  #endif
    free($1);
};

first_entity:  entity_string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity(@1, $1);  //Do it after AddLineBeginToHints so if this is a newly defined entity it one is not included among the hints
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList($2, NULL, FileLineCol());
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity(@1, $1);   //Do it after AddLineBeginToHints so if this is a newly defined entity it one is not included among the hints
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@$));
    $$ = ed->AddAttributeList(NULL, NULL, FileLineCol());
  #endif
    free($1);
}
                   | entity_string full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        EntityApp::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        EntityApp::AttributeValues(csh.hintAttrName, csh);
    csh.AddCSH_KeywordOrEntity(@1, $1);  //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS2(@1, @2));
    $$ = ed->AddAttributeList($2, $3, MSC_POS(@3).start);
  #endif
    free($1);
}
            | entity_string braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_KeywordOrEntity(@1, $1);   //Do it after AddLineBeginToHints so this one is not included
  #else
    EntityApp *ed = new EntityApp($1, &msc);
    ed->SetLineEnd(MSC_POS(@1));
    $$ = ed->AddAttributeList(NULL, $2, MSC_POS(@2).start);
  #endif
    free($1);
};

styledeflist: styledef
             | styledeflist TOK_COMMA styledef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #endif
}
             | styledeflist TOK_COMMA 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAfterPlusOne(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else 
    msc.Error.Error(MSC_POS(@$).end, "Missing style definition here.", "Try just removing the comma.");
#endif
};

styledef: stylenamelist full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
    for (auto &str : *($1))
        if (csh.ForbiddenStyles.find(str) == csh.ForbiddenStyles.end())
            csh.Contexts.back().StyleNames.insert(str);
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        MscStyle().AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        MscStyle().AttributeValues(csh.hintAttrName, csh);
  #else
    for (auto a=($2)->begin(); a!=($2)->end(); a++) {
        std::list<string> problem;
        bool had_generic = false;
        for (auto s = ($1)->begin(); s!=($1)->end(); s++) {
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
        } else if (problem.size() == ($2)->size()) {
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
    delete($2);
  #endif
    delete($1);
}  
                 | stylenamelist 
{
  #ifdef C_S_H_IS_COMPILED
	csh.AddCSH_ErrorAfter(@$, "Missing attribute definitons in square brackets ('[' and ']').");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing attribute definitons in square brackets ('[' and ']').");
  #endif
    delete($1);
};

/* 'string' does not match "++", so we list it separately */
stylenamelist: string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_STYLENAME);
    $$ = new std::list<string>;
    if (strcmp($1, "emphasis")==0)
        ($$)->push_back("box");
    else if (strcmp($1, "emptyemphasis")==0)
        ($$)->push_back("emptybox");
    else ($$)->push_back($1);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new std::list<string>;
    ($$)->push_back(ConvertEmphasisToBox($1, &@1, msc));
  #endif
    free($1);
}
             | TOK_EMPH_PLUS_PLUS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_STYLENAME);
    $$ = new std::list<string>;
	($$)->push_back("++");
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new std::list<string>;
    ($$)->push_back(ConvertEmphasisToBox("++", &@1, msc));
  #endif
}
             | stylenamelist TOK_COMMA 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
	csh.AddCSH_ErrorAfter(@2, "Missing a style name to (re-)define.");
    if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
	$$ = $1;
  #else
    $$ = $1;
    msc.Error.Error(MSC_POS(@$).end, "Missing a style name to (re-)define.");
  #endif
};
             | stylenamelist TOK_COMMA string
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
    if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ($1)->push_back(ConvertEmphasisToBox($3, &@3, msc));
    $$ = $1;
  #endif
    free($3);
}
             | stylenamelist TOK_COMMA TOK_EMPH_PLUS_PLUS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.AddCSH(@3, COLOR_STYLENAME);
    $$ = $1;
	($$)->push_back("++");
    if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_NAME)) {
		csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
    }
  #else
    ($1)->push_back(ConvertEmphasisToBox("++", &@3, msc));
    $$ = $1;
  #endif
};

shapedef: entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
    csh.AddCSH_ErrorAfter(@$, ("Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string($1 ? $1 : "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS(@$).end, "Here should come a shape definition beginning with '{'. Ignoring this malformed shape definition for '"+string($1 ? $1 : "") +"'.");
  #endif	
  free($1);
}
		| entity_string TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH_ErrorAfter(@$, ("Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string($1 ? $1: "") +"'.").c_str());
  #else
    msc.Error.Error(MSC_POS(@$).end, "Here should come a shape definition beginning with 'T', 'H', 'M', 'L', 'C', 'S' or 'E'. Ignoring this malformed shape definition for '"+string($1 ? $1: "") +"'.");
  #endif	
  free($1);
}
		| entity_string TOK_OCBRACKET shapedeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH_ErrorAfter(@3, "Missing a closing brace ('}').");
	csh.AddShapeName($1);
  #else
    msc.Error.Error(MSC_POS(@3).end.NextChar(), "Missing '}'.");
    msc.Error.Error(MSC_POS(@2).start, MSC_POS(@3).end.NextChar(), "Here is the corresponding '{'.");
	if ($3) {
		msc.Shapes.Add(std::string($1), MSC_POS(@1).start, msc.file_url, msc.file_info, $3, msc.Error);
		delete $3;
	}
  #endif	
  free($1);
}
		| entity_string TOK_OCBRACKET shapedeflist TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH(@4, COLOR_BRACE);
	csh.AddShapeName($1);
  #else
	if ($3) {
		msc.Shapes.Add(std::string($1), MSC_POS(@1).start, msc.file_url, msc.file_info, $3, msc.Error);
		delete $3;
	}
  #endif	
  free($1);
}
		| entity_string TOK_OCBRACKET shapedeflist error TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_ATTRVALUE);
    csh.AddCSH(@2, COLOR_BRACE);
    csh.AddCSH(@5, COLOR_BRACE);
	csh.AddShapeName($1);
    csh.AddCSH_Error(@4, "Only numbers can come after shape commands.");
  #else
	if ($3) {
		msc.Shapes.Add(std::string($1), MSC_POS(@1).start, msc.file_url, msc.file_info, $3, msc.Error);
		delete $3;
	}
  #endif	
  free($1);
};

shapedeflist: shapeline TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SEMICOLON);
  #else
    $$ = new Shape;
	if ($1) {
		($$)->Add(std::move(*($1)));
		delete $1;
	}
  #endif	
}
             | error shapeline TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@1, "I do not understand this.");
    csh.AddCSH(@3, COLOR_SEMICOLON);
#else
    $$ = new Shape;
	if ($2) {
		($$)->Add(std::move(*($2)));
		delete $2;
	}
  #endif	
}
             | shapedeflist shapeline TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@3, COLOR_SEMICOLON);
  #else
	if ($2) {
		($1)->Add(std::move(*($2)));
		delete $2;
	}
  #endif	
}
             | shapedeflist error  
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "Only numbers can come after shape commands.");
  #else
  #endif	
};

shapeline: TOK_SHAPE_COMMAND
{
    const int num_args = 0;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args)
		csh.AddCSH_ErrorAfter(@$, ShapeElement::ErrorMsg($1, num_args));
  #else
	$$ = NULL;
	if (should_args != num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
	    $$ = new ShapeElement($1);
  #endif	
}
         | TOK_SHAPE_COMMAND TOK_NUMBER
{
    const int num_args = 1;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	} else if ($1>=ShapeElement::SECTION_BG && (($2)[0]<'0' || ($2)[0]>'2' || ($2)[1]!=0))
		csh.AddCSH_Error(@2, "S (section) commands require an integer between 0 and 2.");
  #else
	$$ = NULL;
	const double a = atof($2);
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@2).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else if ($1>=ShapeElement::SECTION_BG && (a!=0 && a!=1 && a!=2))
		msc.Error.Error(MSC_POS(@2).start, "S (section) commands require an integer between 0 and 2. Ignoring line.");
	else if ($1>=ShapeElement::SECTION_BG)
	    $$ = new ShapeElement(ShapeElement::Type($1 + unsigned(a)));
	else 
		$$ = new ShapeElement($1, a);
  #endif	
  free($2);
}
         | TOK_SHAPE_COMMAND TOK_NUMBER TOK_NUMBER
{
    const int num_args = 2;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2 + @3, msg); break;
		case 1:  csh.AddCSH_Error(@3, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	}
  #else
	$$ = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
		$$ = new ShapeElement($1, atof($2), atof($3));
  #endif	
  free($2);
  free($3);
}
         | TOK_SHAPE_COMMAND TOK_NUMBER TOK_NUMBER TOK_NUMBER
{
    const int num_args = 3;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2 + @4, msg); break;
		case 1:  csh.AddCSH_Error(@3 + @4, msg); break;
		case 2:  csh.AddCSH_Error(@4, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	}
  #else
	$$ = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
		$$ = new ShapeElement($1, atof($2), atof($3), atof($4));
  #endif	
  free($2);
  free($3);
  free($4);
}
         | TOK_SHAPE_COMMAND TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER
{
    const int num_args = 4;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2 + @5, msg); break;
		case 1:  csh.AddCSH_Error(@3 + @5, msg); break;
		case 2:  csh.AddCSH_Error(@4 + @5, msg); break;
		case 3:  csh.AddCSH_Error(@5, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	}
  #else
	$$ = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
		$$ = new ShapeElement($1, atof($2), atof($3), atof($4), atof($5));
  #endif	
  free($2);
  free($3);
  free($4);
  free($5);
}
         | TOK_SHAPE_COMMAND TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER
{
    const int num_args = 5;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2 + @6, msg); break;
		case 1:  csh.AddCSH_Error(@3 + @6, msg); break;
		case 2:  csh.AddCSH_Error(@4 + @6, msg); break;
		case 3:  csh.AddCSH_Error(@5 + @6, msg); break;
		case 4:  csh.AddCSH_Error(@6, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	}
  #else
	$$ = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
		$$ = new ShapeElement($1, atof($2), atof($3), atof($4), atof($5), atof($6));
  #endif	
  free($2);
  free($3);
  free($4);
  free($5);
  free($6);
}
         | TOK_SHAPE_COMMAND TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER TOK_NUMBER
{
    const int num_args = 6;
	const int should_args = ShapeElement::GetNumArgs($1);
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
	if (should_args != num_args) {
		const string msg = ShapeElement::ErrorMsg($1, num_args);
	    switch (ShapeElement::GetNumArgs($1)) {
		case 0:  csh.AddCSH_Error(@2 + @7, msg); break;
		case 1:  csh.AddCSH_Error(@3 + @7, msg); break;
		case 2:  csh.AddCSH_Error(@4 + @7, msg); break;
		case 3:  csh.AddCSH_Error(@5 + @7, msg); break;
		case 4:  csh.AddCSH_Error(@6 + @7, msg); break;
		case 5:  csh.AddCSH_Error(@7, msg); break;
		default: csh.AddCSH_ErrorAfter(@$, msg); break;
		}
	}
  #else
	$$ = NULL;
	if (should_args > num_args)
		msc.Error.Error(MSC_POS(@$).end, ShapeElement::ErrorMsg($1, num_args).append(" Ignoring line."));
	else 
		$$ = new ShapeElement($1, atof($2), atof($3), atof($4), atof($5), atof($6), atof($7));
  #endif	
  free($2);
  free($3);
  free($4);
  free($5);
  free($6);
  free($7);
};

colordeflist: colordef
             | colordeflist TOK_COMMA colordef
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
  #endif
}
             | colordeflist TOK_COMMA 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a color name to (re-)define.");
  #endif
};

color_string: TOK_COLORDEF | string;

colordef : alpha_string TOK_EQUAL color_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_COLORNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor($3);
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[$1] = color;
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Contexts.back().colors.AddColor($1, $3, msc.Error, MSC_POS(@$));
  #endif
    free($1);
    free($3);
}
           |alpha_string TOK_EQUAL TOK_EMPH_PLUS_PLUS color_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_COLORNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_COLORDEF);
    csh.AddCSH(@4, COLOR_COLORDEF);
    ColorType color = csh.Contexts.back().Colors.GetColor("++"+string($4));
    if (color.type!=ColorType::INVALID)
        csh.Contexts.back().Colors[$1] = color;
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @4, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    msc.Contexts.back().colors.AddColor($1, "++"+string($4), msc.Error, MSC_POS(@$));
  #endif
    free($1);
    free($4);
}
           | alpha_string TOK_EQUAL 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_COLORNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE)) {
        csh.AddColorValuesToHints(false);
        csh.hintStatus = HINT_READY;
    }
    csh.AddCSH_ErrorAfter(@$, "Missing color definition.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing color definition.");
  #endif
    free($1);
}
           | alpha_string 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_COLORNAME);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing equal sign ('=') and a color definition.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing equal sign ('=') and a color definition.");
  #endif
    free($1);
};



designdef : TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_DESIGNNAME);
    csh.AddCSH(@4, COLOR_SEMICOLON);
    csh.AddCSH(@5, COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[$1] += csh.Contexts.back();
    csh.PopContext();
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::LINE_START) || 
        csh.CheckHintBetween(@4, @5, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    if (msc.Contexts.back().is_full)
        msc.Designs[$1] = msc.Contexts.back();
    else
        msc.Designs[$1] += msc.Contexts.back();
    msc.PopContext();
  #endif
    free($1);
}
           |TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON error TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_DESIGNNAME);
    csh.AddCSH(@4, COLOR_SEMICOLON);
    csh.AddCSH_Error(@5, "Could not recognize this as part of a design definition.");
    csh.AddCSH(@6, COLOR_BRACE);
    (csh.Contexts.back().full ? csh.FullDesigns : csh.PartialDesigns)[$1] = csh.Contexts.back();
    csh.PopContext();
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::LINE_START) ||
        csh.CheckHintBetween(@4, @5, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    msc.Designs[$1] += msc.Contexts.back();
    msc.PopContext();
  #endif
};


scope_open_empty: TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.PushContext(true);
  #else
    //push empty color & style sets for design definition
    msc.PushContext(true);
  #endif
};

designelementlist: designelement
                   | designelementlist TOK_SEMICOLON designelement
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SEMICOLON);
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    }
#endif
};

designelement: TOK_COMMAND_DEFCOLOR colordeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #endif
    free($1);
}
              | TOK_COMMAND_DEFCOLOR 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddColorValuesToHints(true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing color name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a color name to (re-)define.");
  #endif
    free($1);
}
			  | TOK_COMMAND_DEFSTYLE styledeflist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne(@1, @2, EHintSourceType::ATTR_NAME)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
  #endif
    free($1);
}
              | TOK_COMMAND_DEFSTYLE 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddDesignLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddStylesToHints(true, true);
        csh.hintStatus = HINT_READY;
	}
	csh.AddCSH_ErrorAfter(@$, "Missing style name to (re-)define.");
  #else
    msc.Error.Error(MSC_POS(@$).end, "Missing a style name to (re-)define.");
  #endif
    free($1);
}
              | designoptlist;

designoptlist: designopt
               | designoptlist TOK_COMMA designopt
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintBetween(@2, @3, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
               | designoptlist TOK_COMMA 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    }   
  #endif
}
               | designoptlist error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_Error(@2, "Extra stuff after design options. Maybe missing a comma?");
  #endif
};

designopt:         entity_string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
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
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.AddDesignAttribute(Attribute($1, $3, MSC_POS(@1), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
}
            | entity_string TOK_EQUAL 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing option value. Ignoring this.");
#endif
    free($1);
}
            | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues("msc", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo($3, true);
    if (msg.length())
        csh.AddCSH_Error(@3, msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc", $3, MSC_POS(@$), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_PLUS_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_EQUAL);
    csh.AddCSH(@3, COLOR_DESIGNNAME);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1)) {
        Msc::AttributeValues("msc+", csh);
        csh.hintStatus = HINT_READY;
    }
    std::string msg = csh.SetDesignTo($3, false);
    if (msg.length())
        csh.AddCSH_Error(@3, msg.c_str());
  #else
    msc.AddDesignAttribute(Attribute("msc+", $3, MSC_POS(@$), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc")) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing design name. Ignoring this.");
#endif
    free($1);
}
            | TOK_MSC TOK_PLUS_EQUAL 
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
    csh.AddCSH(@2, COLOR_EQUAL);
    if (csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME)) {
        csh.AddDesignOptionsToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, "msc+")) {
        Msc::AttributeValues($1, csh);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing design name. Ignoring this.");
#endif
    free($1);
};


parallel:    braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        $$ = new ArcParallel(&msc, $1, NULL, false, false);
    else
        $$ = NULL;
  #endif
}
         | full_arcattrlist braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @1))
        ArcParallel::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @1))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, true);
  #else 
    if ($2) {
        $$ = new ArcParallel(&msc, $2, $1, false, false);
    } else {
        $$ = NULL;
        if ($1) delete $1;
    }
  #endif
}
         | parallel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    if ($2==NULL)
        $$ = $1;
    else if ($1)
        $$ = ($1)->AddArcList($2, NULL);
    else
        $$ = new ArcParallel(&msc, $2, NULL, false, false);
  #endif
}
         | parallel full_arcattrlist braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcParallel::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, false);
  #else 
    if ($3==NULL) {
        $$ = $1;
        if ($2) delete $2;
    } else if ($1)
        $$ = ($1)->AddArcList($3, $2);
    else
        $$ = new ArcParallel(&msc, $3, $2, false, false);
  #endif
}
         | parallel full_arcattrlist 
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcParallel::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcParallel::AttributeValues(csh.hintAttrName, csh, false);
    csh.AddCSH_ErrorAfter(@2, 
        "Need an additional parallel block enclosed between '{' and '}'.");
  #else 
    $$ = $1;
    if ($2) delete $2;
    msc.Error.Error(MSC_POS(@2).end.NextChar(),
        "Missing an additional parallel block enclosed between '{' and '}' after the attributes.");
  #endif
};


//This term can return NULL (for a TOK_COMMAND_BOX without a following boxrel)
//It is also recursive, so we need to check if the 'box_list' we
//use is NULL or not. (But if the TOK_BOX_COMMAND is followed by a boxrel
//the parser will turn that to a 'first' box and we are OK.)
box_list: first_box
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    ($1)->SetLineEnd(MSC_POS(@$));
    $$ = new ArcBoxSeries($1);
  #endif
}
             | TOK_COMMAND_BOX
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF);
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), 
        "The keyword '" + string($1) +
        "' should be followed by an entity, or '--', '..', '++' or '=='. "
        "Ignoring this box (series).");
  #endif
    free($1);
}
           | TOK_COMMAND_BOX first_box
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);   
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    ($2)->ExpandLineEnd(MSC_POS(@$));
    $$ = new ArcBoxSeries($2);
  #endif
  free($1);
}
/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
           | box_list boxrel
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddBox($2);
    ($2)->AddAttributeList(NULL); //should come after AddBox
  #endif
}
           | box_list boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
  #else
    ($2)->SetLineEnd(MSC_POS2(@2, @3));
    $$ = ($1)->AddBox($2);
    ($2)->AddAttributeList($3); //should come after AddBox
  #endif
}
           | box_list boxrel braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
  #else
    ($2)->AddArcList($3)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddBox($2);
    ($2)->AddAttributeList(NULL); //should come after AddBox
  #endif
}
           | box_list braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    if ($1) {
        ArcBox *temp = new ArcBox(EArcSymbol::BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
        temp->AddArcList($2);
        $$ = ($1)->AddBox(temp);
        temp->AddAttributeList(NULL); //should come after AddBox
    } else {
        //We have emitted an error message when returning a NULL 
        //ArcBoxSeries for 'box_list', so we delete 'temp' and ignore
        //this box series entirely.
        $$ = NULL;
        delete ($2);
    }
  #endif
}
           | box_list boxrel full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else 
        csh.CheckEntityHintAfter(@4, yylloc, yychar==YYEOF);
  #else
    ($2)->AddArcList($4)->SetLineEnd(MSC_POS2(@2, @3));
    $$ = ($1)->AddBox($2);
    ($2)->AddAttributeList($3); //should come after AddBox
  #endif
}
           | box_list full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
    else     
        csh.CheckEntityHintAfter(@3, yylloc, yychar==YYEOF);
  #else
    if ($1) {
        ArcBox *temp = new ArcBox(EArcSymbol::BOX_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
        temp->AddArcList($3)->SetLineEnd(MSC_POS(@2));
        $$ = ($1)->AddBox(temp);
        temp->AddAttributeList($2); //should come after AddBox
    } else {
        //We have emitted an error message when returning a NULL 
        //ArcBoxSeries for 'box_list', so we delete 'temp' and ignore
        //this box series entirely.
        $$ = NULL;
        delete ($2);
        delete ($3);
    }
  #endif
};

mscgen_box: mscgen_boxrel 
{
  #ifndef C_S_H_IS_COMPILED
    if ($1) {
        ($1)->AddAttributeList(NULL);
        ($1)->SetLineEnd(MSC_POS(@$));
        $$ = new ArcBoxSeries($1);
    } else
        $$ = NULL;
  #endif
}
                | mscgen_boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($1) {
        ($1)->AddAttributeList($2);
        ($1)->SetLineEnd(MSC_POS(@$));
        $$ = new ArcBoxSeries($1);
    } else
        $$ = NULL;
  #endif
};

mscgen_boxlist: mscgen_box
{
  #ifndef C_S_H_IS_COMPILED
    if ($1) {
        $$ = new ArcList;
        ($$)->Append($1);
    } else
        $$ = NULL;
  #endif
}
                | mscgen_boxlist TOK_COMMA mscgen_box
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #else
    if ($3) {
        if ($1) {
            ($1)->back()->SetParallel();
            $$ = $1;
        } else 
            $$ = new ArcList;
        ($$)->Append($3);
    } else
        $$ = $1;
  #endif
};


/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
first_box:   boxrel
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->AddAttributeList(NULL);
    ($1)->SetLineEnd(MSC_POS(@$), false);
    $$ = $1;
  #endif
}
           | boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($1)->AddAttributeList($2);
    ($1)->SetLineEnd(MSC_POS(@$), false);
    $$ = ($1);
  #endif
}
           | boxrel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->AddAttributeList(NULL);
    ($1)->SetLineEnd(MSC_POS(@1), false);
    $$ = ($1)->AddArcList($2);
  #endif
}
           | boxrel full_arcattrlist_with_label braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        ArcBox::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        ArcBox::AttributeValues(csh.hintAttrName, csh);
  #else
    ($1)->SetLineEnd(MSC_POS2(@1, @2), false);
    ($1)->AddArcList($3)->AddAttributeList($2);
    $$ = ($1);
  #endif
};


first_pipe: TOK_COMMAND_PIPE boxrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne(@1, @2, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new ArcPipe($2);
    ($$)->AddAttributeList(NULL);
    ($$)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
}
             | TOK_COMMAND_PIPE error
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBeforePlusOne(@1, @2, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetweenPlusOne(@1, @2, EHintSourceType::ENTITY)) {
        csh.AddEntitiesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
  #else
    $$ = new ArcPipe($2);
    ($$)->AddAttributeList($3);
    ($$)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
};

pipe_list_no_content: first_pipe
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter(@$, yylloc, yychar==YYEOF);
  #else
    if ($1)
      $$ = new ArcPipeSeries($1);
    else
      $$ = NULL;
  #endif
}
             | pipe_list_no_content boxrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAfter(@$, yylloc, yychar==YYEOF);
#else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe($2);
    ap->SetLineEnd(MSC_POS(@2));
    if ($1)
      $$ = ($1)->AddFollowWithAttributes(ap, NULL);
    else {
      ap->AddAttributeList(NULL);
      $$ = new ArcPipeSeries(ap);
    }
  #endif
}
             | pipe_list_no_content boxrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        ArcPipe::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        ArcPipe::AttributeValues(csh.hintAttrName, csh);
    else
        csh.CheckEntityHintAfter(@$, yylloc, yychar==YYEOF);
  #else
    //($2) is never NULL: "boxrel" always return a value (except oo memory)
    ArcPipe *ap = new ArcPipe($2);
    ap->SetLineEnd(MSC_POS(@2));
    if ($1)
      $$ = ($1)->AddFollowWithAttributes(ap, $3);
    else {
      ap->AddAttributeList($3);
      $$ = new ArcPipeSeries(ap);
    }
  #endif
};

pipe_list: pipe_list_no_content
         | pipe_list_no_content braced_arclist
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintBetween(@1, @2);
  #else
    $$ = ($1)->AddArcList($2);
  #endif
};

emphrel: TOK_EMPH_PLUS_PLUS | TOK_EMPH;

boxrel:   entity_string emphrel entity_string
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
           | emphrel entity_string
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
           | entity_string emphrel
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
           | emphrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintAfter(@1, yylloc, yychar==YYEOF);
  #else
    $$ = new ArcBox($1, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
  #endif
};

mscgen_emphrel: TOK_MSCGEN_RBOX | TOK_MSCGEN_ABOX | TOK_COMMAND_BOX | TOK_COMMAND_NOTE;

mscgen_boxrel: entity_string mscgen_emphrel entity_string
{
    //mscgen compatibility: a box with no content or 
#ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
        csh.AddCSH(@2, COLOR_KEYWORD);
    else
        csh.AddCSH(@2, COLOR_KEYWORD_MSCGEN);
    csh.CheckEntityHintAtAndBefore(@2, @3);
    csh.AddCSH_EntityName(@3, $3);
#else
    ArcBox::Emscgen_compat c = ArcBox::MSCGEN_COMPAT_NONE;
    if (CaseInsensitiveEqual($2, "rbox")) c = ArcBox::MSCGEN_COMPAT_RBOX;
    else if (CaseInsensitiveEqual($2, "abox")) c = ArcBox::MSCGEN_COMPAT_ABOX;
    else if (CaseInsensitiveEqual($2, "box")) c = ArcBox::MSCGEN_COMPAT_BOX;
    else if (CaseInsensitiveEqual($2, "note")) c = ArcBox::MSCGEN_COMPAT_NOTE;
    $$ = new ArcBox(c, $1, MSC_POS(@1), $3, MSC_POS(@3), &msc);
    if (msc.mscgen_compat != EMscgenCompat::FORCE_MSCGEN)
        msc.Error.WarnMscgen(MSC_POS(@2).start, "Deprecated box symbol.", "Use '--' and attribute 'line.corner' instead.");
#endif
    free($1);
    free($2);
    free($3);
};

vertxpos: TOK_AT entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
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
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
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
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
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
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH(@4, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } 
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_SIDE, atof($4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_PLUS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
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
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH(@4, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
#else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_SIDE, atof($4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string emphrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    }
  #else
    switch ($3) {
    default:
        _ASSERT(0);
    case EArcSymbol::BOX_SOLID:
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_BY);
        break;
    case EArcSymbol::BOX_DASHED:
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_BY);
        break;
    case EArcSymbol::BOX_DOTTED:
        msc.Error.Error(MSC_POS(@3).start,
                        "unexpected '..', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
        $$ = NULL;
        break;
    case EArcSymbol::BOX_DOUBLE:
        msc.Error.Error(MSC_POS(@3).start,
                        "unexpected '==', expected '-', '--', '+' or '++'."
                        " Ignoring vertical."); 
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
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@4, $4);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore(@3, @4);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_EMPH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@4, $4);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore(@3, @4);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4));
  #endif
    free($1);
    free($2);
    free($4);
}
         | TOK_AT entity_string TOK_DASH entity_string TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@4, $4);
    csh.AddCSH(@5, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore(@3, @4);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4), atof($5));
  #endif
    free($1);
    free($2);
    free($4);
    free($5);
}
         | TOK_AT entity_string TOK_EMPH entity_string TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_EntityName(@2, $2);
    csh.AddCSH(@3, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@4, $4);
    csh.AddCSH(@5, COLOR_ATTRVALUE);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAtAndBeforePlusOne(@1, @2)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } else
        csh.CheckEntityHintAtAndBefore(@3, @4);
  #else
    $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4), atof($5));
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
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne(@1, yylloc, yychar==YYEOF)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(false);
        csh.hintStatus = HINT_READY;
    } 
    csh.AddCSH_ErrorAfter(@1, "Missing an entity name.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing an entity name.");
  #endif
    free($1);
};


empharcrel_straight: emphrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
	$$ = 0; //dummy to supress warning
  #else
    ($$).arc.type = $1;
	($$).arc.lost = EArrowLost::NOT;
	($$).dir = EDirType::INDETERMINATE;
  #endif
}
        | TOK_ASTERISK emphrel
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.CheckEntityHintBetween(@1, @2);
	$$ = 0; //dummy to supress warning	
  #else
    ($$).arc.type = $2;
	($$).arc.lost = EArrowLost::AT_SRC;
    ($$).arc.lost_pos.SetFrom(MSC_POS(@1));
	($$).dir = EDirType::INDETERMINATE;
  #endif
}
        |  emphrel TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
	$$ = 0; //dummy to supress warning	
  #else
    ($$).arc.type = $1;
	($$).arc.lost = EArrowLost::AT_SRC;
    ($$).arc.lost_pos.SetFrom(MSC_POS(@2));
	($$).dir = EDirType::INDETERMINATE;
  #endif
}
             | TOK_ASTERISK emphrel TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.CheckEntityHintBetween(@1, @2);
    $$ = 0; //dummy to supress warning	
  #else
    msc.Error.Error(MSC_POS(@3).start, "Only one loss can be specified. Ignoring second asterisk ('*').");
    ($$).arc.type = $2;
    ($$).arc.lost = EArrowLost::AT_SRC;
    ($$).arc.lost_pos.SetFrom(MSC_POS(@1));
	($$).dir = EDirType::INDETERMINATE;
  #endif
}
        | relation_from
{
  #ifdef C_S_H_IS_COMPILED
	$$ = 0; //dummy to supress warning
  #else
    ($$).arc = $1;
	($$).dir = EDirType::LEFT;
  #endif
}
        | relation_to
{
  #ifdef C_S_H_IS_COMPILED
	$$ = 0; //dummy to supress warning
  #else
    ($$).arc = $1;
	($$).dir = EDirType::RIGHT;
  #endif
}
        | relation_bidir
{
  #ifdef C_S_H_IS_COMPILED
	$$ = 0; //dummy to supress warning
  #else
    ($$).arc = $1;
	($$).dir = EDirType::BIDIR;
  #endif
};

vertrel_no_xpos: entity_string empharcrel_straight entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.AddCSH(@3, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER);
  #else
    $$ = new ArcVerticalArrow($2, $1, $3, &msc);
  #endif
    free($1);
    free($3);
}
       | empharcrel_straight entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_MARKERNAME);
    csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::MARKER);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, $2, &msc);
  #endif
    free($2);
}
       | entity_string empharcrel_straight
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    $$ = new ArcVerticalArrow($2, $1, MARKER_HERE_STR, &msc);
  #endif
    free($1);
}
       | empharcrel_straight
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::MARKER);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
  #endif
}
       | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_MARKERNAME);
    csh.CheckHintAt(@1, EHintSourceType::MARKER);
    csh.AddCSH_ErrorAfter(@1, "Missing a box or arrow symbol.");
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a box or arrow symbol.");
  #endif
    free($1);
}
       /* These are to ensure we do have a valid lookahead above */
       | empharcrel_straight entity_string TOK__NEVER__HAPPENS {free($2); $$=NULL;}
       | entity_string empharcrel_straight entity_string TOK__NEVER__HAPPENS {free($1); free($3); $$=NULL;};


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
    if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
#else
    if ($1) {
        VertXPos vxp(msc);
        $$ = ($1)->AddXpos(&vxp);
    } else
        $$ = NULL;
  #endif
} 
         | vertxpos
{
  #ifdef C_S_H_IS_COMPILED
  #else
    if ($1) {
		ArcTypePlusDir typeplusdir;
		typeplusdir.arc.type = EArcSymbol::ARC_SOLID;
		typeplusdir.arc.lost = EArrowLost::NOT;
		typeplusdir.dir = EDirType::RIGHT;
		ArcVerticalArrow *ava = new ArcVerticalArrow(typeplusdir, MARKER_HERE_STR, MARKER_HERE_STR, &msc);
		ava->AddXpos($1);
		$$ = ava;
		delete $1;
	} else
	    $$ = NULL;
  #endif
};

arcrel:       TOK_SPECIAL_ARC
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
  #else
    $$ = new ArcDivider($1, &msc);
  #endif
}
            | arcrel_arrow TOK_LOST vertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_KEYWORD);
  #else
    $$ = ($1)->AddLostPos($3, MSC_POS2(@2, @3));
  #endif
   free($2);
}
            | arcrel_arrow TOK_LOST
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter(@2, "Missing 'at' clause.");
    if (csh.CheckHintAfterPlusOne(@2, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@2).end, "Missing 'at' clause.");
    $$ = ($1);
  #endif
   free($2);
}
             | arcrel_arrow
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "lost at", 
            "Use the 'lost at' keyword to indicate that the message has been lost.", 
            EHintType::KEYWORD, true)); ///XXX Other can come?
        csh.hintStatus = HINT_READY;
    }
  #endif
    //explicit copy here to suppress bison warning (since the two types are different)
    $$ = $1;

};

arcrel_arrow: arcrel_to | arcrel_from | arcrel_bidir
             | entity_string TOK_REL_MSCGEN entity_string
{
  #ifdef C_S_H_IS_COMPILED
    //This rule can happen only in mscgen compat mode 
    //(or else the lexer does not return TOK_REL_MSCGEN
    _ASSERT(csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN);
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    csh.AddCSH(@2, COLOR_SYMBOL);
    csh.AddCSH_EntityName(@3, $3);
#else
    //This rule can happen only in mscgen compat mode 
    //(or else the lexer does not return TOK_REL_MSCGEN
    _ASSERT(msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN);
    ArrowSegmentData data;
    data.type = $2;
    data.lost = EArrowLost::NOT;
    $$ = msc.CreateArcArrow(data, $1, MSC_POS(@1), $3, true, MSC_POS(@3));
    ($$)->Indicate_Mscgen_Headless();
  #endif
    free($1);
    free($3);
}
             | entity_string TOK_DASH TOK_REL_X entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH(@2 + @3, COLOR_SYMBOL);
            csh.AddCSH_EntityName(@4, $4);
        } else {
            csh.AddCSH_Error(@2, "Not supported arrow symbol ('-').");
        }
    } else if ((@2).last_pos+1 == (@3).first_pos) {
        csh.AddCSH_Error(@2 + @3, "Unsupported mscgen arrow symbol ('-x'), use '-> *' for indicating loss instead.");
    } else {
        csh.AddCSH_Error(@2, "Not supported arrow symbol ('-').");
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (MSC_POS(@2).end.NextChar() == MSC_POS(@3).start) {
            ArrowSegmentData data;
            data.type = EArcSymbol::ARC_SOLID;
            data.lost = EArrowLost::AT_DST;
            data.lost_pos.SetFrom(MSC_POS(@3));
            $$ = msc.CreateArcArrow(data, $1, MSC_POS(@1), $4, true, MSC_POS(@4));
        } else {
            msc.Error.Error(MSC_POS(@2).start, "Unsupported arrow symbol '-'.",
                "Perhaps remove the whitespace after?");
            $$ = NULL;
        }
    } else {
        if (MSC_POS(@2).end.NextChar() == MSC_POS(@3).start) {
            msc.Error.Error(MSC_POS(@2).start, "This arrow symbol ('-x') is supported only in mscgen compatibility mode.",
                "Use an asterisk '*' instead to express a lost message (like '-> *').");
            $$ = NULL;
        } else {
            msc.Error.Error(MSC_POS(@2).start, "Unsupported arrow symbol '-'.");
            $$ = NULL;
        }
    }
  #endif
    free($1);
    free($3);
    free($4);
}
             | entity_string TOK_REL_X TOK_DASH entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH(@2 + @3, COLOR_SYMBOL);
            csh.AddCSH_EntityName(@4, $4);
        } else {
            csh.AddCSH_Error(@2, "Missing arrow symbol before 'x'. (Perhaps remove space after?)");
        }
    } else {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH_Error(@2 + @3, "Unsupported arrow symbol ('x-'), use '* <-' for indicating loss instead.");
        } else {
            csh.AddCSH_Error(@2, "Missing arrow symbol before 'x'.");
        }
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if (MSC_POS(@2).end.NextChar() == MSC_POS(@3).start) {
            ArrowSegmentData data;
            data.type = EArcSymbol::ARC_SOLID;
            data.lost = EArrowLost::AT_DST;
            data.lost_pos.SetFrom(MSC_POS(@2));
            $$ = msc.CreateArcArrow(data, $4, MSC_POS(@4), $1, false, MSC_POS(@1));
        } else {
            msc.Error.Error(MSC_POS(@2).start, "Missing arrow symbol before 'x'.",
                "Perhaps remove the whitespace after?");
            $$ = NULL;
        }
    } else {
        if (MSC_POS(@2).end.NextChar() == MSC_POS(@3).start) {
            msc.Error.Error(MSC_POS(@2).start, "This arrow symbol ('x-') is supported only in mscgen compatibility mode.",
                "Use an asterisk '*' instead to express a lost message (like '-> *').");
            $$ = NULL;
        } else {
            msc.Error.Error(MSC_POS(@2).start, "Missing arrow symbol before 'x'.");
            $$ = NULL;
        }
    }
  #endif
    free($1);
    free($2);
    free($4);
}
             | entity_string TOK_DASH TOK_REL_X 
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH(@2 + @3, COLOR_SYMBOL);
            csh.AddCSH_ErrorAfter(@2, "Missing an entity name.");
        } else {
            csh.AddCSH_Error(@2, "Not supported arrow symbol ('-').");
        }
    } else if ((@2).last_pos+1 == (@3).first_pos) {
        csh.AddCSH_Error(@2 + @3, "Unsupported mscgen arrow symbol ('-x'), use '-> *' for indicating loss instead.");
    } else {
        csh.AddCSH_Error(@2, "Not supported arrow symbol ('-').");
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN && MSC_POS(@2).end.NextChar() == MSC_POS(@3).start)
         msc.Error.Error(MSC_POS(@3).end, "Missing an entity name after the loss arrow symbol.");
    else
        msc.Error.Error(MSC_POS(@2).start, "Unsupported arrow symbol '-'.");
    $$ = NULL;
#endif
    free($1);
    free($3);
}
             | entity_string TOK_REL_X TOK_DASH 
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
    if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN) {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH(@2 + @3, COLOR_SYMBOL);
            csh.AddCSH_ErrorAfter(@2, "Missing an entity name.");
        } else {
            csh.AddCSH_Error(@2, "Missing arrow symbol before 'x'. (Perhaps remove space after?)");
        }
    } else {
        if ((@2).last_pos+1 == (@3).first_pos) {
            csh.AddCSH_Error(@2 + @3, "Unsupported arrow symbol ('x-'), use '* <-' for indicating loss instead.");
        } else {
            csh.AddCSH_Error(@2, "Missing arrow symbol before 'x'.");
        }
    }
  #else
    if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN && MSC_POS(@2).end.NextChar() == MSC_POS(@3).start)
         msc.Error.Error(MSC_POS(@3).end, "Missing an entity name after the loss arrow symbol.");
    else
        msc.Error.Error(MSC_POS(@2).start, "Missing arrow symbol before 'x'.");
    $$ = NULL;
#endif
    free($1);
    free($2);
};


arcrel_to:    entity_string relation_to entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.CheckEntityHintAt(@1);
    csh.AddCSH_EntityName(@1, $1);
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
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, true, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_to relation_to_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
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
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = msc.CreateArcArrow($2, RSIDE_ENT_STR, MSC_POS(@2), $1, false, MSC_POS(@1));
  #endif
    free($1);
}
             | arcrel_from relation_from_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
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
    csh.CheckEntityHintAfter(@2, yylloc, yychar==EOF);
  #else
    $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, true, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_bidir relation_bidir_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
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
    csh.CheckEntityHintAfter(@2, yylloc, yychar==YYEOF);
  #else
    $$ = ($1)->AddSegment($2, NULL, MSC_POS(@2), MSC_POS(@2));
  #endif
};

relation_to_cont_no_loss: TOK_REL_TO | TOK_DASH {$$=EArcSymbol::ARC_UNDETERMINED_SEGMENT;};
relation_from_cont_no_loss: TOK_REL_FROM | TOK_DASH {$$=EArcSymbol::ARC_UNDETERMINED_SEGMENT;};
relation_bidir_cont_no_loss: TOK_REL_BIDIR | TOK_DASH {$$=EArcSymbol::ARC_UNDETERMINED_SEGMENT;};

relation_to: TOK_REL_TO
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK TOK_REL_TO
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | TOK_REL_TO TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK TOK_REL_TO TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1 + @2, COLOR_SYMBOL);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

relation_from: TOK_REL_FROM
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK TOK_REL_FROM
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | TOK_REL_FROM TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK TOK_REL_FROM TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1 + @2, COLOR_SYMBOL);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

relation_bidir: TOK_REL_BIDIR
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 0;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK TOK_REL_BIDIR
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | TOK_REL_BIDIR TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
    csh.asteriskNo = 1;
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK TOK_REL_BIDIR TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1 + @2, COLOR_SYMBOL);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.asteriskNo = 2;
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring subsequent asterisks ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

relation_to_cont: relation_to_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK relation_to_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | relation_to_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@2, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@2, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK relation_to_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

relation_from_cont: relation_from_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK relation_from_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | relation_from_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@2, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@2, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK relation_from_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

relation_bidir_cont: relation_bidir_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@$, COLOR_SYMBOL);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::NOT;
  #endif
}
             | TOK_ASTERISK relation_bidir_cont_no_loss
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
}
             | relation_bidir_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@2, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@2, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    ($$).type = $1;
    ($$).lost = EArrowLost::AT_DST;
    ($$).lost_pos.SetFrom(MSC_POS(@2));
  #endif
}
             | TOK_ASTERISK relation_bidir_cont_no_loss TOK_ASTERISK
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_SYMBOL);
    if (++csh.asteriskNo == 1)
        csh.AddCSH(@1, COLOR_SYMBOL);
    else
        csh.AddCSH_Error(@1, MULTIPLE_ASTERISK_ERROR_MSG);
    csh.AddCSH_Error(@3, MULTIPLE_ASTERISK_ERROR_MSG);
  #else
    msc.Error.Error(MSC_POS(@3).start, "One arrow may be lost only once. Ignoring this asterisk ('*').");
    ($$).type = $2;
    ($$).lost = EArrowLost::AT_SRC;
    ($$).lost_pos.SetFrom(MSC_POS(@1));
  #endif
};

//This is there so that when the user starts typing "left" and is at
//"lef", we match the entity string here.
extvertxpos: extvertxpos_no_string
             | entity_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName(@1, $1);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
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
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new ExtVertXPos($1, MSC_POS(@1), $2);
  #endif
    free($1);
}
             | TOK_AT_POS
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ExtvxposDesignatorName(@1, $1);
    if (csh.CheckHintAt(@1, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddVertXPosSyntaxNonSelectableToHints(true);
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = NULL;
  #endif
    free($1);
};

 /** Here we allow entity string for 'arc' and 'rectange', symbol_string for '...' 
  * and COMMAND TEXT for 'text'*/
symbol_type_string: entity_string | symbol_string | TOK_COMMAND_TEXT;
 
symbol_command_no_attr: TOK_COMMAND_SYMBOL symbol_type_string markerrel_no_string extvertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@3, @4, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@4, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
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
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@3, @4, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@4, @5, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@5, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
#else
    $$ = new CommandSymbol(&msc, $2, $3, $4, $5);
  #endif
    free($1);
    free($2);
}
//This is to cater for when the user has entered multiple extvertpos stuff - we indicate only one is needed
                | TOK_COMMAND_SYMBOL symbol_type_string markerrel_no_string extvertxpos extvertxpos extvertxpos
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@3, @4, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@4, @5, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@4, @6, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@6, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
    csh.AddCSH_Error(@6, "Too many left/center/right specifiers here, at most two can be given.");
#else
    $$ = new CommandSymbol(&msc, $2, $3, $4, $5);
    if ($6)
        delete $6;
    msc.Error.Error(MSC_POS(@6).start, "Too many specifiers here, ignoring last one.", "At most two can be given.");
  #endif
    free($1);
    free($2);
}
                | TOK_COMMAND_SYMBOL symbol_type_string extvertxpos_no_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@2, @3, EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints(); //markers, plus left/center/right
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@3, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
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
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@2, @3, EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints(); //markers, plus left/center/right
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@3, @4, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@4, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
    }
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
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER)) {
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@3, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_FILLING;
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
    csh.AddCSH_SymbolName(@2, $2);
    csh.AddCSH_LeftRightCenterMarker(@3, $3);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    $$ = new CommandSymbol(&msc, $2, NULL, NULL, NULL);
    msc.Error.Error(MSC_POS(@3).start, "Expecting 'left', 'right', 'center' or a marker name followed by a dash. Ignoring this piece.");
  #endif
    free($1);
    free($2);
    free($3);
}
                | TOK_COMMAND_SYMBOL symbol_type_string
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_SymbolName(@2, $2);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAtAndBefore(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@2, yylloc, yychar==YYEOF, EHintSourceType::MARKER)) {
        csh.AddLeftRightCenterToHints();
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
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintAfterPlusOne(@1, yylloc, yychar==YYEOF, EHintSourceType::KEYWORD)) {
        csh.AddSymbolTypesToHints();
        csh.hintStatus = HINT_READY;
    }
  #else
    msc.Error.Error(MSC_POS(@1).end, "Missing a symbol type.", "Use 'arc', '...', 'text' or 'rectangle'.");
    $$ = NULL;
  #endif
    free($1);
};

symbol_command: symbol_command_no_attr
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        ($1)->AddAttributeList(NULL);
    $$ = ($1);
  #endif
}
                | symbol_command_no_attr full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandSymbol::AttributeNames(csh);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandSymbol::AttributeValues(csh.hintAttrName, csh);
  #else
    if ($1)
        ($1)->AddAttributeList($2);
    $$ = ($1);
  #endif
};

note:            TOK_COMMAND_NOTE TOK_AT string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    csh.AddCSH_EntityOrMarkerName(@3, $3);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @4))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @4))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
    else if (csh.CheckEntityHintAtAndBefore(@2, @3))
        csh.addMarkersAtEnd = true; //after 'at' a marker or an entity may come
  #else
    $$ = new CommandNote(&msc, $3, MSC_POS(@3));
    ($$)->AddAttributeList($4);
  #endif
    free($1);
    free($2);
    free($3);
}
               | TOK_COMMAND_NOTE full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintBetween(@1, @2, EHintSourceType::KEYWORD)) {
        csh.AddToHints(CshHint(csh.HintPrefix(COLOR_KEYWORD) + "at", NULL, EHintType::KEYWORD, true)); //XXX Other can come
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    $$ = new CommandNote(&msc);
    ($$)->AddAttributeList($2);
  #endif
    free($1);
}
               | TOK_COMMAND_NOTE TOK_AT
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter(@2, "Missing an entity or marker name.");
    csh.AddCSH_ErrorAfter(@2, "Notes need a label.");
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne(@2, yylloc, yychar==YYEOF)) 
        csh.addMarkersAtEnd = true; //after 'at' a marker or an entity may come
  #else
    $$ = NULL;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing an entity or marker name after 'at'.");
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing a label, ignoring note.", "Notes and comments must have text, specify a label.");
#endif
    free($1);
    free($2);
}
               | TOK_COMMAND_NOTE TOK_AT full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH(@2, COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter(@2, "Missing an entity or marker name.");
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckEntityHintAfterPlusOne(@2, yylloc, yychar==YYEOF))
        csh.addMarkersAtEnd = true;
    if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @3))
        CommandNote::AttributeNames(csh, true);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @3))
        CommandNote::AttributeValues(csh.hintAttrName, csh, true);
  #else
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Missing an entity or marker name after 'at'. Ignoring 'at' clause.");
    $$ = new CommandNote(&msc);
    ($$)->AddAttributeList($3);
  #endif
    free($1);
    free($2);
};

comment_command: TOK_COMMAND_COMMENT
{
  #ifndef C_S_H_IS_COMPILED
    $$= ESide::LEFT;
  #endif
    free($1);
}
               | TOK_COMMAND_ENDNOTE
{
  #ifndef C_S_H_IS_COMPILED
    $$= ESide::END;
  #endif
    free($1);
}
               | TOK_COMMAND_FOOTNOTE
{
  #ifndef C_S_H_IS_COMPILED
    $$= ESide::END;
  #endif
    free($1);
};

comment:            comment_command full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } else if (csh.CheckHintLocated(EHintSourceType::ATTR_NAME, @2))
        CommandNote::AttributeNames(csh, false);
    else if (csh.CheckHintLocated(EHintSourceType::ATTR_VALUE, @2))
        CommandNote::AttributeValues(csh.hintAttrName, csh, false);
  #else
    $$ = new CommandNote(&msc, $1);
    ($$)->AddAttributeList($2);
  #endif
}
               | comment_command
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_KEYWORD);
    csh.AddCSH_ErrorAfter(@1, "Comments and notes need a label.");
    if (csh.CheckHintAt(@1, EHintSourceType::LINE_START)) {
        csh.AddLineBeginToHints();
        csh.hintStatus = HINT_READY;
    } 
  #else
    msc.Error.Error(MSC_POS(@1).end.NextChar(), "Missing a label, ignoring comment/note.", "Notes and comments must have text, specify a label.");
    $$ = NULL;
  #endif
};

colon_string: TOK_COLON_QUOTED_STRING
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint(@1, $1, false);
    csh.AddColonLabel(@1);
#endif
    $$ = $1;
}
             | TOK_COLON_STRING
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH_ColonString_CheckAndAddEscapeHint(@1, $1, true);
	csh.AddColonLabel(@1);
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
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
  #else
    $$ = new AttributeList;
  #endif
}
                   | TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@3, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
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
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
  #else
    $$ = $2;
  #endif
}
                   | TOK_OSBRACKET error TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_Error(@2, "Could not recognize this as an attribute or style name.");
        csh.AddCSH(@3, COLOR_BRACKET);
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
  #else
    $$ = new AttributeList;
    msc.Error.Error(MSC_POS(@2).start, "Expecting an attribute or style name. Ignoring all until the closing square bracket (']').");
#endif
}
                   | TOK_OSBRACKET arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH_ErrorAfter(@2, "Missing a square bracket (']').");
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
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
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
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
        csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME);
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
        csh.CheckHintBetween(@1, @2, EHintSourceType::ATTR_NAME);
  #else
    $$ = new AttributeList;
    msc.Error.Error(MSC_POS(@2).start, "Missing ']'.");
  #endif
};

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
    csh.CheckHintBetween(@2, @3, EHintSourceType::ATTR_NAME);
  #else
    $$ = ($1)->Append($3);
  #endif
}
              | arcattrlist TOK_COMMA
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
    csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_NAME);
    csh.AddCSH_ErrorAfter(@2, "Missing attribute or style name.");
  #else
    $$ = $1;
    msc.Error.Error(MSC_POS(@2).end.NextChar(), "Expecting an attribute or style name here.");
  #endif
}
              | arcattrlist TOK_COMMA TOK__NEVER__HAPPENS;

arcattr:         alpha_string TOK_EQUAL color_string
{
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint(@3, $3, $1);
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, $3, MSC_POS(@1), MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
                | alpha_string TOK_EQUAL TOK_EMPH_PLUS_PLUS color_string
{
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint(@3+@4, (string("++")+$4).c_str(), $1);
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3+@4, EHintSourceType::ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, string("++")+$4, MSC_POS(@1), MSC_POS2(@3,@4));
  #endif
    free($1);
    free($4);
}
                | alpha_string TOK_EQUAL TOK_EMPH_PLUS_PLUS 
{
  //string=string
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_AttrValue_CheckAndAddEscapeHint(@3, "++", $1);
		csh.AddCSH_ErrorAfter(@3, "Continue with a color name or definition.");
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, "++", MSC_POS(@1), MSC_POS(@3));
  #endif
    free($1);
}
        | alpha_string TOK_EQUAL TOK_NUMBER
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
        csh.CheckHintAtAndBefore(@2, @3, EHintSourceType::ATTR_VALUE, $1);
  #else
        $$ = new Attribute($1, atof($3), MSC_POS(@$), MSC_POS(@3), $3);
  #endif
    free($1);
    free($3);
}
        | alpha_string TOK_EQUAL
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
        csh.CheckHintAfter(@2, yylloc, yychar==YYEOF, EHintSourceType::ATTR_VALUE, $1);
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
  //here we accept non alpha strings for "->" and similar style names
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName(@1, $1);
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
  #else
        $$ = new Attribute($1, MSC_POS(@$));
  #endif
    free($1);
}
 /* 'string' does not match "++", so we list it separately */
        | TOK_EMPH_PLUS_PLUS
{
  //here we accept non alpha strings for "->" and similar style names
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName(@1, "++");
        csh.CheckHintAt(@1, EHintSourceType::ATTR_NAME);
  #else
        $$ = new Attribute("++", MSC_POS(@$));
  #endif
};

vertical_shape: TOK_VERTICAL_SHAPE 
{
  #ifndef C_S_H_IS_COMPILED
    if (CaseInsensitiveEqual($1, "brace")) $$ = ArcVerticalArrow::BRACE;
    else if (CaseInsensitiveEqual($1, "bracket")) $$ = ArcVerticalArrow::BRACKET;
    else if (CaseInsensitiveEqual($1, "range")) $$ = ArcVerticalArrow::RANGE;
    else if (CaseInsensitiveEqual($1, "pointer")) $$ = ArcVerticalArrow::POINTER; 
    else {
        $$ = ArcVerticalArrow::ARROW_OR_BOX;
        _ASSERT(0);
    }
  #endif
  free($1);
} 
               | TOK_COMMAND_BOX
{
  #ifndef C_S_H_IS_COMPILED
	$$ = ArcVerticalArrow::ARROW_OR_BOX;
  #endif
  free($1);
}


//cannot be a reserved word, symbol or style name
entity_string: TOK_QSTRING
{
  #ifdef C_S_H_IS_COMPILED
	csh.AddQuotedString(@1);
  #endif
  $$ = $1;
}
               | TOK_STRING 
               | TOK_REL_X
               | TOK_SHAPE_COMMAND
{
	$$ = (char*)malloc(2);
	($$)[0] = ShapeElement::act_code[$1];
	($$)[1] = 0;
};

reserved_word_string: TOK_MSC 
    | TOK_COMMAND_HEADING | TOK_COMMAND_NUDGE | TOK_COMMAND_NEWPAGE
    | TOK_COMMAND_DEFSHAPE | TOK_COMMAND_DEFCOLOR | TOK_COMMAND_DEFSTYLE | TOK_COMMAND_DEFDESIGN
    | TOK_COMMAND_BIG | TOK_COMMAND_BOX | TOK_COMMAND_PIPE | TOK_COMMAND_MARK | TOK_COMMAND_PARALLEL | TOK_COMMAND_OVERLAP
    | TOK_VERTICAL | TOK_VERTICAL_SHAPE | TOK_AT | TOK_LOST | TOK_AT_POS
    | TOK_SHOW | TOK_HIDE | TOK_ACTIVATE | TOK_DEACTIVATE | TOK_BYE
    | TOK_COMMAND_VSPACE | TOK_COMMAND_HSPACE | TOK_COMMAND_SYMBOL | TOK_COMMAND_NOTE
    | TOK_COMMAND_COMMENT | TOK_COMMAND_ENDNOTE | TOK_COMMAND_FOOTNOTE
    | TOK_COMMAND_TITLE | TOK_COMMAND_SUBTITLE
    | TOK_MSCGEN_RBOX | TOK_MSCGEN_ABOX;

symbol_string: TOK_REL_TO  
{
    switch ($1) {
    case EArcSymbol::ARC_DOTTED: $$ = strdup(">"); break;
    case EArcSymbol::ARC_DASHED: $$ = strdup(">>"); break;
    case EArcSymbol::ARC_SOLID:  $$ = strdup("->"); break;
    case EArcSymbol::ARC_DOUBLE: $$ = strdup("=>"); break;
    case EArcSymbol::ARC_DBLDBL: $$ = strdup("=>>"); break;
    case EArcSymbol::ARC_DOUBLE2:
  #ifdef C_S_H_IS_COMPILED
        if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #else
        if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #endif
            $$ = strdup(":>"); 
        else 
            $$ = strdup("==>");
        break;
    default: _ASSERT(0);
    }
}
       | TOK_REL_FROM      
{
    switch ($1) {
    case EArcSymbol::ARC_DOTTED: $$ = strdup("<"); break;
    case EArcSymbol::ARC_DASHED: $$ = strdup("<<"); break;
    case EArcSymbol::ARC_SOLID:  $$ = strdup("<-"); break;
    case EArcSymbol::ARC_DOUBLE: $$ = strdup("<="); break;
    case EArcSymbol::ARC_DBLDBL: $$ = strdup("=>>"); break;
    case EArcSymbol::ARC_DOUBLE2:
  #ifdef C_S_H_IS_COMPILED
        if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #else
        if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #endif
            $$ = strdup("<:"); 
        else 
            $$ = strdup("<==");
        break;
    default: _ASSERT(0);
    }
}

       | TOK_REL_BIDIR      
{
    switch ($1) {
    case EArcSymbol::ARC_DOTTED: $$ = strdup("<>"); break;
    case EArcSymbol::ARC_DASHED: $$ = strdup("<<>>"); break;
    case EArcSymbol::ARC_SOLID:  $$ = strdup("<->"); break;
    case EArcSymbol::ARC_DOUBLE: $$ = strdup("<=>"); break;
    case EArcSymbol::ARC_DBLDBL: $$ = strdup("=>>"); break;
    case EArcSymbol::ARC_DOUBLE2:
  #ifdef C_S_H_IS_COMPILED
        if (csh.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #else
        if (msc.mscgen_compat == EMscgenCompat::FORCE_MSCGEN)
  #endif
            $$ = strdup("<:>"); 
        else 
            $$ = strdup("<==>");
        break;
    default: _ASSERT(0);
    }
}
       | TOK_REL_MSCGEN
{
    //This can only come in mscgen mode
    switch ($1) {
    case EArcSymbol::ARC_DOTTED: $$ = strdup(".."); break;
    case EArcSymbol::ARC_SOLID:  $$ = strdup("--"); break;
    case EArcSymbol::ARC_DOUBLE: $$ = strdup("=="); break;
    case EArcSymbol::ARC_DOUBLE2: $$ = strdup("::"); break;
    default: _ASSERT(0);
    }
}
       | TOK_SPECIAL_ARC
{
    switch ($1) {
    case EArcSymbol::DIV_DIVIDER:  $$ = strdup("---"); break;
    case EArcSymbol::DIV_DISCO:    $$ = strdup("..."); break;
    case EArcSymbol::DIV_VSPACE:   $$ = strdup("|||"); break;
    default: _ASSERT(0);
    }
}
       | TOK_EMPH
{
    switch ($1) {
    case EArcSymbol::BOX_SOLID:  $$ = strdup("--"); break;
    case EArcSymbol::BOX_DASHED: $$ = strdup("++"); break; //will likely not happen due to special handling in TOK_COLORDEF
    case EArcSymbol::BOX_DOTTED: $$ = strdup(".."); break;
    case EArcSymbol::BOX_DOUBLE: $$ = strdup("=="); break;
    default: _ASSERT(0);
    }
};

alpha_string: entity_string | reserved_word_string;

string: alpha_string | symbol_string | TOK_STYLE_NAME;

scope_open: TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@1, COLOR_BRACE);
    csh.PushContext();
    if (csh.CheckHintAfter(@1, yylloc, yychar==YYEOF, EHintSourceType::LINE_START)) {
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
    csh.AddCSH(@1, COLOR_BRACE);
  #else
    std::pair<bool, double> hscale = msc.Contexts.back().hscale;
    $$ = msc.PopContext();
    if (hscale.first)
        msc.Contexts.back().hscale = hscale;
  #endif
};


%%


/* END OF FILE */
