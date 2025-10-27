#include <stdio.h>
#include <string.h>

void gen_code_for_operator(char *inp, char operator, char *reg)
{
    char temp[100];
    int i = 0, j = 0;

    while (inp[i] != '\0')
    {
        if (inp[i] == operator)
        {
            // Print the 3-address instruction
            printf("%c\t%c\t%c\t%c\n", operator, *reg, inp[i - 1], inp[i + 1]);

            // Replace left operand with register
            temp[j - 1] = *reg;

            // Skip operator and right operand
            i += 2;

            // Move to next temporary register (Z → Y → X …)
            (*reg)--;

            continue;
        }

        temp[j++] = inp[i++];
    }

    temp[j] = '\0';
    strcpy(inp, temp);
}

void gen_code(char *inp)
{
    // Operator precedence: /, *, +, -, =
    char reg = 'Z';

    gen_code_for_operator(inp, '/', &reg);
    gen_code_for_operator(inp, '*', &reg);
    gen_code_for_operator(inp, '+', &reg);
    gen_code_for_operator(inp, '-', &reg);
    gen_code_for_operator(inp, '=', &reg);
}

int main()
{
    char inp[100];
    printf("Enter expression: ");
    scanf("%s", inp);

    printf("\nOp\tDestn\tArg1\tArg2\n");
    printf("--------------------------------\n");

    gen_code(inp);

    return 0;
}
