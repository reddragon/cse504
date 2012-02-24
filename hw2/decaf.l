%{
#include<iostream>
#include "decaf.tab.h"

using namespace std;

int lno = 1;
#define YY_DECL extern "C" int yylex()

#define RETURN_STYPE(TYPE) { \
  yylval.id = strdup(yytext); \
  return TYPE; \
}

#define RETURN_ITYPE(TYPE) { \
  yylval.ival = atoi(yytext); \
  return TYPE; \
}

#define RETURN_CTYPE(TYPE) return TYPE;
#define RETURN_CHAR return yytext[0];
%}

%%

[\ \t]          ;
[\n]            { lno++; }
(print)         { RETURN_STYPE(NAME); }
(read)          { RETURN_STYPE(READ); }
(int)|(float)|(boolean)   { RETURN_STYPE(SIMPLE_TYPE); }
(void)          { RETURN_STYPE(VOID); }
(if)            { RETURN_STYPE(IF); }
(else)          { RETURN_STYPE(ELSE); }
(while)         { RETURN_STYPE(WHILE); }
(new)           { RETURN_STYPE(NEW); }
(this)          { RETURN_STYPE(THIS); }
(return)        { RETURN_STYPE(RETURN); }
(null)          { RETURN_STYPE(_NULL); }
(class)         { RETURN_STYPE(CLASS); }
(break)         { RETURN_STYPE(BREAK); }
(continue)      { RETURN_STYPE(CONTINUE); }
(do)            { RETURN_STYPE(DO); }
(extends)       { RETURN_STYPE(EXTENDS); }
(true)          { RETURN_STYPE(TRUE); }
(false)         { RETURN_STYPE(FALSE); }
(for)           { RETURN_STYPE(FOR); }
(public)        { RETURN_STYPE(PUBLIC); }
(private)       { RETURN_STYPE(PRIVATE); }
(static)        { RETURN_STYPE(STATIC); }
(super)         { RETURN_STYPE(SUPER); }
[+\-!]          { RETURN_STYPE(UNARY_OP); }
([%*/])|(&&)    { RETURN_STYPE(PRODUCT_OP); }
(>=)|(<=)|(==)|(!=)|(<)|(>)                   { RETURN_STYPE(RELATION_OP); }
=                                             { RETURN_STYPE(ASSIGN_OP); }
(\+)|(\-)|(\|\|)                              { RETURN_STYPE(SUM_OP); }
[\;\(\){},\[\]\.]                             { RETURN_CHAR; }
[a-zA-Z][a-zA-Z0-9_]*                         { RETURN_STYPE(IDENTIFIER); }
\"((\\.)|[^\\"])*\"                           { RETURN_STYPE(STRING_LITERAL); /* TODO: Why this works? */ }
[0-9]+                                        { RETURN_ITYPE(INT); }
\/\*((\*+[^\/\*])|[^\*])*\*+\/                ;
\/\/[^\r\n]*                                  ;
[\+\-]?[0-9]*(\.[0-9]+)?([eE][\+\-]?[0-9]+)?  { RETURN_STYPE(FLOAT); }
%%
/*
int
main() {
  int token = yylex();
  while (token) {
    printf("Text: %s, Token: %d\n", yytext, token);
    token = yylex();
  }
  return 0;
} */
