%{
	#include <stdio.h>
	#include <stdlib.h>
	#define YYSTYPE double
	int yylex();
	int yyerror();	
%}

%token INT NL
%left '+' '-'
%left '*' '/'
%left '(' ')'
%right UMINUS

%%
s : e NL		{printf("=%f\n", $1); return 0;}
  ;
e : e '+' e		{$$ = $1 + $3;}
  | e '-' e             {$$ = $1 - $3;}
  | e '*' e             {$$ = $1 * $3;}
  | e '/' e             {$$ = $1 / $3;}
  | '-' e	%prec	UMINUS	{$$ =  -$2;}
  | '(' e ')'		{$$ = $2;}
  | INT			{$$ = $1;}
  ;
%%

int main()
{
	return yyparse();
}
int yyerror()
{
	printf("Error Occured\n");
	return 0;
}
