#include <stdio.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 5

int n_states, n_symbols;
char finals[MAX_STATES];
int dfa[MAX_STATES][MAX_SYMBOLS];
char partitions[MAX_STATES][MAX_STATES];
int opt_dfa[MAX_STATES][MAX_SYMBOLS];

// Print DFA transition table
void print_dfa(int table[][MAX_SYMBOLS], int states, int symbols, char *final_states) {
    printf("\n--- DFA TRANSITION TABLE ---\n");
    printf("State | ");
    for (int i = 0; i < symbols; i++) printf("%c ", '0' + i);
    printf("\n------+");
    for (int i = 0; i < symbols; i++) printf("--");
    printf("\n");

    for (int i = 0; i < states; i++) {
        printf(" %c   | ", 'A' + i);
        for (int j = 0; j < symbols; j++) {
            printf("%c ", table[i][j]);
        }
        printf("\n");
    }

    printf("Final states: %s\n", final_states);
}

// Load a sample DFA
void load_sample_dfa() {
    dfa[0][0] = 'B'; dfa[0][1] = 'C'; // A
    dfa[1][0] = 'E'; dfa[1][1] = 'F'; // B
    dfa[2][0] = 'A'; dfa[2][1] = 'A'; // C
    dfa[3][0] = 'F'; dfa[3][1] = 'E'; // D
    dfa[4][0] = 'D'; dfa[4][1] = 'F'; // E
    dfa[5][0] = 'D'; dfa[5][1] = 'E'; // F

    strcpy(finals, "EF");

    n_states = 6;
    n_symbols = 2;
}

// Find which partition a state belongs to
int find_partition(char state, int num_partitions) {
    for (int i = 0; i < num_partitions; i++) {
        if (strchr(partitions[i], state)) {
            return i;
        }
    }
    return -1;
}

// Minimize DFA
int minimize_dfa() {
    int non_final = 0, final_count = 0;

    // Initial partition: non-final states
    for (int i = 0; i < n_states; i++) {
        if (!strchr(finals, 'A' + i)) {
            partitions[0][non_final++] = 'A' + i;
        }
    }
    partitions[0][non_final] = '\0';

    // Initial partition: final states
    for (int i = 0; i < strlen(finals); i++) {
        partitions[1][final_count++] = finals[i];
    }
    partitions[1][final_count] = '\0';

    int num_partitions = 2;
    printf("Initial partitions: [%s] [%s]\n", partitions[0], partitions[1]);

    int changed = 1;
    int iteration = 1;

    while (changed && iteration <= 10) {
        changed = 0;
        printf("Iteration %d:\n", iteration);

        for (int p = 0; p < num_partitions; p++) {
            if (strlen(partitions[p]) <= 1) continue;

            char first_state = partitions[p][0];
            int first_idx = first_state - 'A';

            for (int i = 1; partitions[p][i]; i++) {
                char curr_state = partitions[p][i];
                int curr_idx = curr_state - 'A';
                int distinguishable = 0;

                for (int sym = 0; sym < n_symbols; sym++) {
                    char next1 = dfa[first_idx][sym];
                    char next2 = dfa[curr_idx][sym];

                    int part1 = find_partition(next1, num_partitions);
                    int part2 = find_partition(next2, num_partitions);

                    if (part1 != part2) {
                        distinguishable = 1;
                        break;
                    }
                }

                if (distinguishable) {
                    printf(" Splitting partition %d: %c and %c are distinguishable\n", p, first_state, curr_state);

                    // Move curr_state to a new partition
                    int new_part = num_partitions;
                    partitions[new_part][0] = curr_state;
                    partitions[new_part][1] = '\0';

                    // Remove curr_state from current partition
                    for (int j = i; partitions[p][j]; j++) {
                        partitions[p][j] = partitions[p][j + 1];
                    }

                    num_partitions++;
                    changed = 1;
                    break; // restart current iteration
                }
            }
        }

        iteration++;
    }

    printf("Final partitions (%d states):\n", num_partitions);
    for (int i = 0; i < num_partitions; i++) {
        printf(" %d: [%s]\n", i, partitions[i]);
    }

    // Build minimized DFA
    for (int i = 0; i < num_partitions; i++) {
        char rep = partitions[i][0];
        int rep_idx = rep - 'A';

        for (int sym = 0; sym < n_symbols; sym++) {
            char next = dfa[rep_idx][sym];
            int next_part = find_partition(next, num_partitions);
            opt_dfa[i][sym] = 'A' + next_part;
        }
    }

    return num_partitions;
}

// Get final states of minimized DFA
char* get_new_finals(int num_partitions) {
    static char new_finals[MAX_STATES];
    int count = 0;

    for (int i = 0; i < num_partitions; i++) {
        for (int j = 0; partitions[i][j]; j++) {
            if (strchr(finals, partitions[i][j])) {
                new_finals[count++] = 'A' + i;
                break;
            }
        }
    }

    new_finals[count] = '\0';
    return new_finals;
}

int main() {
    printf("DFA Minimization Program\n");

    load_sample_dfa();

    printf("\nORIGINAL DFA:");
    print_dfa(dfa, n_states, n_symbols, finals);

    printf("\nMinimizing DFA...\n");
    int opt_states = minimize_dfa();

    char *new_finals = get_new_finals(opt_states);

    printf("\nMINIMIZED DFA:");
    print_dfa(opt_dfa, opt_states, n_symbols, new_finals);

    printf("\nReduction: %d states -> %d states\n", n_states, opt_states);

    return 0;
}
