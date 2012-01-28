%{
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int lno;
extern "C" char *yytext;

void yyerror(const char *s);

typedef struct ASTNode {
    int type; // yytokentype::...
    std::string id;
    int val;
    ASTNode *left, *right;

    ASTNode(int _type)
    : type(_type), left(NULL), right(NULL)
    { }
} ASTNode;

static void
print_AST(struct ASTNode *n);

%}

%union {
    char *id;
    struct ASTNode *astnode;
}

%token <id> STRING
%token ENDL IMPLIES

%type<astnode> S T U V

%%

LINES: LINES { cerr<<"Menghani\n"; } LINE | LINE { cerr<<"Men2\n"; };
LINE:  S '.' {
    print_AST($1); cout<<endl;
 } ENDL
 | ENDL;

S:     T IMPLIES S {
    ASTNode *nn = new ASTNode(IMPLIES);
    nn->left    = $1;
    nn->right   = $3;
    $$ = nn;
 }
| T {
    $$ = $1;
  };

T:     U '|' T {
    ASTNode *nn = new ASTNode('|');
    nn->left    = $1;
    nn->right   = $3;
    $$ = nn;
 }
| U {
    $$ = $1;
  };

U:     V '&' U {
    ASTNode *nn = new ASTNode('&');
    nn->left    = $1;
    nn->right   = $3;
    $$ = nn;
 }
| V {
    $$ = $1;
  };

V:     '(' S ')' {
    $$ = $2;
 }
| '!' V {
    ASTNode *nn = new ASTNode('!');
    nn->right   = $2;
    $$ = nn;
 }
| STRING {
    ASTNode *nn = new ASTNode(STRING);
    nn->id = $1;
    $$ = nn;
  };

%%

static void
print_AST(struct ASTNode *n) {
    if (!n) return;
    print_AST(n->left);
    switch (n->type) {
    case STRING:
        printf(" %s ", n->id.c_str());
        break;
    case IMPLIES:
        printf(" -> ");
        break;
    default:
        printf(" %c ", (char)n->type);
     }
    print_AST(n->right);
}

int
main() {
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    if (yytext && yytext[0] == '\n') {
        fprintf(stderr, "Error on line %d, expected '.'\n", lno);
    }
    else {
        fprintf(stderr, "Error on line %d, unexpected token '%s'\n", lno, yytext);
    }
    exit(-1);
}
