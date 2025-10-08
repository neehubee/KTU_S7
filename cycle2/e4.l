%{
#include "y.tab.h"
#include <stdlib.h>
#include <stdio.h>

int yylex(void);
int yyerror(const char *s);
%}

%option noyywrap

%%
[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
[+\-*/()]           { return yytext[0]; }
[ \t]               ;       // ignore spaces and tabs
\n                  { return 0; }   // end of input
.                   { return yytext[0]; }
%%
