//This file is used if we do parsing for language and not for CSH
//Need to include this for YYLTYPE
#include "msc.h"

#ifndef CHAR_IF_CSH
#define CHAR_IF_CSH(A) A
#endif

#include "language.h"

#define MSC_POS(A) MSC_POS2(A,A)
#define MSC_POS2(A, B) file_line_range(                                    \
            file_line(msc.current_file, (A).first_line, (A).first_column), \
            file_line(msc.current_file, (B).last_line, (B).last_column))

void msc_jump_line(YYLTYPE *loc);
char *msc_remove_head_tail_whitespace(char *s);
char* msc_process_colon_string(const char *s, YYLTYPE *loc, unsigned file_no);

std::string ConvertEmphasisToBox(const std::string &style, const YYLTYPE *loc, Msc &msc);

inline bool str2bool(const char*s)
{
   return (s[0]!='n' && s[0]!='N') || (s[1]!='o' && s[1]!='O');
}

/* yyerror
 *  Error handling function.  The TOK_XXX names are substituted for more
 *  understandable values that make more sense to the user.
 */
void yyerror(YYLTYPE*loc, Msc &msc, void *yyscanner, const char *str);

