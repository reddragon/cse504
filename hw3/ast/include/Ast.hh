		
//
// AST headers
//

#ifndef AST_H

#define AST_H

#include <list>
#include "AstSymbols.hh"

using namespace std;
using namespace __gnu_cxx;

class Statement {

public:  
  Statement() {}
  virtual ~Statement() {}
  virtual void print() = 0;
  int lineno;

};

class Expression{
public:
  Expression() {};
  virtual ~Expression() {};

  virtual void print() = 0;
};


typedef enum {
  ADD, SUB, MUL, DIV, EQ, NEQ, LT, LEQ, GT, GEQ, AND, OR
} BinaryOperator;

typedef enum {
  NEG, UMINUS
} UnaryOperator;

typedef enum {PRE_INCR, POST_INCR, PRE_DECR, POST_DECR} AutoOperator;

class LiteralExpression:public Expression{
public:
  LiteralExpression() {};
  virtual ~LiteralExpression() {};

  virtual void print() = 0;
};


class Type {
public:
  Type() {};
  virtual ~Type() {};
  virtual void print() = 0;
};

class IfStatement:public Statement{
   public:
     IfStatement(Expression* expr,Statement* thenpart,Statement* elsepart);
     virtual ~IfStatement();

   public:
     virtual Expression* expr();
     virtual Statement* thenpart();
     virtual Statement* elsepart();

               
     void print();  // no longer pure virtual
   private:
     Expression* expr_;
     Statement* thenpart_;
     Statement* elsepart_;

};

class WhileStatement:public Statement{
   public:
     WhileStatement(Expression* expr,Statement* body);
     virtual ~WhileStatement();

   public:
     virtual Expression* expr();
     virtual Statement* body();

          
     void print();  // no longer pure virtual
   private:
     Expression* expr_;
     Statement* body_;

};

class ForStatement:public Statement{
   public:
     ForStatement(Statement* init,Expression* guard,Statement* update,Statement* body);
     virtual ~ForStatement();

   public:
     virtual Statement* init();
     virtual Expression* guard();
     virtual Statement* update();
     virtual Statement* body();

                    
     void print();  // no longer pure virtual
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
   private:
     list<Statement*>* stmt_list_;

};

class DeclStatement:public Statement{
   public:
     DeclStatement(list<Entity*>* var_list);
     virtual ~DeclStatement();

   public:
     virtual list<Entity*>* var_list();

     
     void print();  // no longer pure virtual
   private:
     list<Entity*>* var_list_;

};

class ExprStatement:public Statement{
   public:
     ExprStatement(Expression* expr);
     virtual ~ExprStatement();

   public:
     virtual Expression* expr();

     
     void print();  // no longer pure virtual
   private:
     Expression* expr_;

};

class BreakStatement:public Statement{
   public:
     BreakStatement();
     virtual ~BreakStatement();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class ContinueStatement:public Statement{
   public:
     ContinueStatement();
     virtual ~ContinueStatement();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class SkipStatement:public Statement{
   public:
     SkipStatement();
     virtual ~SkipStatement();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class BinaryExpression:public Expression{
   public:
     BinaryExpression(BinaryOperator binary_operator,Expression* lhs,Expression* rhs);
     virtual ~BinaryExpression();

   public:
     virtual BinaryOperator binary_operator();
     virtual Expression* lhs();
     virtual Expression* rhs();

               
     void print();  // no longer pure virtual
   private:
     BinaryOperator binary_operator_;
     Expression* lhs_;
     Expression* rhs_;

};

class AssignExpression:public Expression{
   public:
     AssignExpression(Expression* lhs,Expression* rhs);
     virtual ~AssignExpression();

   public:
     virtual Expression* lhs();
     virtual Expression* rhs();

          
     void print();  // no longer pure virtual
   private:
     Expression* lhs_;
     Expression* rhs_;

};

class ArrayAccess:public Expression{
   public:
     ArrayAccess(Expression* base,Expression* idx);
     virtual ~ArrayAccess();

   public:
     virtual Expression* base();
     virtual Expression* idx();

          
     void print();  // no longer pure virtual
   private:
     Expression* base_;
     Expression* idx_;

};

class FieldAccess:public Expression{
   public:
     FieldAccess(Expression* base,char* name);
     virtual ~FieldAccess();

   public:
     virtual Expression* base();
     virtual char* name();

          
     void print();  // no longer pure virtual
   private:
     Expression* base_;
     char* name_;

};

class MethodInvocation:public Expression{
   public:
     MethodInvocation(Expression* base,char* name,list<Expression*>* args);
     virtual ~MethodInvocation();

   public:
     virtual Expression* base();
     virtual char* name();
     virtual list<Expression*>* args();

               
     void print();  // no longer pure virtual
   private:
     Expression* base_;
     char* name_;
     list<Expression*>* args_;

};

class UnaryExpression:public Expression{
   public:
     UnaryExpression(UnaryOperator unary_operator,Expression* arg);
     virtual ~UnaryExpression();

   public:
     virtual UnaryOperator unary_operator();
     virtual Expression* arg();

          
     void print();  // no longer pure virtual
   private:
     UnaryOperator unary_operator_;
     Expression* arg_;

};

class AutoExpression:public Expression{
   public:
     AutoExpression(AutoOperator auto_operator,Expression* arg);
     virtual ~AutoExpression();

   public:
     virtual AutoOperator auto_operator();
     virtual Expression* arg();

          
     void print();  // no longer pure virtual
   private:
     AutoOperator auto_operator_;
     Expression* arg_;

};

class NewArrayInstance:public Expression{
   public:
     NewArrayInstance(Type* type,int dimension,list<Expression*>* bounds);
     virtual ~NewArrayInstance();

   public:
     virtual Type* type();
     virtual int dimension();
     virtual list<Expression*>* bounds();

               
     void print();  // no longer pure virtual
   private:
     Type* type_;
     int dimension_;
     list<Expression*>* bounds_;

};

class NewInstance:public Expression{
   public:
     NewInstance(Entity* class_entity,list<Expression*>* args);
     virtual ~NewInstance();

   public:
     virtual Entity* class_entity();
     virtual list<Expression*>* args();

          
     void print();  // no longer pure virtual
   private:
     Entity* class_entity_;
     list<Expression*>* args_;

};

class ThisExpression:public Expression{
   public:
     ThisExpression();
     virtual ~ThisExpression();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class SuperExpression:public Expression{
   public:
     SuperExpression();
     virtual ~SuperExpression();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class IdExpression:public Expression{
   public:
     IdExpression(Entity* id);
     virtual ~IdExpression();

   public:
     virtual Entity* id();

     
     void print();  // no longer pure virtual
   private:
     Entity* id_;

};

class NullExpression:public LiteralExpression{
   public:
     NullExpression();
     virtual ~NullExpression();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class IntegerConstant:public LiteralExpression{
   public:
     IntegerConstant(int value);
     virtual ~IntegerConstant();

   public:
     virtual int value();

     
     void print();  // no longer pure virtual
   private:
     int value_;

};

class FloatConstant:public LiteralExpression{
   public:
     FloatConstant(float value);
     virtual ~FloatConstant();

   public:
     virtual float value();

     
     void print();  // no longer pure virtual
   private:
     float value_;

};

class BooleanConstant:public LiteralExpression{
   public:
     BooleanConstant(bool value);
     virtual ~BooleanConstant();

   public:
     virtual bool value();

     
     void print();  // no longer pure virtual
   private:
     bool value_;

};

class StringConstant:public LiteralExpression{
   public:
     StringConstant(char* value);
     virtual ~StringConstant();

   public:
     virtual char* value();

     
     void print();  // no longer pure virtual
   private:
     char* value_;

};

class IntType:public Type{
   public:
     IntType();
     virtual ~IntType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class FloatType:public Type{
   public:
     FloatType();
     virtual ~FloatType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class BooleanType:public Type{
   public:
     BooleanType();
     virtual ~BooleanType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class StringType:public Type{
   public:
     StringType();
     virtual ~StringType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class VoidType:public Type{
   public:
     VoidType();
     virtual ~VoidType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};

class ClassType:public Type{
   public:
     ClassType(Entity* classtype);
     virtual ~ClassType();

   public:
     virtual Entity* classtype();

     
     void print();  // no longer pure virtual
   private:
     Entity* classtype_;

};

class ErrorType:public Type{
   public:
     ErrorType();
     virtual ~ErrorType();

   public:
     

     
     void print();  // no longer pure virtual
   private:
     

};



#endif
