#include <stdio.h>
#include <string.h>

int states, symbols, transitions;
int trans[10][10][10];     // trans[state][symbol][next_state]
int epsilon[10][10];       // epsilon closures
int final[10];             // final states

void computeEpsilonClosure(int state, int closure[]) {
    closure[state] = 1;
    for (int i = 0; i < states; i++) {
        // epsilon transition assumed to be last symbol in alpha
        if (trans[state][symbols - 1][i] && !closure[i]) {
            computeEpsilonClosure(i, closure);
        }
    }
}

void printSet(int set[]) {
    printf("{");
    for (int i = 0; i < states; i++) {
        if (set[i])
            printf("q%d ", i);
    }
    printf("}");
}

int main() {
    char alpha[10];
    int start, numFinal;

    printf("Enter number of states: ");
    scanf("%d", &states);

    printf("Enter number of symbols (including epsilon): ");
    scanf("%d", &symbols);

    printf("Enter symbols (use e for epsilon): ");
    for (int i = 0; i < symbols; i++) {
        scanf(" %c", &alpha[i]);
    }

    printf("Enter start state: ");
    scanf("%d", &start);

    printf("Enter number of final states: ");
    scanf("%d", &numFinal);

    printf("Enter final states: ");
    for (int i = 0; i < numFinal; i++) {
        int f;
        scanf("%d", &f);
        final[f] = 1;
    }

    printf("Enter number of transitions: ");
    scanf("%d", &transitions);

    printf("Enter transitions (from symbol to):\n");
    for (int i = 0; i < transitions; i++) {
        int from, to;
        char symbol;
        scanf("%d %c %d", &from, &symbol, &to);

        int sym_index = -1;
        for (int j = 0; j < symbols; j++) {
            if (alpha[j] == symbol) {
                sym_index = j;
                break;
            }
        }

        if (sym_index != -1)
            trans[from][sym_index][to] = 1;
        else
            printf("Invalid symbol %c ignored.\n", symbol);
    }

    // Compute epsilon closures
    for (int i = 0; i < states; i++) {
        computeEpsilonClosure(i, epsilon[i]);
    }

    printf("\nEquivalent NFA without epsilon:\n");

    printf("Start state: ");
    printSet(epsilon[start]);
    printf("\n");

    printf("Transitions:\n");
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < symbols - 1; j++) { // exclude epsilon
            int result[10] = {0};

            // For each state in epsilon closure of i
            for (int k = 0; k < states; k++) {
                if (epsilon[i][k]) {
                    // Add epsilon closure of all states reachable by symbol j
                    for (int l = 0; l < states; l++) {
                        if (trans[k][j][l]) {
                            for (int m = 0; m < states; m++) {
                                if (epsilon[l][m])
                                    result[m] = 1;
                            }
                        }
                    }
                }
            }

            printSet(epsilon[i]);
            printf(" on %c -> ", alpha[j]);
            printSet(result);
            printf("\n");
        }
    }

    printf("\nFinal states: ");
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < states; j++) {
            if (epsilon[i][j] && final[j]) {
                printSet(epsilon[i]);
                break;
            }
        }
    }

    printf("\n");
    return 0;
}
