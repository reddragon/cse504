%{
#include <cstdio>
#include <iostream>
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);
%}

%union {
  int ival;
  float fval;
  char *sval;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%%
snazzle:
  snazzle INT { cout << "Found an int: " << $2 << endl; }
  | snazzle FLOAT { cout << "Found a float: " << $2 << endl;  }
  | snazzle STRING { cout << "Found a string: " << $2 << endl; }
  | INT { cout << "Found an int: " << $1 << endl; }
  | FLOAT { cout << "Found a float: " << $1 << endl;  }
  | STRING { cout << "Found a string: " << $1 << endl; }
  ;
%%

main() {
  FILE * myfile = fopen("snazzle-file", "r");
  if(!myfile) {
    cout << "Can't open" << endl;
    return -1;
  }

  yyin = myfile;
  do {
    yyparse();
  } while(!feof(yyin));
}

void yyerror(const char *s) {
  cout << "Parse error! Message: " << s << endl;
  exit(-1);
}
