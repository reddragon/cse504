#include <iostream>
#include "Ast.hh"
#include "Error.hh"
#include <cassert>

extern Error *error;
extern ClassEntity *objectclass;
extern list<Entity *> *toplevel;  // list of top-level classes
extern EntityTable *global_symtab; // global symbol table

MethodEntity *current_method = NULL;
ConstructorEntity *current_constructor = NULL;
ClassEntity *current_class = NULL;

void initialize_typechecker() {
  // initialize any needed variables here...
    current_method = NULL;
    current_constructor = NULL;
    current_class = NULL;
}

bool isOfType(Type *t, TypeKind k) {
    return (t->kind() == k);
}

bool areSameTypes(Type *t1, Type *t2) {
    return t1->kind() == t2->kind();
}

bool isIntegerType(Type* t) {
    return isOfType(t, INT_TYPE);
}

bool isNumericType(Type* t) {
    return isOfType(t, INT_TYPE) || isOfType(t, FLOAT_TYPE);
}

bool isBooleanType(Type *t) {
    return isOfType(t, BOOLEAN_TYPE);
}

bool isStringType(Type *t) {
    return isOfType(t, STRING_TYPE);
}

bool isErrorType(Type *t) {
    return isOfType(t, ERROR_TYPE);
}

bool isValidMethodInvocation(MethodInvocation *pmi, MethodEntity *pme) {
    if (strcmp(pmi->name(), pme->name())) {
        return false;
    }
    if (pmi->args()->size() != pme->formal_params()->size()) {
        return false;
    }

    list<Expression*>::iterator j = pmi->args()->begin();
    for (list<Entity*>::iterator i = pme->formal_params()->begin(); 
         i != pme->formal_params()->end(); ++i) {
        VariableEntity *pparam = (VariableEntity*)(*i);
        if (!(*j)->typeinfer()->isSubtypeOf(pparam->type())) {
            return false;
        }
        ++j;
    }
    return true;
}

bool isValidNewInstance(NewInstance* pni, ConstructorEntity* pce) {
    if (pni->args()->size() != pce->formal_params()->size()) {
        return false;
    }

    list<Expression*>::iterator j = pni->args()->begin();
    for (list<Entity*>::iterator i = pce->formal_params()->begin(); 
         i != pce->formal_params()->end(); ++i) {
        VariableEntity *pparam = (VariableEntity*)(*i);
        if (!(*j)->typeinfer()->isSubtypeOf(pparam->type())) {
            return false;
        }
        ++j;
    }
    return true;

}

Entity*
lookup_entity(ClassEntity *pc, std::string name, int depth, int &found_at) {
    found_at = depth;
    if (pc == NULL) {
        return NULL;
    }

    for (list<Entity*>::iterator i = pc->class_members()->begin(); 
         i != pc->class_members()->end(); ++i) {
        if (name == (*i)->name()) {
            return *i;
        }
    }

    return lookup_entity(pc->superclass(), name, depth+1, found_at);
}

#define ERROR_GUARD(T) if (isErrorType(T)) { return T; }
#define ERROR_GUARD_NO_RETURN(T) if (isErrorType(T)) { return; }

enum MethodInvocationResult { METHODFOUND=0, EMNOTFOUND, EMMULTIPLEDECL, EMPRIVATEACCESS };

MethodInvocationResult
lookup_method_entity(ClassEntity* pc, MethodInvocation* pmi, MethodEntity** m) {
    // Method not found
    if (pc == NULL) {
        *m = NULL;
        return EMNOTFOUND;
    }

    int count = 0;
    for (list<Entity*>::iterator i = pc->class_members()->begin();
        i != pc->class_members()->end(); ++i) {
            if ((*i)->kind() == METHOD_ENTITY) {
                if (isValidMethodInvocation(pmi, (MethodEntity *)*i)) {
                    
                    if (!((MethodEntity *)(*i))->visibility_flag() && 
                        (current_class != pc)) {
                        // Is a private function, and, the method we
                        // are calling it from is not a method of the
                        // class in which it exists.
                        *m = NULL;
                        return EMPRIVATEACCESS;
                    }

                    count = count + 1;
                    *m = (MethodEntity*) (*i);
                    
                    // Multiple matching Method Invocations
                    if (count > 1) {
                        *m = NULL;
                        return EMMULTIPLEDECL;
                    }
                }
            }
        }
    
    // One matching function found
    if (count == 1) {
        return METHODFOUND;
    }
    
    if (!count) {
        return lookup_method_entity(pc->superclass(), pmi, m);
    }
}

enum NewInstanceResult { CFOUND=0, ECNOTFOUND, ECMULTIPLEDECL, ECPRIVATEACCESS };

NewInstanceResult
lookup_constructor_entity(ClassEntity* pc, NewInstance* pni, ConstructorEntity** c) {
    // Method not found
    if (pc == NULL) {
        *c = NULL;
        return ECNOTFOUND;
    }

    int count = 0;
    for (list<Entity*>::iterator i = pc->class_members()->begin();
        i != pc->class_members()->end(); ++i) {
            if ((*i)->kind() == CONSTRUCTOR_ENTITY) {
                if (isValidNewInstance(pni, (ConstructorEntity *)*i)) {
                    
                    // Is a private function, and,
                    // the method we are calling it from is not
                    // a method of the class in which it exists.
                    if (!((ConstructorEntity *)(*i))->visibility_flag() && (current_class != pc)) {
                        *c = NULL;
                        return ECPRIVATEACCESS;
                    }

                    count = count + 1;
                    *c = (ConstructorEntity*) (*i);
                    
                    // Multiple matching Method Invocations
                    if (count > 1) {
                        *c = NULL;
                        return ECMULTIPLEDECL;
                    }
                }
            }
        }
    
    // One matching function found
    if (count == 1) {
        return CFOUND;
    }
    
    *c = NULL;
    return ECNOTFOUND;
}


Entity*
get_class_entity(const char *name) {
    for (list<Entity*>::iterator i = toplevel->begin(); 
         i != toplevel->end(); ++i) {
        if (!strcmp(name, (*i)->name())) {
            return *i;
        }
    }
    return NULL;
}

void ClassEntity::typecheck() {
    current_class = this;
    list<Entity *>::iterator it = this->class_members()->begin();
    for (; it != this->class_members()->end(); it++) {
        switch ((*it)->kind()) {
            case METHOD_ENTITY:
                ((MethodEntity*)(*it))->typecheck();
                break;
            case CONSTRUCTOR_ENTITY:
                ((ConstructorEntity*)(*it))->typecheck();
                break;
        }
    }
    current_class = NULL;
}

void MethodEntity::typecheck() {
    current_method = this;
    this->method_body()->typecheck();
    current_method = NULL;
}

void ConstructorEntity::typecheck() {
    current_constructor = this;
    this->constructor_body()->typecheck();
    current_constructor = NULL;
}

// Typecheck method for IfStatement
void IfStatement::typecheck() {
    Type* expr = this->expr()->typeinfer();
    ERROR_GUARD_NO_RETURN(expr);

    if (!isBooleanType(expr)) {
        error->type_error(this->lineno(), "Expected boolean", expr);
        return;
    }

    this->thenpart()->typecheck();
    this->elsepart()->typecheck();
}

// Typecheck method for WhileStatement
void WhileStatement::typecheck() {
    Type* expr = this->expr()->typeinfer();
    ERROR_GUARD_NO_RETURN(expr);

    if (!isBooleanType(expr)) {
        error->type_error(this->lineno(), "Expected boolean", expr);
        return;
    }

    this->body()->typecheck();
}



// Typecheck method for ForStatement
void ForStatement::typecheck() {
    Type* guard = this->guard()->typeinfer();
    ERROR_GUARD_NO_RETURN(guard);

    if (!isBooleanType(guard)) {
        error->type_error(this->lineno(), "Expected boolean", guard);
        return;
    }

    this->init()->typecheck();
    this->update()->typecheck();
    this->body()->typecheck();
}

// Typecheck method for ReturnStatement
void ReturnStatement::typecheck() {
    if (!current_method) {
        error->syntax_error(this->lineno(), "Return statement not in a function");
        return;
    }

    Type *expr = this->expr()->typeinfer();
    ERROR_GUARD_NO_RETURN(expr);

    if (!expr->isSubtypeOf(current_method->return_type())) {
        error->type_error(this->lineno(), "Return type incompatible with the declared type", expr, current_method->return_type());
        return;
    }
}


// Typecheck method for BlockStatement
void BlockStatement::typecheck() {
    list<Statement *>::iterator it = this->stmt_list()->begin();
    for(; it != this->stmt_list()->end(); it++) {
        (*it)->typecheck();
    }
}

// Typecheck method for ExprStatement
void ExprStatement::typecheck() {
    this->expr()->typeinfer();
}

// Typecheck method for DeclStatement
void DeclStatement::typecheck() {
    // Nothing to be done here
}



// Typecheck method for BreakStatement
void BreakStatement::typecheck() {
    // Nothing to be done here
}



// Typecheck method for ContinueStatement
void ContinueStatement::typecheck() {
    // Nothing to be done here
}



// Typecheck method for SkipStatement
void SkipStatement::typecheck() {
    //  Nothing to be done here
}



// Typeinfer method for BinaryExpression
Type* BinaryExpression::typeinfer() {
    Type* lhs_type = this->lhs()->typeinfer();
    Type* rhs_type = this->rhs()->typeinfer();
    ERROR_GUARD(lhs_type);
    ERROR_GUARD(rhs_type);

    switch (this->binary_operator()) {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            // cout<<"lhs type: ";
            // lhs_type->print();
            // cout<<endl<<"rhs type: ";
            // rhs_type->print();
            // cout<<std::endl;

            if (!isNumericType(lhs_type)) {
                error->type_error(this->lineno(), "Expected int or float", lhs_type);
                return new ErrorType;
            }
            if (!isNumericType(rhs_type)) {
                error->type_error(this->lineno(), "Expected int or float", rhs_type);
                return new ErrorType;
            }

            if (isOfType(lhs_type, FLOAT_TYPE) || isOfType(rhs_type, FLOAT_TYPE)) {
                return new FloatType;
            }
            return new IntType;

        case LT:
        case LEQ:
        case GT:
        case GEQ:
            if (!isNumericType(lhs_type)) {
                error->type_error(this->lineno(), "Expected int or float", lhs_type);
                return new ErrorType;
            }
            if (!isNumericType(rhs_type)) {
                error->type_error(this->lineno(), "Expected int or float", rhs_type);
                return new ErrorType;
            }
            return new BooleanType();
        
        case EQ:
        case NEQ:
            // cout<<"lhs type: ";
            // lhs_type->print();
            // cout<<endl<<"rhs type: ";
            // rhs_type->print();
            // cout<<std::endl;
            if (!lhs_type->isSubtypeOf(rhs_type) && !rhs_type->isSubtypeOf(lhs_type)) {
                error->type_error(this->lineno(), "Binary (in)equality", lhs_type, rhs_type);
                return new ErrorType;
            }
            return new BooleanType;

        case AND:
        case OR:
            if (!isOfType(lhs_type, BOOLEAN_TYPE)) {
                error->type_error(this->lineno(), "Expected boolean", lhs_type);
                return new ErrorType;
            }

            if (!isOfType(rhs_type, BOOLEAN_TYPE)) {
                error->type_error(this->lineno(), "Expected boolean", rhs_type);
                return new ErrorType;
            }

            return new BooleanType;
    }
    return new ErrorType;
}



// Typeinfer method for AssignExpression
Type* AssignExpression::typeinfer() {
    Type* lhs_type = this->lhs()->typeinfer();
    Type* rhs_type = this->rhs()->typeinfer();
    ERROR_GUARD(lhs_type);
    ERROR_GUARD(rhs_type);

    if (!rhs_type->isSubtypeOf(lhs_type)) {
        error->type_error(this->lineno(), "Two incompatible types are used in assignment", rhs_type, lhs_type);
        return new ErrorType;
    }
    return rhs_type;
}



// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() {
    Type* base_type = this->base()->typeinfer();
    Type* idx_type = this->idx()->typeinfer();
    ERROR_GUARD(base_type);
    ERROR_GUARD(idx_type);

    if (!isOfType(base_type, ARRAY_TYPE)) {
        error->type_error(this->lineno(), "Expected array", base_type);
        return new ErrorType;
    }

    if (!isOfType(idx_type, INT_TYPE)) {
        error->type_error(this->lineno(), "Expected int", idx_type);
        return new ErrorType;
    }

    ArrayType* array = (ArrayType *)this->base()->typeinfer();
    return array->elementtype();
}


// Typeinfer method for FieldAccess:
Type* FieldAccess::typeinfer() {
    // Fetch the InstanceType based on this->base()
    // cout<<"field access: base.name: ";
    // this->base()->print();
    // cout<<'.'<<this->name()<<endl;

    Type *pt = this->base()->typeinfer();
    ERROR_GUARD(pt);

    if (!(pt->kind() == INSTANCE_TYPE || pt->kind() == CLASS_TYPE)) {
        // Error "Invalid base type. Excepted INSTANCE or CLASS type"
        error->type_error(this->lineno(), "Expceted instance or class type while infering type", pt);
        return new ErrorType;
    }

    ClassEntity *pce = pt->kind() == INSTANCE_TYPE ? ((InstanceType*)pt)->classtype() : ((ClassType*)pt)->classtype();
    int found_at = 0;
    FieldEntity *e = (FieldEntity*)lookup_entity(pce, this->name(), 0, found_at);

    if (!e) {
        // Field 'e' not found
        error->syntax_error(this->lineno(), "Field named '" + string(this->name()) + "' was not found");
        return new ErrorType;
    }

    bool is_public = e->visibility_flag();
    bool is_static = e->static_flag();
    if (!is_static) {
        // non-static field

        // Check if base is an InstanceType
        if (pt->kind() != INSTANCE_TYPE) {
            return new ErrorType();
        }

    } else {
        // static field
    }

    if (!is_public) {
        // Private. Check if the current method is that of the same
        // class) as the one where the field was declared.
        // TODO Remove this assert after testing
        // current_class should always be set
        assert(current_class);

        // cerr<<"Checking for private field '"<<this->name()<<"'\n";
        // cerr<<"found_at: "<<found_at<<endl;

        if (found_at > 0 || strcmp(pce->name(), current_class->name())) {
            error->syntax_error(this->lineno(), "Error: Trying to access private member '" + string(this->name()) + "' of class '" + pce->name() + "'");
            return new ErrorType();
        }
    }

    // Return the type of the field named this->name()
    FieldEntity *pfe = (FieldEntity*)e;
    return pfe->type();
}

// Typeinfer method for MethodInvocation
Type* MethodInvocation::typeinfer() {
    Type *pt = this->base()->typeinfer();
    ERROR_GUARD(pt);
    
    /*
    cout<<"method invocation::base.name: ";
    pt->print();
    cout<<'.'<<this->name();
    cout<<endl;
    */

    if (!(pt->kind() == INSTANCE_TYPE || pt->kind() == CLASS_TYPE)) {
        error->type_error(this->lineno(), "Expected an instance or a class type", pt);
        return new ErrorType();
    }
    
    ClassEntity *pce = pt->kind() == INSTANCE_TYPE ? ((InstanceType*)pt)->classtype() : ((ClassType*)pt)->classtype();
    MethodEntity *m;

    switch (lookup_method_entity(pce, this, &m)) {
        case METHODFOUND:
            if (pt->kind() == INSTANCE_TYPE && m->static_flag()) {
                m = NULL;
                error->type_error(this->lineno(), "Invalid access of static member function ", (char *)this->name());
                break;
            }
            return m->return_type();
        
        case EMNOTFOUND:
            error->type_error(this->lineno(), "Could not find a matching method for ", (char *)this->name());
            break;

        case EMMULTIPLEDECL:
            error->type_error(this->lineno(), "Multiple declarations found for method ", (char *)this->name());
            break;
        
        case EMPRIVATEACCESS:
            error->type_error(this->lineno(), "Trying to access private member function ", (char *)this->name());
            break;
    }

    return(new ErrorType());
}



// Typeinfer method for UnaryExpression
Type* UnaryExpression::typeinfer() {
    Type* type = this->arg()->typeinfer();
    ERROR_GUARD(type);

    switch (this->unary_operator()) {
        case NEG:
            if (!isOfType(type, BOOLEAN_TYPE)) {
                error->type_error(this->lineno(), "Expected boolean", type);
                return(new ErrorType());
            }
            return new BooleanType();
        case UMINUS:
            if (!(isNumericType(type))) {
                error->type_error(this->lineno(), "Expected int or float", type);
                return(new ErrorType());
            }
            return type;
    }
    return(new ErrorType());
}



// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() {
    Type* type = this->arg()->typeinfer();
    ERROR_GUARD(type);
    
    if (!isOfType(type, INT_TYPE)) {
        error->type_error(this->lineno(), "Expected int", type);
        return(new ErrorType());
    }
    
    return new IntType();
}

// Typeinfer method for NewArrayInstance: 
Type* NewArrayInstance::typeinfer() {
    for (list<Expression*>::iterator j = this->bounds()->begin(); 
         j != this->bounds()->end(); ++j) {
        if (!isIntegerType((*j)->typeinfer())) {
            return new ErrorType();
        }
    }

    Type *base = new ArrayType(this->type());
    int i = this->dimension() - 1;

    while (i--) {
        base = new ArrayType(base);
    }

    return base;
}


// Typeinfer method for NewInstance:
Type* NewInstance::typeinfer() {
    ConstructorEntity *c;
    switch (lookup_constructor_entity(this->class_entity(), this, &c)) {
        case CFOUND:
            return new InstanceType(this->class_entity());

        case ECNOTFOUND:
            error->type_error(this->lineno(), "Could not find a matching constructor for the constructor of class ", (char *)this->class_entity()->name());
            break;
        
        case ECMULTIPLEDECL:
            error->type_error(this->lineno(), "Multiple declarations of constructors of the same type in the class ", (char *)this->class_entity()->name());
            break;
        
        case ECPRIVATEACCESS:
            error->type_error(this->lineno(), "Invalid invocation of a private constructor of the class ", (char *)this->class_entity()->name());
            break;
    }
    return  (new ErrorType());
}


// Typeinfer method for ThisExpression
Type* ThisExpression::typeinfer() {
    if (current_constructor || !current_method->static_flag()) {
        return new InstanceType(current_class);
    }
    else {
        return new ClassType(current_class);
    }
}


// Typeinfer method for SuperExpression
Type* SuperExpression::typeinfer() {
    if (current_constructor || current_method->static_flag()) {
        return new InstanceType(current_class->superclass());
    }
    else
        return new ClassType(current_class->superclass());
}

// Typeinfer method for IdExpression
Type* IdExpression::typeinfer() {
    switch (this->id()->kind()) {
        case CLASS_ENTITY:
            return new ClassType((ClassEntity *)this->id());
        case VARIABLE_ENTITY:
            return ((VariableEntity *)(this->id()))->type();
    }
}


// Typeinfer method for NullExpression
Type* NullExpression::typeinfer() {
    return new NullType();
}


// Typeinfer method for BooleanConstant
Type* BooleanConstant::typeinfer() {
    return new BooleanType();
}

// Typeinfer method for IntegerConstant
Type* IntegerConstant::typeinfer() {
    return new IntType();
}



// Typeinfer method for FloatConstant
Type* FloatConstant::typeinfer() {
    return new FloatType();
}


// Typeinfer method for StringConstant
Type* StringConstant::typeinfer() {
    return new StringType();
}


