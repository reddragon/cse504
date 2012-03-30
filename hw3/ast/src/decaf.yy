%{ /* -*-  Mode:C; c-basic-offset:4; indent-tabs-mode:nil -*- */
/* vim:set ft=yy ts=4 sw=4 sts=4 cindent: */
#include <stdio.h>
#include <iostream>
#include <stack>
#include <cassert>
#include <cstring>
#include "Ast.hh"
using namespace std;

// Prototypes to lexer functions
extern void yyerror (const char *error);
extern int  yylex ();
extern int yylineno;

// interface to the outside world
extern list<Entity *> *toplevel;  // list of top-level classes
extern EntityTable *global_symtab; // global symbol table

// Global Variables
list<Entity *> *class_list, *formal_params;
Entity * new_method;
char * method_name;
Statement * method_body;
stack< list<Entity *> * > block_stmts;
list<Statement *> * stmt_list;
bool visibility_flag, static_flag;
stack<int> scope_stack;

#define VISIBILITY_PRIVATE 0
#define VISIBILITY_PUBLIC  1
#define STATIC_OFF         0
#define STATIC_ON          2
#define VISIBILITY_MASK    1
#define STATIC_MASK        2

#define SCOPE_OTHER        1
#define SCOPE_TRUE         4
#define SCOPE_FALSE        5

void enter_block(int requester = SCOPE_TRUE) {
    if (requester == SCOPE_TRUE && 
        !scope_stack.empty() && 
        scope_stack.top() == SCOPE_OTHER) {
        requester = SCOPE_FALSE;
    }
    scope_stack.push(requester);
    if (requester != SCOPE_FALSE) {
        global_symtab->enter_block();
    }
}

void leave_block() {
    assert(!scope_stack.empty());
    if (scope_stack.top() != SCOPE_FALSE) {
        global_symtab->leave_block();
    }
    scope_stack.pop();
}

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
  Statement* stmt;
  Expression* expr; 
  list<Expression*>* exprs;
  Entity* entity;
  list<Statement*>* stmts;
  Type* type;
  list<Type*>* type_list;
  list<Entity*>* entity_list;
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
%type <int_val> TOK_INT_CONST Modifier StaticOpt VisibilityOpt DimStar
%type <float_val> TOK_FLOAT_CONST
%type <entity> ClassDeclaration FieldDecl MethodDecl ConstructorDecl ClassBodyDecl MethodHead ExtendsOpt Variable FormalParam
%type <entity_list> ClassDeclarations ClassBodyDecls VariablesCommaList Variables FormalsOpt FormalParamCommaList
%type <stmt> Stmt OptElsePart StmtExprOpt Block StmtExpr VarDecl
%type <stmts> StmtStar
%type <expr> Expr Literal Primary MethodInvocation LeftHandSide FieldAccess ArrayAccess Assignment ExprOpt DimExpr
%type <exprs> ArgumentListOpt CommaExprStar DimExprPlus
%type <type> Type
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

Program :  
    ClassDeclarations {
        /* In the action for this production, set 
        the global variable "toplevel" to the list of entities representing 
        the classes (i.e. the attribute of ClassDeclarations).
        */
        toplevel = $1; 
    }
;

ClassDeclarations:
    ClassDeclarations ClassDeclaration {
        $1->push_back($2);
        $$ = $1;
    }
    | { $$ = new list<Entity*>; }
;

ClassDeclaration:
    TOK_CLASS TOK_ID ExtendsOpt {
        bool current;
        ClassEntity* c = (ClassEntity *)global_symtab->find_entity($2, CLASS_ENTITY, & current);
        // FIXME Add error handling for duplicate class
        assert(!c);
        new ClassEntity($2, $3, NULL);
    }
    TOK_OPEN_BRACE {
        enter_block(SCOPE_OTHER);
        // global_symtab->enter_block();
    }
    ClassBodyDecls TOK_CLOSE_BRACE {
        bool current;
        $$ = global_symtab->find_entity($2, CLASS_ENTITY, &current);
        ((ClassEntity*)$$)->set_class_members($7);
        // global_symtab->leave_block();
        leave_block();
    }
;

ExtendsOpt:
    TOK_EXTENDS TOK_ID {
        bool current;
        ClassEntity *c = (ClassEntity*)global_symtab->find_entity($2, CLASS_ENTITY, &current);
        if (!c) {
            // FIXME - Call error handling routine
            assert(false);
        }
        $$ = c;
    }
    | { $$ = NULL }
;

ClassBodyDecls:
    ClassBodyDecls ClassBodyDecl {
        $$ = $1;
        $$->push_back($2);
    }
    | ClassBodyDecl {
        $$ = new list<Entity*>;
        $$->push_back($1);
    }
;

ClassBodyDecl:	
    FieldDecl { $$ = $1; }
    | MethodDecl { $$ = $1; }
    | ConstructorDecl { $$ = $1; }
;

FieldDecl: 
    Modifier Type TOK_ID DimStar TOK_SEMICOLON {
        bool current;
        FieldEntity* f = (FieldEntity *)global_symtab->find_entity($3, FIELD_ENTITY, &current);
        // FIXME Replace with error handling code for duplicate field declaration
        assert(!f);
        // FIXME Fix the dimensions
        int m = $1;
        $$ = new FieldEntity($3, m & VISIBILITY_MASK, 
                             m & STATIC_MASK, $2, $4);
    }
;

Modifier: 
    VisibilityOpt StaticOpt {
        $$ = $1 | $2;
    }
;

VisibilityOpt : 
    TOK_PUBLIC {
        $$ = VISIBILITY_PUBLIC;
    }
    | TOK_PRIVATE {
        $$ = VISIBILITY_PRIVATE;
    }
    | {
        $$ = VISIBILITY_PUBLIC;
    }
;

StaticOpt:
    TOK_STATIC {
        $$ = STATIC_ON;
    }
    | { 
        $$ = STATIC_OFF;
    }
;

VarDecl:
    Type Variables TOK_SEMICOLON {
        for (list<Entity*>::iterator i = $2->begin(); 
             i != $2->end(); ++i) {
            ((VariableEntity*)(*i))->set_type($1);
        }
        $$ = new DeclStatement($2);
    }
;

Type:	  
    TOK_INT {
        $$ = new IntType();
    }
    | TOK_FLOAT {
        $$ = new FloatType();
    }
    | TOK_BOOLEAN {
        $$ = new BooleanType();
    }
    | TOK_ID {
        bool current;
        ClassEntity *c = (ClassEntity*)global_symtab->find_entity($1, CLASS_ENTITY, &current);
        if (!c) {
            // FIXME - Call error handling routine
            assert(false);
        }
        $$ = new ClassType(c);
    }
;

Variables:
    Variable VariablesCommaList {
        $2->push_front($1);
        $$ = $2;
    }
;

Variable: 
    TOK_ID DimStar {
      bool current;
      VariableEntity* v = (VariableEntity *)global_symtab->find_entity($1, VARIABLE_ENTITY, &current);
      // FIXME Add error handling for duplicate variable
      //if(!(!v || (v && !current))) cout << "Variable: " << $1 << endl;
      assert(!v || (v && !current));
      $$ = new VariableEntity($1, NULL, $2);
    }
;

VariablesCommaList:
    TOK_COMMA Variable VariablesCommaList {
        $3->push_front($2);
        $$ = $3;
    }
    | { $$ = new list<Entity*>; }
;

MethodDecl:
    MethodHead {
        enter_block(SCOPE_OTHER);
    }
    TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block  {
        // TODO Fix these
        // formal_params = new list<Entity *>;
        $$ = $1;
        ((MethodEntity *)$$)->set_formal_params($4);
        ((MethodEntity *)$$)->set_method_body($6);
        leave_block();
    }
;

MethodHead: 
    Modifier Type TOK_ID {
        $$ = new MethodEntity($3, visibility_flag, static_flag, $2, NULL, NULL);
    }
    | Modifier TOK_VOID TOK_ID {
        $$ = new MethodEntity($3, visibility_flag, static_flag, new VoidType(), NULL, NULL);
    }
;

FormalsOpt:
    FormalParam FormalParamCommaList {
        $$ = $2;
        $$->push_front($1);
    }
    | {
        $$ = new list<Entity*>;
    }
;

FormalParam: 
    Type Variable {
        $$ = $2;
        ((VariableEntity*)$$)->set_type($1);
    }
;

FormalParamCommaList: 
    TOK_COMMA FormalParam FormalParamCommaList {
        $$ = $3;
        $$->push_front($2);
    }
    | {
        $$ = new list<Entity*>;
    }
;

ConstructorDecl:
    Modifier TOK_ID TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block {
        $$ = new ConstructorEntity($2, visibility_flag, $4, $6);
    }
;

/**/
/*****************************   STATEMENTS ********************************/
/**/


Block:
    TOK_OPEN_BRACE {
        // global_symtab->enter_block();
        enter_block(SCOPE_TRUE);
    } StmtStar TOK_CLOSE_BRACE {
        $$ = new BlockStatement($3);
        // global_symtab->leave_block();
        leave_block();
    }
;

StmtStar:
    Stmt StmtStar {
        $2->push_front($1);
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
        $$ = new WhileStatement($3, $5);
    }
    | TOK_FOR {
        enter_block(SCOPE_OTHER);
    }
        TOK_OPEN_PAREN StmtExprOpt
        TOK_SEMICOLON ExprOpt
        TOK_SEMICOLON StmtExprOpt
        TOK_CLOSE_PAREN Stmt {
        $$ = new ForStatement($4, $6, $8, $10);
        leave_block();
    }
    | TOK_RETURN Expr TOK_SEMICOLON {
        $$ = new ReturnStatement($2);
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
        $$ = new ExprStatement($1);
    }
    | MethodInvocation {
        $$ = new ExprStatement($1);
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
        $2->push_front($1);
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
        bool current = 0;
        Entity * e = global_symtab->find_entity($1, VARIABLE_ENTITY, &current);
        
        if(!e) {
            e = global_symtab->find_entity($1, FIELD_ENTITY, &current);
        }
        
        if(!e) {
            e = global_symtab->find_entity($1, CLASS_ENTITY, &current);
        }
        
        if(!e) {
            $$ = new FieldAccess(new ThisExpression, $1);
        }
        else
            $$ = new IdExpression(e);
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
        $$ = $1;
    }
    | TOK_NEW Type DimExprPlus DimStar {
        // TODO Fill this
        $$ = new NewArrayInstance($2, $3->size() + $4, $3);
    }
;

DimExprPlus:
    DimExprPlus DimExpr {
        $$ = $1;
        $$->push_back($2);
    }
    | DimExpr {
        $$ = new list<Expression*>;
        $$->push_back($1);
    }
;

DimExpr:
    TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET {
        $$ = $2;
    }
;

DimStar:
    Dim DimStar {
        $$ = $2 + 1;
    }
    | { $$ = 0; }
;

Dim:	  TOK_OPEN_SQ_BRACKET  TOK_CLOSE_SQ_BRACKET ;


Assignment:
    LeftHandSide TOK_EQUAL Expr {
        $$ = new AssignExpression($1, $3);
    }
    | LeftHandSide TOK_PLUS_PLUS {
        $$ = new AutoExpression(POST_INCR, $1);
    }
    | LeftHandSide TOK_MINUS_MINUS {
        $$ = new AutoExpression(POST_DECR, $1);
    }
    | TOK_PLUS_PLUS LeftHandSide {
        $$ = new AutoExpression(PRE_INCR, $2);
    }
    | TOK_MINUS_MINUS LeftHandSide {
        $$ = new AutoExpression(PRE_DECR, $2);
    }
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
