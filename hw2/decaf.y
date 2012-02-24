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
    int ival;
    double dval;
}


%error-verbose 
%token <id> STRING UNARY_OP RELATION_OP NAME READ SIMPLE_TYPE VOID
%token <id> IF ELSE WHILE NEW THIS RETURN _NULL CLASS PRODUCT_OP SUM_OP
%token <id> IDENTIFIER ASSIGN_OP BREAK CONTINUE DO EXTENDS INCDEC
%token <id> TRUE FALSE FOR PUBLIC PRIVATE STATIC SUPER STRING_LITERAL
%token <dval> FLOAT
%token <ival> INT
%token ENDL


%%

program: class_decls
;

class_decls: class_decls class_decl
           |
;

class_decl: CLASS IDENTIFIER optionally_extends '{' class_body_contents '}' 
              { cout << "New class " << $2 << " defined " << endl; }
;

optionally_extends: EXTENDS IDENTIFIER
                    { cout << "This class extends the class " << $2 << endl; }
                  |
;

class_body_contents: class_body_contents class_body_decl
                   | class_body_decl
;

class_body_decl: field_decl
               | method_decl
               | constructor_decl
;

field_decl: modifier var_decl
;

modifier: access static
;

access: PUBLIC
       | PRIVATE
       |
;

static: STATIC
       | 
;

var_decl: type variables ';'
;

type: SIMPLE_TYPE
    | IDENTIFIER
;

variables:  variable
         |  variable ',' variable
;

variable: IDENTIFIER array_dimensions 
          { cout << "New variable " << $1 << endl; }
;

array_dimensions: array_dimensions '[' ']'
                |
;

method_decl: modifier type IDENTIFIER '(' formals ')' block
             { cout << "New function " << $3 << endl; }
           | modifier VOID IDENTIFIER '(' formals ')' block
             { cout << "New function " << $3 << endl; }
;

constructor_decl: modifier IDENTIFIER '(' formals ')' block
;

formals: formal_param
       | formals ',' formal_param
       | 
;

formal_param: type variable
;

block: '{' statements '}'
;

statements: statements statement
          |
;

statement: IF '(' expr ')' statement else
           { cout << "If-Else block on line number " << lno << endl; }
         | WHILE '(' expr ')' statement
           { cout << "While statement on line number " << lno << endl; }
         | FOR '(' optional_statement_expr ';' expr ';' optional_statement_expr ')' statement
           { cout << "For statement on line number " << lno << endl; }
         | RETURN optional_expr
           { cout << "Return statement on line number " << lno << endl; }
         | statement_expr
         | BREAK ';'
           { cout << "Break statement on line number " << lno << endl; }
         | CONTINUE ';'
           { cout << "Continue statement on line number " << lno << endl; }
         | block
         | var_decl
         | ';'
;

else: ELSE statement
      |
;

optional_statement_expr:  statement_expr
                       |
;

optional_expr: expr
             | 
;

statement_expr: TRUE
;

expr: primary
    | assign
;

literal: INT
         { cout << "Integer literal encountered: " << $1 << " on line number " << lno << endl; }
       | FLOAT
         { cout << "Float literal encounterd: " << $1 << " on line number " << lno << endl; }
       | _NULL
         { cout << "NULL encountered: " << $1 << " on line number " << lno << endl; }
       | TRUE
         { cout << "true encountered: " << $1 << " on line number " << lno << endl; }
       | FALSE
         { cout << "false encountered: " << $1 << " on line number " << lno << endl; }
;

primary:  literal
       |  THIS
       |  SUPER
       |  '(' expr ')'
       |  NEW IDENTIFIER '(' optional_arguments ')'
       |  lhs
       |  method_invocation
;

optional_arguments: arguments
                  |
;

arguments:  expr
         |  arguments ',' expr
;

lhs:  field_access
   |  array_access
;

field_access: primary '.' IDENTIFIER
            | IDENTIFIER
;

array_access: primary '[' expr ']'
;

method_invocation: field_access '(' optional_arguments ')'
                   { cout << "Invoked a method on line number " << lno << endl; }
;

assign: lhs '=' expr
        | lhs INCDEC
        | INCDEC lhs
;


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
