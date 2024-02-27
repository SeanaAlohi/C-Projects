#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// PART 1: DFA SIMULATION:
//DFA structure definition
typedef struct DFA
{
    int states;            // # of states
    int initialState;      // the initial state
    int acceptingState;    // an accepting state
    int currentState;      // the current state
    char *alphabet;        // the total alphabet
    int **transitionTable; // transition table
    enum
    {
        EXACT_CSC,
        CONTAINS_END,
        STARTS_WITH_VOWEL,
        EVEN_BINARY
    } dfaType; // Type of DFA
} DFA;

//Creating any DFA
DFA *createDFA(int states, int initialState, int acceptingState, char *alphabet, int dfaType)
{
    DFA *dfa = (DFA *)malloc(sizeof(DFA));
    dfa->states = states;
    dfa->initialState = initialState;
    dfa->acceptingState = acceptingState; // Store the single accepting state
    dfa->alphabet = strdup(alphabet);
    dfa->dfaType = dfaType; // Set the type of DFA

    // Initialize the transition table
    dfa->transitionTable = (int **)malloc(states * sizeof(int *));
    for (int i = 0; i < states; i++)
    {
        dfa->transitionTable[i] = (int *)malloc(strlen(alphabet) * sizeof(int));
        memset(dfa->transitionTable[i], -1, strlen(alphabet) * sizeof(int)); // Initialize to -1
    }

    return dfa;
}

// Setting the transitions for a DFA's transition table
void setTransition(DFA *dfa, int fromState, char input, int toState)
{
    char *pos = strchr(dfa->alphabet, input);
    if (pos != NULL)
    {
        int index = pos - dfa->alphabet;
        dfa->transitionTable[fromState][index] = toState;
    }
}

// DFA memory allocation
void freeDFA(DFA *dfa)
{
    for (int i = 0; i < dfa->states; i++)
    {
        free(dfa->transitionTable[i]);
    }
    free(dfa->transitionTable);
    free(dfa->alphabet);
    free(dfa);
}

// run the DFA to assess if the input 
//is accpeted or rejected by a DFA's language
bool DFA_run(DFA *dfa, const char *input)
{
    int i = 0;
    dfa->currentState = dfa->initialState;

    if (dfa->dfaType == EXACT_CSC)
    {
        // Process input for EXACT_CSC DFA type
        while (input[i] != '\0')
        {
            char symbol = input[i];
            char *pos = strchr(dfa->alphabet, symbol);

            if (pos == NULL)
            {
                return false; // Reject if input symbol is NOT in the alphabet
            }

            int index = pos - dfa->alphabet;
            int nextState = dfa->transitionTable[dfa->currentState][index];

            if (nextState == -1)
            {
                return false; // Reject if there is no transition defined
            }

            dfa->currentState = nextState;
            i++;
        }

        // Check if the current state is the accepting state
        if (dfa->currentState == dfa->acceptingState)
        {
            //printf("input accepted ");
            return true; // Input is accepted
        }
    }
    else if (dfa->dfaType == CONTAINS_END)
    {
        // Process input for CONTAINS_END DFA type
        while (input[i] != '\0')
        {
            char symbol = input[i];
            if (strchr(dfa->alphabet, symbol) == NULL)
            {
                // Skip characters that are NOT in the alphabet
                i++;
                continue;
            }
            int index = strchr(dfa->alphabet, symbol) - dfa->alphabet;
            dfa->currentState = dfa->transitionTable[dfa->currentState][index];

            if (dfa->currentState == -1)
            {
                return false; // Reject if there is no transition defined
            }

            i++;
        }

        // Check if the current state is the accepting state
        if (dfa->currentState == dfa->acceptingState)
        {
            return true; // Input is accepted
        }
    }
    else if (dfa->dfaType == STARTS_WITH_VOWEL)
    {
        // Process input for STARTS_WITH_VOWEL DFA type
        while (input[i] != '\0')
        {
            char symbol = input[i];
            char *pos = strchr(dfa->alphabet, symbol);

            if (pos == NULL)
            {
                return false; // Reject if the input symbol is NOT in the alphabet
            }

            int index = pos - dfa->alphabet;
            int nextState = dfa->transitionTable[dfa->currentState][index];

            if (nextState == -1)
            {
                return false; // Reject if there is no transition defined
            }

            dfa->currentState = nextState;

            // Check if the current state is the accepting state
            if (dfa->currentState == dfa->acceptingState)
            {
                return true; // Input is accepted
            }

            i++;
        }
    } else if (dfa->dfaType == EVEN_BINARY) 
    {
        // Process input for EVEN_BINARY DFA type
        while (input[i] != '\0')
        {
            char symbol = input[i];
            char *pos = strchr(dfa->alphabet, symbol);

            if (pos == NULL)
            {
                return false; // Reject if the input symbol NOT in the alphabet
            }

            int index = pos - dfa->alphabet;
            int nextState = dfa->transitionTable[dfa->currentState][index];

            if (nextState == -1)
            {
                return false; // Reject if no transition defined
            }

            dfa->currentState = nextState;
            i++;
        }

        // Check if the current state is the accepting state
        if (dfa->currentState == dfa->acceptingState)
        {
            return true; // Input is accepted
        }
    }
    printf("input rejected");
    // If input gets to this point, it's rejected
    return false;
}

// DFA REPL function
void DFA_repl(DFA *dfa)
{
    char input[100];
    while (1)
    {
        printf("Enter an input string ('quit' to quit): ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }
        input[strcspn(input, "\n")] = '\0'; // Removes newline character
        if (strcmp(input, "quit") == 0)
        {
            break;
        }
        dfa->currentState = dfa->initialState; // Reset the DFA state

        bool result = DFA_run(dfa, input);
        if (result)
        {
            printf("Result for input \"%s\": true (Accepted)\n", input);
        }
        else
        {
            printf("Result for input \"%s\": false (Rejected)\n", input);
        }
    }
}

//REQUIRED DFA'S AND THEIR DEFINITIONS:

// accepts the exact string "CSC"
DFA *DFA_for_exact_CSC(void)
{
    DFA *dfa = createDFA(4, 0, 3, "CSC", EXACT_CSC);
    setTransition(dfa, 0, 'C', 1);
    setTransition(dfa, 1, 'S', 2);
    setTransition(dfa, 2, 'C', 3);

    return dfa;
}

//  accepts strings containing "end"
DFA *DFA_for_contains_end(void)
{
    DFA *dfa = createDFA(4, 0, 3, "end", CONTAINS_END);
    // Define transitions for 'e', 'n', and 'd' (in any order)
    dfa->transitionTable[0][0] = 1; // Transition from state 0 to state 1 on 'e'
    dfa->transitionTable[0][1] = 0; // Stay in state 0 for 'n'
    dfa->transitionTable[0][2] = 0; // Stay in state 0 for 'd'
    dfa->transitionTable[1][0] = 1; // Stay in state 1 for 'e'
    dfa->transitionTable[1][1] = 2; // Transition from state 1 to state 2 on 'n'
    dfa->transitionTable[1][2] = 0; // Stay in state 1 for 'd'
    dfa->transitionTable[2][0] = 1; // Stay in state 2 for 'e'
    dfa->transitionTable[2][1] = 0; // Stay in state 2 for 'n'
    dfa->transitionTable[2][2] = 3; // Transition from state 2 to state 3 on 'd'
    dfa->transitionTable[3][0] = 1; // Transition from state 3 to state 1 on 'e'
    dfa->transitionTable[3][1] = 0; // Stay in state 3 for 'n'
    dfa->transitionTable[3][2] = 0; // Stay in state 3 for 'd';

    return dfa;
}

// accepts strings starting with a vowel
DFA *DFA_starts_with_vowel(void)
{
    // Define the alphabet for this DFA
    char alphabet[] = "aeiou";
    int initialState = 0;

    DFA *dfa = createDFA(2, initialState, 1, alphabet, STARTS_WITH_VOWEL);

    // Initialize the transition table
    dfa->transitionTable = (int **)malloc(2 * sizeof(int *));
    for (int i = 0; i < 2; i++)
    {
        dfa->transitionTable[i] = (int *)malloc(strlen(alphabet) * sizeof(int));
        for (int j = 0; j < strlen(alphabet); j++)
        {
            dfa->transitionTable[i][j] = 1; // Transition to accepting state for any character
        }
    }

    return dfa;
}

// accepts binary input with an even number of both 0’s and 1’s
DFA *DFA_even_binary(void)
{
    char alphabet[] = "01";
    int initialState = 0;

    DFA *dfa = createDFA(2, initialState, 1, alphabet, EVEN_BINARY);

    // Initialize the transition table
    dfa->transitionTable = (int **)malloc(2 * sizeof(int *));
    for (int i = 0; i < 2; i++)
    {
        dfa->transitionTable[i] = (int *)malloc(strlen(alphabet) * sizeof(int));
        for (int j = 0; j < strlen(alphabet); j++)
        {
            dfa->transitionTable[i][j] = 1; // Transition to accepting state for any character
        }
    }

    return dfa;
}

//PART 2: NFA SIMULATION

// same thing as DFA, to keep track of which NFA is being ran
#define ENDS_WITH_AT_NFA 0
#define CONTAINS_GOT_NFA 1
#define SPECIAL_NFA 2

// Definition of NFA structure
typedef struct {
    // these represent about the same as in the DFA
    int numStates;
    int alphabetSize;
    int initialState;
    int finalState;
    int** transitions;
    int nfaType; // this represents the type of NFA
} NFA;

// runs an NFA on an input string
bool NFA_run(NFA* nfa, char* input) {
    int currentState = nfa->initialState;
    int inputLength = strlen(input);

    for (int i = 0; i < inputLength; i++) {
        char currentChar = input[i];
        int charIndex = currentChar - 'a';

        if (charIndex < 0 || charIndex >= nfa->alphabetSize) {
            return false; // Invalid character in input
        }

        currentState = nfa->transitions[currentState][charIndex];

        // Check the NFA type and adapt the logic accordingly
        if (nfa->nfaType == ENDS_WITH_AT_NFA) {
            // Check if it's reached the end of the input string and in the final state
            if (i == inputLength - 1 && currentState == 2) {
                return true; // Accept the string
            }
        } else if (nfa->nfaType == CONTAINS_GOT_NFA) {
            // Check if the NFA has reached the final state
            if (currentState == nfa->finalState) {
                return true; // Accept the string
            }
        }
        else if (nfa->nfaType == SPECIAL_NFA) {
            // Check if it's reached the end of the input string and in the final state
            if (i == inputLength - 1 && currentState == nfa->finalState) {
                return true; // Accept the string
            }
        }
    }

    return currentState == nfa->finalState;
}

// NFA REPL
void NFA_repl(NFA* nfa) {
    char input[100];

    printf("Enter a string (type 'quit' to quit): ");
    while (scanf("%s", input) == 1) {
        if (strcmp(input, "quit") == 0) {
            break;
        }

        if (NFA_run(nfa, input)) {
            printf("Accepted\n");
        } else {
            printf("Rejected\n");
        }

        printf("Enter a string (type 'quit' to quit): ");
    }
}

// memory allocation
void NFA_free(NFA* nfa) {
    for (int i = 0; i < nfa->numStates; i++) {
        free(nfa->transitions[i]);
    }
    free(nfa->transitions);
    free(nfa);
}

// accepts strings ending with 'at'
NFA* NFA_for_ends_with_at(void) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    if (nfa == NULL) {
        return NULL; // Memory allocation failed
    }

    // Defining NFA properties
    nfa->numStates = 4;
    nfa->alphabetSize = 26; // Assuming lowercase alphabet
    nfa->initialState = 0;
    nfa->finalState = 3;
    nfa->nfaType = ENDS_WITH_AT_NFA; // Set the NFA type

    // Allocate memory for transitions
    nfa->transitions = (int**)malloc(nfa->numStates * sizeof(int*));
    if (nfa->transitions == NULL) {
        free(nfa);
        return NULL;
    }

    for (int i = 0; i < nfa->numStates; i++) {
        nfa->transitions[i] = (int*)malloc(nfa->alphabetSize * sizeof(int));
        if (nfa->transitions[i] == NULL) {
            // Memory allocation failed, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(nfa->transitions[j]);
            }
            free(nfa->transitions);
            free(nfa);
            return NULL;
        }
    }

    // Initialize transitions for the 'ends_with_at' NFA 
    int transitions[4][26] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // State 0
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0}, // State 1
    };

    // Copy the transition table to the NFA
    for (int i = 0; i < nfa->numStates; i++) {
        for (int j = 0; j < nfa->alphabetSize; j++) {
            nfa->transitions[i][j] = transitions[i][j];
        }
    }

    return nfa;
}


// accepts strings containing 'got'
NFA* NFA_for_contains_got(void) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    if (nfa == NULL) {
        return NULL; // Memory allocation failed
    }

    // Defining NFA properties
    nfa->numStates = 4;
    nfa->alphabetSize = 26; 
    nfa->initialState = 0;
    nfa->finalState = 3;
    nfa->nfaType = CONTAINS_GOT_NFA; // Set the NFA type

    // Allocate memory for transitions
    nfa->transitions = (int**)malloc(nfa->numStates * sizeof(int*));
    if (nfa->transitions == NULL) {
        free(nfa);
        return NULL;
    }

    for (int i = 0; i < nfa->numStates; i++) {
        nfa->transitions[i] = (int*)malloc(nfa->alphabetSize * sizeof(int));
        if (nfa->transitions[i] == NULL) {
            // Memory allocation failed, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(nfa->transitions[j]);
            }
            free(nfa->transitions);
            free(nfa);
            return NULL;
        }
    }

    // Initialize transitions for the 'contains_got' NFA 
    int transitions[4][26] = {
     //  a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // State 0
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, // State 1
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 2, 0, 0}, // State 2
    };
    // Copy the transition table to the NFA
    for (int i = 0; i < nfa->numStates; i++) {
        for (int j = 0; j < nfa->alphabetSize; j++) {
            nfa->transitions[i][j] = transitions[i][j];
        }
    }

    return nfa;
}

// accepts strings that have more than one a, e, h, i, or g, or more than two n’s or p’s.
NFA* NFA_special(void) {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    if (nfa == NULL) {
        return NULL; // Memory allocation failed
    }

    // Define NFA properties
    nfa->numStates = 4;
    nfa->alphabetSize = 26; // Assuming lowercase alphabet
    nfa->initialState = 0;
    nfa->finalState = 3;
    nfa->nfaType = SPECIAL_NFA; // Set the NFA type to SPECIAL_NFA

    // Allocate memory for transitions
    nfa->transitions = (int**)malloc(nfa->numStates * sizeof(int*));
    if (nfa->transitions == NULL) {
        free(nfa);
        return NULL;
    }

    for (int i = 0; i < nfa->numStates; i++) {
        nfa->transitions[i] = (int*)malloc(nfa->alphabetSize * sizeof(int));
        if (nfa->transitions[i] == NULL) {
            // Memory allocation failed, free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(nfa->transitions[j]);
            }
            free(nfa->transitions);
            free(nfa);
            return NULL;
        }
    }

    // Initialize transitions for this NFA
    int transitions[4][26] = {
        // a  b  c  d  e  f  g  h  i  j  k  l  m  n  o  p  q  r  s  t  u  v  w  x  y  z
        {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // State 0
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // State 1
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // State 2
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // State 3
    };

    // Copy the transition table to the NFA
    for (int i = 0; i < nfa->numStates; i++) {
        for (int j = 0; j < nfa->alphabetSize; j++) {
            nfa->transitions[i][j] = transitions[i][j];
        }
    }

    return nfa;
}

int main(void)
{
    //PART 1
    DFA *dfa_for_CSC = DFA_for_exact_CSC();
    //DFA_run(dfa_for_CSC, "CSC");

    DFA *dfa_contains_end = DFA_for_contains_end();

    DFA *dfa_even_binary = DFA_even_binary();

    DFA *dfa_starts_with_vowel = DFA_starts_with_vowel();

    // DFA REPL
    printf("DFA for the exact string \"CSC\":\n");
    DFA_repl(dfa_for_CSC);

    printf("DFA for strings containing \"end\":\n");
    DFA_repl(dfa_contains_end);

    printf("DFA for strings starting with a vowel:\n");
    DFA_repl(dfa_starts_with_vowel);

    printf("DFA for Binary input with an even number of both 0s and 1s:\n");
    DFA_repl(dfa_even_binary);

    //free
    freeDFA(dfa_for_CSC);
    freeDFA(dfa_contains_end);
    freeDFA(dfa_starts_with_vowel);
    freeDFA(dfa_even_binary);

    //PART 2

    NFA* nfa_ends_with_at = NFA_for_ends_with_at();
    NFA* nfa_contains_got = NFA_for_contains_got();
    NFA* nfa_special = NFA_special(); 

    if (nfa_ends_with_at == NULL || nfa_contains_got == NULL || nfa_special == NULL) {
        printf("Error creating NFA\n");
        return 1;
    }

    //NFA REPL
    printf("NFA for strings ending with 'at'\n");
    NFA_repl(nfa_ends_with_at); 

    printf("NFA for strings containing 'got'\n");
    NFA_repl(nfa_contains_got); 

    printf("NFA for strings that have more than one a, e, h, i, or g, or more than two ns or ps.\n");
    NFA_repl(nfa_special); 

    // Free allocated memory
    NFA_free(nfa_ends_with_at);
    NFA_free(nfa_contains_got);
    NFA_free(nfa_special); 

    return 0;
}


