%{
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <cstdlib>

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" int lno;
extern "C" char *yytext;
extern "C" int yylineno;

void yyerror(const char *s);

%}

%union {
    char * id;
    int ival;
    double dval;
}


%error-verbose 
%token <id> STRING NAME SIMPLE_TYPE VOID
%token <id> IF ELSE WHILE NEW THIS RETURN _NULL CLASS
%token <id> IDENTIFIER BREAK CONTINUE DO EXTENDS INCDEC
%token <id> TRUE FALSE FOR PUBLIC PRIVATE STATIC SUPER STRING_LITERAL
%token <dval> FLOAT
%token <ival> INT
%token ENDL

%nonassoc LOWEST

%right '=' EQ_OP
%left BOOL_OR_OP "||"
%left BOOL_AND_OP "&&"
%nonassoc EQUALITY_OP "!=" "=="
%nonassoc INEQUALITY_OP "<=" ">=" '<' '>'
%left '+' '-' SUM_OP
%left PRODUCT_OP '*' '/'
%left UNARY_OP '!'

%%

program: class_decls
;

class_decls: class_decls class_decl
           |
;

class_decl: CLASS IDENTIFIER optionally_extends '{' class_body_contents '}' 
              { cerr << "New class " << $2 << " defined " << endl; }
;

optionally_extends: EXTENDS IDENTIFIER
                    { cerr << "This class extends the class " << $2 << endl; }
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

variables: variables ',' variable
         | variable
;

variable: IDENTIFIER array_dimensions
          { cerr << "New array variable " << $1 << endl; }
        | IDENTIFIER
          { cerr << "New variable " << $1 << endl; }
;

array_dimensions: array_dimensions '[' ']'
                | '[' ']'
;

method_decl: modifier type IDENTIFIER paren_formals block
             { cerr << "New function " << $3 << endl; }
           | modifier VOID IDENTIFIER paren_formals block
             { cerr << "New function " << $3 << endl; }
;

constructor_decl: modifier IDENTIFIER paren_formals block
;

paren_formals: '(' ')' 
             | '(' required_formals ')'
;

required_formals: required_formals ',' formal_param
       | formal_param

/*
formals: formal_param
       | formals ',' formal_param
       | 
;
*/

formal_param: type variable
;

block: '{' non_empty_statements '}'
     | '{' '}'
;

non_empty_statements: non_empty_statements statement
                    | statement
;

/*
statements: statements statement
          |
;
*/

statement: IF '(' expr ')' statement ELSE statement
           { cerr << "If-Else block on line number " << yylineno << endl; }
         | IF '(' expr ')' statement
           { cerr << "If block on line number " << yylineno << endl; }
         | WHILE '(' expr ')' statement
           { cerr << "While statement on line number " << yylineno << endl; }
         | FOR '(' optional_statement_expr ';' expr ';' optional_statement_expr ')' statement
           { cerr << "For statement on line number " << yylineno << endl; }
         | FOR '(' optional_statement_expr ';' ';' optional_statement_expr ')' statement
           { cerr << "For statement on line number " << yylineno << endl; }
         | RETURN expr ';'
           { cerr << "Return statement on line number " << yylineno << endl; }
         | RETURN ';'
         | statement_expr ';'
         | BREAK ';'
           { cerr << "Break statement on line number " << yylineno << endl; }
         | CONTINUE ';'
           { cerr << "Continue statement on line number " << yylineno << endl; }
         | block
         | var_decl
         | ';'
;

optional_statement_expr:  statement_expr
                       |
;

statement_expr: assign
              | method_invocation
;


expr: primary
    | assign
    | new_array
    | expr sum_op expr         %prec SUM_OP
    | expr product_op expr     %prec PRODUCT_OP
    | expr INEQUALITY_OP expr  %prec INEQUALITY_OP
    | expr EQUALITY_OP expr    %prec EQUALITY_OP
    | expr BOOL_OR_OP expr     %prec BOOL_OR_OP
    | expr BOOL_AND_OP expr    %prec BOOL_AND_OP
    | unary_op expr            %prec UNARY_OP
;


literal: INT
         { cerr << "Integer literal encountered: " << $1 << " on line number " << yylineno << endl; }
       | FLOAT
         { cerr << "Float literal encounterd: " << $1 << " on line number " << yylineno << endl; }
       | _NULL
         { cerr << "NULL encountered: " << $1 << " on line number " << yylineno << endl; }
       | TRUE
         { cerr << "true encountered: " << $1 << " on line number " << yylineno << endl; }
       | FALSE
         { cerr << "false encountered: " << $1 << " on line number " << yylineno << endl; }
       | STRING_LITERAL
         { cerr << "string encountered: " << $1 << " on line number " << yylineno << endl; }
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
                   { cerr << "Invoked a method on line number " << yylineno << endl; }
;

assign: lhs '=' expr %prec EQ_OP
        | lhs INCDEC
        | INCDEC lhs
;

new_array: NEW type array_dimensions_exprs array_dimensions
         | NEW type array_dimensions_exprs
;

array_dimensions_exprs: array_dimensions_exprs array_dimensions_expr
                        | array_dimensions_expr
;

array_dimensions_expr: '[' expr ']'
;

sum_op: '+'
      | '-'
;

product_op: PRODUCT_OP
;

unary_op: '+' %prec UNARY_OP
        | '-' %prec UNARY_OP
        | '!' %prec UNARY_OP
;

%%

void 
yyerror(const char * s) {
   cerr << "Error on line number" << yylineno << ": " << s << endl;
}

int
main(int argc, char ** argv) {
#ifndef DEBUG
    freopen("/dev/null", "w", stderr);
#endif
    if (argc >= 2) {
        if(freopen(argv[1], "r", stdin) == NULL) {
            cout << "Could not open file" << endl;
            return 1;
        }
    }

    int ret = yyparse();
    if (!ret) {
        cout << "Yes" << endl;
    } else {
        cout << "Failed to Parse." << endl;
    }
    return ret;
}
