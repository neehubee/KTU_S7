#include <stdio.h>
#include <string.h>

int main() {
    char stack[20], ip[20], opt[10][10][2], ter[10];
    int i, j, k, n, top = 0, col = -1, row = -1;

    // Initialize arrays
    for (i = 0; i < 20; i++) {
        stack[i] = '\0';
        ip[i] = '\0';
    }

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            opt[i][j][0] = '\0';
        }
    }

    // Input terminals
    printf("Enter the number of terminals: ");
    scanf("%d", &n);

    printf("Enter the terminals (no spaces, e.g., +*i$): ");
    scanf("%s", ter);

    // Input precedence relations
    printf("\nEnter the operator precedence table (<, =, > or -):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Relation between (%c, %c): ", ter[i], ter[j]);
            scanf("%s", opt[i][j]);
        }
    }

    // Display operator precedence table
    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n\t");
    for (i = 0; i < n; i++)
        printf("%c\t", ter[i]);
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%c\t", ter[i]);
        for (j = 0; j < n; j++)
            printf("%c\t", opt[i][j][0]);
        printf("\n");
    }

    // Input string
    stack[top] = '$';
    printf("\nEnter the input string (end with $): ");
    scanf("%s", ip);

    i = 0;
    printf("\nSTACK\t\tINPUT\t\tACTION\n");
    printf("%s\t\t%s\t\t", stack, ip);

    while (1) {
        // Find precedence relation indices
        col = row = -1;
        for (k = 0; k < n; k++) {
            if (stack[top] == ter[k]) col = k;
            if (ip[i] == ter[k]) row = k;
        }

        if (col == -1 || row == -1) {
            printf("\nError: Invalid symbol!\n");
            break;
        }

        // Acceptance condition
        if (stack[top] == '$' && ip[i] == '$') {
            printf("Accept\n");
            break;
        } 
        // Shift condition (< or =)
        else if (opt[col][row][0] == '<' || opt[col][row][0] == '=') {
            stack[++top] = ip[i];
            printf("Shift %c\n", ip[i]);
            i++;
        } 
        // Reduce condition (>)
        else if (opt[col][row][0] == '>') {
            // Pop until a '<' is found
            while (top > 0 && stack[top] != '<')
                top--;
            if (top > 0)
                top--;  // Remove the '<' marker
            printf("Reduce\n");
        } 
        // Invalid entry
        else {
            printf("\nString is not accepted!\n");
            break;
        }

        // Display updated stack and input
        for (k = 0; k <= top; k++)
            printf("%c", stack[k]);
        printf("\t\t");
        for (k = i; k < strlen(ip); k++)
            printf("%c", ip[k]);
        printf("\t\t");
    }

    return 0;
}
