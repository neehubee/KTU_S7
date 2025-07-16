#include <stdio.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_INPUTS 10
#define MAX_DFA_STATES 100

// NFA transition function: Fa[state][input][list of next states]
int Fa[MAX_STATES][MAX_INPUTS][MAX_STATES];
int fa_count[MAX_STATES][MAX_INPUTS]; // Number of next states for each transition

// DFA transition function: dfa[state][input] = next_state
int dfa[MAX_DFA_STATES][MAX_INPUTS];

// Final states of NFA and DFA
int nfa_final_states[MAX_STATES];
int dfa_final_states[MAX_DFA_STATES];

// Keep track of DFA states, each DFA state is a subset of NFA states
// Represented as bitmask for convenience
int dfa_states[MAX_DFA_STATES];
int dfa_state_count = 0;

int nfa_state_count = 0;
int input_count = 0;

// Check if a DFA state (bitmask) is already recorded, return index or -1
int find_dfa_state(int subset) {
    for (int i = 0; i < dfa_state_count; i++) {
        if (dfa_states[i] == subset)
            return i;
    }
    return -1;
}

// Add a new DFA state (subset), return its index
int add_dfa_state(int subset) {
    dfa_states[dfa_state_count] = subset;
    dfa_final_states[dfa_state_count] = 0; // Initialize as not final

    // Check if any NFA final states are included
    for (int i = 0; i < nfa_state_count; i++) {
        if ((subset & (1 << i)) && nfa_final_states[i]) {
            dfa_final_states[dfa_state_count] = 1;
            break;
        }
    }

    return dfa_state_count++;
}

// Given a subset of NFA states and an input symbol, find the resulting subset
int move(int subset, int input) {
    int result = 0;
    for (int state = 0; state < nfa_state_count; state++) {
        if (subset & (1 << state)) {
            // For each next state of NFA from this state on input
            for (int k = 0; k < fa_count[state][input]; k++) {
                int next_state = Fa[state][input][k];
                if (next_state != -1) {
                    result |= (1 << next_state);
                }
            }
        }
    }
    return result;
}

int main() {
    printf("Enter the number of NFA states (max %d): ", MAX_STATES);
    scanf("%d", &nfa_state_count);

    printf("Enter the number of input symbols (max %d): ", MAX_INPUTS);
    scanf("%d", &input_count);

    // Initialize transitions with -1 (no transition)
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_INPUTS; j++) {
            fa_count[i][j] = 0;
            for (int k = 0; k < MAX_STATES; k++) {
                Fa[i][j][k] = -1;
            }
        }
    }

    printf("Enter NFA transitions in the format: state input next_state1 next_state2 ... -1\n");
    printf("For example: 0 0 1 2 -1\n");
    printf("Enter -1 -1 -1 to stop inputting transitions.\n");

    while (1) {
        int state, input;
        scanf("%d %d", &state, &input);
        if (state == -1 && input == -1)
            break;

        if (state < 0 || state >= nfa_state_count || input < 0 || input >= input_count) {
            printf("Invalid state or input. Try again.\n");
            continue;
        }

        int next_state;
        fa_count[state][input] = 0;
        while (1) {
            scanf("%d", &next_state);
            if (next_state == -1)
                break;
            Fa[state][input][fa_count[state][input]++] = next_state;
        }
    }

    // Initialize final states array
    for (int i = 0; i < nfa_state_count; i++)
        nfa_final_states[i] = 0;

    printf("Enter number of final states: ");
    int final_count;
    scanf("%d", &final_count);
    printf("Enter final states: ");
    for (int i = 0; i < final_count; i++) {
        int fs;
        scanf("%d", &fs);
        if (fs >= 0 && fs < nfa_state_count)
            nfa_final_states[fs] = 1;
    }

    // Initialize DFA transitions to -1
    for (int i = 0; i < MAX_DFA_STATES; i++)
        for (int j = 0; j < input_count; j++)
            dfa[i][j] = -1;

    // Subset construction algorithm
    // Start state of DFA is the subset containing only NFA start state 0
    int start_subset = 1 << 0;
    add_dfa_state(start_subset);

    int processed = 0;
    while (processed < dfa_state_count) {
        int current_subset = dfa_states[processed];

        for (int input = 0; input < input_count; input++) {
            int next_subset = move(current_subset, input);
            if (next_subset == 0) {
                dfa[processed][input] = -1;
            } else {
                int index = find_dfa_state(next_subset);
                if (index == -1) {
                    index = add_dfa_state(next_subset);
                }
                dfa[processed][input] = index;
            }
        }
        processed++;
    }

    // Print DFA transition table
    printf("\nDFA Transition Table:\n");
    printf("State\\Input |");
    for (int input = 0; input < input_count; input++)
        printf(" %d ", input);
    printf("\n");
    printf("------------");
    for (int input = 0; input < input_count; input++)
        printf("----");
    printf("\n");

    for (int i = 0; i < dfa_state_count; i++) {
        printf("     %2d     |", i);
        for (int input = 0; input < input_count; input++) {
            if (dfa[i][input] == -1)
                printf(" - ");
            else
                printf(" %2d", dfa[i][input]);
        }
        printf("\n");
    }

    // Print final states of DFA
    printf("\nFinal states of DFA:\n");
    for (int i = 0; i < dfa_state_count; i++) {
        if (dfa_final_states[i])
            printf("%d ", i);
    }
    printf("\n");

    return 0;
}

