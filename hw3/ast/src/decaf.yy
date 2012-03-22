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

// Global Variables
list<Entity *> *class_list, *class_members, *formal_params;
Entity * new_class, * new_method;
bool visibility_flag, static_flag; 
Type * type;
char * method_name;
Statement * method_body;
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
%type <string_val> TOK_ID 
%type <entity_list> ClassDeclarations
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
      list<Statement *> * stmt_list = new list<Statement *>;
      method_body = new BlockStatement(stmt_list);
      new_method = new MethodEntity(method_name, visibility_flag, static_flag, type, formal_params, method_body);
      class_members->push_back(new_method);
    }
	  ;

MethodHead: Modifier Type TOK_ID {
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
