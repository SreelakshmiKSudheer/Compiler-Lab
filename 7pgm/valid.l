%{
	#define YYSTYPE double
	#include "calc.tab.h"
%}
%option noyywrap

%%
[0-9]+	{yylval=atof(yytext);return INT;}
\n	{return NL;}
[\t]
.	{return yytext[0];}
%%
