#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define VARLEN 10
#define MAXSIZE 50

char opndstack[MAXSIZE][VARLEN] = {'\0'};
char opstack[MAXSIZE] = {'\0'};
int opstktop = -1, opndstktop = -1;
char expn[MAXSIZE*5] = {'\0'};

int priority(char c)
{
	return (c == '+' | c == '-') ? 1 : (c == '*' | c == '/') ? 2 : c == '='? -1 : 0;
}

void icg_from_infix()
{
	int i = 00, temp_count = 1;
	char tempopnd[MAXSIZE] = {'\0'};
	while (expn[i] != '\0')
	{
		int j = 0;

		for (; isalnum(expn[i]);)
			tempopnd[j++] = expn[i++];

		if(j != 0)
			strcpy(opndstack[++opndstktop], tempopnd);

		if(expn[i] == ' ')
		{
			i++;
			continue;
		}

		if((opndstktop >= 0 && priority(opstack[opstktop]) < priority(expn[i])) | opstktop < 0)
			opstack[++opstktop] = expn[i++];
		else if(opstktop >= 0)
		{
			char op[2] = {'\0'};
			op[0] = opstack[opstktop--];
			printf("t%d = %s %s %s\n", temp_count, opndstack[opndstktop-1], op, opndstack[opndstktop]);
			sprintf(opndstack[--opndstktop], "t%d", temp_count++);
		}
	}

	while (opstktop >= 0)
	{
		char op[2] = {'\0'};
		op[0] = opstack[opstktop--];

		if(strcmp(op, "="))
		{
			printf("t%d = %s %s %s\n", temp_count, opndstack[opndstktop-1], op, opndstack[opndstktop]);
                        sprintf(opndstack[--opndstktop], "t%d", temp_count++);
		}
		else
			printf("%s = t%d\n", opndstack[opndstktop-1], temp_count-1);
	}
}

int main()
{
	printf("Enter an expression without spaces: ");
	scanf(" %[^\n]", expn);
	icg_from_infix();
}
