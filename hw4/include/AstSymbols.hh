#ifndef AST_SYMBOLS_H

#define AST_SYMBOLS_H

#include <list>
#include "Symbols.hh"

using namespace std;
using namespace __gnu_cxx;

class ClassEntity:public Entity {
public:
  ClassEntity(const char*name, ClassEntity *superclass, list<Entity*> *class_members);
  virtual ~ClassEntity();
  
public:
  virtual ClassEntity* superclass();
  virtual list<Entity*> *class_members();
  virtual void set_class_members(list<Entity*> *class_members);
  virtual void add_class_member(Entity* e);
  virtual void print();

  virtual void typecheck();
private:
  ClassEntity* superclass_;
  list<Entity*> *class_members_;

};


class Statement;  // This is a declaration; Statement will be defined elsewhere
class Type;       // This is also a declaration; definition is elsewhere

  
class MethodEntity:public Entity{
public:
  MethodEntity(const char*name, bool visibility_flag, bool static_flag,
	       Type *return_type, list<Entity*> *formal_params,
	       Statement *method_body);
  virtual ~MethodEntity();

public:
  virtual bool visibility_flag();
  virtual bool static_flag();
  virtual Type *return_type();
  virtual list <Entity*> *formal_params();
  virtual Statement *method_body();
  virtual void set_visibility_flag(bool visibility_flag);
  virtual void set_static_flag(bool static_flag);
  virtual void set_return_type(Type *return_type);
  virtual void set_formal_params(list<Entity*> *formal_params);
  virtual void set_method_body(Statement *method_body);
  virtual void print();

  virtual void typecheck();
private:
  bool visibility_flag_;
  bool static_flag_;
  Type *return_type_;
  list<Entity*> *formal_params_;
  Statement* method_body_;
};

class FieldEntity:public Entity{
public:
  FieldEntity(const char*name, bool visibility_flag, bool static_flag,
	      Type* elementtype, int dimensions);
  virtual ~FieldEntity();

public:
  virtual bool visibility_flag();
  virtual bool static_flag();
  virtual Type *type();
  virtual void print();

private:
  bool visibility_flag_;
  bool static_flag_;
  Type *type_;
};

class ConstructorEntity:public Entity{
public:
  ConstructorEntity(const char*name, bool visibility_flag,
	       list<Entity*> *formal_params,
	       Statement *constructor_body);
  virtual ~ConstructorEntity();

public:
  virtual bool visibility_flag();
  virtual list <Entity*> *formal_params();
  virtual Statement *constructor_body();
  virtual void set_visibility_flag(bool visibility_flag);
  virtual void set_formal_params(list<Entity*> *formal_params);
  virtual void set_constructor_body(Statement *constructor_body);
  virtual void print();

  virtual void typecheck();

private:
  bool visibility_flag_;
  list<Entity*> *formal_params_;
  Statement* constructor_body_;
};
  
class VariableEntity:public Entity{
public:
  VariableEntity(const char*name, Type* elementtype, int dimensions);
  virtual ~VariableEntity();

public:
  virtual Type *type();

  virtual void set_type(Type *t);
  virtual void print();

private:
  Type *type_;
};

#endif
