%{
#include <stdio.h>
%}

%%

.*nee.*      { printf("Rejected: contains 'nee'\n"); }
.*           { printf("Accepted: %s\n", yytext); }

%%

int yywrap(void) {
    return 1;
}

int main(void) {
    yylex();
    return 0;
}
