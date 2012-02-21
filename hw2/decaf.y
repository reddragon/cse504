%{
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" int lno;
extern "C" char *yytext;

void yyerror(const char *s);

%}

%union {
    char * id;
    int val;
}


%error-verbose 
%token <id> STRING UNARY_OP RELATION_OP NAME READ SIMPLE_TYPE VOID
%token <id> IF ELSE WHILE NEW THIS RETURN _NULL CLASS PRODUCT_OP SUM_OP
%token <id> IDENTIFIER COMMENT_START ASSIGN_OP BREAK CONTINUE DO EXTENDS
%token <id> TRUE FALSE FOR PUBLIC PRIVATE STATIC SUPER STRING_LITERAL
%token <val> NUMBER


%token ENDL


%%

LINES: 	LINES ENDL LINE
	| LINE

LINE:	STRING
	| ENDL

%%

void 
yyerror(const char * s) {
    fprintf(stderr, "Error on line number %d: %s\n", lno, s);
}

int
_1main() {
    int ret = yyparse();
    return ret;
}
