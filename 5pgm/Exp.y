%{
	#include <stdio.h>
	int yylex();
	int yyerror();
%}
%token id num
%left '+' '-'
%left '*' '/'
%left '(' ')'

%%
s: e '\n'		{printf("Valid expression\n"); return 0;}
e: e '+' e |  e '-' e |  e '*' e |  e '/' e | '(' e ')' | id | num
 
%%

int main()
{
	return yyparse();
}

int yyerror()
{
	printf("Invalid expression\n");
	return 0;
}
