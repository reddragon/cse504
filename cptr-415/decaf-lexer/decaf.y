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
}


%error-verbose 
%token <id> STRING;
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
main() {
    int ret = yyparse();
    return ret;
}
