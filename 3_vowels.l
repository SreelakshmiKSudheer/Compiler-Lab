%{
	#include <stdio.h>
	int vowels = 0, letters = 0;
%}
%option noyywrap
%%
[aeiouAEIOU]                    { vowels++; }
[a-zA-Z]			{ letters++; }
.|\n				{}
%%
int main(){
	yylex();
	printf("No. of vowels: %d\n", vowels);
	printf("No. of consonants: %d\n", letters);

	return 0;
}
