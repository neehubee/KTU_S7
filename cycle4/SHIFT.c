#include <stdio.h>
#include <string.h>

char input[20], stack[50];
int i, j, k, c = 0;
char action[20];

void check();

int main()
{
    printf("GRAMMAR:\n");
    printf("E -> E+E\nE -> E*E\nE -> (E)\nE -> id\n\n");

    printf("Enter input string: ");
    gets(input);  // e.g., id+id*id

    c = strlen(input);
    strcpy(action, "SHIFT->");

    printf("\nStack\t\tInput\t\tAction\n");
    printf("-----\t\t-----\t\t------\n");

    i = 0;
    j = 0;

    while (j < c)
    {
        if (input[j] == 'i' && input[j + 1] == 'd')
        {
            stack[i] = input[j];
            stack[i + 1] = input[j + 1];
            stack[i + 2] = '\0';
            input[j] = input[j + 1] = ' ';
            printf("$%s\t\t%s$\t\t%sid", stack, input, action);
            check();
            i++;
            j += 2;
        }
        else
        {
            stack[i] = input[j];
            stack[i + 1] = '\0';
            input[j] = ' ';
            printf("\n$%s\t\t%s$\t\t%ssymbol", stack, input, action);
            check();
            i++;
            j++;
        }
    }

    // final reduction
    check();

    if (strcmp(stack, "E") == 0)
        printf("\n\n String ACCEPTED.\n");
    else
        printf("\n\nString REJECTED.\n");

    return 0;
}

void check()
{
    strcpy(action, "REDUCE TO E");

    // replace id → E
    for (k = 0; k < c; k++)
    {
        if (stack[k] == 'i' && stack[k + 1] == 'd')
        {
            stack[k] = 'E';
            stack[k + 1] = '\0';
            printf("\n$%s\t\t%s$\t\t%s", stack, input, action);
        }
    }

    // replace E+E → E
    for (k = 0; k < c; k++)
    {
        if (stack[k] == 'E' && stack[k + 1] == '+' && stack[k + 2] == 'E')
        {
            stack[k] = 'E';
            stack[k + 1] = stack[k + 2] = '\0';
            printf("\n$%s\t\t%s$\t\t%s", stack, input, action);
            k = -1; // restart scanning
        }
    }

    // replace E*E → E
    for (k = 0; k < c; k++)
    {
        if (stack[k] == 'E' && stack[k + 1] == '*' && stack[k + 2] == 'E')
        {
            stack[k] = 'E';
            stack[k + 1] = stack[k + 2] = '\0';
            printf("\n$%s\t\t%s$\t\t%s", stack, input, action);
            k = -1;
        }
    }

    // replace (E) → E
    for (k = 0; k < c; k++)
    {
        if (stack[k] == '(' && stack[k + 1] == 'E' && stack[k + 2] == ')')
        {
            stack[k] = 'E';
            stack[k + 1] = stack[k + 2] = '\0';
            printf("\n$%s\t\t%s$\t\t%s", stack, input, action);
            k = -1;
        }
    }
}
