%{
#include<iostream>
#include "decaf.tab.h"

using namespace std;

int lno = 0;
#define YY_DECL extern "C" int yylex()

#define RETURN_STYPE(TYPE) { \
  yylval.id = strdup(yytext); \
  return TYPE; \
}

#define RETURN_ITYPE(TYPE) { \
  yylval.val = atoi(yytext); \
  return TYPE; \
}

#define RETURN_CTYPE(TYPE) return TYPE;
#define RETURN_CHAR return yytext[0];
%}

%x COMMENT
%%
"//"            { 
  BEGIN(COMMENT);
  return COMMENT_START;
}
<COMMENT>[^\n]+ ;
<COMMENT>\n     { BEGIN(INITIAL); return ENDL; }

[\ \t]          ;
[\n]            { lno++; }

(print)         { RETURN_STYPE(NAME); }
(read)          { RETURN_STYPE(READ); }
(int)           { RETURN_STYPE(SIMPLE_TYPE); }
(void)          { RETURN_STYPE(VOID); }
(if)            { RETURN_STYPE(IF); }
(else)          { RETURN_STYPE(ELSE); }
(while)         { RETURN_STYPE(WHILE); }
(new)           { RETURN_STYPE(NEW); }
(this)          { RETURN_STYPE(THIS); }
(return)        { RETURN_STYPE(RETURN); }
(null)          { RETURN_STYPE(_NULL); }
(class)         { RETURN_STYPE(CLASS); }

[+\-!]          { RETURN_STYPE(UNARY_OP); }
([%*/])|(&&)    { RETURN_STYPE(PRODUCT_OP); }
(>=)|(<=)|(==)|(!=)|(<)|(>) { RETURN_STYPE(RELATION_OP); }
=               { RETURN_STYPE(ASSIGN_OP); }
(\+)|(\-)|(\|\|) { RETURN_STYPE(SUM_OP); }
[\;\(\){},\[\]\.]       { RETURN_CHAR; }
[a-zA-Z][a-zA-Z0-9]*    { RETURN_STYPE(IDENTIFIER); }

[0-9]+          { RETURN_ITYPE(NUMBER); }

%%

int
main() {
  int token = yylex();
  while (token) {
    printf("Text: %s, Token: %d\n", yytext, token);
    token = yylex();
  }
  return 0;
}