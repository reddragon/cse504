%{

#include<iostream>
using namespace std;

int lno = 1;
#define YY_DECL extern "C" int yylex()

#include "proptp.tab.h"
%}
%%
[ \t]  ;
[\n]          { ++lno; return ENDL; }
\->           { return IMPLIES; }
[&\|\(\)\.]   { return yytext[0]; }
[a-zA-Z0-9]+  {
  yylval.id = strdup(yytext);
  return STRING;
}
. ;
%%

