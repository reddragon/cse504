#include<iostream>
#include "Ast.hh"
#include "Error.hh"

extern int yylineno;
extern Error *error;

list<ClassEntity*>* Program::classes() {
    return classes_;
}

int Statement::lineno() {
    return lineno_;
}

int Expression::lineno() {
    return lineno_;
}

Type* Expression::type() {
     return type_;
}

void Expression::set_type(Type *type) {
     type_ = type;
}

void Expression::rcode() {
     error->implementation_error(lineno(), "Rcode not defined for expression");
}

void LhsExpression::lcode() {
     error->implementation_error(lineno(), "Lcode not defined for expression");
}

		



// Constructor for IfStatement
IfStatement::IfStatement(Expression* expr , Statement* thenpart , Statement* elsepart){
   expr_ = expr;   thenpart_ = thenpart;   elsepart_ = elsepart; 
   lineno_ = yylineno;        
}

// Destructor for IfStatement
IfStatement::~IfStatement() { // Do nothing!
}

// get methods (if any) for IfStatement
Expression* IfStatement::expr() {
   return expr_;
}

Statement* IfStatement::thenpart() {
   return thenpart_;
}

Statement* IfStatement::elsepart() {
   return elsepart_;
}

// Set methods (if any) for IfStatement






// Constructor for WhileStatement
WhileStatement::WhileStatement(Expression* expr , Statement* body){
   expr_ = expr;   body_ = body; 
   lineno_ = yylineno;        
}

// Destructor for WhileStatement
WhileStatement::~WhileStatement() { // Do nothing!
}

// get methods (if any) for WhileStatement
Expression* WhileStatement::expr() {
   return expr_;
}

Statement* WhileStatement::body() {
   return body_;
}

// Set methods (if any) for WhileStatement





// Constructor for ForStatement
ForStatement::ForStatement(Statement* init , Expression* guard , Statement* update , Statement* body){
   init_ = init;   guard_ = guard;   update_ = update;   body_ = body; 
   lineno_ = yylineno;        
}

// Destructor for ForStatement
ForStatement::~ForStatement() { // Do nothing!
}

// get methods (if any) for ForStatement
Statement* ForStatement::init() {
   return init_;
}

Expression* ForStatement::guard() {
   return guard_;
}

Statement* ForStatement::update() {
   return update_;
}

Statement* ForStatement::body() {
   return body_;
}

// Set methods (if any) for ForStatement







// Constructor for ReturnStatement
ReturnStatement::ReturnStatement(Expression* expr){
   expr_ = expr; 
   lineno_ = yylineno;        
}

// Destructor for ReturnStatement
ReturnStatement::~ReturnStatement() { // Do nothing!
}

// get methods (if any) for ReturnStatement
Expression* ReturnStatement::expr() {
   return expr_;
}

// Set methods (if any) for ReturnStatement




// Constructor for BlockStatement
BlockStatement::BlockStatement(list<Statement*>* stmt_list){
   stmt_list_ = stmt_list; 
   lineno_ = yylineno;        
}

// Destructor for BlockStatement
BlockStatement::~BlockStatement() { // Do nothing!
}

// get methods (if any) for BlockStatement
list<Statement*>* BlockStatement::stmt_list() {
   return stmt_list_;
}

// Set methods (if any) for BlockStatement




// Constructor for DeclStatement
DeclStatement::DeclStatement(list<VariableEntity*>* var_list){
   var_list_ = var_list; 
   lineno_ = yylineno;        
}

// Destructor for DeclStatement
DeclStatement::~DeclStatement() { // Do nothing!
}

// get methods (if any) for DeclStatement
list<VariableEntity*>* DeclStatement::var_list() {
   return var_list_;
}

// Set methods (if any) for DeclStatement




// Constructor for ExprStatement
ExprStatement::ExprStatement(Expression* expr){
   expr_ = expr; 
   lineno_ = yylineno;        
}

// Destructor for ExprStatement
ExprStatement::~ExprStatement() { // Do nothing!
}

// get methods (if any) for ExprStatement
Expression* ExprStatement::expr() {
   return expr_;
}

// Set methods (if any) for ExprStatement




// Constructor for BreakStatement
BreakStatement::BreakStatement(){
    
   lineno_ = yylineno;        
}

// Destructor for BreakStatement
BreakStatement::~BreakStatement() { // Do nothing!
}

// get methods (if any) for BreakStatement

// Set methods (if any) for BreakStatement




// Constructor for ContinueStatement
ContinueStatement::ContinueStatement(){
    
   lineno_ = yylineno;        
}

// Destructor for ContinueStatement
ContinueStatement::~ContinueStatement() { // Do nothing!
}

// get methods (if any) for ContinueStatement

// Set methods (if any) for ContinueStatement




// Constructor for SkipStatement
SkipStatement::SkipStatement(){
    
   lineno_ = yylineno;        
}

// Destructor for SkipStatement
SkipStatement::~SkipStatement() { // Do nothing!
}

// get methods (if any) for SkipStatement

// Set methods (if any) for SkipStatement




// Constructor for NativeStatement
NativeStatement::NativeStatement(char* function_name){
   function_name_ = function_name; 
   lineno_ = yylineno;        
}

// Destructor for NativeStatement
NativeStatement::~NativeStatement() { // Do nothing!
}

// get methods (if any) for NativeStatement
char* NativeStatement::function_name() {
   return function_name_;
}

// Set methods (if any) for NativeStatement








// Constructor for ArrayAccess
ArrayAccess::ArrayAccess(Expression* base , Expression* idx){
   base_ = base;   idx_ = idx; 
       lineno_ = yylineno;    
}

// Destructor for ArrayAccess
ArrayAccess::~ArrayAccess() { // Do nothing!
}

// get methods (if any) for ArrayAccess
Expression* ArrayAccess::base() {
   return base_;
}

Expression* ArrayAccess::idx() {
   return idx_;
}

// Set methods (if any) for ArrayAccess





// Constructor for FieldAccess
FieldAccess::FieldAccess(Expression* base , char* name ){
   base_ = base;   name_ = name;    
       lineno_ = yylineno;    
}

// Destructor for FieldAccess
FieldAccess::~FieldAccess() { // Do nothing!
}

// get methods (if any) for FieldAccess
Expression* FieldAccess::base() {
   return base_;
}

char* FieldAccess::name() {
   return name_;
}

FieldEntity* FieldAccess::entity() {
   return entity_;
}

// Set methods (if any) for FieldAccess


void FieldAccess::set_entity(FieldEntity* entity) {
   entity_ = entity;}




// Constructor for IdExpression
IdExpression::IdExpression(Entity* id){
   id_ = id; 
       lineno_ = yylineno;    
}

// Destructor for IdExpression
IdExpression::~IdExpression() { // Do nothing!
}

// get methods (if any) for IdExpression
Entity* IdExpression::id() {
   return id_;
}

// Set methods (if any) for IdExpression






// Constructor for BinaryExpression
BinaryExpression::BinaryExpression(BinaryOperator binary_operator , Expression* lhs , Expression* rhs){
   binary_operator_ = binary_operator;   lhs_ = lhs;   rhs_ = rhs; 
       lineno_ = yylineno;    
}

// Destructor for BinaryExpression
BinaryExpression::~BinaryExpression() { // Do nothing!
}

// get methods (if any) for BinaryExpression
BinaryOperator BinaryExpression::binary_operator() {
   return binary_operator_;
}

Expression* BinaryExpression::lhs() {
   return lhs_;
}

Expression* BinaryExpression::rhs() {
   return rhs_;
}

// Set methods (if any) for BinaryExpression






// Constructor for AssignExpression
AssignExpression::AssignExpression(LhsExpression* lhs , Expression* rhs){
   lhs_ = lhs;   rhs_ = rhs; 
       lineno_ = yylineno;    
}

// Destructor for AssignExpression
AssignExpression::~AssignExpression() { // Do nothing!
}

// get methods (if any) for AssignExpression
LhsExpression* AssignExpression::lhs() {
   return lhs_;
}

Expression* AssignExpression::rhs() {
   return rhs_;
}

// Set methods (if any) for AssignExpression





// Constructor for MethodInvocation
MethodInvocation::MethodInvocation(Expression* base , char* name , list<Expression*>* args ){
   base_ = base;   name_ = name;   args_ = args;    
       lineno_ = yylineno;    
}

// Destructor for MethodInvocation
MethodInvocation::~MethodInvocation() { // Do nothing!
}

// get methods (if any) for MethodInvocation
Expression* MethodInvocation::base() {
   return base_;
}

char* MethodInvocation::name() {
   return name_;
}

list<Expression*>* MethodInvocation::args() {
   return args_;
}

MethodEntity* MethodInvocation::entity() {
   return entity_;
}

// Set methods (if any) for MethodInvocation



void MethodInvocation::set_entity(MethodEntity* entity) {
   entity_ = entity;}




// Constructor for UnaryExpression
UnaryExpression::UnaryExpression(UnaryOperator unary_operator , Expression* arg){
   unary_operator_ = unary_operator;   arg_ = arg; 
       lineno_ = yylineno;    
}

// Destructor for UnaryExpression
UnaryExpression::~UnaryExpression() { // Do nothing!
}

// get methods (if any) for UnaryExpression
UnaryOperator UnaryExpression::unary_operator() {
   return unary_operator_;
}

Expression* UnaryExpression::arg() {
   return arg_;
}

// Set methods (if any) for UnaryExpression





// Constructor for AutoExpression
AutoExpression::AutoExpression(AutoOperator auto_operator , LhsExpression* arg){
   auto_operator_ = auto_operator;   arg_ = arg; 
       lineno_ = yylineno;    
}

// Destructor for AutoExpression
AutoExpression::~AutoExpression() { // Do nothing!
}

// get methods (if any) for AutoExpression
AutoOperator AutoExpression::auto_operator() {
   return auto_operator_;
}

LhsExpression* AutoExpression::arg() {
   return arg_;
}

// Set methods (if any) for AutoExpression





// Constructor for NewArrayInstance
NewArrayInstance::NewArrayInstance(Type* type , int dimension , list<Expression*>* bounds){
   type_ = type;   dimension_ = dimension;   bounds_ = bounds; 
       lineno_ = yylineno;    
}

// Destructor for NewArrayInstance
NewArrayInstance::~NewArrayInstance() { // Do nothing!
}

// get methods (if any) for NewArrayInstance
Type* NewArrayInstance::type() {
   return type_;
}

int NewArrayInstance::dimension() {
   return dimension_;
}

list<Expression*>* NewArrayInstance::bounds() {
   return bounds_;
}

// Set methods (if any) for NewArrayInstance






// Constructor for NewInstance
NewInstance::NewInstance(ClassEntity* class_entity , list<Expression*>* args ){
   class_entity_ = class_entity;   args_ = args;    
       lineno_ = yylineno;    
}

// Destructor for NewInstance
NewInstance::~NewInstance() { // Do nothing!
}

// get methods (if any) for NewInstance
ClassEntity* NewInstance::class_entity() {
   return class_entity_;
}

list<Expression*>* NewInstance::args() {
   return args_;
}

ConstructorEntity* NewInstance::entity() {
   return entity_;
}

// Set methods (if any) for NewInstance


void NewInstance::set_entity(ConstructorEntity* entity) {
   entity_ = entity;}




// Constructor for ThisExpression
ThisExpression::ThisExpression(){
    
       lineno_ = yylineno;    
}

// Destructor for ThisExpression
ThisExpression::~ThisExpression() { // Do nothing!
}

// get methods (if any) for ThisExpression

// Set methods (if any) for ThisExpression




// Constructor for SuperExpression
SuperExpression::SuperExpression(){
    
       lineno_ = yylineno;    
}

// Destructor for SuperExpression
SuperExpression::~SuperExpression() { // Do nothing!
}

// get methods (if any) for SuperExpression

// Set methods (if any) for SuperExpression




// Constructor for NullExpression
NullExpression::NullExpression(){
    
       lineno_ = yylineno;    
}

// Destructor for NullExpression
NullExpression::~NullExpression() { // Do nothing!
}

// get methods (if any) for NullExpression

// Set methods (if any) for NullExpression




// Constructor for IntegerConstant
IntegerConstant::IntegerConstant(int value){
   value_ = value; 
       lineno_ = yylineno;    
}

// Destructor for IntegerConstant
IntegerConstant::~IntegerConstant() { // Do nothing!
}

// get methods (if any) for IntegerConstant
int IntegerConstant::value() {
   return value_;
}

// Set methods (if any) for IntegerConstant




// Constructor for FloatConstant
FloatConstant::FloatConstant(float value){
   value_ = value; 
       lineno_ = yylineno;    
}

// Destructor for FloatConstant
FloatConstant::~FloatConstant() { // Do nothing!
}

// get methods (if any) for FloatConstant
float FloatConstant::value() {
   return value_;
}

// Set methods (if any) for FloatConstant




// Constructor for BooleanConstant
BooleanConstant::BooleanConstant(bool value){
   value_ = value; 
       lineno_ = yylineno;    
}

// Destructor for BooleanConstant
BooleanConstant::~BooleanConstant() { // Do nothing!
}

// get methods (if any) for BooleanConstant
bool BooleanConstant::value() {
   return value_;
}

// Set methods (if any) for BooleanConstant




// Constructor for StringConstant
StringConstant::StringConstant(int value){
   value_ = value; 
       lineno_ = yylineno;    
}

// Destructor for StringConstant
StringConstant::~StringConstant() { // Do nothing!
}

// get methods (if any) for StringConstant
int StringConstant::value() {
   return value_;
}

// Set methods (if any) for StringConstant






// Constructor for IntType
IntType::IntType(){
    
           kind_ = INT_TYPE;
}

// Destructor for IntType
IntType::~IntType() { // Do nothing!
}

// get methods (if any) for IntType

// Set methods (if any) for IntType




// Constructor for FloatType
FloatType::FloatType(){
    
           kind_ = FLOAT_TYPE;
}

// Destructor for FloatType
FloatType::~FloatType() { // Do nothing!
}

// get methods (if any) for FloatType

// Set methods (if any) for FloatType




// Constructor for BooleanType
BooleanType::BooleanType(){
    
           kind_ = BOOLEAN_TYPE;
}

// Destructor for BooleanType
BooleanType::~BooleanType() { // Do nothing!
}

// get methods (if any) for BooleanType

// Set methods (if any) for BooleanType




// Constructor for StringType
StringType::StringType(){
    
           kind_ = STRING_TYPE;
}

// Destructor for StringType
StringType::~StringType() { // Do nothing!
}

// get methods (if any) for StringType

// Set methods (if any) for StringType




// Constructor for VoidType
VoidType::VoidType(){
    
           kind_ = VOID_TYPE;
}

// Destructor for VoidType
VoidType::~VoidType() { // Do nothing!
}

// get methods (if any) for VoidType

// Set methods (if any) for VoidType




// Constructor for ClassType
ClassType::ClassType(ClassEntity* classtype){
   classtype_ = classtype; 
           kind_ = CLASS_TYPE;
}

// Destructor for ClassType
ClassType::~ClassType() { // Do nothing!
}

// get methods (if any) for ClassType
ClassEntity* ClassType::classtype() {
   return classtype_;
}

// Set methods (if any) for ClassType




// Constructor for InstanceType
InstanceType::InstanceType(ClassEntity* classtype){
   classtype_ = classtype; 
           kind_ = INSTANCE_TYPE;
}

// Destructor for InstanceType
InstanceType::~InstanceType() { // Do nothing!
}

// get methods (if any) for InstanceType
ClassEntity* InstanceType::classtype() {
   return classtype_;
}

// Set methods (if any) for InstanceType




// Constructor for ErrorType
ErrorType::ErrorType(){
    
           kind_ = ERROR_TYPE;
}

// Destructor for ErrorType
ErrorType::~ErrorType() { // Do nothing!
}

// get methods (if any) for ErrorType

// Set methods (if any) for ErrorType




// Constructor for ArrayType
ArrayType::ArrayType(Type* elementtype){
   elementtype_ = elementtype; 
           kind_ = ARRAY_TYPE;
}

// Destructor for ArrayType
ArrayType::~ArrayType() { // Do nothing!
}

// get methods (if any) for ArrayType
Type* ArrayType::elementtype() {
   return elementtype_;
}

// Set methods (if any) for ArrayType



// UniverseType is the top-most in type hierarchy; every type is in UniverseType

// Constructor for UniverseType
UniverseType::UniverseType(){
    
           kind_ = UNIVERSE_TYPE;
}

// Destructor for UniverseType
UniverseType::~UniverseType() { // Do nothing!
}

// get methods (if any) for UniverseType

// Set methods (if any) for UniverseType



// NullType is a bottom in type hierarchy; it is in every class type 

// Constructor for NullType
NullType::NullType(){
    
           kind_ = NULL_TYPE;
}

// Destructor for NullType
NullType::~NullType() { // Do nothing!
}

// get methods (if any) for NullType

// Set methods (if any) for NullType



