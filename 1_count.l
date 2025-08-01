%{
	#include <stdio.h>

	// declarations
	int lines = 0, words = 0, chars = 0;
%}
%option noyywrap
%%

[a-zA-Z]+		{ words++; chars += yyleng; }
\n			{ lines++; chars++; }
.			{ chars++; }
%%

int main(int argc, char **argv)
{
	if(argc > 1){
		FILE *fp = fopen(argv[1], "r");

		if(!fp)
		{
			printf("Can't open the file\n");
			return 1;
		}	

		yyin = fp;
	
	}

	yylex();

	printf("Lines : %d \n", lines);
	printf("Words : %d \n", words);
	printf("Characters : %d \n", chars);

	return 0;
}
