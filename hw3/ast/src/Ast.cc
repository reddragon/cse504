#include<iostream>
#include "Ast.hh"

		

// Constructor for IfStatement
IfStatement::IfStatement(Expression* expr, Statement* thenpart, Statement* elsepart){
   expr_ = expr;
   thenpart_ = thenpart;
   elsepart_ = elsepart;
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
WhileStatement::WhileStatement(Expression* expr, Statement* body){
   expr_ = expr;
   body_ = body;
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
ForStatement::ForStatement(Statement* init, Expression* guard, Statement* update, Statement* body){
   init_ = init;
   guard_ = guard;
   update_ = update;
   body_ = body;
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
DeclStatement::DeclStatement(list<Entity*>* var_list){
   var_list_ = var_list;
}

// Destructor for DeclStatement
DeclStatement::~DeclStatement() { // Do nothing!
}

// get methods (if any) for DeclStatement
list<Entity*>* DeclStatement::var_list() {
   return var_list_;
}

// Set methods (if any) for DeclStatement




// Constructor for ExprStatement
ExprStatement::ExprStatement(Expression* expr){
   expr_ = expr;
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
   
}

// Destructor for BreakStatement
BreakStatement::~BreakStatement() { // Do nothing!
}

// get methods (if any) for BreakStatement

// Set methods (if any) for BreakStatement




// Constructor for ContinueStatement
ContinueStatement::ContinueStatement(){
   
}

// Destructor for ContinueStatement
ContinueStatement::~ContinueStatement() { // Do nothing!
}

// get methods (if any) for ContinueStatement

// Set methods (if any) for ContinueStatement




// Constructor for SkipStatement
SkipStatement::SkipStatement(){
   
}

// Destructor for SkipStatement
SkipStatement::~SkipStatement() { // Do nothing!
}

// get methods (if any) for SkipStatement

// Set methods (if any) for SkipStatement




// Constructor for BinaryExpression
BinaryExpression::BinaryExpression(BinaryOperator binary_operator, Expression* lhs, Expression* rhs){
   binary_operator_ = binary_operator;
   lhs_ = lhs;
   rhs_ = rhs;
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
AssignExpression::AssignExpression(Expression* lhs, Expression* rhs){
   lhs_ = lhs;
   rhs_ = rhs;
}

// Destructor for AssignExpression
AssignExpression::~AssignExpression() { // Do nothing!
}

// get methods (if any) for AssignExpression
Expression* AssignExpression::lhs() {
   return lhs_;
}

Expression* AssignExpression::rhs() {
   return rhs_;
}

// Set methods (if any) for AssignExpression





// Constructor for ArrayAccess
ArrayAccess::ArrayAccess(Expression* base, Expression* idx){
   base_ = base;
   idx_ = idx;
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
FieldAccess::FieldAccess(Expression* base, char* name){
   base_ = base;
   name_ = name;
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

// Set methods (if any) for FieldAccess





// Constructor for MethodInvocation
MethodInvocation::MethodInvocation(Expression* base, char* name, list<Expression*>* args){
   base_ = base;
   name_ = name;
   args_ = args;
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

// Set methods (if any) for MethodInvocation






// Constructor for UnaryExpression
UnaryExpression::UnaryExpression(UnaryOperator unary_operator, Expression* arg){
   unary_operator_ = unary_operator;
   arg_ = arg;
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
AutoExpression::AutoExpression(AutoOperator auto_operator, Expression* arg){
   auto_operator_ = auto_operator;
   arg_ = arg;
}

// Destructor for AutoExpression
AutoExpression::~AutoExpression() { // Do nothing!
}

// get methods (if any) for AutoExpression
AutoOperator AutoExpression::auto_operator() {
   return auto_operator_;
}

Expression* AutoExpression::arg() {
   return arg_;
}

// Set methods (if any) for AutoExpression





// Constructor for NewArrayInstance
NewArrayInstance::NewArrayInstance(Type* type, int dimension, list<Expression*>* bounds){
   type_ = type;
   dimension_ = dimension;
   bounds_ = bounds;
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
NewInstance::NewInstance(Entity* class_entity, list<Expression*>* args){
   class_entity_ = class_entity;
   args_ = args;
}

// Destructor for NewInstance
NewInstance::~NewInstance() { // Do nothing!
}

// get methods (if any) for NewInstance
Entity* NewInstance::class_entity() {
   return class_entity_;
}

list<Expression*>* NewInstance::args() {
   return args_;
}

// Set methods (if any) for NewInstance





// Constructor for ThisExpression
ThisExpression::ThisExpression(){
   
}

// Destructor for ThisExpression
ThisExpression::~ThisExpression() { // Do nothing!
}

// get methods (if any) for ThisExpression

// Set methods (if any) for ThisExpression




// Constructor for SuperExpression
SuperExpression::SuperExpression(){
   
}

// Destructor for SuperExpression
SuperExpression::~SuperExpression() { // Do nothing!
}

// get methods (if any) for SuperExpression

// Set methods (if any) for SuperExpression




// Constructor for IdExpression
IdExpression::IdExpression(Entity* id){
   id_ = id;
}

// Destructor for IdExpression
IdExpression::~IdExpression() { // Do nothing!
}

// get methods (if any) for IdExpression
Entity* IdExpression::id() {
   return id_;
}

// Set methods (if any) for IdExpression




// Constructor for NullExpression
NullExpression::NullExpression(){
   
}

// Destructor for NullExpression
NullExpression::~NullExpression() { // Do nothing!
}

// get methods (if any) for NullExpression

// Set methods (if any) for NullExpression




// Constructor for IntegerConstant
IntegerConstant::IntegerConstant(int value){
   value_ = value;
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
StringConstant::StringConstant(char* value){
   value_ = value;
}

// Destructor for StringConstant
StringConstant::~StringConstant() { // Do nothing!
}

// get methods (if any) for StringConstant
char* StringConstant::value() {
   return value_;
}

// Set methods (if any) for StringConstant




// Constructor for IntType
IntType::IntType(){
   
}

// Destructor for IntType
IntType::~IntType() { // Do nothing!
}

// get methods (if any) for IntType

// Set methods (if any) for IntType




// Constructor for FloatType
FloatType::FloatType(){
   
}

// Destructor for FloatType
FloatType::~FloatType() { // Do nothing!
}

// get methods (if any) for FloatType

// Set methods (if any) for FloatType




// Constructor for BooleanType
BooleanType::BooleanType(){
   
}

// Destructor for BooleanType
BooleanType::~BooleanType() { // Do nothing!
}

// get methods (if any) for BooleanType

// Set methods (if any) for BooleanType




// Constructor for StringType
StringType::StringType(){
   
}

// Destructor for StringType
StringType::~StringType() { // Do nothing!
}

// get methods (if any) for StringType

// Set methods (if any) for StringType




// Constructor for VoidType
VoidType::VoidType(){
   
}

// Destructor for VoidType
VoidType::~VoidType() { // Do nothing!
}

// get methods (if any) for VoidType

// Set methods (if any) for VoidType




// Constructor for ClassType
ClassType::ClassType(Entity* classtype){
   classtype_ = classtype;
}

// Destructor for ClassType
ClassType::~ClassType() { // Do nothing!
}

// get methods (if any) for ClassType
Entity* ClassType::classtype() {
   return classtype_;
}

// Set methods (if any) for ClassType




// Constructor for ErrorType
ErrorType::ErrorType(){
   
}

// Destructor for ErrorType
ErrorType::~ErrorType() { // Do nothing!
}

// get methods (if any) for ErrorType

// Set methods (if any) for ErrorType



