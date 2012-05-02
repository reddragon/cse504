#include<iostream>
#include "Symbols.hh"

int entity_counter;

// First, the constructor:
Entity::Entity(const char* name, Kind kind) {
  name_ = name;
  kind_ = kind;
  same_name_ = NULL;
  same_scope_ = NULL;
  level_number_ = 0;
  entity_number_ = ++entity_counter;
}

// Next, the destructor
Entity::~Entity() {
  // do nothing!
}


const char* Entity::name() const{
  return name_;
}

Kind Entity::kind() const {
  return kind_;
}

Entity* Entity::same_name() const{
  return same_name_;
}

Entity* Entity::same_scope() const{
  return same_scope_;
}

int Entity::level_number() const{
  return level_number_;
}

void Entity::same_name(Entity* other) {
  same_name_ = other;
}

void Entity::same_scope(Entity* other) {
  same_scope_ = other;
}

void Entity::level_number(int l) {
  level_number_ = l;
}

void Entity::print() {
  printf("%s_%d", name_, entity_number_);
}

char* Entity::distinct_name() {
  int ndigits = 0;
  int i = entity_number_;
  for(ndigits = 1; i != 0; i =i/10, ndigits++);
  char *retval = (char *)malloc(strlen(name_) + ndigits + 1);  
  sprintf(retval, "%s_%d", name_, entity_number_);
  return retval;
}


// Entity Table:
// First, the constructor:
EntityTable::EntityTable() {
  entity_counter = 0;
  level_ = 0;
}

// Next, the destructor
EntityTable::~EntityTable() {
  // do nothing!
}

void EntityTable::print_name_table_entries() {
  hash_map<const char *, Entity*, hash<const char *>, eqstr> ::iterator i;
  for(i = name_table_.begin(); i != name_table_.end(); i++) {
    printf("Key: %s\n", (*i).first);
  }
}

Entity* EntityTable::find_entity(const char *name, Kind kind, bool *current) {
  Entity* e = name_table_[name];
  //  print_name_table_entries();
  if (name_table_.count(name) > 0) {
    // name exists in name table; now search through same name stack
    Entity* entity = name_table_[name];
    while (entity != NULL) {
      if (entity->kind() == kind) {
	*current = (entity->level_number() == level_);
	return entity;
      } else
	entity = entity->same_name();
    }
  }
  // search was futile
  return NULL;
}

void EntityTable::add_entity(Entity* entity) {
  if (name_table_.count(entity->name()) > 0)
    // name is in name table; so add this too
    entity->same_name(name_table_[entity->name()]);
  else 
    // name not in name table
    entity->same_name(NULL);
  
  name_table_[entity->name()] = entity;
  
  entity->same_scope(scope_stack_.front());
  scope_stack_.front() = entity;  // make this the beginning of same_scope list
  
  entity->level_number(level_);
}

void EntityTable::enter_block() {
  scope_stack_.push_front(NULL);
  level_++;
}

void EntityTable::leave_block() {
  bool d;
  if (scope_stack_.empty())
    return; //fail silently

  Entity* entity =  scope_stack_.front();
  // remove name->entity link for the current scope entries
  while (entity != NULL) {
    Entity* next = entity->same_scope();
    name_table_[entity->name()] = entity->same_name();
    entity = next;
  }

  scope_stack_.pop_front();
  level_--;

}

list<Entity*> EntityTable::defined_in_current_scope() {
  list<Entity*> *ret = new list<Entity*>();
  if (! scope_stack_.empty()) {
    // there are entries in scope stack
    Entity* entity = scope_stack_.front();
    while (entity != NULL) {
      ret->push_front(entity);
      entity = entity->same_scope();
    }
  }
  return *ret;
}

void EntityTable::print_symbols_in_scope() {
  list<Entity*>::iterator st;
  int i = 0;
  for(st = scope_stack_.begin(); st != scope_stack_.end(); st++) {
    cout << "Level " << i++ << endl;
    Entity* entity = (*st);
    while (entity != NULL) {
      cout << entity->distinct_name() << " " << entity->kind() << endl;
      entity = entity->same_scope();
    }
    cout << "---------" << endl;
  }
}

