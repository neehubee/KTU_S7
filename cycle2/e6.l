%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%
"for"           { return FOR; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }
"="             { return ASSIGN; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }
"=="            { return EQ; }
"++"            { return INC; }
"--"            { return DEC; }

[0-9]+          { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return ID; }

[ \t\n]         ;   // ignore whitespace
.               { return yytext[0]; }
%%

int yywrap() { return 1; }
