%{
	#include <stdio.h>
	int yylex();
	int yyerror();
%}
%token ALPHA DIGIT

%%
start: S '\n'		{printf("Valid identifier\n"); return 0;}
S: ALPHA A
 ;
A: ALPHA A
 | DIGIT A
 |
 ;

%%
int main()
{
	return yyparse();
}
int yyerror()
{
	printf("Invalid identifier\n");
}
