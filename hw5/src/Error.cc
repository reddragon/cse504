#include <iostream>
#include "Error.hh"

Error::Error(string s) {
  sourcefile = s;
  numerrors = 0;
}

void Error::prelude(int lineno) {
  cerr << sourcefile << ":" << lineno << ": ";
  numerrors++;
}

void Error::implementation_error(string s) {
  cerr << s << endl;
  numerrors++;
}

void Error::implementation_error(int lineno, string s) {
  prelude(lineno);
  cerr << s << endl;
  numerrors++;
}

void Error::syntax_error(int lineno, string s) {
  prelude(lineno);
  cerr << s << endl;
  numerrors++;
}

void Error::type_error(int lineno, string s, Type* t) {
  prelude(lineno);
  cerr << s << "; found: ";
  t->print();
  cerr << endl;
  numerrors++;
}

void Error::type_error(int lineno, string s, Type* t1, Type* t2) {
  prelude(lineno);
  cerr <<  s << "; found ";
  t1->print();
  cerr << ", expected ";
  t2->print();
  cerr << endl;
  numerrors++;
}

void Error::type_error(int lineno, string s1, char *s2) {
  prelude(lineno);
  cerr << s1 << s2 << endl;
  numerrors++;
}

void Error::type_error(int lineno, string s, Entity* e) {
  prelude(lineno);
  cerr << s << e->distinct_name() << endl;
  numerrors++;
}

void Error::syntax_error(int lineno, string s1, string s2) {
  prelude(lineno);
  cerr << s1 << " near token "<< s2 << endl;
  numerrors++;
}

bool Error::hasErrors() {
  return numerrors > 0;
}
