%locations
%pure-parser
%lex-param {yyscan_t *yyscanner}
%parse-param{YYMSC_RESULT_TYPE &RESULT}
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
#include "arcs.h" //Needed for MSC_XXX declarations

//redefine default loc action for CSH
#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do								\
      if (YYID (N))                                             \
	{							\
        (Current).first_pos = YYRHSLOC (Rhs, 1).first_pos;	\
	  (Current).last_pos  = YYRHSLOC (Rhs, N).last_pos;	\
	}						        \
      else							\
	{							\
	  (Current).first_pos = (Current).last_pos   =		\
	    YYRHSLOC (Rhs, 0).last_pos;				\
	}							\
    while (YYID (0))

/* yyerror
 *  Error handling function.  Do nothing for CSH
 */
void yyerror(YYLTYPE*loc, Csh &csh, void *yyscanner, const char *str)
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
       TOK_VERTICAL TOK_AT
%union
{
    char                          *str;
    CHAR_IF_CSH(Msc)              *msc;
    CHAR_IF_CSH(ArcBase)          *arcbase;
    CHAR_IF_CSH(ArcList)          *arclist;
    CHAR_IF_CSH(ArcArrow)         *arcarrow;
    CHAR_IF_CSH(ArcEmphasis)      *arcemph;
    CHAR_IF_CSH(ArcParallel)      *arcparallel;
    CHAR_IF_CSH(MscArcType)        arctype;
    CHAR_IF_CSH(EntityDef)        *entity;
    CHAR_IF_CSH(EntityDefList)    *entitylist;
    CHAR_IF_CSH(Attribute)        *attrib;
    CHAR_IF_CSH(AttributeList)    *attriblist;
    CHAR_IF_CSH(VertXPos)         *vertxpos;
    std::list<std::string>        *stringlist;
};

%type <msc>        msc
%type <arcbase>    arcrel arc arc_with_parallel complete_arc opt vertrel
%type <arcarrow>   arcrel_to arcrel_from arcrel_bidir
%type <arcemph>    emphrel emphasis_list pipe_emphasis first_emphasis pipe_def pipe_def_list
%type <arcparallel>parallel
%type <arclist>    top_level_arclist arclist braced_arclist mscenclosed optlist
%type <entity>     entity first_entity
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
%type <str>        entity_string reserved_word_string string symbol_string colon_string
                   TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_COLON_QUOTED_STRING
                   TOK_STYLE_NAME TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE
                   TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
                   TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
                   TOK_COMMAND_PARALLEL TOK_COMMAND_MARK
                   TOK_NUMBER TOK_BOOLEAN TOK_VERTICAL TOK_AT
%type <stringlist> tok_stringlist

%destructor {if (!C_S_H) delete $$;} vertrel
%destructor {if (!C_S_H) delete $$;} vertxpos
%destructor {if (!C_S_H) delete $$;} arcrel arc arc_with_parallel complete_arc opt
%destructor {if (!C_S_H) delete $$;} arcrel_to arcrel_from arcrel_bidir
%destructor {if (!C_S_H) delete $$;} emphrel first_emphasis emphasis_list pipe_emphasis pipe_def pipe_def_list
%destructor {if (!C_S_H) delete $$;} parallel
%destructor {if (!C_S_H) delete $$;} top_level_arclist arclist braced_arclist mscenclosed optlist
%destructor {if (!C_S_H) delete $$;} entity first_entity entitylist
%destructor {if (!C_S_H) delete $$;} arcattr arcattrlist full_arcattrlist full_arcattrlist_with_label tok_stringlist
%destructor {free($$);}  entity_string reserved_word_string string symbol_string
%destructor {free($$);}  TOK_STRING TOK_QSTRING TOK_COLON_STRING TOK_COLON_QUOTED_STRING TOK_STYLE_NAME
%destructor {free($$);}  TOK_MSC TOK_COMMAND_BIG TOK_COMMAND_PIPE TOK_COMMAND_PARALLEL
%destructor {free($$);}  TOK_COMMAND_DEFCOLOR TOK_COMMAND_DEFSTYLE TOK_COMMAND_DEFDESIGN
%destructor {free($$);}  TOK_COMMAND_NEWPAGE TOK_COMMAND_HEADING TOK_COMMAND_NUDGE
%destructor {free($$);}  TOK_NUMBER TOK_BOOLEAN TOK_COMMAND_MARK

%%

msc:
{
    //no action for empty file
}
             | top_level_arclist
{
  #ifdef C_S_H_IS_COMPILED
  #else
        msc.AddArcs($1);
  #endif
};

top_level_arclist: arclist
                 | arclist error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_ERROR);
  #else
        $$ = $1;
  #endif
}
                 | arclist TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_ERROR);
  #else
        $$ = $1;
  #endif
}
                 | arclist TOK_CCBRACKET top_level_arclist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_ERROR);
  #else
        //Merge $3 into $1
        ($1)->splice(($1)->end(), *($3));
        delete ($3);
        $$ = $1;
        msc.Error.Error(MSC_POS(@3).start, "Unexpected '}'.");
  #endif
};


braced_arclist: scope_open arclist scope_close
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACE);
        csh.AddCSH(@3, COLOR_BRACE);
  #else
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
  #endif
}
            | scope_open arclist error scope_close
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACE);
        csh.AddCSH(@3, COLOR_ERROR);
        csh.AddCSH(@4, COLOR_BRACE);
  #else
        $$ = $2;
  #endif
    yyerrok;
}
            | scope_open arclist error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACE);
        csh.AddCSH(@3, COLOR_ERROR);
  #else
        $$ = $2;
        //Do not pop context, as the missing scope_close would have done
  #endif
};


arclist:    complete_arc
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = new ArcList;
  #endif
}
            | arclist complete_arc
{
  #ifndef C_S_H_IS_COMPILED
    if ($2)
        ($1)->Append($2);     /* Add to existing list */
    $$ = ($1);
  #endif
}
            | mscenclosed
{
  #ifndef C_S_H_IS_COMPILED
    $$ = $1;
  #endif
}
            | arclist mscenclosed
{
  #ifndef C_S_H_IS_COMPILED
    //Merge $2 into $1
    ($1)->splice(($1)->end(), *($2));
    delete ($2);
    $$ = $1;
  #endif
};

//here we do not use open_scope, but only TOK_OCBRACKET, because no new
//scope is opened for an msc {...} arclist: anything you do there spills over
//to later sections
mscenclosed: msckey TOK_OCBRACKET arclist TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_BRACE);
        csh.AddCSH(@4, COLOR_BRACE);
  #else
        $$ = $3;
  #endif
}
              |msckey TOK_OCBRACKET TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_BRACE);
        csh.AddCSH(@3, COLOR_BRACE);
  #else
        $$ = new ArcList;
  #endif
};
              |msckey TOK_OCBRACKET arclist error TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_BRACE);
        csh.AddCSH(@4, COLOR_ERROR);
        csh.AddCSH(@5, COLOR_BRACE);
  #else
        $$ = $3;
  #endif
    yyerrok;
}
              |msckey TOK_OCBRACKET arclist error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_BRACE);
        csh.AddCSH(@4, COLOR_ERROR);
  #else
        $$ = $3;
  #endif
};

msckey:       TOK_MSC
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
  #endif
    free($1);
}
              | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_DESIGNNAME);
  #else
        msc.AddDesignAttribute(Attribute("msc", $3, MSC_POS(@1), MSC_POS(@3)));
  #endif
    free($1);
    free($3);
};

complete_arc: TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SEMICOLON);
  #else
        $$=NULL;
  #endif
}
              | error TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_ERROR);
        csh.AddCSH(@2, COLOR_SEMICOLON);
  #else
        $$=NULL;
  #endif
}
              |arc_with_parallel TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SEMICOLON);
  #else
        if ($1) ($1)->SetLineEnd(MSC_POS(@$));
        $$=$1;
  #endif
}
              |arc_with_parallel error TOK_SEMICOLON
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_ERROR);
        csh.AddCSH(@3, COLOR_SEMICOLON);
  #else
        if ($1) ($1)->SetLineEnd(MSC_POS(@$));
        $$=$1;
  #endif
    yyerrok;
};

arc_with_parallel: arc
                  | TOK_COMMAND_PARALLEL arc
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        ($2)->SetParallel();
        $$ = $2;
  #endif
    free($1);
}

arc:            arcrel
              | arcrel full_arcattrlist_with_label
{
  #ifndef C_S_H_IS_COMPILED
    $$ = ($1)->AddAttributeList($2);
  #endif
}
              |TOK_COMMAND_BIG arcrel
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        $$ = msc.CreateArcBigArrow($2);
        delete $2;
  #endif
    free($1);
}
              | TOK_COMMAND_BIG arcrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        //Returns NULL, if BIG is before a self-pointing arrow
        ArcBigArrow *arrow = msc.CreateArcBigArrow($2);
        if (arrow) arrow->AddAttributeList($3);
        $$ = arrow;
        delete $2;
  #endif
    free($1);
}
              |TOK_VERTICAL vertrel
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        $$ = $2;
  #endif
    free($1);
}
              | TOK_VERTICAL vertrel full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        $$ = ($2)->AddAttributeList($3);
  #endif
    free($1);
}
              | full_arcattrlist_with_label
{
  #ifndef C_S_H_IS_COMPILED
    $$ = (new ArcDivider(MSC_ARC_VSPACE, &msc))->AddAttributeList($1);
  #endif
}
              | first_entity
{
  #ifndef C_S_H_IS_COMPILED
    $$ = new CommandEntity((new EntityDefList)->Append($1), &msc);
  #endif
}
            |  first_entity TOK_COMMA entitylist
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #else
    $$ = new CommandEntity(($3)->Prepend($1), &msc);
  #endif
}
              | optlist
{
  #ifndef C_S_H_IS_COMPILED
    /* If there were arcs defined by the options (e.g., background)
     * enclose them in a single item parallel element. */
    if ($1) {
        $$ = (new ArcParallel(&msc))->AddArcList($1);
    } else
        $$ = NULL;
  #endif
}
              | emphasis_list
{
  #ifndef C_S_H_IS_COMPILED
    $$ = $1; //to remove warning for downcast
  #endif
}
              | parallel
{
  #ifndef C_S_H_IS_COMPILED
    $$ = $1;
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
        $$ = new CommandEntity(NULL, &msc);
  #endif
    free($1);
}
              | TOK_COMMAND_HEADING full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
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
        $$ = new ArcDivider(MSC_COMMAND_NUDGE, &msc);
  #endif
    free($1);
}
	      | TOK_COMMAND_NUDGE  full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
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
  #else
        $$ = new CommandMark($2, MSC_POS(@$), &msc);
  #endif
    free($1);
    free($2);
}
              | TOK_COMMAND_MARK string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH(@1, COLOR_MARKERNAME);
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
        $$ = new CommandNewpage(&msc);
  #endif
    free($1);
}
              | TOK_COMMAND_NEWPAGE full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        $$ = (new CommandNewpage(&msc))->AddAttributeList($2);
  #endif
    free($1);
};

optlist:     opt
{
  #ifndef C_S_H_IS_COMPILED
    if ($1)
        $$ = (new ArcList)->Append($1); /* New list */
    else
        $$ = NULL;
  #endif
}
           | optlist TOK_COMMA opt
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_COMMA);
  #else
    if ($3) {
        if ($1)
            $$ = ($1)->Append($3);     /* Add to existing list */
        else
            $$ = (new ArcList)->Append($3); /* New list */
    }
    $$ = $1;
  #endif
};

opt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
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
  #else
        Attribute a($1, $3, MSC_POS(@1), MSC_POS(@3));
        MscFillAttr fill;
        fill.Empty();
        if (a.StartsWith("background") && fill.AddAttribute(a, &msc, STYLE_OPTION)) {
            $$ = new CommandNewBackground(&msc, fill);
        } else {
            msc.AddAttribute(a);
            $$ = NULL;
        }
  #endif
    free($1);
    free($3);
}
            | TOK_MSC TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
  #else
        msc.AddAttribute(Attribute("msc", $3, MSC_POS(@$), MSC_POS(@3)));
        $$ = NULL;
  #endif
    free($1);
    free($3);
};

entitylist:   entity
{
  #ifndef C_S_H_IS_COMPILED
    $$ = (EntityDefList*)((new EntityDefList)->Append($1));
  #endif
}
            | entitylist TOK_COMMA entity
{
  #ifdef C_S_H_IS_COMPILED
    csh.AddCSH(@2, COLOR_COMMA);
  #else
    $$ = (EntityDefList*)(($1)->Append($3));
  #endif
};

entity:       entity_string full_arcattrlist_with_label
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
  #else
        $$ = (new EntityDef($1, &msc))->AddAttributeList($2, &msc);
        ($$)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
  #else
        $$ = (new EntityDef($1, &msc))->AddAttributeList(NULL, &msc);
        ($$)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
};

first_entity:  entity_string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_KeywordOrEntity(@1, $1);
  #else
        $$ = (new EntityDef($1, &msc))->AddAttributeList($2, &msc);
        ($$)->SetLineEnd(MSC_POS(@$));
  #endif
    free($1);
}
            | entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_KeywordOrEntity(@1, $1);
  #else
        $$ = (new EntityDef($1, &msc))->AddAttributeList(NULL, &msc);
        ($$)->SetLineEnd(MSC_POS(@$));
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
  #else
    for (std::list<std::string>::iterator i = ($1)->begin(); i!=($1)->end(); i++) {
        MscStyle style = msc.StyleSets.top().GetStyle(*i);
        AttributeList::iterator j=($2)->begin();
        while (j!=($2)->end())
           style.AddAttribute(**(j++), &msc);
        msc.StyleSets.top()[*i] = style;
    }
    delete($1);
    delete($2);
  #endif
};

tok_stringlist : string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_STYLENAME);
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
  #else
        msc.ColorSets.top().AddColor($1, $3, msc.Error, MSC_POS(@$));
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
  #else
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
}
           |TOK_STRING scope_open_empty designelementlist TOK_SEMICOLON error TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_DESIGNNAME);
        csh.AddCSH(@2, COLOR_BRACE);
        csh.AddCSH(@4, COLOR_SEMICOLON);
        csh.AddCSH(@5, COLOR_ERROR);
        csh.AddCSH(@6, COLOR_BRACE);
  #else
    //if closing brace missing, still do the design definition
    //cope_open_empty pushed an empty color & style set onto the stack
    //then designelementlist added color & style definitions, now we harvest those
    Design &design = msc.Designs[$1];
    design.colors = msc.ColorSets.top();
    design.styles = msc.StyleSets.top();
    design.hscale = msc.hscale;
    msc.hscale = msc.saved_hscale;
    msc.PopContext();
  #endif
};


scope_open_empty: TOK_OCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
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
        csh.AddCSH(@2, COLOR_ERROR);
  #endif
};

designopt:         entity_string TOK_EQUAL TOK_BOOLEAN
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_OPTIONNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH(@3, COLOR_ATTRVALUE);
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
        csh.AddCSH(@3, COLOR_ATTRVALUE);
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

emphasis_list: first_emphasis
{
  #ifndef C_S_H_IS_COMPILED
    $$ = $1;
  #endif
}
           | pipe_emphasis
{
    $$ = $1;
}
/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
           | emphasis_list emphrel
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow(($2)->ChangeStyleForFollow());
  #endif
}
           | emphasis_list emphrel full_arcattrlist_with_label
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->ChangeStyleForFollow()->AddAttributeList($3);
    ($2)->SetLineEnd(MSC_POS2(@2, @3));
    $$ = ($1)->AddFollow($2);
  #endif
}
           | emphasis_list emphrel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow(($2)->AddArcList($3)->ChangeStyleForFollow());
  #endif
}
           | emphasis_list braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
    temp->AddArcList($2)->ChangeStyleForFollow($1);
    $$ = ($1)->AddFollow(temp);
  #endif
}
           | emphasis_list emphrel full_arcattrlist_with_label braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS2(@2, @3));
    ($2)->AddArcList($4)->ChangeStyleForFollow()->AddAttributeList($3);
    $$ = ($1)->AddFollow($2);
  #endif
}
           | emphasis_list full_arcattrlist_with_label braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ArcEmphasis *temp = new ArcEmphasis(MSC_EMPH_UNDETERMINED_FOLLOW, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
    temp->SetLineEnd(MSC_POS(@2));
    temp->AddArcList($3)->ChangeStyleForFollow($1)->AddAttributeList($2);
    $$ = ($1)->AddFollow(temp);
  #endif
};

/* ALWAYS Add Arclist before Attributes. AddArcList changes default attributes!! */
first_emphasis:   emphrel
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->SetLineEnd(MSC_POS(@$));
    $$ = $1;
  #endif
}
           | emphrel full_arcattrlist_with_label
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->AddAttributeList($2);
    ($1)->SetLineEnd(MSC_POS(@$));
    $$ = ($1);
  #endif
}
           | emphrel braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->SetLineEnd(MSC_POS(@1));
    $$ = ($1)->AddArcList($2);
  #endif
}
           | emphrel full_arcattrlist_with_label braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->SetLineEnd(MSC_POS2(@1, @2));
    ($1)->AddArcList($3)->AddAttributeList($2);
    $$ = ($1);
  #endif
};

pipe_def:    emphrel
{
  #ifndef C_S_H_IS_COMPILED
    $$ = ($1)->SetPipe();
  #endif
}
           | emphrel full_arcattrlist_with_label
{
  #ifndef C_S_H_IS_COMPILED
    ($1)->SetPipe()->AddAttributeList($2);
    $$ = ($1);
  #endif
};

pipe_def_list: TOK_COMMAND_PIPE pipe_def
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
  #else
        ($2)->SetLineEnd(MSC_POS(@$));
        $$ = $2;
  #endif
    free($1);
}
             | pipe_def_list pipe_def
{
  #ifndef C_S_H_IS_COMPILED
    ($2)->SetLineEnd(MSC_POS(@2));
    $$ = ($1)->AddFollow($2);
  #endif
}
             | pipe_def_list TOK_COMMAND_PIPE pipe_def
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_KEYWORD);
  #else
        ($3)->SetLineEnd(MSC_POS2(@2, @3));
        $$ = ($1)->AddFollow($3);
  #endif
    free($2);
};

/* You can add arclist after setpipe safely */
pipe_emphasis: pipe_def_list
             | pipe_def_list braced_arclist
{
  #ifndef C_S_H_IS_COMPILED
    $$ = ($1)->AddArcList($2);
  #endif
};

emphrel:   entity_string TOK_EMPH entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = new ArcEmphasis($2, $1, MSC_POS(@1), $3, MSC_POS(@3), &msc);
  #endif
    free($1);
    free($3);
}
           | TOK_EMPH entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@2, $2);
  #else
        $$ = new ArcEmphasis($1, NULL, MSC_POS(@1), $2, MSC_POS(@2), &msc);
  #endif
    free($2);
}
           | entity_string TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = new ArcEmphasis($2, $1, MSC_POS(@1), NULL, MSC_POS(@2), &msc);
  #endif
    free($1);
}
           | TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
  #else
        $$ = new ArcEmphasis($1, NULL, MSC_POS(@1), NULL, MSC_POS(@1), &msc);
  #endif
};

vertxpos: TOK_AT entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH_EntityName(@2, $2);
  #else
        $$ = new VertXPos(msc, $2, MSC_POS(@2));
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_DASH
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH_EntityName(@2, $2);
        csh.AddCSH(@3, COLOR_SYMBOL);
  #else
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_LEFT_SIDE);
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_PLUS
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH_EntityName(@2, $2);
        csh.AddCSH(@3, COLOR_SYMBOL);
  #else
        $$ = new VertXPos(msc, $2, MSC_POS(@2), VertXPos::POS_RIGHT_SIDE);
  #endif
    free($1);
    free($2);
}
         | TOK_AT entity_string TOK_EMPH
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_KEYWORD);
        csh.AddCSH_EntityName(@2, $2);
        csh.AddCSH(@3, COLOR_SYMBOL);
  #else
        switch ($3) {
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
        csh.AddCSH_EntityName(@2, $2);
        csh.AddCSH(@3, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@4, $4);
  #else
        $$ = new VertXPos(msc, $2, MSC_POS(@2), $4, MSC_POS(@4), VertXPos::POS_CENTER);
  #endif
    free($1);
    free($2);
    free($4);
};

empharcrel_straight: TOK_EMPH | relation_to | relation_bidir;

vertrel: entity_string empharcrel_straight entity_string vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_MARKERNAME);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH(@3, COLOR_MARKERNAME);
  #else
        $$ = new ArcVerticalArrow($2, $1, $3, $4, &msc);
        delete $4;
  #endif
    free($1);
    free($3);
}
       | empharcrel_straight entity_string vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH(@2, COLOR_MARKERNAME);
  #else
        $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, $2, $3, &msc);
        delete $3;
  #endif
    free($2);
}
       | entity_string empharcrel_straight vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_MARKERNAME);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = new ArcVerticalArrow($2, $1, MARKER_HERE_STR, $3, &msc);
        delete $3;
  #endif
    free($1);
}
       | empharcrel_straight vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
  #else
        $$ = new ArcVerticalArrow($1, MARKER_HERE_STR, MARKER_PREV_PARALLEL_STR, $2, &msc);
        delete $2;
  #endif
}
       | entity_string relation_from entity_string vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_MARKERNAME);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH(@3, COLOR_MARKERNAME);
  #else
        $$ = new ArcVerticalArrow($2, $3, $1, $4, &msc);
        delete $4;
  #endif
    free($1);
    free($3);
}
       | relation_from entity_string vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH(@2, COLOR_MARKERNAME);
  #else
        $$ = new ArcVerticalArrow($1, $2, MARKER_HERE_STR, $3, &msc);
        delete $3;
  #endif
    free($2);
}
       | entity_string relation_from vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_MARKERNAME);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = new ArcVerticalArrow($2, MARKER_HERE_STR, $1, $3, &msc);
        delete $3;
  #endif
    free($1);
}
       | relation_from vertxpos
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
  #else
    $$ = new ArcVerticalArrow($1, MARKER_PREV_PARALLEL_STR, MARKER_HERE_STR, $2, &msc);
    delete $2;
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
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), $3, MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
            | relation_to entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@2, $2);
  #else
        $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, MSC_POS(@1), $2, MSC_POS(@2));
  #endif
    free($2);
}
            | entity_string relation_to
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_to relation_to_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = ($1)->AddSegment($3, MSC_POS(@3), true, MSC_POS2(@2, @3));
  #endif
    free($3);
}
            | arcrel_to relation_to_cont
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
    $$ = ($1)->AddSegment(NULL, MSC_POS(@2), true, MSC_POS(@2));
  #endif
};


arcrel_from:    entity_string relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = msc.CreateArcArrow($2, $3, MSC_POS(@3), $1, MSC_POS(@1));
  #endif
    free($1);
    free($3);
}
             | relation_from entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@2, $2);
  #else
        $$ = msc.CreateArcArrow($1, $2, MSC_POS(@2), LSIDE_ENT_STR, MSC_POS(@1));
  #endif
    free($2);
}
             | entity_string relation_from
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = msc.CreateArcArrow($2, RSIDE_ENT_STR, MSC_POS(@2), $1, MSC_POS(@1));
  #endif
    free($1);
}
             | arcrel_from relation_from_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = ($1)->AddSegment($3, MSC_POS(@3), false, MSC_POS2(@2, @3));
  #endif
    free($3);
}
             | arcrel_from relation_from_cont
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
    $$ = ($1)->AddSegment(NULL, MSC_POS(@2), false, MSC_POS(@2));
  #endif
};

arcrel_bidir:    entity_string relation_bidir entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), $3, MSC_POS(@3));
  #endif
    free($1);
    free($3);
}
            | relation_bidir entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@2, $2);
  #else
        $$ = msc.CreateArcArrow($1, LSIDE_ENT_STR, MSC_POS(@1), $2, MSC_POS(@2));
  #endif
    free($2);
}
            | entity_string relation_bidir
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_EntityName(@1, $1);
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
        $$ = msc.CreateArcArrow($2, $1, MSC_POS(@1), RSIDE_ENT_STR, MSC_POS(@2));
  #endif
    free($1);
}
            | arcrel_bidir relation_bidir_cont entity_string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
        csh.AddCSH_EntityName(@3, $3);
  #else
        $$ = ($1)->AddSegment($3, MSC_POS(@3), true, MSC_POS2(@2, @3));
  #endif
    free($3);
}
            | arcrel_bidir relation_bidir_cont
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_SYMBOL);
  #else
    $$ = ($1)->AddSegment(NULL, MSC_POS(@2), true, MSC_POS(@2));
  #endif
};

relation_to:   TOK_REL_SOLID_TO | TOK_REL_DOUBLE_TO | TOK_REL_DASHED_TO | TOK_REL_DOTTED_TO;
relation_from: TOK_REL_SOLID_FROM | TOK_REL_DOUBLE_FROM | TOK_REL_DASHED_FROM | TOK_REL_DOTTED_FROM;
relation_bidir: TOK_REL_SOLID_BIDIR | TOK_REL_DOUBLE_BIDIR | TOK_REL_DASHED_BIDIR | TOK_REL_DOTTED_BIDIR;

relation_to_cont : relation_to | TOK_DASH;
relation_from_cont : relation_from | TOK_DASH;
relation_bidir_cont : relation_bidir | TOK_DASH;


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
              | full_arcattrlist colon_string
{
  #ifdef C_S_H_IS_COMPILED
  #else
        $$ = ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol()));
  #endif
    free($2);
}
              | full_arcattrlist colon_string full_arcattrlist
{
  #ifdef C_S_H_IS_COMPILED
  #else
        ($1)->Append(new Attribute("label", $2, MSC_POS(@2), MSC_POS(@2).IncStartCol()));
        //Merge $3 at the end of $1
        ($1)->splice(($1)->end(), *($3));
        delete ($3); //empty list now
        $$ = $1;
  #endif
    free($2);
}
              | full_arcattrlist;


full_arcattrlist: TOK_OSBRACKET TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@2, COLOR_BRACKET);
  #else
    $$ = new AttributeList;
  #endif
}
                   | TOK_OSBRACKET arcattrlist TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@3, COLOR_BRACKET);
  #else
    $$ = $2;
  #endif
}
                   | TOK_OSBRACKET arcattrlist error TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@3, COLOR_ERROR);
        csh.AddCSH(@4, COLOR_BRACKET);
  #else
    $$ = $2;
  #endif
}
                   | TOK_OSBRACKET error TOK_CSBRACKET
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@2, COLOR_ERROR);
        csh.AddCSH(@3, COLOR_BRACKET);
  #else
    $$ = new AttributeList;
  #endif
}
                   | TOK_OSBRACKET arcattrlist error
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@1, COLOR_BRACKET);
        csh.AddCSH(@3, COLOR_ERROR);
  #else
    $$ = $2;
  #endif
}

arcattrlist:    arcattr
{
  #ifndef C_S_H_IS_COMPILED
    $$ = (new AttributeList)->Append($1);
  #endif
}
              | arcattrlist TOK_COMMA arcattr
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH(@2, COLOR_COMMA);
  #else
    $$ = ($1)->Append($3);
  #endif
};

arcattr:         string TOK_EQUAL string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_AttrName(@1, $1, COLOR_ATTRNAME);
        csh.AddCSH(@2, COLOR_EQUAL);
        csh.AddCSH_AttrValue(@3, $3, $1);
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
  #else
        $$ = new Attribute($1, (char*)NULL, MSC_POS(@$), MSC_POS(@$));
  #endif
    free($1);
}
	    | string
{
  #ifdef C_S_H_IS_COMPILED
        csh.AddCSH_StyleOrAttrName(@1, $1);
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
                       TOK_COMMAND_MARK | TOK_AT

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
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.PushContext();
  #endif
};

scope_close: TOK_CCBRACKET
{
  #ifdef C_S_H_IS_COMPILED
  #else
    msc.PopContext();
  #endif
};


%%


/* END OF FILE */
