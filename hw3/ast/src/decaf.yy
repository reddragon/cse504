%{ /* -*-  Mode:C; c-basic-offset:4; indent-tabs-mode:nil -*- */
/* vim:set ft=yy ts=4 sw=4 sts=4 cindent: */
#include <stdio.h>
#include <iostream>
#include  <stack>
#include "Ast.hh"
using namespace std;

// Prototypes to lexer functions
extern void yyerror (const char *error);
extern int  yylex ();

// interface to the outside world
extern list<Entity *> *toplevel;  // list of top-level classes
extern EntityTable *global_symtab; // global symbol table

// Global Variables
list<Entity *> *class_list, *class_members, *formal_params;
Entity * new_class, * new_method;
bool visibility_flag, static_flag; 
Type * type;
char * method_name;
Statement * method_body;
stack< list<Entity *> * > block_stmts;
list<Statement *> * stmt_list;
%}

%token TOK_BOOLEAN TOK_BREAK TOK_CLASS TOK_CONTINUE TOK_ELSE 
    TOK_EXTENDS TOK_FALSE TOK_FLOAT TOK_FOR TOK_IF TOK_INT TOK_NEW 
    TOK_NULL TOK_PRIVATE TOK_PUBLIC TOK_RETURN TOK_STATIC TOK_SUPER
    TOK_THIS TOK_TRUE TOK_VOID TOK_WHILE TOK_DO
    TOK_INT_CONST TOK_FLOAT_CONST TOK_STRING_CONST 
    TOK_ID TOK_COMMA TOK_DOT TOK_SEMICOLON
    TOK_OPEN_SQ_BRACKET TOK_CLOSE_SQ_BRACKET
    TOK_OPEN_PAREN TOK_CLOSE_PAREN TOK_OPEN_BRACE TOK_CLOSE_BRACE 
    TOK_PLUS TOK_MINUS TOK_MULTIPLY TOK_DIVIDE 
    TOK_PLUS_PLUS TOK_MINUS_MINUS TOK_EQUAL TOK_AND TOK_OR TOK_NOT 
    TOK_GREATER TOK_LESSER TOK_EQUAL_EQUAL TOK_NOT_EQUAL
    TOK_GREATER_OR_EQUAL TOK_LESSER_OR_EQUAL
    TOK_SYNTAX_ERROR TOK_RUNAWAY_STRING

%union{
  char* string_val;
  int   int_val;
  float float_val;
  char char_val;
  bool bool_val;
  list<Entity*>* entity_list;
  Statement * stmt;
  Expression * expr; 
  list<Expression*>* exprs;
  /****
    Add fields to hold other types of attribute values here

Example:
list<Entity*>* entity_list;	
   ****/
}

%right TOK_EQUAL
%left TOK_OR
%left TOK_AND
%left TOK_EQUAL_EQUAL TOK_NOT_EQUAL 
%nonassoc TOK_LESSER TOK_GREATER TOK_LESSER_OR_EQUAL TOK_GREATER_OR_EQUAL
%left TOK_PLUS TOK_MINUS 
%left TOK_MULTIPLY TOK_DIVIDE
%left TOK_NOT 

%error-verbose
%type <string_val> TOK_ID TOK_STRING_CONST
%type <int_val> TOK_INT_CONST
%type <float_val> TOK_FLOAT_CONST
%type <entity_list> ClassDeclarations

%type <stmt> Stmt OptElsePart
%type <stmts> StmtStar
%type <expr> Expr Literal Primary MethodInvocation LeftHandSide FieldAccess ArrayAccess
%type <exprs> ArgumentListOpt CommaExprStar
/*****
  Define the type of attribute values for grammar symbols here.

  Examples:
  %type <int_val> TOK_INT_CONST
  %type <bool_val> VisibilityOpt
  %type <entity_list> ClassDeclarations
  The above declarations say that TOK_INT_CONST's attribute value is in the 
  int_val field of YYSTYPE (defined by the "union" earlier); 
  VisibilityOpt's attribute value is in bool_val field of YYSTYPE; 
  and the attribute value of ClassDeclarations is in the entity_list field 
  of YYSTYPE

 *****/

%%

/**/
/***************************** DECLARATIONS  ********************************/
/**/

Program :  { 
  class_list = new list<Entity *>;
}
ClassDeclarations {
  /* In the action for this production, set 
     the global variable "toplevel" to the list of entities representing 
     the classes (i.e. the attribute of ClassDeclarations).
   */
  toplevel = class_list;  // This is a placeholder. Change this!!
}
;

ClassDeclarations:
ClassDeclarations ClassDeclaration 
| 
;

ClassDeclaration:
TOK_CLASS TOK_ID ExtendsOpt {
  // FIXME Use $$ instead of new class
  // FIXME Can there be a class inside a class?
  //       If yes, then, use a stack instead of class_members
  class_members = new list<Entity *>;
  new_class = new ClassEntity($2, NULL, class_members);
}
TOK_OPEN_BRACE {
  // enter_scope();
}
ClassBodyDecls 
TOK_CLOSE_BRACE {
  // leave_scope();
  class_list->push_back(new_class);
}
;

ExtendsOpt:
    TOK_EXTENDS TOK_ID {
        // TODO Fill this up
    }
    | 
;

ClassBodyDecls:
    ClassBodyDecls ClassBodyDecl
    | ClassBodyDecl
;

ClassBodyDecl:	
    FieldDecl
    | MethodDecl
    | ConstructorDecl
;

FieldDecl: Modifier Type TOK_ID DimStar TOK_SEMICOLON {
             // TODO Fix the dimensions
             Entity * new_field = new FieldEntity($3, visibility_flag, static_flag, type, 0);
             // FIXME Potential problem here
             class_members->push_back(new_field);
           }
;
Modifier: VisibilityOpt StaticOpt
;


VisibilityOpt : 
TOK_PUBLIC {
  visibility_flag = true;
}
| TOK_PRIVATE {
  visibility_flag = false;
}
| 
;

StaticOpt:
TOK_STATIC {
  static_flag = true;
}
| { static_flag = false; }
;

VarDecl: Type Variables TOK_SEMICOLON 
;

Type:	  TOK_INT {
            type = new IntType(); 
          }
| TOK_FLOAT {
  type = new FloatType();
}
| TOK_BOOLEAN {
  type = new BooleanType();
}
| TOK_ID {
  type = new StringType();
}
;

Variables: Variable VariablesCommaList 	
;

Variable: TOK_ID DimStar 
;

VariablesCommaList:
TOK_COMMA Variable VariablesCommaList
| 
;

MethodDecl:
MethodHead TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block  {
  // TODO Fix these
  formal_params = new list<Entity *>;
  //formal_params->push_back(new SkipStatement());
  stmt_list = new list<Statement *>;
  method_body = new BlockStatement(stmt_list);
  //method_body = $5;
  new_method = new MethodEntity(method_name, visibility_flag, static_flag, type, formal_params, method_body);
  class_members->push_back(new_method);
}
;

MethodHead: 
Modifier Type TOK_ID {
  method_name = $3;
}
| Modifier TOK_VOID TOK_ID {
  method_name = $3;
  type = new VoidType();
}
;

FormalsOpt:
FormalParam FormalParamCommaList
| 
;

FormalParam: 
Type Variable
;

FormalParamCommaList: 
TOK_COMMA FormalParam FormalParamCommaList
| 
;

ConstructorDecl:
Modifier TOK_ID TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block
;

/**/
/*****************************   STATEMENTS ********************************/
/**/


Block:	  
    TOK_OPEN_BRACE {
        // enter_block();
    } StmtStar {
    } TOK_CLOSE_BRACE {
        // leave_block();
    }
;

StmtStar:
    Stmt StmtStar {
        $2.push_front($1);
        $$ = $2;
    }
    | {
        $$ = new list<Statement*>();
    }
;

Stmt:
    TOK_IF TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt OptElsePart {
        $$ = new IfStatement($3, $5, $6); 
    }
    | TOK_WHILE TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt {
        $$ = new WhileStatement($3, $4);
    }
    | TOK_FOR TOK_OPEN_PAREN StmtExprOpt
        TOK_SEMICOLON ExprOpt
        TOK_SEMICOLON StmtExprOpt
        TOK_CLOSE_PAREN Stmt {
        $$ = new ForStatement($3, $5, $7, $9);
    }
    | TOK_RETURN Expr TOK_SEMICOLON {
        $$ = new ReturnStatement($1);
    }
    | Block {
        $$ = $1;
    }
    | StmtExpr TOK_SEMICOLON {
        $$ = $1;
    }
    | VarDecl {
        $$ = $1;
    }
    | TOK_BREAK TOK_SEMICOLON {
        $$ = new BreakStatement();
    }
    | TOK_CONTINUE TOK_SEMICOLON {
    $$ = new ContinueStatement();
    }
    | TOK_SEMICOLON {
        $$ = new SkipStatement();
    }
    | error TOK_SEMICOLON {
        /* Error production to synchronize at SEMICOLON on any parse error */
        $$ = new SkipStatement();
    }
;


OptElsePart:
    TOK_ELSE Stmt {
        $$ = $2;
    }
    | {
        $$ = new SkipStatement();
    }
;

StmtExprOpt:
    StmtExpr {
        $$ = $1;
    }
    | {
        $$ = new SkipStatement();
    }
;

ExprOpt:
    Expr {
        $$ = $1;
    }
    | {
        $$ = new NullExpression();
    }
;

StmtExpr:
    Assignment {
        $$ = $1;
    }
    | MethodInvocation {
        $$ = $1;
    }
;

/**/
/*****************************   EXPRESSIONS ********************************/
/**/


Primary:  
    Literal {
        $$ = $1;
    }
    | TOK_THIS {
        $$ = new ThisExpression();
    }
    | TOK_SUPER {
        $$ = new SuperExpression();
    }
    | TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN {
        $$ = $2;
    }
    | TOK_NEW TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN {
        // TODO Complete this
        // TOK_ID is a class. 
        // TODO: Check if TOK_ID has been declared in the scope.
        Entity *class_entity = NULL; // TODO: Lookup symbol table.
        $$ = new NewInstance(class_entity, $4);
    }
    | LeftHandSide {
        $$ = $1;
    }
    | MethodInvocation {
        $$ = $1;
    }
;

ArgumentListOpt: 	
    Expr CommaExprStar {
        $2->push_back($1);
        $$ = $2;
    }
    | {
        $$ = new list<Expression *>;
    }
;

CommaExprStar:
    TOK_COMMA Expr CommaExprStar {
        $3->push_back($2);
        $$ = $3;
    }
    | {
        $$ = new list<Expression *>;
    }
;

FieldAccess:
    Primary TOK_DOT TOK_ID {
        $$ = new FieldAccess($1, $3);
    }
    | TOK_ID {
        // TODO: Is this correct?
        $$ = new FieldAccess(new NullExpression(), $1);
    }
;

ArrayAccess:
    Primary TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET {
        // TODO: Check
        $$ = new ArrayAccess($1, $3);
    }
;

MethodInvocation:
    Primary TOK_DOT TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN {
        $$ = new MethodInvocation($1, $3, $5);
    }
    |
    TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN {
        // TODO Fill this up
        $$ = new MethodInvocation(new ThisExpression(), $1, $3);
    }
;

Expr:	  
    Expr TOK_MULTIPLY Expr {
        $$ = new BinaryExpression(MUL, $1, $3);
    }
    | Expr TOK_DIVIDE Expr {
        $$ = new BinaryExpression(DIV, $1, $3);
    }
    | Expr TOK_PLUS Expr {
        $$ = new BinaryExpression(ADD, $1, $3);
    }
    | Expr TOK_MINUS Expr {
        $$ = new BinaryExpression(SUB, $1, $3);
    }
    | Expr TOK_AND Expr {
        $$ = new BinaryExpression(AND, $1, $3);
    }
    | Expr TOK_OR Expr {
        $$ = new BinaryExpression(OR, $1, $3);
    }
    | Expr TOK_EQUAL_EQUAL Expr {
        $$ = new BinaryExpression(EQ, $1, $3);
    }
    | Expr TOK_NOT_EQUAL Expr {
        $$ = new BinaryExpression(NEQ, $1, $3);
    }
    | Expr TOK_LESSER Expr {
        $$ = new BinaryExpression(LT, $1, $3);
    }
    | Expr TOK_GREATER Expr {
        $$ = new BinaryExpression(GT, $1, $3);
    }
    | Expr TOK_LESSER_OR_EQUAL Expr	{
        $$ = new BinaryExpression(LEQ, $1, $3);
    }
    | Expr TOK_GREATER_OR_EQUAL Expr {
        $$ = new BinaryExpression(GEQ, $1, $3);
    }
    | TOK_MINUS Expr	%prec TOK_NOT {
        $$ = new UnaryExpression(UMINUS, $2);
    }
    | TOK_PLUS Expr		%prec TOK_NOT	 {
        $$ = $2;
    }
    | TOK_NOT Expr {
        $$ = new UnaryExpression(NEG, $2);
    }
    | Primary {
        $$ = $1;
    }
    | Assignment {
        // TODO Fill this
    }
    | TOK_NEW Type DimExprPlus DimStar {
        // TODO Fill this
    }
;	

DimExprPlus:
DimExprPlus DimExpr
| DimExpr
;

DimExpr:  TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET
;

DimStar:  Dim DimStar
|
;

Dim:	  TOK_OPEN_SQ_BRACKET  TOK_CLOSE_SQ_BRACKET ;


Assignment:
LeftHandSide TOK_EQUAL Expr
| LeftHandSide TOK_PLUS_PLUS
| LeftHandSide TOK_MINUS_MINUS
| TOK_PLUS_PLUS LeftHandSide
| TOK_MINUS_MINUS LeftHandSide
;

LeftHandSide:
    FieldAccess {
        $$ = $1;
    }
    | ArrayAccess {
        $$ = $1;
    }
;

Literal:  
    TOK_INT_CONST {
        $$ = new IntegerConstant($1);  
    }
    | TOK_FLOAT_CONST {
        $$ = new FloatConstant($1);
    }
    | TOK_STRING_CONST {
        $$ = new StringConstant($1);
    }
    | TOK_NULL {
        $$ = new NullExpression();
    }
    | TOK_TRUE {
        $$ = new BooleanConstant(true);
    }
    | TOK_FALSE {
        $$ = new BooleanConstant(false);
    }
;

%%
