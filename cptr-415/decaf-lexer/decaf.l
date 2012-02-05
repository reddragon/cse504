%{
#include<iostream>
#include "decaf.tab.h"

using namespace std;

int lno = 0;
#define YY_DECL extern "C" int yylex()
#define RETURN_STR \
                    yylval.id = strdup(yytext); \
                    return STRING
#define RETURN_CHAR return yytext[0]
%}

%%
[\ \t]  ;
[\n]    { lno++; return ENDL;  }
(print|read)   { RETURN_STR; }
(int|void)     { RETURN_STR; }
[+\-%=*/!<>]    { RETURN_CHAR; }
(=>)|(>=)|(==)|(!=)|(&&)|(\|\|) { RETURN_STR; }
(if|else)   { RETURN_STR; }
while   { RETURN_STR; }
[;\(\){}]   { RETURN_CHAR; }
[a-zA-Z0-9]+    { RETURN_STR; }
<<EOF>>		{ static int once = 0; return once++ ? 0 : ENDL; }	
.   ;

%%

