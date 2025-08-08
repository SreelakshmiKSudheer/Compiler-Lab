%{
#include "8pgm.h"

int yyerror(char *);
int yylex();

ASTNode *root = NULL;
%}

%union
{
	int INT;
	ASTNode *ptr;
}

%token <INT> NUMBER
%token NL

%left '+' '-'
%left '*' '/'

%%
stmt: expr NL				{ root=$<ptr>1; return 0; }									

expr: expr '+' expr			{ $<ptr>$ = makeNode('+', $<ptr>1, $<ptr>3); }
    | expr '-' expr                     { $<ptr>$ = makeNode('-', $<ptr>1, $<ptr>3); }
    | expr '*' expr                     { $<ptr>$ = makeNode('*', $<ptr>1, $<ptr>3); }
    | expr '/' expr                     { $<ptr>$ = makeNode('/', $<ptr>1, $<ptr>3); }
    | NUMBER				{ $<ptr>$ = makeLeaf($<INT>1);}
    ;

%%


int main()
{
	yyparse();
	printf("Generated tree: Post Order traversal\n");
	printAst(root);
	printf("\n");

	printf("Generated tree: Pre Order traversal\n");
        preAst(root);
        printf("\n");
	return 0;
}

int yyerror(char *msg)
{
	fprintf(stderr,"%s\n", msg);
	return 0;
}

