%{
 
/*
 * Parser.y file
 * To generate the parser run: "bison --defines=Parser.h Parser.y"
 */
 
#include "TypeParser.h"
#include "ParserParam.h"
 
%}
 
%define api.pure
 
%left '+' TOKEN_PLUS
%left '*' TOKEN_MULTIPLY
 
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_PLUS
%token TOKEN_MULTIPLY
 
%token <value> TOKEN_NUMBER
 
%type <expression> expr
 
%%
 
input: 
        expr { ((SParserParam*)data)->expression = $1; }
        ;
 
expr:
      expr TOKEN_PLUS expr { $$ = createOperation( ePLUS, $1, $3 ); }
    | expr TOKEN_MULTIPLY expr { $$ = createOperation( eMULTIPLY, $1, $3 ); }
    | TOKEN_LPAREN expr TOKEN_RPAREN { $$ = $2; }
    | TOKEN_NUMBER { $$ = createNumber($1); }
;
 
%%
