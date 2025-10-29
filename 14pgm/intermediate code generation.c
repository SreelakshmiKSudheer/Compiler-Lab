#include <stdio.h>     // Standard I/O functions (printf, fgets)
#include <string.h>    // String utilities (strcspn)
#include <ctype.h>     // Character classification (isalnum, isspace, etc.)

#define VARLEN 32      // Max length of a variable/operand string stored
#define MAXSIZE 100    // Max number of entries in operator/operand stacks

char opndstack[MAXSIZE][VARLEN]; // Stack to hold operand strings (identifiers, temps)
char opstack[MAXSIZE];           // Stack to hold operators as characters
int opstktop = -1, opndstktop = -1; // Top indices for operator and operand stacks (-1 = empty)
char expn[MAXSIZE*2] = {'\0'};  // Input expression buffer (initialized to empty)

/* Return precedence/priority of operator c (higher = higher precedence) */
int priority(char c)
{
	if (c == '+' || c == '-') return 1;   // + and - have low precedence
	if (c == '*' || c == '/') return 2;   // * and / have higher precedence
	if (c == '=') return -1;              // assignment has lowest (special handling)
	if (c == '(') return 0;               // '(' used as marker (lowest for comparisons)
	return 0;                             // default fallback
}

/* Generate one intermediate code operation using top operator and operands.
   temp_count is pointer to counter for temporary variable numbers. */
static void gen_op(int *temp_count)
{
	if (opndstktop < 1 || opstktop < 0) 
		return; // Need at least two operands and one operator

	char op = opstack[opstktop--];          // Pop operator from operator stack
	char *right = opndstack[opndstktop--];  // Pop right operand from operand stack
	char *left = opndstack[opndstktop--];   // Pop left operand from operand stack

	if (op == '=') {
		/* assignment: left = right */
		printf("%s = %s\n", left, right);   // Emit assignment directly
		/* result of assignment is the left operand (address/variable) */
		snprintf(opndstack[++opndstktop], VARLEN, "%s", left); // push left back as result
	} else {
		/* For binary operators, emit a new temporary tN = left op right */
		printf("t%d = %s %c %s\n", *temp_count, left, op, right);
		/* push the temporary name onto operand stack as result of this op */
		snprintf(opndstack[++opndstktop], VARLEN, "t%d", *temp_count);
		(*temp_count)++; // increment temporary counter
	}
}

/* Convert infix expression in global 'expn' into intermediate code,
   using two stacks (operators and operands) and generating code via gen_op. */
void icg_from_infix()
{
	int i = 0, temp_count = 1; // i = index into expn, temp_count = next temp number
	while (expn[i] != '\0') {   // iterate until end of string
		/* skip spaces */
		if (expn[i] == ' ') 
		{ 
			i++; 
			continue; 
		} // ignore whitespace and advance

		/* operand (identifier / number) */
		if (isalnum((unsigned char)expn[i])) {
			char tempopnd[VARLEN]; // buffer to collect the operand characters
			int j = 0;
			/* collect consecutive alphanumeric characters into tempopnd */
			while (isalnum((unsigned char)expn[i]) && j < VARLEN-1)
				tempopnd[j++] = expn[i++];
			tempopnd[j] = '\0'; // null-terminate the operand string
			/* push operand onto operand stack */
			snprintf(opndstack[++opndstktop], VARLEN, "%s", tempopnd);
			continue; // processed an operand, go to next char
		}

		/* opening parenthesis */
		if (expn[i] == '(') {
			opstack[++opstktop] = expn[i++]; // push '(' onto operator stack and advance
			continue;
		}

		/* closing parenthesis: pop until '(' */
		if (expn[i] == ')') {
			i++; // consume ')'
			/* generate operations until matching '(' is found on opstack */
			while (opstktop >= 0 && opstack[opstktop] != '(') {
				gen_op(&temp_count);
			}
			/* pop the '(' if present */
			if (opstktop >= 0 && opstack[opstktop] == '(') 
				opstktop--;
			continue;
		}

		/* operator (e.g., + - * / =) */
		char cur = expn[i++]; // read current operator and advance
		/* while top operator has >= precedence, pop and generate (except '(') */
		while (opstktop >= 0 && opstack[opstktop] != '(' &&
			   priority(opstack[opstktop]) >= priority(cur)) {
			gen_op(&temp_count); // generate for the operator on top of stack
		}
		opstack[++opstktop] = cur; // push current operator onto operator stack
	}

	/* flush remaining operators on the stack after processing input */
	while (opstktop >= 0) {
		if (opstack[opstktop] == '(' || opstack[opstktop] == ')') {
			opstktop--; // ignore any stray parentheses
			continue;
		}
		gen_op(&temp_count); // generate code for remaining operators
	}
}

int main()
{
	printf("Enter an expression (spaces allowed, e.g. a=b+c*(d+e)):\n"); // prompt
	if (!fgets(expn, sizeof(expn), stdin)) return 0; // read line into expn; exit on EOF/error
	/* trim newline characters from the input (both CR and LF) */
	expn[strcspn(expn, "\r\n")] = '\0';

	icg_from_infix(); // perform intermediate code generation and print the results
	return 0; // normal termination
}
