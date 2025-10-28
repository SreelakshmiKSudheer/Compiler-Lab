#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26    /* A..Z */
#define MAX_PROD 50
#define MAX_LEN  100
#define EPSILON  '#'
#define ENDMARK  '$'

/* Productions stored as strings "A=rhs" */
static char productions[MAX_PROD][MAX_LEN];
static int  num_productions = 0;

/* FIRST and FOLLOW sets indexed by letter: index = ch - 'A' */
static char firstsets[ALPHABET_SIZE][MAX_LEN];
static char followsets[ALPHABET_SIZE][MAX_LEN];

/* set of nonterminals encountered (string of unique uppercase letters) */
static char nonterminals[MAX_LEN];
static char start_symbol = '\0';

/* -------------------- Utility set operations -------------------- */
static int is_nonterminal(char c) { return (c >= 'A' && c <= 'Z'); }

static void set_add(char *set, char elem)
{
    if (elem == '\0') return;
    if (!strchr(set, elem)) {
        size_t l = strlen(set);
        set[l]   = elem;
        set[l+1] = '\0';
    }
}

/* Union src into dest. Return 1 if dest changed. */
static int set_union_inplace(char *dest, const char *src)
{
    int changed = 0;
    for (size_t i = 0; src[i] != '\0'; ++i) {
        if (!strchr(dest, src[i])) {
            size_t l = strlen(dest);
            dest[l]   = src[i];
            dest[l+1] = '\0';
            changed = 1;
        }
    }
    return changed;
}

/* Remove a character from set */
static int set_remove(char *set, char elem)
{
    char *p = strchr(set, elem);
    if (!p) return 0;
    memmove(p, p+1, strlen(p+1)+1);
    return 1;
}

/* Return 1 if set contains elem */
static int set_contains(const char *set, char elem) { return strchr(set, elem) != NULL; }

/* Print set nicely */
static void print_set(const char *label, const char *set)
{
    printf("%s {", label);
    for (size_t i = 0; set[i] != '\0'; ++i) {
        if (i) printf(",");
        putchar(set[i]);
    }
    printf("}\n");
}

/* -------------------- Input -------------------- */
static void read_productions(void)
{
    printf("Note: Grammar should not have left recursion (program assumes acyclic derivation)\n");
    printf("Enter number of productions: ");
    if (scanf("%d", &num_productions) != 1 || num_productions <= 0) {
        fprintf(stderr, "Invalid number of productions\n");
        exit(1);
    }
    getchar(); /* consume newline */

    printf("Enter productions in form A=BC or A=aB or A=# for epsilon (no spaces preferred)\n");
    for (int i = 0; i < num_productions; ++i) {
        if (!fgets(productions[i], sizeof(productions[i]), stdin)) {
            fprintf(stderr, "Unexpected input error\n");
            exit(1);
        }
        /* trim newline */
        productions[i][strcspn(productions[i], "\r\n")] = '\0';
        if (strlen(productions[i]) < 3 || productions[i][1] != '=') {
            fprintf(stderr, "Invalid production format on line %d: %s\n", i+1, productions[i]);
            exit(1);
        }
        char lhs = productions[i][0];
        if (!is_nonterminal(lhs)) {
            fprintf(stderr, "LHS must be an uppercase nonterminal (A-Z): %c\n", lhs);
            exit(1);
        }
        /* collect nonterminals */
        if (!strchr(nonterminals, lhs)) {
            size_t l = strlen(nonterminals);
            nonterminals[l] = lhs;
            nonterminals[l+1] = '\0';
        }
    }
    start_symbol = productions[0][0];
}

/* -------------------- FIRST helpers -------------------- */

/* Compute FIRST of a sequence (string starting at rhs[pos]) into out.
   Returns 1 if the sequence can derive epsilon (i.e., FIRST contains EPSILON). */
static int first_of_sequence(const char *rhs, int pos, char *out)
{
    /* rhs[pos] is first symbol of sequence */
    int rhs_nullable = 1; /* assume nullable until a non-epsilon is found */

    for (int k = pos; rhs[k] != '\0'; ++k) {
        char sym = rhs[k];

        if (sym == EPSILON) {
            set_add(out, EPSILON);
            rhs_nullable = 1;
            break;
        }

        if (!is_nonterminal(sym)) { /* terminal */
            set_add(out, sym);
            rhs_nullable = 0;
            break;
        }

        /* sym is nonterminal: add FIRST(sym) \ {EPSILON} */
        int idx = sym - 'A';
        for (size_t t = 0; firstsets[idx][t] != '\0'; ++t) {
            if (firstsets[idx][t] != EPSILON)
                set_add(out, firstsets[idx][t]);
        }

        if (set_contains(firstsets[idx], EPSILON)) {
            /* sym can be epsilon, continue to next symbol */
            rhs_nullable = 1;
            continue;
        } else {
            rhs_nullable = 0;
            break;
        }
    }

    if (rhs[0] == '\0') {
        /* empty RHS -> epsilon */
        set_add(out, EPSILON);
        rhs_nullable = 1;
    }

    return rhs_nullable;
}

/* Iteratively compute FIRST sets until no change */
static void compute_first_sets(void)
{
    /* initialize empty sets */
    for (int i = 0; i < ALPHABET_SIZE; ++i) firstsets[i][0] = '\0';

    int changed;
    do {
        changed = 0;
        for (int p = 0; p < num_productions; ++p) {
            char A = productions[p][0];
            const char *rhs = productions[p] + 2; /* skip 'A=' */
            char temp[MAX_LEN] = {0};

            /* FIRST(rhs) */
            int nullable = first_of_sequence(rhs, 0, temp);

            /* union temp into FIRST(A) */
            int idx = A - 'A';
            if (set_union_inplace(firstsets[idx], temp)) changed = 1;
            /* if rhs_nullable, ensure EPSILON is in FIRST(A) (union_inplace already added it) */
            (void)nullable;
        }
    } while (changed);
}

/* -------------------- FOLLOW helpers -------------------- */

/* Check if entire sequence rhs[pos]..end can derive epsilon using current FIRSTs */
static int sequence_nullable(const char *rhs, int pos)
{
    for (int k = pos; rhs[k] != '\0'; ++k) {
        char sym = rhs[k];
        if (sym == EPSILON) return 1;
        if (!is_nonterminal(sym)) return 0;
        if (!set_contains(firstsets[sym - 'A'], EPSILON)) return 0;
    }
    /* reached end -> nullable */
    return 1;
}

/* Iteratively compute FOLLOW sets until no change */
static void compute_follow_sets(void)
{
    /* initialize empty sets */
    for (int i = 0; i < ALPHABET_SIZE; ++i) followsets[i][0] = '\0';

    /* start symbol gets $ */
    if (start_symbol) set_add(followsets[start_symbol - 'A'], ENDMARK);

    int changed;
    do {
        changed = 0;
        for (int p = 0; p < num_productions; ++p) {
            char A = productions[p][0];
            const char *rhs = productions[p] + 2;

            for (int i = 0; rhs[i] != '\0'; ++i) {
                char B = rhs[i];
                if (!is_nonterminal(B)) continue;

                /* FIRST of beta (suffix after B) */
                char first_beta[MAX_LEN] = {0};
                int beta_nullable = 1;
                int j = i + 1;
                if (rhs[j] == '\0') {
                    beta_nullable = 1;
                } else {
                    beta_nullable = first_of_sequence(rhs, j, first_beta);
                }

                /* Add FIRST(beta) \ {EPSILON} to FOLLOW(B) */
                for (size_t t = 0; first_beta[t] != '\0'; ++t) {
                    if (first_beta[t] == EPSILON) continue;
                    if (!strchr(followsets[B - 'A'], first_beta[t])) {
                        set_add(followsets[B - 'A'], first_beta[t]);
                        changed = 1;
                    }
                }

                /* If beta nullable, add FOLLOW(A) to FOLLOW(B) */
                if (beta_nullable) {
                    if (set_union_inplace(followsets[B - 'A'], followsets[A - 'A'])) changed = 1;
                }
            }
        }
    } while (changed);
}

/* -------------------- MAIN -------------------- */
int main(void)
{
    read_productions();

    /* compute FIRST and FOLLOW iteratively (fixpoint) */
    compute_first_sets();
    compute_follow_sets();

    /* print FIRST sets only for nonterminals encountered */
    printf("\nFIRST sets:\n");
    for (size_t i = 0; nonterminals[i] != '\0'; ++i) {
        char label[8]; sprintf(label, "First(%c):", nonterminals[i]);
        print_set(label, firstsets[nonterminals[i] - 'A']);
    }

    printf("\nFOLLOW sets:\n");
    for (size_t i = 0; nonterminals[i] != '\0'; ++i) {
        char label[9]; sprintf(label, "Follow(%c):", nonterminals[i]);
        /* follow sets should not contain EPSILON */
        char temp[MAX_LEN];
        strcpy(temp, followsets[nonterminals[i] - 'A']);
        set_remove(temp, EPSILON);
        print_set(label, temp);
    }

    return 0;
}