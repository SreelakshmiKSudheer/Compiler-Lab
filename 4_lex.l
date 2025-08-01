%{
	#include <stdio.h>
%}
%option noyywrap

preprocessor					#.*
keyword						if|else|int|float|char|double|void|return|for|while|do|break|continue
identifier					[_a-zA-Z][_a-zA-Z0-9]*
singleComment					"//".*
multiComment					"/*"(.|"\n")*"*/"	
assignment					"="|"+="|"-="|"*="|"/="|"%="
logicop                                         "||"|"&&"|"!"
relop						"=="|"!="|"<"|"<="|">"|">="
arithmetic					[\-+*/]
integer						[\-+]?[0-9]+
string						\".\"
punct						[:;{}]


%%
{preprocessor}					{printf("Preprocessor: %s\n", yytext);}
{keyword}					{printf("Keyword: %s\n", yytext);}
{identifier}					{printf("Identifier: %s\n", yytext);}
{singleComment}                                 {printf("singleComment: %s\n", yytext);}
{multiComment}                                  {printf("multiComment: %s\n", yytext);}
{assignment}                                    {printf("assignment: %s\n", yytext);}
{logicop}                                    	{printf("logicop: %s\n", yytext);}
{relop}                                    	{printf("relop: %s\n", yytext);}
{arithmetic}                                    {printf("arithmetic: %s\n", yytext);}
{integer}                                    	{printf("integer: %s\n", yytext);}
{string}                                    	{printf("string: %s\n", yytext);}
{punct}                                    	{printf("punctuation: %s\n", yytext);}
.|\n						{}
%%

int main()
{
	FILE *fp = fopen("4_input.c","r");

	if(!fp)
	{
		printf("File doesn't exist\n");
		return 1; 
	}
	yyin = fp;
	yylex();
	fclose(yyin);
	return 0;

}
