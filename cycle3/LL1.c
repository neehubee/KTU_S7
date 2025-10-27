#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXIN 256

char input[MAXIN];
int ip_ptr = 0;

/* Forward declarations */
void E(void);
void E_prime(void);
void T(void);
void T_prime(void);
void F(void);

void syntax_error(const char *msg)
{
    printf("\nSyntax error: %s at input position %d (char '%c')\n",
           msg, ip_ptr, input[ip_ptr] ? input[ip_ptr] : '$');
    exit(1);
}

void advance(void)
{
    if (input[ip_ptr] != '\0') ip_ptr++;
}

/* E -> T E' */
void E(void)
{
    printf("E -> T E'\n");
    T();
    E_prime();
}

/* E' -> + T E' | ε */
void E_prime(void)
{
    if (input[ip_ptr] == '+')
    {
        printf("E' -> + T E'\n");
        advance();     /* consume + */
        T();
        E_prime();
    }
    else
    {
        printf("E' -> ε\n");
        /* do nothing (epsilon) */
    }
}

/* T -> F T' */
void T(void)
{
    printf("T -> F T'\n");
    F();
    T_prime();
}

/* T' -> * F T' | ε */
void T_prime(void)
{
    if (input[ip_ptr] == '*')
    {
        printf("T' -> * F T'\n");
        advance();     /* consume * */
        F();
        T_prime();
    }
    else
    {
        printf("T' -> ε\n");
    }
}

/* F -> ( E ) | i */
void F(void)
{
    if (input[ip_ptr] == 'i' || input[ip_ptr] == 'I')
    {
        printf("F -> i\n");
        advance();     /* consume i */
    }
    else if (input[ip_ptr] == '(')
    {
        printf("F -> ( E )\n");
        advance();     /* consume ( */
        E();
        if (input[ip_ptr] == ')')
        {
            advance(); /* consume ) */
        }
        else
        {
            syntax_error("expected ')'");
        }
    }
    else
    {
        syntax_error("expected 'i' or '('");
    }
}

int main(void)
{
    printf("Grammar (no left recursion):\n");
    printf(" E -> T E'\n E' -> + T E' | ε\n T -> F T'\n T' -> * F T' | ε\n F -> ( E ) | i\n\n");

    printf("Enter input (use 'i' for id, e.g. id+id*id -> i+i*i):\n> ");

    if (!fgets(input, sizeof(input), stdin))
        return 0;

    /* remove trailing newline */
    size_t L = strlen(input);
    if (L > 0 && input[L-1] == '\n') input[L-1] = '\0';

    /* Optionally, you may compress "id" into single 'i' token before parsing.
       If user inputs "id+id*id" we can convert each occurrence "id" -> 'i'. */
    /* Let's support both formats: if user typed 'id' convert to 'i' */
    char tmp[MAXIN]; int ti = 0;
    for (size_t p = 0; p < strlen(input); )
    {
        if (input[p] == 'i' && input[p+1] == 'd')
        {
            tmp[ti++] = 'i';
            p += 2;
        }
        else
        {
            tmp[ti++] = input[p++];
        }
    }
    tmp[ti] = '\0';
    strcpy(input, tmp);
    ip_ptr = 0;

    printf("\nProductions used (top-down):\n");
    E();

    if (input[ip_ptr] == '\0')
    {
        printf("\nParsing completed. Input ACCEPTED.\n");
    }
    else
    {
        printf("\nExtra symbols after parsing at position %d: '%s'\n", ip_ptr, &input[ip_ptr]);
        printf("Input REJECTED.\n");
    }

    return 0;
}
