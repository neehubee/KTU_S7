%{
#include "y.tab.h"
%}

%%
[a-zA-Z_][a-zA-Z_0-9]*   return letter;   // identifiers
[0-9]+                   return digit;    // digits
.                        return yytext[0];
\n                       return 0;        // end of input
%%

int yywrap() { return 1; }
