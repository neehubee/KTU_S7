#include <stdio.h>
#include <string.h>

#define MAX 20

int n, a, s;                 // states, alphabets, start
int f[MAX];                  // list of final states
int t;                       // number of transitions

// T[from][symbol_index][k] = to (k-th target)
int T[MAX][MAX][MAX];
int Tcount[MAX][MAX];        // number of targets for T[from][symbol]

// DFA states as boolean sets: DFA[dfa_index][n] (1 if original state present)
int DFA[MAX][MAX];
int dfaCount = 0;

char alpha[MAX];

int main() {
    int i, j, k, from, to;
    char sym;

    // initialize arrays
    memset(T, 0, sizeof(T));
    memset(Tcount, 0, sizeof(Tcount));
    memset(DFA, 0, sizeof(DFA));
    memset(f, -1, sizeof(f)); // fill with -1 so unused entries are distinguishable

    printf("States, Alphabets, Start: ");
    if (scanf("%d %d %d", &n, &a, &s) != 3) {
        printf("Invalid input.\n");
        return 1;
    }

    if (n > MAX || a > MAX) {
        printf("MAX (%d) exceeded for states or alphabets.\n", MAX);
        return 1;
    }

    printf("Alphabets: ");
    for (i = 0; i < a; i++) {
        scanf(" %c", &alpha[i]); // leading space to skip newline
    }

    int finCount;
    printf("Final count & states: ");
    if (scanf("%d", &finCount) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    if (finCount > MAX) finCount = MAX;

    for (i = 0; i < finCount; i++) {
        if (scanf("%d", &f[i]) != 1) {
            printf("Invalid input for final states.\n");
            return 1;
        }
    }

    printf("Transitions count: ");
    if (scanf("%d", &t) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Transitions (from symbol to):\n");
    for (i = 0; i < t; i++) {
        if (scanf("%d %c %d", &from, &sym, &to) != 3) {
            printf("Invalid transition input.\n");
            return 1;
        }
        // find symbol index
        int symIndex = -1;
        for (j = 0; j < a; j++) {
            if (alpha[j] == sym) {
                symIndex = j;
                break;
            }
        }
        if (symIndex == -1) {
            printf("Warning: symbol '%c' not in alphabet, transition ignored.\n", sym);
            continue;
        }
        if (from < 0 || from >= n || to < 0 || to >= n) {
            printf("Warning: transition with invalid state ignored.\n");
            continue;
        }
        T[from][symIndex][ Tcount[from][symIndex]++ ] = to;
    }

    // initialize DFA with start state set
    DFA[0][s] = 1;
    dfaCount = 1;

    // subset construction (BFS-like): iterate while new DFA states are added
    for (i = 0; i < dfaCount; i++) {
        for (j = 0; j < a; j++) {
            int newSet[MAX] = {0};

            // for each NFA state present in DFA[i], add its T[state][j] targets
            for (k = 0; k < n; k++) {
                if (DFA[i][k]) {
                    for (int l = 0; l < Tcount[k][j]; l++) {
                        int targ = T[k][j][l];
                        newSet[targ] = 1;
                    }
                }
            }

            // if newSet is empty, we still store/compare it (could be sink)
            int found = -1;
            for (int m = 0; m < dfaCount; m++) {
                if (memcmp(DFA[m], newSet, sizeof(newSet)) == 0) {
                    found = m;
                    break;
                }
            }

            if (found == -1) {
                // add new DFA state
                memcpy(DFA[dfaCount], newSet, sizeof(newSet));
                found = dfaCount;
                dfaCount++;
                if (dfaCount >= MAX) {
                    printf("Reached DFA state limit (%d). Stopping.\n", MAX);
                    break;
                }
            }
            // (Optional) You could store the transition DFA[i] --alpha[j]--> found in a table.
        }
        if (dfaCount >= MAX) break;
    }

    // print DFA states (each as a set of NFA states)
    printf("\nDFA States:\n");
    for (i = 0; i < dfaCount; i++) {
        printf("Q%d = {", i);
        for (j = 0; j < n; j++) {
            if (DFA[i][j]) printf("q%d ", j);
        }
        printf("}\n");
    }

    // determine final DFA states (those that contain any NFA final state)
    printf("\nFinal DFA States: ");
    for (i = 0; i < dfaCount; i++) {
        int isFinal = 0;
        for (j = 0; j < n && !isFinal; j++) {
            if (DFA[i][j]) {
                for (k = 0; k < finCount; k++) {
                    if (f[k] == j) {
                        isFinal = 1;
                        break;
                    }
                }
            }
        }
        if (isFinal) printf("Q%d ", i);
    }
    printf("\n");

    return 0;
}
