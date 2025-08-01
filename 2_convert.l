%{
	#include <stdio.h>
%}
%%
abc			{ printf("ABC"); }
//"abc"			{ for(int i = 0; yytext[i] != '\0'; i++){ yytext[i] -= 32; } }
.|\n			{ printf("%s", yytext); }
%%
int main()
{
	yylex();
	return 0;
}
