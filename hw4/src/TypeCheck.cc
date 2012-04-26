#include <iostream>
#include "Ast.hh"
#include "Error.hh"
#include <cassert>

extern Error *error;
extern ClassEntity *objectclass;

void initialize_typechecker() {
  // initialize any needed variables here...
}

bool isOfType(Type *t, TypeKind k) {
    return (t->kind() == k);
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

void ClassEntity::typecheck() {
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
}

void MethodEntity::typecheck() {
    return this->method_body()->typecheck();
}

void ConstructorEntity::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}

// Typecheck method for IfStatement
void IfStatement::typecheck() {
    Type* expr = this->expr()->typeinfer();

    if (!isBooleanType(expr) && !isErrorType(expr)) {
        error->type_error(this->lineno(), "Expected BOOLEAN", expr);
    }

    this->thenpart()->typecheck();
    this->elsepart()->typecheck();
}

// Typecheck method for WhileStatement
void WhileStatement::typecheck() {
    Type* expr = this->expr()->typeinfer();

    if (!isBooleanType(expr) && !isErrorType(expr)) {
        error->type_error(this->lineno(), "Expected BOOLEAN", expr);
    }

    this->body()->typecheck();
}



// Typecheck method for ForStatement
void ForStatement::typecheck() {
    Type* guard = this->guard()->typeinfer();

    if (!isBooleanType(guard) && !isErrorType(guard)) {
        error->type_error(this->lineno(), "Expected BOOLEAN", guard);
    }

    this->init()->typecheck();
    this->update()->typecheck();
    this->body()->typecheck();
}

// Typecheck method for ReturnStatement
void ReturnStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}


// Typecheck method for BlockStatement
void BlockStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}

// Typecheck method for ExprStatement
void ExprStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typecheck method for DeclStatement
void DeclStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typecheck method for BreakStatement
void BreakStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typecheck method for ContinueStatement
void ContinueStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typecheck method for SkipStatement
void SkipStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typeinfer method for BinaryExpression
Type* BinaryExpression::typeinfer() {
    Type* lhs_type = this->lhs()->typeinfer();
    Type* rhs_type = this->rhs()->typeinfer();
    switch (this->binary_operator()) {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            // TODO: Remove assertion and replace with an actual error.
            assert(isNumericType(lhs_type) && isNumericType(rhs_type));
            if (isOfType(lhs_type, FLOAT_TYPE) || isOfType(rhs_type, FLOAT_TYPE))
                return new FloatType();
            return new IntType();

        case LT:
        case LEQ:
        case GT:
        case GEQ:
            assert(isNumericType(lhs_type) && isNumericType(rhs_type));
            return new BooleanType();
        
        case EQ:
        case NEQ:
            assert(lhs_type->isSubtypeOf(rhs_type));
            return new BooleanType();

        case AND:
        case OR:
            assert(isOfType(lhs_type, BOOLEAN_TYPE) && isOfType(rhs_type, BOOLEAN_TYPE));
            return new BooleanType();
    }
    return(new ErrorType());
}



// Typeinfer method for AssignExpression
Type* AssignExpression::typeinfer() {
    Type* lhs_type = this->lhs()->typeinfer();
    Type* rhs_type = this->rhs()->typeinfer();
    // TODO: Remove assertion and replace with an actual error.
    assert(lhs_type->isSubtypeOf(rhs_type));
    return rhs_type;
}



// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() {
    Type* base_type = this->base()->typeinfer();
    Type* idx_type = this->idx()->typeinfer();
    // TODO: Remove assertion and replace with an actual error.
    assert(isOfType(base_type, ARRAY_TYPE));
    assert(isOfType(idx_type, INT_TYPE));
    ArrayType* array = (ArrayType *)this->base()->typeinfer();
    return array->elementtype();
}


// Typeinfer method for FieldAccess:
Type* FieldAccess::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}

// Typeinfer method for MethodInvocation
Type* MethodInvocation::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}



// Typeinfer method for UnaryExpression
Type* UnaryExpression::typeinfer() {
    Type* type = this->arg()->typeinfer();
    switch (this->unary_operator()) {
        case NEG:
            // TODO: Remove assertion and replace with an actual error.
            assert(isOfType(type, BOOLEAN_TYPE));
            return new BooleanType();
        case UMINUS:
            // TODO: Remove assertion and replace with an actual error.
            assert(isNumericType(type));
            return type;
    }
    return(new ErrorType());
}



// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() {
    Type* type = this->arg()->typeinfer();
    // TODO: Remove assertion and replace with an actual error.
    assert(isOfType(type, INT_TYPE));
    return new IntType();
}

// Typeinfer method for NewArrayInstance: 
Type* NewArrayInstance::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}



// Typeinfer method for NewInstance:
Type* NewInstance::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


// Typeinfer method for ThisExpression
Type* ThisExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


// Typeinfer method for SuperExpression
Type* SuperExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}

// Typeinfer method for IdExpression
Type* IdExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


// Typeinfer method for NullExpression
Type* NullExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


// Typeinfer method for BooleanConstant
Type* BooleanConstant::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}

// Typeinfer method for IntegerConstant
Type* IntegerConstant::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}



// Typeinfer method for FloatConstant
Type* FloatConstant::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


// Typeinfer method for StringConstant
Type* StringConstant::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}


