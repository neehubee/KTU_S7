#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n;
char prod[10][10];
char result[20];
int m;

void first(char c);
void follow(char c);
void add_to_result(char c);

int main() {
    int i, choice;
    char c, ch;

    printf("Enter the number of productions: ");
    scanf("%d", &n);
    printf("Enter the productions (e.g., E=TR, R=+TR, R=$):\n");
    for (i = 0; i < n; i++) {
        scanf("%s", prod[i]);
    }

    do {
        m = 0;
        printf("\nEnter the element whose FIRST & FOLLOW is to be found: ");
        scanf(" %c", &c);

        first(c);
        printf("FIRST(%c) = { ", c);
        for (i = 0; i < m; i++)
            printf("%c ", result[i]);
        printf("}\n");

        m = 0; // reset for FOLLOW
        follow(c);
        printf("FOLLOW(%c) = { ", c);
        for (i = 0; i < m; i++)
            printf("%c ", result[i]);
        printf("}\n");

        printf("\nContinue? (1 for yes / 0 for no): ");
        scanf("%d", &choice);
    } while (choice == 1);

    return 0;
}

void add_to_result(char c) {
    for (int i = 0; i < m; i++) {
        if (result[i] == c)
            return; // already present
    }
    result[m++] = c;
}

void first(char c) {
    if (!isupper(c)) {
        add_to_result(c);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (prod[i][0] == c) {
            if (prod[i][2] == '$')
                add_to_result('$');
            else if (!isupper(prod[i][2]))
                add_to_result(prod[i][2]);
            else
                first(prod[i][2]);
        }
    }
}

void follow(char c) {
    if (prod[0][0] == c)
        add_to_result('$'); // $ in FOLLOW of start symbol

    for (int i = 0; i < n; i++) {
        for (int j = 2; j < strlen(prod[i]); j++) {
            if (prod[i][j] == c) {
                if (prod[i][j + 1] != '\0') {
                    if (!isupper(prod[i][j + 1]))
                        add_to_result(prod[i][j + 1]);
                    else
                        first(prod[i][j + 1]);
                } else if (c != prod[i][0]) {
                    follow(prod[i][0]);
                }
            }
        }
    }
}
