#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define LEN 5
#define SIZE 50

struct
{
	char res[LEN];
	char op;
	char opnd1[LEN];
	char opnd2[LEN];
} tac[SIZE];

int num_instr = 0;
char instr[SIZE];

void readThreeAddressCode()
{
	printf("Enter the number of three address instructions: ");
	scanf("%d", &num_instr);

	printf("Enter each instruction, seperate result opcodes and operands by space\n");
	printf("Eg. s = a + b\n");

	for(int i = 0; i < num_instr; i++)
		scanf("%s = %s %c %s", tac[i].res, tac[i].opnd1, &tac[i].op, tac[i].opnd2);
}

char *get_instr(char op)
{
	// ASCII values are * = 42,  + = 43, - = 45, / = 47
	char mnemonics[][LEN] = {"MUL", "ADD", "", "SUB", "", "DIV"};
	strcpy(instr, mnemonics[op - '*']);
	return instr;
}

void generate_target_code()
{
	for(int i = 0; i < num_instr; i++)
		printf("MOV AX, %s\n%s AX, %s \nMOV %s, AX\n", tac[i].opnd1, get_instr(tac[i].op), tac[i].opnd2, tac[i].res);
}

int main()
{
	readThreeAddressCode();
	printf("Equivalent assembly code is:\n");
	generate_target_code();
}
