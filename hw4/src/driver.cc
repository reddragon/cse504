#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Symbols.hh"
#include "AstSymbols.hh"
#include "Ast.hh"
#include "Error.hh"

extern FILE *yyin;
extern int yyparse();

extern int yylineno;
extern char* yytext;

int numerrors = 0;
list<Entity *> *toplevel;
EntityTable *global_symtab;
ClassEntity* objectclass;
Error *error;

char* sourcefile;

extern void initialize_typechecker();

void yyerror(const char *errtype)
{
  fprintf(stderr, "%s:%d: %s near token `%s'\n", sourcefile, 
                yylineno, errtype, yytext);
  numerrors++;
}


void initialize() {
  global_symtab = new EntityTable();

  objectclass = new ClassEntity(" Object ", NULL, new list<Entity*>());
  error = new Error(sourcefile);

}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(0);
  }

  if (strcmp(argv[1], "-") == 0) {
    // take input from stdin
    yyin = stdin;
    sourcefile = (char *)malloc(strlen("stdin")+1);
    strcpy(sourcefile, "stdin");    
  } else if ((yyin = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error in opening file %s\n", argv[1]);
    exit(0);
  } else {
    sourcefile = (char *)malloc(strlen(argv[1])+1);
    strcpy(sourcefile, argv[1]);
  }

  initialize();
  printf("Parsing...\n");

  if (yyparse() || (numerrors > 0)) {
    printf("%s contains one or more syntax errors.\n", argv[1]);
  }
  else {
    initialize_typechecker();
    printf("Typechecking...\n");
    list<Entity*>::iterator i;
    if (toplevel != NULL)
      for(i=toplevel->begin(); i != toplevel->end(); i++) {
	// top-level entities will all be classes.
	ClassEntity *e = ((ClassEntity*)(*i));
	e->typecheck();
    }
    printf("Done\n");
  }
}

