%{
#include <stdio.h>
#include <iostream>

#include "Ast.hh"

using namespace std;

// Prototypes to lexer functions
extern void yyerror (const char *error);
extern int  yylex ();

// interface to the outside world
extern Program *program;  // top-level classes in program
extern EntityTable *global_symtab; // global symbol table
extern ClassEntity* objectclass;   // top-level "Object" class
extern int install_string(char *s);	   //

// Global vars to simulate certain inherited attributes
Type *current_type;

%}

%token TOK_BOOLEAN TOK_BREAK TOK_CLASS TOK_CONTINUE TOK_ELSE 
        TOK_EXTENDS TOK_FALSE TOK_FLOAT TOK_FOR TOK_IF TOK_INT TOK_NEW 
        TOK_NULL TOK_PRIVATE TOK_PUBLIC TOK_RETURN TOK_STATIC TOK_STRING
	TOK_SUPER TOK_THIS TOK_TRUE TOK_VOID TOK_WHILE TOK_DO
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
	TOK_NATIVE

%union{
        char* string_val;
        int   int_val;
        float float_val;
        char char_val;
	bool bool_val;
	Entity *entity;
	ClassEntity *class_entity;
	VariableEntity *variable;
	Type *type_val;
	Statement *statement;
	Expression *expression;
	LhsExpression *lhs_expression;
	list<Entity*> *entity_list;
	list<ClassEntity*> *class_list;
	list<VariableEntity*> *var_list;
	list<Statement *> *statement_list;
	list<Expression *> *expression_list;
	pair<bool, bool> *pair_val;
}

%right TOK_EQUAL
%left TOK_OR
%left TOK_AND
%left TOK_EQUAL_EQUAL TOK_NOT_EQUAL 
%nonassoc TOK_LESSER TOK_GREATER TOK_LESSER_OR_EQUAL TOK_GREATER_OR_EQUAL
%left TOK_PLUS TOK_MINUS 
%left TOK_MULTIPLY TOK_DIVIDE
%left TOK_NOT 

%type <class_entity> ClassDeclaration ExtendsOpt
%type <entity>  ClassBodyDecl FieldDecl MethodDecl MethodHead ConstructorDecl
%type <variable> Variable FormalParam
%type <class_list> ClassDeclarations
%type <entity_list> ClassBodyDecls
%type <var_list> Variables VarDecl VariablesCommaList
%type <var_list> FormalsOpt FormalParamCommaList
%type <string_val> TOK_ID
%type <bool_val> VisibilityOpt StaticOpt
%type <pair_val> Modifier
%type <type_val> Type
%type <int_val> DimStar
%type <statement>  Block Stmt OptElsePart StmtExprOpt StmtExpr
%type <statement_list> StmtStar 
%type <expression> Primary Literal MethodInvocation
%type <expression> Expr DimExpr Assignment ExprOpt
%type <expression_list> ArgumentListOpt CommaExprStar DimExprPlus
%type <lhs_expression> LeftHandSide FieldAccess ArrayAccess 
%type <int_val> TOK_INT_CONST
%type <float_val> TOK_FLOAT_CONST
%type <string_val> TOK_STRING_CONST

%%

/**/
/***************************** DECLARATIONS  ********************************/
/**/

Program :  ClassDeclarations {
	program = new Program($1);
	}
	;

ClassDeclarations:
	  ClassDeclarations ClassDeclaration 
	  {
	  $1->push_back($2);
	  $$=$1;
	  }
        | {$$=new list<ClassEntity*>();};

ClassDeclaration:
	TOK_CLASS TOK_ID ExtendsOpt
		{
		   bool flag;
	     	   ClassEntity *cd = (ClassEntity*)
		     	    global_symtab->find_entity($2, CLASS_ENTITY, &flag);
	     	   if (cd != NULL) {// Already present
	     	      yyerror("Duplicate class declaration");
	           }
	     	   else { // create a new entity for this field
	     	   	cd = new ClassEntity($2, (ClassEntity*)$3, new list<Entity*>());
		   }
		   $<class_entity>$ = cd;
		}
		TOK_OPEN_BRACE 
		{global_symtab->enter_block();}
		ClassBodyDecls 
		{
		   ClassEntity* ce = $<class_entity>4;
		   ce->set_class_members($7);
		   // Add a default constructor method if none has been defined
		   list<Entity*>::iterator m;
		   bool found = false;
		   for(m=ce->class_members()->begin(); 
		       ((m != ce->class_members()->end()) && (!found)) ; m++)
		     if ((*m)->kind() == CONSTRUCTOR_ENTITY &&
		         ((ConstructorEntity*)(*m))->formal_params()->size() == 0)
			 found = true;
		    if (!found) // default constructor was not
		                // explicitly defined so define it here
		    {
		       Entity *e = new ConstructorEntity($2, true /*public*/,
     	    	       			new list<VariableEntity*>() /*formals*/,
					new SkipStatement());
		       ce->add_class_member(e);
		    }
                }
		TOK_CLOSE_BRACE
		{
		   global_symtab->leave_block();
		   $$ = $<class_entity>4;
		}
		;

ExtendsOpt:
	  TOK_EXTENDS TOK_ID {
	     bool flag;  	     
	     ClassEntity* e = (ClassEntity*)
	     		  global_symtab->find_entity($2, CLASS_ENTITY, &flag);
	     if (e == NULL) {
	        yyerror("Superclass not found");
		$$ = objectclass;
	     }   else
	       $$ = e;
	  }
        | { $$ = objectclass; };

ClassBodyDecls:
	  ClassBodyDecls ClassBodyDecl
          { 
	    $1->push_back($2);
	    $$ = $1;
	  }
	| ClassBodyDecl
          {
	    $$ = new list<Entity*>(); $$->push_back($1);
	  }

	;
ClassBodyDecl:	
	    FieldDecl	{$$ = $1;}
	  | MethodDecl	{$$ = $1;}
          | ConstructorDecl {$$ = $1;}
	  ;

FieldDecl: Modifier Type TOK_ID DimStar TOK_SEMICOLON
           {
	     bool flag;
	     Entity *fd = global_symtab->find_entity($3, FIELD_ENTITY, &flag);
	     if ((fd != NULL) && flag) {// in the same scope, so error
	     	yyerror("Duplicate field declaration");
	     }
	     else { // create a new entity for this field
	     	fd = new FieldEntity($3, $1->first, $1->second, 
	   	      	    $2, $4);
	     }
	     $$ = fd;
	   }
	 ;
Modifier: VisibilityOpt StaticOpt {$$ = new pair<bool, bool>($1, $2);};


VisibilityOpt : 
          TOK_PUBLIC  {$$ = true;}
        | TOK_PRIVATE {$$ = false;}
        | {$$ = true;};

StaticOpt:
          TOK_STATIC {$$ = true;}
        | {$$ = false;};

VarDecl: Type {current_type = $1;} Variables TOK_SEMICOLON 
         {
	   $$ = $3;
	   current_type = NULL;
	 }
	 ;

Type:	  TOK_INT	{$$ = new IntType();}
	| TOK_FLOAT	{$$ = new FloatType();}
	| TOK_BOOLEAN	{$$ = new BooleanType();}
	| TOK_STRING	{$$ = new StringType();}
	| TOK_ID 	
          {
		bool flag;
		Entity *e = global_symtab->find_entity($1, CLASS_ENTITY, &flag);
		if (e == NULL) {
		   yyerror("Class type not found");
		   $$ = new ErrorType();
		}  else
		   $$ = new InstanceType((ClassEntity*)e);
	  }
	  ;

Variables: Variable VariablesCommaList 	
          {
	    $2->push_front($1);
	    $$ = $2;
	  }
	  ;

Variable: TOK_ID DimStar 
          {
	     bool flag;
	     VariableEntity *vd = 
	     	    (VariableEntity*) global_symtab->find_entity($1, VARIABLE_ENTITY, &flag);
	     if ((vd != NULL) && flag) {// in the same scope, so error
	     	yyerror("Duplicate variable declaration");
	     }
	     else { // create a new entity for this field
	     	vd = new VariableEntity($1, current_type, $2);
	     }
	     $$ = vd;
	  }
	  ;

VariablesCommaList:
	  TOK_COMMA Variable VariablesCommaList
          {
	    $3->push_front($2);
	    $$ = $3;
	  }

        | {$$ = new list<VariableEntity*>();}
	;

MethodDecl:
	  MethodHead 
	  	    {global_symtab->enter_block();}
		    TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block 
          {
	    $$=$1;
	    ((MethodEntity *) $$)->set_formal_params($4);
	    ((MethodEntity *) $$)->set_method_body($6);
	    global_symtab->leave_block();
	  };

MethodHead: Modifier Type TOK_ID 
            { // no duplicate checks for method declaration!
	      $$ = new MethodEntity($3, $1->first, $1->second, $2, NULL, NULL);
	    }
         | Modifier TOK_VOID TOK_ID 
            {
	      $$ = new MethodEntity($3, $1->first, $1->second, new VoidType(), NULL, NULL);
	    };


FormalsOpt:
	  FormalParam FormalParamCommaList
          {
	    $$=$2;
	    $$->push_front($1);
	  }
        | {$$ = new list<VariableEntity*>();}
	;

FormalParam: Type {current_type = $1;} Variable
             {
	       $$=$3;
	       current_type = NULL;
	     };

FormalParamCommaList: 
	  TOK_COMMA FormalParam FormalParamCommaList
          {
	    $$=$3;
	    $$->push_front($2);
	  }
        | { $$= new list<VariableEntity*>();};

ConstructorDecl:
	 Modifier TOK_ID 
	 	  	    {global_symtab->enter_block();}
			    TOK_OPEN_PAREN FormalsOpt TOK_CLOSE_PAREN Block
         {
	   $$=new ConstructorEntity($2, $1->first, $5, $7);
	   global_symtab->leave_block();
	 };

/**/
/*****************************   STATEMENTS ********************************/
/**/


Block:	  TOK_OPEN_BRACE 
	  {global_symtab->enter_block();}
	  StmtStar TOK_CLOSE_BRACE 	
	  {
	     $$ = new BlockStatement($3);
	     global_symtab->leave_block();
	  }
	  ;

StmtStar:
	  Stmt StmtStar {
	    $$=$2;
	    $$->push_front($1);
	  }
	| {
	    $$= new list<Statement*>();
	};


Stmt:	  TOK_IF TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt OptElsePart
	  {
	    $$ = new IfStatement($3, $5, $6);
	  }

	| TOK_WHILE TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN Stmt
	  {
	    $$ = new WhileStatement($3, $5);
	  }

	| TOK_FOR TOK_OPEN_PAREN StmtExprOpt
	                    TOK_SEMICOLON ExprOpt
			    TOK_SEMICOLON StmtExprOpt
		TOK_CLOSE_PAREN Stmt
	  {
	    $$ = new ForStatement($3, $5, $7, $9);
	  }

	| TOK_RETURN ExprOpt TOK_SEMICOLON
	  {
	    $$ = new ReturnStatement($2);
	  }
	| Block
	  	 	{$$ = $1;}
	| StmtExpr TOK_SEMICOLON
	  	 	{$$ = $1;}
	| VarDecl
	  	 	{$$ = new DeclStatement($1);}
	| TOK_BREAK TOK_SEMICOLON
	  	 	{$$ = new BreakStatement();}
	| TOK_CONTINUE TOK_SEMICOLON
	  	 	{$$ = new ContinueStatement();}
	| TOK_SEMICOLON
	  	 	{$$ = new SkipStatement();}
	| TOK_NATIVE TOK_STRING_CONST TOK_SEMICOLON
	  	        {$$ = new NativeStatement($2);}
	| error TOK_SEMICOLON 
	  {$$ = new SkipStatement(); // fake a skip statement on error
	  }
	;

OptElsePart:
	  TOK_ELSE Stmt {$$ = $2;}
	| 	   	{$$ = new SkipStatement();}
	;

StmtExprOpt:
	  StmtExpr	{$$ = $1;}
	| 		{$$ = new SkipStatement();}
	;

ExprOpt:
	  Expr		{$$ = $1;}
	| 		{$$ = NULL;}
	;

StmtExpr:  Assignment		{$$ = new ExprStatement($1);}
	| MethodInvocation	{$$ = new ExprStatement($1);}
	;

/**/
/*****************************   EXPRESSIONS ********************************/
/**/


Primary:  Literal	{$$ = $1;}
	| TOK_THIS	{$$ = new ThisExpression();}
	| TOK_SUPER	{$$ = new SuperExpression();}
	| TOK_OPEN_PAREN Expr TOK_CLOSE_PAREN
  		 	{$$ = $2;}
	| TOK_NEW TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN 
	  	  	{bool flag;
			 ClassEntity *e = (ClassEntity*)global_symtab->find_entity($2, CLASS_ENTITY, &flag);
			 if (e == NULL)
			   yyerror("Class type not found");
			 $$ = new NewInstance(e, $4);
			}
	| LeftHandSide		{$$ = $1;}
	| MethodInvocation 	{$$ = $1;};

ArgumentListOpt: 	
	  Expr CommaExprStar	{$$ = $2;  $$->push_front($1);}
	| 			{$$ = new list<Expression*>();}
	;

CommaExprStar: TOK_COMMA Expr CommaExprStar 	{$$ = $3;  $$->push_front($2);}
	| 			{$$ = new list<Expression*>();}
	;

FieldAccess:
	  Primary TOK_DOT TOK_ID	{$$ = new FieldAccess($1, $3);}
	| TOK_ID 	
	  { // resolve this name
	    bool flag;
	    Entity *e = global_symtab->find_entity($1, VARIABLE_ENTITY, &flag);
	    if (e == NULL) {
	       // there is no variable with this name; find a field
	       e = global_symtab->find_entity($1, FIELD_ENTITY, &flag);
	       if (e == NULL) {
	       	  // there is no variable or field with this name; find a class
	          e = global_symtab->find_entity($1, CLASS_ENTITY, &flag);
		  if (e == NULL) {
		  // this identifier is not in scope; possibly inherited;
		  // so create a field expression of the form this.ID
		     $$ = new FieldAccess(new ThisExpression(), $1);
		  } else
		     $$ = new IdExpression(e); // e is a class entity
	       }  else
		 $$ = new FieldAccess(new ThisExpression(), $1); // field entity
  	    } else
	    $$ = new IdExpression(e); // e is a variable entity
	  }
	;
ArrayAccess: Primary TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET 
	{$$ = new ArrayAccess($1, $3);};

MethodInvocation:
	  Primary TOK_DOT TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN
	{
	 $$ = new MethodInvocation($1, $3, $5);
	}  
	|
	  TOK_ID TOK_OPEN_PAREN ArgumentListOpt TOK_CLOSE_PAREN
	{
	 $$ = new MethodInvocation(new ThisExpression(), $1, $3);
	}  
	;
Expr:	  Expr TOK_MULTIPLY Expr	
	  {$$ = new BinaryExpression(MUL, $1, $3);}
	| Expr TOK_DIVIDE Expr
	  {$$ = new BinaryExpression(DIV, $1, $3);}
	| Expr TOK_PLUS Expr	
	  {$$ = new BinaryExpression(ADD, $1, $3);}
	| Expr TOK_MINUS Expr	
	  {$$ = new BinaryExpression(SUB, $1, $3);}
	| Expr TOK_AND Expr	
	  {$$ = new BinaryExpression(AND, $1, $3);}
	| Expr TOK_OR Expr	
	  {$$ = new BinaryExpression(OR, $1, $3);}
	| Expr TOK_EQUAL_EQUAL Expr
	  {$$ = new BinaryExpression(EQ, $1, $3);}
	| Expr TOK_NOT_EQUAL Expr	
	  {$$ = new BinaryExpression(NEQ, $1, $3);}
	| Expr TOK_LESSER Expr		
	  {$$ = new BinaryExpression(LT, $1, $3);}
	| Expr TOK_GREATER Expr		
	  {$$ = new BinaryExpression(GT, $1, $3);}
	| Expr TOK_LESSER_OR_EQUAL Expr	
	  {$$ = new BinaryExpression(LEQ, $1, $3);}
	| Expr TOK_GREATER_OR_EQUAL Expr
	  {$$ = new BinaryExpression(GEQ, $1, $3);}
	| TOK_MINUS Expr	%prec TOK_NOT
	  {$$ = new UnaryExpression(UMINUS, $2);}
	| TOK_PLUS Expr	%prec TOK_NOT	 
	  {$$ = $2;}
	| TOK_NOT Expr
	  {$$ = new UnaryExpression(NEG, $2);}
	| Primary	{$$ = $1;}
	| Assignment	{$$ = $1;}
	| TOK_NEW Type DimExprPlus DimStar 
	  {$$ = new NewArrayInstance($2, $3->size() + $4, $3);
	  }
	;	

DimExprPlus:
	  DimExprPlus DimExpr  {$$=$1; $$->push_back($2);}
	| DimExpr     	       {$$ = new list<Expression *>; $$->push_back($1);}
	;

DimExpr:  TOK_OPEN_SQ_BRACKET Expr TOK_CLOSE_SQ_BRACKET {$$ = $2;};

DimStar:  Dim DimStar	 {$$ = $2+1;}
        | {$$=0;};

Dim:	  TOK_OPEN_SQ_BRACKET  TOK_CLOSE_SQ_BRACKET ;


Assignment:
	  LeftHandSide TOK_EQUAL Expr   {$$ = new AssignExpression($1, $3);}
	| LeftHandSide TOK_PLUS_PLUS 	{$$ = new AutoExpression(POST_INCR, $1);}
	| LeftHandSide TOK_MINUS_MINUS 	{$$ = new AutoExpression(POST_DECR, $1);}
	| TOK_PLUS_PLUS LeftHandSide 	{$$ = new AutoExpression(PRE_INCR, $2);}
	| TOK_MINUS_MINUS LeftHandSide 	{$$ = new AutoExpression(PRE_DECR, $2);}
	;

LeftHandSide:
	  FieldAccess	  	{$$ = $1;}
	| ArrayAccess 	  	{$$ = $1;};

Literal:  TOK_INT_CONST		{$$ = new IntegerConstant($1);}
	| TOK_FLOAT_CONST	{$$ = new FloatConstant($1);}
	| TOK_STRING_CONST	{$$ = new StringConstant(install_string($1));}
	| TOK_NULL	{$$ = new NullExpression();}
	| TOK_TRUE	{$$ = new BooleanConstant(true);}
	| TOK_FALSE 	{$$ = new BooleanConstant(false);};

%%
