#if !defined(PARSERHELPER_H)
#define PARSERHELPER_H

#ifndef HAVE_UNISTD_H
#define YY_NO_UNISTD_H
extern int isatty(int);
#endif


#ifndef YYMSC_RESULT_TYPE
#error You must set YYMSC_RESULT_TYPE before you include parserhelper.h
#endif

//Parser and lexer related stuff
typedef struct
{
    void              *yyscanner;
    char              *buf;
    int                pos;
    int                length;
    YYMSC_RESULT_TYPE *RESULT;
} parse_parm;

#define YY_EXTRA_TYPE   parse_parm *

int     yylex(YYSTYPE *, YYLTYPE *, void *);
int     yylex_init(void **);
int     yylex_destroy(void *);
void    yyset_extra(YY_EXTRA_TYPE, void *);
int     yyparse(YYMSC_RESULT_TYPE&, void *);
int     yyget_lineno(void* scanner);

#endif //PARSERHELPER_H
