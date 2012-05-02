		
//
// AST headers
//

#ifndef AST_H

#define AST_H

#include <list>
#include "AstSymbols.hh"

using namespace std;
using namespace __gnu_cxx;

class Program {

public:
  Program(list<ClassEntity*> *classes) {classes_ = classes;}
  virtual ~Program() {}
  virtual list<ClassEntity*> *classes();
  virtual void print();
  virtual void typecheck();
  virtual void allocate();
  virtual void code();

private:
  list<ClassEntity*> *classes_;
};

#include "StatementContext.hh"

class Statement {

public:  
  Statement() {}
  virtual ~Statement() {}

  virtual void print() = 0;
  virtual int lineno();

  virtual void typecheck() = 0;
  virtual int allocate(int offset) = 0;
  virtual void code(StatementContext* c) = 0;
protected:
  int lineno_;

};

class Expression{
public:
  Expression() {};
  virtual ~Expression() {};

  virtual void print() = 0;
  virtual int lineno();

  virtual Type* typeinfer() = 0;

  virtual Type* type();
  virtual void set_type(Type *t);

  virtual void rcode() = 0;

protected:
  int lineno_;
  Type *type_;
};


class LhsExpression : public Expression{
public:
  LhsExpression() {};
  virtual ~LhsExpression() {};

  virtual void lcode() = 0;
  virtual void store() = 0;
  virtual void load() = 0;
  virtual bool indirect() = 0;
};


typedef enum {
  ADD, SUB, MUL, DIV, EQ, NEQ, LT, LEQ, GT, GEQ, AND, OR
} BinaryOperator;

typedef enum {
  NEG, UMINUS
} UnaryOperator;

typedef enum {PRE_INCR, POST_INCR, PRE_DECR, POST_DECR} AutoOperator;


typedef enum {INT_TYPE, FLOAT_TYPE, BOOLEAN_TYPE, STRING_TYPE, VOID_TYPE,
	INSTANCE_TYPE, CLASS_TYPE, ARRAY_TYPE, ERROR_TYPE, 
	UNIVERSE_TYPE, NULL_TYPE} TypeKind;

class Type {
public:
  Type() {};
  virtual ~Type() {};

  virtual TypeKind kind();
  virtual void print() = 0;
  virtual bool isSubtypeOf(Type *t);

protected:
   TypeKind kind_;
};



class IfStatement:public Statement{
   public:
     IfStatement(Expression* expr , Statement* thenpart , Statement* elsepart);
     virtual ~IfStatement();

   public:
     virtual Expression* expr();
     virtual Statement* thenpart();
     virtual Statement* elsepart();

               
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     Expression* expr_;
     Statement* thenpart_;
     Statement* elsepart_;



};

class WhileStatement:public Statement{
   public:
     WhileStatement(Expression* expr , Statement* body);
     virtual ~WhileStatement();

   public:
     virtual Expression* expr();
     virtual Statement* body();

          
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     Expression* expr_;
     Statement* body_;



};

class ForStatement:public Statement{
   public:
     ForStatement(Statement* init , Expression* guard , Statement* update , Statement* body);
     virtual ~ForStatement();

   public:
     virtual Statement* init();
     virtual Expression* guard();
     virtual Statement* update();
     virtual Statement* body();

                    
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     Statement* init_;
     Expression* guard_;
     Statement* update_;
     Statement* body_;



};

class ReturnStatement:public Statement{
   public:
     ReturnStatement(Expression* expr);
     virtual ~ReturnStatement();

   public:
     virtual Expression* expr();

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     Expression* expr_;



};

class BlockStatement:public Statement{
   public:
     BlockStatement(list<Statement*>* stmt_list);
     virtual ~BlockStatement();

   public:
     virtual list<Statement*>* stmt_list();

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     list<Statement*>* stmt_list_;



};

class DeclStatement:public Statement{
   public:
     DeclStatement(list<VariableEntity*>* var_list);
     virtual ~DeclStatement();

   public:
     virtual list<VariableEntity*>* var_list();

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     list<VariableEntity*>* var_list_;



};

class ExprStatement:public Statement{
   public:
     ExprStatement(Expression* expr);
     virtual ~ExprStatement();

   public:
     virtual Expression* expr();

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     Expression* expr_;



};

class BreakStatement:public Statement{
   public:
     BreakStatement( );
     virtual ~BreakStatement();

   public:
     

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     



};

class ContinueStatement:public Statement{
   public:
     ContinueStatement( );
     virtual ~ContinueStatement();

   public:
     

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     



};

class SkipStatement:public Statement{
   public:
     SkipStatement( );
     virtual ~SkipStatement();

   public:
     

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     



};

class NativeStatement:public Statement{
   public:
     NativeStatement(char* function_name);
     virtual ~NativeStatement();

   public:
     virtual char* function_name();

     
     void print();  // no longer pure virtual

     virtual void typecheck();
     

     virtual int allocate(int offset);

     virtual void code(StatementContext* c);
     

     
     
     
     
     
   private:
     char* function_name_;



};





class ArrayAccess:public LhsExpression{
   public:
     ArrayAccess(Expression* base , Expression* idx);
     virtual ~ArrayAccess();

   public:
     virtual Expression* base();
     virtual Expression* idx();

          
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     virtual void lcode();
     virtual void load();
     virtual void store();
     virtual bool indirect();
     
   private:
     Expression* base_;
     Expression* idx_;



};

class FieldAccess:public LhsExpression{
   public:
     FieldAccess(Expression* base , char* name );
     virtual ~FieldAccess();

   public:
     virtual Expression* base();
     virtual char* name();
     virtual FieldEntity* entity();

               virtual void set_entity(FieldEntity* entity);

     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     virtual void lcode();
     virtual void load();
     virtual void store();
     virtual bool indirect();
     
   private:
     Expression* base_;
     char* name_;
     FieldEntity* entity_;



};

class IdExpression:public LhsExpression{
   public:
     IdExpression(Entity* id);
     virtual ~IdExpression();

   public:
     virtual Entity* id();

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     virtual void lcode();
     virtual void load();
     virtual void store();
     virtual bool indirect();
     
   private:
     Entity* id_;



};



class BinaryExpression:public Expression{
   public:
     BinaryExpression(BinaryOperator binary_operator , Expression* lhs , Expression* rhs);
     virtual ~BinaryExpression();

   public:
     virtual BinaryOperator binary_operator();
     virtual Expression* lhs();
     virtual Expression* rhs();

               
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     BinaryOperator binary_operator_;
     Expression* lhs_;
     Expression* rhs_;



};

class AssignExpression:public Expression{
   public:
     AssignExpression(LhsExpression* lhs , Expression* rhs);
     virtual ~AssignExpression();

   public:
     virtual LhsExpression* lhs();
     virtual Expression* rhs();

          
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     LhsExpression* lhs_;
     Expression* rhs_;



};

class MethodInvocation:public Expression{
   public:
     MethodInvocation(Expression* base , char* name , list<Expression*>* args );
     virtual ~MethodInvocation();

   public:
     virtual Expression* base();
     virtual char* name();
     virtual list<Expression*>* args();
     virtual MethodEntity* entity();

                    virtual void set_entity(MethodEntity* entity);

     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     Expression* base_;
     char* name_;
     list<Expression*>* args_;
     MethodEntity* entity_;



};

class UnaryExpression:public Expression{
   public:
     UnaryExpression(UnaryOperator unary_operator , Expression* arg);
     virtual ~UnaryExpression();

   public:
     virtual UnaryOperator unary_operator();
     virtual Expression* arg();

          
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     UnaryOperator unary_operator_;
     Expression* arg_;



};

class AutoExpression:public Expression{
   public:
     AutoExpression(AutoOperator auto_operator , LhsExpression* arg);
     virtual ~AutoExpression();

   public:
     virtual AutoOperator auto_operator();
     virtual LhsExpression* arg();

          
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     AutoOperator auto_operator_;
     LhsExpression* arg_;



};

class NewArrayInstance:public Expression{
   public:
     NewArrayInstance(Type* type , int dimension , list<Expression*>* bounds);
     virtual ~NewArrayInstance();

   public:
     virtual Type* type();
     virtual int dimension();
     virtual list<Expression*>* bounds();

               
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     Type* type_;
     int dimension_;
     list<Expression*>* bounds_;



};

class NewInstance:public Expression{
   public:
     NewInstance(ClassEntity* class_entity , list<Expression*>* args );
     virtual ~NewInstance();

   public:
     virtual ClassEntity* class_entity();
     virtual list<Expression*>* args();
     virtual ConstructorEntity* entity();

               virtual void set_entity(ConstructorEntity* entity);

     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     ClassEntity* class_entity_;
     list<Expression*>* args_;
     ConstructorEntity* entity_;



};

class ThisExpression:public Expression{
   public:
     ThisExpression( );
     virtual ~ThisExpression();

   public:
     

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     



};

class SuperExpression:public Expression{
   public:
     SuperExpression( );
     virtual ~SuperExpression();

   public:
     

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     



};

class NullExpression:public Expression{
   public:
     NullExpression( );
     virtual ~NullExpression();

   public:
     

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     



};

class IntegerConstant:public Expression{
   public:
     IntegerConstant(int value);
     virtual ~IntegerConstant();

   public:
     virtual int value();

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     int value_;



};

class FloatConstant:public Expression{
   public:
     FloatConstant(float value);
     virtual ~FloatConstant();

   public:
     virtual float value();

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     float value_;



};

class BooleanConstant:public Expression{
   public:
     BooleanConstant(bool value);
     virtual ~BooleanConstant();

   public:
     virtual bool value();

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     bool value_;



};

class StringConstant:public Expression{
   public:
     StringConstant(int value);
     virtual ~StringConstant();

   public:
     virtual int value();

     
     void print();  // no longer pure virtual

     
     virtual Type* typeinfer();

     

     
     virtual void rcode();

     
     
     
     
     
   private:
     int value_;



};



class IntType:public Type{
   public:
     IntType( );
     virtual ~IntType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class FloatType:public Type{
   public:
     FloatType( );
     virtual ~FloatType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class BooleanType:public Type{
   public:
     BooleanType( );
     virtual ~BooleanType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class StringType:public Type{
   public:
     StringType( );
     virtual ~StringType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class VoidType:public Type{
   public:
     VoidType( );
     virtual ~VoidType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class ClassType:public Type{
   public:
     ClassType(ClassEntity* classtype);
     virtual ~ClassType();

   public:
     virtual ClassEntity* classtype();

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     ClassEntity* classtype_;



};

class InstanceType:public Type{
   public:
     InstanceType(ClassEntity* classtype);
     virtual ~InstanceType();

   public:
     virtual ClassEntity* classtype();

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     ClassEntity* classtype_;



};

class ErrorType:public Type{
   public:
     ErrorType( );
     virtual ~ErrorType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

class ArrayType:public Type{
   public:
     ArrayType(Type* elementtype);
     virtual ~ArrayType();

   public:
     virtual Type* elementtype();

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     Type* elementtype_;



};

// UniverseType is the top-most in type hierarchy; every type is in UniverseType
class UniverseType:public Type{
   public:
     UniverseType( );
     virtual ~UniverseType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};

// NullType is a bottom in type hierarchy; it is in every class type 
class NullType:public Type{
   public:
     NullType( );
     virtual ~NullType();

   public:
     

     
     void print();  // no longer pure virtual

     
     

     

     
     

     
     
     
     
     
   private:
     



};



#endif
