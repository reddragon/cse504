#include <iostream>
#include "Ast.hh"
#include "Error.hh"
#include <cassert>

extern Error *error;
extern ClassEntity *objectclass;

void initialize_typechecker() {
  // initialize any needed variables here...
}


void ClassEntity::typecheck() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
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
    TypeKind lhs_type = this->lhs()->typeinfer()->kind();
    TypeKind rhs_type = this->rhs()->typeinfer()->kind();
    switch (this->binary_operator()) {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            assert(lhs_type == INT_TYPE || lhs_type == FLOAT_TYPE);
            assert(rhs_type == INT_TYPE || rhs_type == FLOAT_TYPE);
            if (lhs_type == FLOAT_TYPE || rhs_type == FLOAT_TYPE)
                return new FloatType();
            return new IntType();

        case LT:
        case LEQ:
        case GT:
        case GEQ:
            assert(lhs_type == INT_TYPE || lhs_type == FLOAT_TYPE);
            assert(rhs_type == INT_TYPE || rhs_type == FLOAT_TYPE);
            return new BooleanType();
        
        case EQ:
        case NEQ:
            // TODO
            break;

        case AND:
        case OR:
            assert(lhs_type == BOOLEAN_TYPE && rhs_type == BOOLEAN_TYPE);
            return new BooleanType();
    }
    return(new ErrorType());
}



// Typeinfer method for AssignExpression
Type* AssignExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}



// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
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
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
}



// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() {
   error->implementation_error("Type checking/inference not implemented (yet)\n");
   return(new ErrorType());
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


