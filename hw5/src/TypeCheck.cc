#include<iostream>
#include "Ast.hh"
#include "Error.hh"

extern Error *error;
extern ClassEntity *objectclass;

Type *current_method_return_type;
bool current_method_static_flag;
ClassEntity *current_class;
Kind current_entity_kind;

UniverseType *universetype;
BooleanType *booltype;
IntType *inttype;
FloatType *floattype;
BooleanType *booleantype;
StringType *stringtype;
ErrorType *errortype;
ClassType *classtype;
InstanceType *instancetype;
ArrayType *arraytype;
NullType *nulltype;
VoidType *voidtype;

void initialize_typechecker() {
  universetype = new UniverseType();
  booltype = new BooleanType();
  inttype = new IntType();
  floattype = new FloatType();
  booleantype = new BooleanType();
  stringtype = new StringType();
  errortype = new ErrorType();
  classtype = new ClassType(objectclass);
  instancetype = new InstanceType(objectclass);
  arraytype = new ArrayType(universetype);
  nulltype = new NullType();
  voidtype = new VoidType();
}


void Program::typecheck() {
  initialize_typechecker();
  list<ClassEntity*>::iterator i;
  for(i=classes_->begin(); i != classes_->end(); i++) {
    (*i)->typecheck();	     
  }
}


void ClassEntity::typecheck() {
  current_class = this;

  list<Entity*>::iterator i;
  for(i=class_members()->begin(); i != class_members()->end(); i++) {
    Entity* member = (*i);
    if (member->kind() == METHOD_ENTITY)  {
      current_entity_kind = METHOD_ENTITY;
      ((MethodEntity*) member)->typecheck();
    } else if (member->kind() == CONSTRUCTOR_ENTITY) {
      current_entity_kind = CONSTRUCTOR_ENTITY;
      ((ConstructorEntity*) member)->typecheck();
    }
  }
}

void MethodEntity::typecheck() {
  current_method_return_type = return_type();
  current_method_static_flag = static_flag();
  method_body()->typecheck();
}

void ConstructorEntity::typecheck() {
  current_method_return_type = errortype;
  current_method_static_flag = false; // Constructors are not static
  constructor_body()->typecheck();
}

// Typecheck method for IfStatement
void IfStatement::typecheck() {
  Type* t = expr()->typeinfer();
  if (!(t->isSubtypeOf(booltype)))
    error->type_error(lineno(), "Boolean type expected in condition of IF statement", t);
  thenpart()->typecheck();
  elsepart()->typecheck();
}

// Typecheck method for WhileStatement
void WhileStatement::typecheck() {
  Type* t = expr()->typeinfer();
  if (!(t->isSubtypeOf(booltype)))
    error->type_error(lineno(), "Boolean type expected in condition of WHILE statement", t);    
  body()->typecheck();
}



// Typecheck method for ForStatement
void ForStatement::typecheck() {
  if (init() != NULL)
    init()->typecheck();
  Type* t = guard()->typeinfer();
  if (!(t->isSubtypeOf(booltype)))
    error->type_error(lineno(), "Boolean type expected in guard of FOR statement", t);    
  if (update() != NULL)
    update()->typecheck();
  body()->typecheck();
}

// Typecheck method for ReturnStatement
void ReturnStatement::typecheck() {
  if (current_entity_kind != METHOD_ENTITY)
    error->syntax_error(lineno(), "Return statement not valid in a constructor");
  Type *t;
  if (expr() != NULL) 
    t = expr()->typeinfer();
  else
    t = voidtype;

  if (!(t->isSubtypeOf(current_method_return_type))) 
    error->type_error(lineno(), 
		      "Return type incompatible with the declared type",  
		      t, current_method_return_type);
}

// Typecheck method for BlockStatement
void BlockStatement::typecheck() {
  list<Statement*>::iterator i;
  for(i=stmt_list()->begin(); i != stmt_list()->end(); i++)
    (*i)->typecheck();
}

// Typecheck method for ExprStatement
void ExprStatement::typecheck() {
  expr()->typeinfer();
  // returned values are silently thrown away; should really warn about this!
}



// Typecheck method for DeclStatement
void DeclStatement::typecheck() {
}



// Typecheck method for BreakStatement
void BreakStatement::typecheck() {
}



// Typecheck method for ContinueStatement
void ContinueStatement::typecheck() {
}



// Typecheck method for SkipStatement
void SkipStatement::typecheck() {
}

// Typecheck method for NativeStatement
void NativeStatement::typecheck() {
}



// Typeinfer method for BinaryExpression
Type* BinaryExpression::typeinfer() {
  Type *t1 = lhs()->typeinfer();
  Type *t2 = rhs()->typeinfer();

  switch(binary_operator()) {
  case ADD:
  case SUB:
  case MUL:
  case DIV:
    // All these have int/float arguments;
    // returns int/float (larger of the types)
    if (!(t1->isSubtypeOf(inttype) || t1->isSubtypeOf(floattype))) {
      error->type_error(lineno(), "Integer/floating point type expected", t1);
      return errortype;
    }
    if (!(t2->isSubtypeOf(inttype) || t2->isSubtypeOf(floattype))) {
      error->type_error(lineno(), "Integer/floating point type expected", t2);
      return errortype;
    }

    if ((t1->isSubtypeOf(inttype)) && (t2->isSubtypeOf(inttype))) {
      this->set_type(inttype);
      return inttype;
    } else {
      this->set_type(inttype);
      return floattype;
    }

  case LT:
  case LEQ:
  case GT:
  case GEQ:
    // All these have int/float arguments;
    // returns boolean
    if (!(t1->isSubtypeOf(inttype) || t1->isSubtypeOf(floattype))) {
      error->type_error(lineno(), "Integer/floating point type expected", t1);
      return errortype;
    }
    if (!(t2->isSubtypeOf(inttype) || t2->isSubtypeOf(floattype))) {
      error->type_error(lineno(), "Integer/floating point type expected", t2);
      return errortype;
    }
    this->set_type(booleantype);
    return booleantype;


  case EQ:
  case NEQ:
    // these operators are ok for all types as long as the two arg types 
    // are compatible.

    if (t1->isSubtypeOf(t2)) {
      this->set_type(booleantype);
      return booleantype;
    } else if (t1->isSubtypeOf(t2) || t2->isSubtypeOf(t1)) {
      this->set_type(booleantype);
      return booleantype;
    } else {
      error->type_error(lineno(), "Two incompatible types are compared for equality", t1, t2);
      return errortype;
    }

  case AND:
  case OR:
    // Both should have boolean arguments;
    if (!(t1->isSubtypeOf(booleantype))) {
      error->type_error(lineno(), "Boolean type expected", t1);
      return errortype;
    }
    
    if (!(t2->isSubtypeOf(booleantype))) {
      error->type_error(lineno(), "Boolean type expected", t2);
      return errortype;
    }
    
    this->set_type(booleantype);
    return booleantype;
  }
}



// Typeinfer method for AssignExpression
Type* AssignExpression::typeinfer() {
  Type *t1 = lhs()->typeinfer();
  Type *t2 = rhs()->typeinfer();
  if (! t2->isSubtypeOf(t1)) {
      error->type_error(lineno(), "Two incompatible types are used in assignment", t1, t2);
  }
  this->set_type(t2);
  return t2;
}



// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() {
  Type *t1 = base()->typeinfer();
  Type *t2 = idx()->typeinfer();
  if (!t2->isSubtypeOf(inttype)) {
    error->type_error(lineno(), "Index of array access must be integer type", t2);
    return errortype;
  }
  if (!t1->isSubtypeOf(arraytype)) {
    error->type_error(lineno(), "Base of array access must be an array", t1);
    return errortype;
  }
  if (t1->kind() == ARRAY_TYPE) {
    Type *rettype = ((ArrayType *)t1)->elementtype();
    this->set_type(rettype);
    return rettype;
  } else
    return errortype;
}


// Typeinfer method for FieldAccess:
Type* FieldAccess::typeinfer() {
  Type* b = base()->typeinfer();   // infer the type of base();  
  // if base is a classname, look for a static field of name()
  // if base is an object expression, look for a non-static field of name()
  ClassEntity *e;
  bool static_flag;
  bool local;

  if (b->kind() == CLASS_TYPE)  {
    e = ((ClassType*)b)->classtype();
    static_flag = true;
  }  else if (b->kind() == INSTANCE_TYPE)  {
    e = ((InstanceType*)b)->classtype();
    static_flag = false;
  }  else {
    error->type_error(lineno(), "Base of a field access is not a class or instance; found type ", b);
    return errortype;
  }

  local = (current_class == e);
  while (e != NULL) {
    list<Entity*>::iterator i;
    for(i=e->class_members()->begin(); i!= e->class_members()->end(); i++) {
      Entity* member = *i;
      if ((member->kind() == FIELD_ENTITY) &&
	  (strcmp(member->name(), this->name()) == 0) &&
	  (static_flag == ((FieldEntity*)member)->static_flag())) {
	FieldEntity *f = (FieldEntity *)member;
	if (local || f->visibility_flag()) {
	  this->set_entity(f);
	  this->set_type(f->type());
	  return f->type();
	} else {
	  // There is a matching field, but its visibility flags are wrong
	  error->type_error(lineno(), "There is no accessible field with name ", name());
	  return errortype; 
	}
      }
      // otherwise, go on with searching
    }
    // no such field in the current class, check super class
    e = e->superclass();
    local = false;
  }
  // no matching field found
  error->type_error(lineno(), "There is no accessible field with name ", name());
  return errortype; 
}

bool argsmatch(list<VariableEntity*> *formal_params, list<Type*> *argtypes)
{
  list<VariableEntity*>::iterator f;  // iterator over formal params
  list<Type*>::iterator a;    // iterator over actual arguments
  f = formal_params->begin();
  a = argtypes->begin();
  while (f != formal_params->end() && a != argtypes->end()) {
    if (! (*a)->isSubtypeOf((*f)->type()))
      // actual argument is NOT a subtype of the declared formal parameter
      return false;
    f++;
    a++;  // advance both the formal param and argument indices
  }
  if (f != formal_params->end() || a != argtypes->end()) 
    // one of the two lists ended before the other
    return false;
  else
    return true;
}
	

// Typeinfer method for MethodInvocation
Type* MethodInvocation::typeinfer() {
  Type* b = base()->typeinfer();   // infer the type of base();  
  // if base is a classname, look for a static field of name()
  // if base is an object expression, look for a non-static field of name()
  // when doing the search, keep track of private/public.
  // and the types of argument expressions to resolve overloading.
  list<Expression*>::iterator i;
  list<Type*> *argtypes = new list<Type*>();

  for(i=args()->begin(); i != args()->end();) {
    argtypes->push_back((*i)->typeinfer());
    i++;
  }

  ClassEntity *e;
  bool static_flag;
  bool local;
  Type* rettype;
  int matchcount = 0;

  if (b->kind() == ERROR_TYPE)
    return errortype;
  if (b->kind() == CLASS_TYPE)  {
    e = ((ClassType*)b)->classtype();
    static_flag = true;
  }  else if (b->kind() == INSTANCE_TYPE)  {
    e = ((InstanceType*)b)->classtype();
    static_flag = false;
  }  else {
    error->type_error(lineno(), "Base of a method access is not a class or instance;", b);
    return errortype;
  }

  local = (current_class == e);
  rettype = NULL;
  while (e != NULL) {
    list<Entity*>::iterator i;
    for(i=e->class_members()->begin(); i!= e->class_members()->end(); i++) {
      Entity* member = *i;
      if ((member->kind() == METHOD_ENTITY) &&
	  (strcmp(member->name(), this->name()) == 0) &&
	  (static_flag == ((MethodEntity*)member)->static_flag())) {
	MethodEntity *m = (MethodEntity *)member;
	// check if the arguments match
	if (! argsmatch(m->formal_params(), argtypes)) {
	  continue; // next member please
	}
	if (local || m->visibility_flag()) {
	  matchcount++;
	  this->set_entity(m);
	  if (rettype == NULL)
	    rettype = m->return_type();
	} else {
	  // There is a matching method, but its visibility flag is wrong
	  error->type_error(lineno(), "There is no accessible method with name ", name());
	  return errortype; 
	}
      }
      // otherwise, go on with searching
    }
    if (matchcount == 1) { // one match was found
      this->set_type(rettype);
      return rettype;
    } else if (matchcount == 0) { // match is not found yet
      // no such field in the current class, check super class
      e = e->superclass();
      local = false;
      continue;
    } else { // more than one matches found
      error->type_error(lineno(), "Multiple methods match call: ", name());
      return errortype; 
    }
  }
  // no matching field found
  error->type_error(lineno(), "There is no accessible method with name ", name());
  return errortype; 
}



// Typeinfer method for UnaryExpression
Type* UnaryExpression::typeinfer() {
  Type *t = arg()->typeinfer();
  switch(unary_operator()) {
  case NEG: 
    if (t->isSubtypeOf(booltype)) {
      this->set_type(booltype);
      return booltype;
    } else {
      error->type_error(lineno(), "Boolean type expected as argument of NEG", t);
      return errortype;
    }
  case UMINUS:
    if (t->isSubtypeOf(inttype) || t->isSubtypeOf(floattype)) {
      this->set_type(t);
      return t;
    } else {
      error->type_error(lineno(), "Integer/Floattype expected as argument of UMINUS", t);
      return errortype;
    }
  }
}



// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() {
  Type* t = arg()->typeinfer();
  if (t->isSubtypeOf(inttype)) {
    this->set_type(t);
    return t;
  } else {
    error->type_error(lineno(), "Integer argument expected for auto incr/decr operator", t);
    return errortype;
  }
}

// Typeinfer method for NewArrayInstance: 
Type* NewArrayInstance::typeinfer() {
  int n = dimension();
  int i;
  Type* rettype = type();
  for(i = 0; i< n; i++) {
    rettype = new ArrayType(rettype);
  }
  this->set_type(rettype);
  return rettype;
}



// Typeinfer method for NewInstance:
Type* NewInstance::typeinfer() {
  // look for a constructor of class_entity()
  //  with the given arguments of type:
  list<Type*> *argtypes = new list<Type*>();
  list<Expression*>::iterator i;
  for(i=args()->begin(); i != args()->end();) {
    argtypes->push_back((*i)->typeinfer());
    i++;
  }

  ClassEntity *e = class_entity();
  list<Entity*>::iterator ei;
  for(ei=e->class_members()->begin(); ei!= e->class_members()->end(); ei++) {
    Entity* member = *ei;
    if (member->kind() == CONSTRUCTOR_ENTITY) {
      ConstructorEntity *c = (ConstructorEntity *)member;
      // check if the arguments match
      if (! argsmatch(c->formal_params(), argtypes)) {
	continue; // next member please
      }
      if ((current_class == e) // i.e., local access
	  || c->visibility_flag()) {
	Type *rettype = new InstanceType(e);
	this->set_entity(c);
	this->set_type(rettype);
	return rettype;
      } else {
	// There is a matching constructor, but its visibility flag is wrong
	error->type_error(lineno(), "There is no accessible constructor ", e);
	return errortype; 
      }
    }
  }
  // There is no matching constructor
  error->type_error(lineno(), "There is no matching constructor ", e);
  return errortype; 
}


// Typeinfer method for ThisExpression
Type* ThisExpression::typeinfer() {
  Type *rettype;
  if (current_method_static_flag) {
    rettype = new ClassType(current_class);
    this->set_type(rettype);
    return rettype;
  } else {
    rettype = new InstanceType(current_class);
    this->set_type(rettype);
    return rettype;
  }
}


// Typeinfer method for SuperExpression
Type* SuperExpression::typeinfer() {
  ClassEntity* e = current_class;
  ClassEntity *s = e->superclass();
  if (s == NULL) {
    error->syntax_error(lineno(), "Current class does not have a superclass");
    return errortype;
  } else {
    Type *rettype;
    if (current_method_static_flag) {
      rettype = new ClassType(s);
      this->set_type(rettype);
      return rettype;
    } else {
      rettype = new InstanceType(s);
      this->set_type(rettype);
      return rettype;
    }
  }
}

// Typeinfer method for IdExpression
Type* IdExpression::typeinfer() {
  Entity* e = id();
  Type *rettype;

  if (e->kind() == VARIABLE_ENTITY) {
    rettype = ((VariableEntity *)e)->type();
    this->set_type(rettype);
    return rettype;
  } else if (e->kind() == CLASS_ENTITY) {
    rettype = new ClassType(((ClassEntity *)e));
    this->set_type(rettype);
    return rettype;
  } else {
    // Something else found where a variable/class name is expected
    error->type_error(lineno(), "Variable/Class name expected, found ", e);
    return errortype;
  }
}


// Typeinfer method for NullExpression
Type* NullExpression::typeinfer() {
  this->set_type(nulltype);
  return nulltype;
}


// Typeinfer method for IntegerConstant
Type* IntegerConstant::typeinfer() {
  this->set_type(inttype);
  return inttype;
}


// Typeinfer method for FloatConstant
Type* FloatConstant::typeinfer() {
  this->set_type(floattype);
  return floattype;
}


// Typeinfer method for BooleanConstant
Type* BooleanConstant::typeinfer() {
  this->set_type(booleantype);
  return booleantype;
}


// Typeinfer method for StringConstant
Type* StringConstant::typeinfer() {
  this->set_type(stringtype);
  return stringtype;
}


