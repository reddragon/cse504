%{
#include <stdio.h>
#include <iostream>

#include "Ast.hh"

using namespace std;

// Prototypes to lexer functions
extern void yyerror (const char *error);
extern int  yylex ();

// interface to the outside world
extern list<Entity *> *toplevel;  // list of top-level classes
extern EntityTable *global_symtab; // global symbol table

%}

%token TOK_BOOLEAN TOK_BREAK TOK_CLASS TOK_CONTINUE TOK_ELSE 
        TOK_EXTENDS TOK_FALSE TOK_FLOAT TOK_FOR TOK_IF TOK_INT TOK_NEW 
        TOK_NULL TOK_PRIVATE TOK_PUBLIC TOK_RETURN TOK_STATIC TOK_SUPER
        TOK_THIS TOK_TRUE TOK_VOID TOK_WHILE TOK_DO
        TOK_INT_CONST TOK_FLOAT_CONST TOK_STRING_CONST 
        TOK_ID 
        TOK_COMMA TOK_DOT TOK_SEMICOLON
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

Program :  ClassDeclarations {
	/* In the action for this production, set 
	the global variable "toplevel" to the list of entities representing 
	the classes (i.e. the attribute of ClassDeclarations).
	*/
	toplevel = NULL;  // This is a placeholder. Change this!!
	}
	;

ClassDeclarations:
	  ClassDeclarations ClassDeclaration 
        | 
	;

ClassDeclaration:
	TOK_CLASS TOK_ID ExtendsOpt
		TOK_OPEN_BRACE 
		ClassBodyDecls 
		TOK_CLOSE_BRACE
		;

ExtendsOpt:
	  TOK_EXTENDS TOK_ID 
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

FieldDecl: Modifier Type TOK_ID DimStar TOK_SEMICOLON
	 ;
Modifier: VisibilityOpt StaticOpt
	 ;


VisibilityOpt : 
          TOK_PUBLIC
        | TOK_PRIVATE
        | 
	;

StaticOpt:
          TOK_STATIC
        |
	;

VarDecl: Type Variables TOK_SEMICOLON 
	 ;

Type:	  TOK_INT
	| TOK_FLOAT
	| TOK_BOOLEAN
	| TOK_ID
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
	  MethodHead TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block 
	  ;

MethodHead: Modifier Type TOK_ID 
         | Modifier TOK_VOID TOK_ID 
	 ;

FormalsOpt:
	  FormalParam FormalParamCommaList
        | 
	;

FormalParam: Type Variable
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


Block:	  TOK_OPEN_BRACE  StmtStar TOK_CLOSE_BRACE 	
	  ;

StmtStar:
	  Stmt StmtStar 
	| 
	;


Stmt:	  TOK_IF TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt OptElsePart
	| TOK_WHILE TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt
	| TOK_FOR TOK_OPEN_PAREN StmtExprOpt
	                    TOK_SEMICOLON ExprOpt
			    TOK_SEMICOLON StmtExprOpt
		TOK_CLOSE_PAREN Stmt

	| TOK_RETURN Expr TOK_SEMICOLON
	| Block
	| StmtExpr TOK_SEMICOLON
	| VarDecl
	| TOK_BREAK TOK_SEMICOLON
	| TOK_CONTINUE TOK_SEMICOLON
	| TOK_SEMICOLON
	| error TOK_SEMICOLON 
	  /* Error production to synchronize at SEMICOLON on any parse error */
	;

OptElsePart:
	  TOK_ELSE Stmt 
	|
	;

StmtExprOpt:
	  StmtExpr
	|
	;

ExprOpt:
	  Expr
	|
	;

StmtExpr:  Assignment
	| MethodInvocation
	;

/**/
/*****************************   EXPRESSIONS ********************************/
/**/


Primary:  Literal
	| TOK_THIS
	| TOK_SUPER
	| TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN
	| TOK_NEW TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN 
	| LeftHandSide
	| MethodInvocation

ArgumentListOpt: 	
	  Expr CommaExprStar
	|
	;

CommaExprStar: TOK_COMMA Expr CommaExprStar
	|
	;

FieldAccess:
	  Primary TOK_DOT TOK_ID
	| TOK_ID 	
	;
ArrayAccess: Primary TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET 
	;

MethodInvocation:
	  Primary TOK_DOT TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN
	|
	  TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN
	;

Expr:	  Expr TOK_MULTIPLY Expr	
	| Expr TOK_DIVIDE Expr
	| Expr TOK_PLUS Expr	
	| Expr TOK_MINUS Expr	
	| Expr TOK_AND Expr	
	| Expr TOK_OR Expr	
	| Expr TOK_EQUAL_EQUAL Expr
	| Expr TOK_NOT_EQUAL Expr	
	| Expr TOK_LESSER Expr		
	| Expr TOK_GREATER Expr		
	| Expr TOK_LESSER_OR_EQUAL Expr	
	| Expr TOK_GREATER_OR_EQUAL Expr
	| TOK_MINUS Expr	%prec TOK_NOT
	| TOK_PLUS Expr		%prec TOK_NOT	 
	| TOK_NOT Expr
	| Primary
	| Assignment
	| TOK_NEW Type DimExprPlus DimStar 
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
	  FieldAccess
	| ArrayAccess
	;

Literal:  TOK_INT_CONST
	| TOK_FLOAT_CONST
	| TOK_STRING_CONST
	| TOK_NULL
	| TOK_TRUE
	| TOK_FALSE
	;

%%
