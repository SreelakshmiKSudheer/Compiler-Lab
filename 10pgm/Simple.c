#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 26
#define LENGTH 50

// -------------------- Global Variables --------------------
char productions[SIZE][LENGTH] = { '\0' };
char firstSets[SIZE][LENGTH]   = { '\0' };
char nonTerminals[SIZE]        = { '\0' };
char startSymbol               = '\0';
int  numProductions            = 0;

// -------------------- Utility Functions --------------------
int indexOf(char nt) { return nt - 'A'; }

int isInSet(char *set, char c) {
    return strchr(set, c) != NULL;
}

void addToSet(char *set, char elem) {
    if (!isInSet(set, elem)) {
        int len = strlen(set);
        set[len] = elem;
        set[len + 1] = '\0';
    }
}

void unionSets(char *dest, const char *src) {
    for (int i = 0; src[i] != '\0'; i++)
        addToSet(dest, src[i]);
}

void removeFromSet(char *set, char elem) {
    char *pos = strchr(set, elem);
    if (pos != NULL)
        memmove(pos, pos + 1, strlen(pos)); // shift remaining chars left
}

void printSet(const char *set) {
    printf("{");
    for (int i = 0; set[i] != '\0'; i++) {
        printf("%c", set[i]);
        if (set[i + 1] != '\0')
            printf(",");
    }
    printf("}\n");
}

// -------------------- Grammar Input --------------------
void readProductions() {
    printf("⚠️ Ensure the grammar has no left recursion.\n");
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);
    getchar(); // clear newline

    printf("Enter productions (Format: A=abc | A=# for epsilon)\n");

    for (int i = 0; i < numProductions; i++) {
        scanf("%[^\n]", productions[i]);
        getchar(); // clear newline
        addToSet(nonTerminals, productions[i][0]);
    }
    startSymbol = productions[0][0];
}
// -------------------- FIRST Set Computation --------------------

// void calcFirst(char symbol, char *result) 
// {
//     // Base Case: Terminal or epsilon
//     if (!isupper(symbol)) {
//         removeFromSet(result, '#'); // Remove epsilon if terminal appears
//         addToSet(result, symbol);
//         return;
//     }

//     // Recursive Case: Non-terminal
//     for (int i = 0; i < numProductions; i++) {
//         if (productions[i][0] == symbol) {
//             int pos = 2; // Skip 'A=' part
//             while (productions[i][pos] != '\0') {
//                 char temp[LENGTH] = { '\0' };
//                 calcFirst(productions[i][pos], temp);
//                 unionSets(result, temp);

//                 // If epsilon in FIRST of next symbol, move ahead
//                 if (isInSet(temp, '#'))
//                     pos++;
//                 else {
//                     removeFromSet(result, '#');
//                     break;
//                 }
//             }
//         }
//     }
// }

// ...existing code...
void calcFirst(char symbol, char *result) {
    if (!isupper(symbol)) {               // terminal or '#'
        addToSet(result, symbol);
        return;
    }

    for (int i = 0; i < numProductions; i++) {
        if (productions[i][0] != symbol) continue;
        int pos = 2; // skip "A="
        int allNullable = 1;

        while (productions[i][pos] != '\0') {
            char temp[LENGTH] = { '\0' };
            calcFirst(productions[i][pos], temp);

            // add FIRST(sym) \ {#}
            for (int k = 0; temp[k] != '\0'; k++)
                if (temp[k] != '#')
                    addToSet(result, temp[k]);

            if (isInSet(temp, '#')) {
                pos++; // symbol can be epsilon, continue to next symbol
            } else {
                allNullable = 0;
                break;
            }
        }

        if (allNullable) // whole RHS can become epsilon
            addToSet(result, '#');
    }
}
// ...existing code...

// -------------------- FOLLOW Set Computation --------------------
void calcFollow(char symbol, char *result) {
    if (!isupper(symbol))
        return;

    if (symbol == startSymbol)
        addToSet(result, '$'); // $ for start symbol

    for (int i = 0; i < numProductions; i++) {
        char *rhs = strchr(productions[i], '=') + 1;

        for (int j = 0; rhs[j] != '\0'; j++) {
            if (rhs[j] == symbol) {
                char next = rhs[j + 1];

                // Case 1: Next symbol is terminal
                if (next != '\0' && !isupper(next) && next != '#')
                    addToSet(result, next);

                // Case 2: Next symbol is non-terminal
                else if (next != '\0' && isupper(next)) {
                    unionSets(result, firstSets[indexOf(next)]);
                    if (isInSet(firstSets[indexOf(next)], '#'))
                        calcFollow(productions[i][0], result);
                }

                // Case 3: Symbol is at end
                else if (next == '\0' && productions[i][0] != symbol)
                    calcFollow(productions[i][0], result);
            }
        }
    }
}

// -------------------- MAIN --------------------
int main() {
    char temp[LENGTH];

    readProductions();

    printf("\n---- FIRST Sets ----\n");
    for (int i = 0; nonTerminals[i] != '\0'; i++) {
        strcpy(temp, "");
        calcFirst(nonTerminals[i], temp);
        strcpy(firstSets[indexOf(nonTerminals[i])], temp);
        printf("FIRST(%c) = ", nonTerminals[i]);
        printSet(temp);
    }

    printf("\n---- FOLLOW Sets ----\n");
    for (int i = 0; nonTerminals[i] != '\0'; i++) {
        strcpy(temp, "");
        calcFollow(nonTerminals[i], temp);
        removeFromSet(temp, '#');
        printf("FOLLOW(%c) = ", nonTerminals[i]);
        printSet(temp);
    }

    return 0;
}
