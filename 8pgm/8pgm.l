%option noyywrap
%{
	#include "8pgm.h"
	#include "8pgm.tab.h"
%}

%%
[0-9]+		{yylval.INT = atoi(yytext); return NUMBER; }
\n		{return NL;}
[ \t]+		;
. 		{return yytext[0];}
%%

