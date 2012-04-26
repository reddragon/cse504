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
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}

void ConstructorEntity::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}

// Typecheck method for IfStatement
void IfStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}

// Typecheck method for WhileStatement
void WhileStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
}



// Typecheck method for ForStatement
void ForStatement::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
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
            // TODO
            break;

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
    if (!lhs_type->isSubtypeOf(rhs_type))
        return(new ErrorType());
    // TODO - Fix this
    // Return a type of the same kind as rhs_type
    return rhs_type;
}



// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() {
    Type* base_type = this->base()->typeinfer();
    Type* idx_type = this->idx()->typeinfer();
    assert(isOfType(base_type, ARRAY_TYPE));
    assert(isOfType(idx_type, INT_TYPE));
    // TODO - Fix this
    // Return a type of the same kind as element_type
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
            assert(isOfType(type, BOOLEAN_TYPE));
            return new BooleanType();
        case UMINUS:
            assert(isNumericType(type));
            // TODO Fix this
            // Return a type of the same kind as type
            return type;
    }
    return(new ErrorType());
}



// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() {
   Type* type = this->arg()->typeinfer();
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


