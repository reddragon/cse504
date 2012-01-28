%{
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stack>

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int lno;
extern "C" char *yytext;

void yyerror(const char *s);

std::stack<std::string> tokens;

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
%token ENDL IMPLIES UNKNOWN

%type<astnode> S T U V

%%

LINES: LINES {
    cerr<<"[1] Next line starts\n";
 } LINE
 | LINE {
     cerr<<"[2] Next line starts\n";
   };

LINE:  S '.' {
    print_AST($1);
    while (!tokens.empty()) {
        tokens.pop();
    }
    cout<<endl;
 } ENDL
 | ENDL;

S:     T IMPLIES S {
    tokens.push("->");
    ASTNode *nn = new ASTNode(IMPLIES);
    nn->left    = $1;
    nn->right   = $3;
    $$ = nn;
 }
| T {
    $$ = $1;
  };

T:     U '|' T {
    tokens.push("|");
    ASTNode *nn = new ASTNode('|');
    nn->left    = $1;
    nn->right   = $3;
    $$ = nn;
 }
| U {
    $$ = $1;
  };

U:     V '&' U {
    tokens.push("&");
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
    tokens.push("!");
    ASTNode *nn = new ASTNode('!');
    nn->right   = $2;
    $$ = nn;
 }
| STRING {
    tokens.push($1);
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
        if (tokens.empty()) {
            fprintf(stderr, "Error on line %d, unexpected token '%s' at the beginning of the line\n", lno, yytext);
        }
        else {
            fprintf(stderr, "Error on line %d, unexpected token '%s' after '%s'\n", lno, yytext, tokens.top().c_str());
        }
    }
    exit(-1);
}
