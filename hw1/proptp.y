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

std::stack<ASTNode*> types;

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
    // Evaluate expression here.
    cerr<<"[1] Next line starts\n";
 } LINE
 | error { yyerrok; } LINE
 | LINE {
     // Evaluate expression here.
     cerr<<"[2] Next line starts\n";
   };


LINE:  S '.' {
    cout<<"AST: ";
    print_AST($1);
    while (!types.empty()) {
        types.pop();
    }
    cout<<endl;
 } ENDL
 | ENDL;


S:     T IMPLIES S {
    // cerr<<"Pushing: ->\n";
    ASTNode *nn = new ASTNode(IMPLIES);
    nn->left    = $1;
    nn->right   = $3;
    types.pop(); types.pop();
    types.push(nn);
    $$ = nn;
 }
| T {
    $$ = $1;
  };

T:     U '|' T {
    ASTNode *nn = new ASTNode('|');
    nn->left    = $1;
    nn->right   = $3;
    types.pop(); types.pop();
    types.push(nn);
    $$ = nn;
 }
| U {
    $$ = $1;
  };

U:     V '&' U {
    ASTNode *nn = new ASTNode('&');
    nn->left    = $1;
    nn->right   = $3;
    types.pop(); types.pop();
    types.push(nn);
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
    types.pop();
    types.push(nn);
    $$ = nn;
 }
| STRING {
    // cerr<<"Pushing: "<<$1<<endl;
    ASTNode *nn = new ASTNode(STRING);
    nn->id = $1;
    types.push(nn);
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


static struct ASTNode*
last_token(struct ASTNode *n) {
    if (!n) {
        return NULL;
    }
    struct ASTNode *nn = last_token(n->right);
    if (nn) {
        return nn;
    }
    nn = last_token(n->left);
    if (nn) {
        return nn;
    }
    return n;
}

std::string
token_to_string(struct ASTNode* n) {
    // cerr<<"token_to_string, "<<n<<"\n";
    if (n->type == STRING) {
        return n->id;
    }
    switch (n->type) {
    case ')':
        return ")";
    case '(':
        return "(";
    case IMPLIES:
        return "->";
    case '&':
        return "&";
    case '|':
        return "|";
    case '!':
        return "!";
    default:
        return "AIEE!!";
    }
}

int
main() {
    int ret = yyparse();
    return ret;
}

void yyerror(const char *s) {
    if (yytext && yytext[0] == '\n') {
        fprintf(stderr, "Error on line %d, expected '.'\n", lno);
    }
    else {
        if (types.empty()) {
            fprintf(stderr, "Error on line %d, unexpected token '%s' at the beginning of the line\n", lno, yytext);
        }
        else {
            std::string lt = token_to_string(last_token(types.top()));
            fprintf(stderr, "Error on line %d, unexpected token '%s' after '%s'\n", lno, yytext, lt.c_str());
        }
    }
    // exit(-1);
}
