%{
	#include "validId.tab.h"
%}
%option noyywrap

%%
[_a-zA-Z] {return ALPHA;}
[0-9]	  {return DIGIT;}
.	  {return yytext[0];}
\n	  {return '\n';}
%%
