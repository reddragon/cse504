%{
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iterator>

#include <assert.h>

using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int lno;
extern "C" char *yytext;

void yyerror(const char *s);



typedef struct ASTNode {
    int type; // yytokentype enum
    std::string id;
    ASTNode *left, *right;

    ASTNode(int _type, ASTNode *l = NULL, ASTNode *r = NULL)
    : type(_type), left(l), right(r)
    { }
} ASTNode;

typedef std::map<std::string, bool> symtab_t;
ASTNode* root = NULL;
std::stack<ASTNode*> types;
symtab_t symtab, exp_symtab;

// Number of valid expressions entered.
int nexpr = 0;


static void
print_AST(struct ASTNode *n);

static void
check_validity();

static bool
evaluate(ASTNode *n);


// If the function 'f' returns 'true', then recursion will stop.
template <typename Func>
bool
_all_combinations(int n, Func const &f, std::vector<bool> &bit_string) {
    // Generates all 2^n bit-strings and passes them to 'f'
    if (!n) {
        return f(bit_string);
    }

    bit_string.push_back(0);
    int ret = _all_combinations(n-1, f, bit_string);
    if (ret) {
        return ret;
    }
    bit_string.back() = 1;
    ret = _all_combinations(n-1, f, bit_string);
    bit_string.pop_back();
    return ret;
}


// If the function 'f' returns 'true', then recursion will stop.
//
// The return value is either 'true' if the function ever returns
// true, or 'flase' if it never returns true.
template <typename Func>
bool
all_combinations(int n, Func const &f) {
    std::vector<bool> bit_string;
    return _all_combinations(n, f, bit_string);
}


void clear_types() {
    // Clear the 'types' stack.
    while (!types.empty()) {
        types.pop();
    }
}

%}

%union {
    char *id;
    struct ASTNode *astnode;
}

%token <id> STRING
%token ENDL IMPLIES UNKNOWN

%type<astnode> S T U V

%%

LINES: LINES LINE
| LINE;

LINE:  S '.' {
    // cout<<"AST: "; print_AST($1); cout<<endl;

    ++nexpr;

    // Add the AST to the list of expressions.
    if (!root) {
        root = $1;
    }
    else {
        // We chain the new expression with the
		// old tree of expressions that we already have,
		// using a conjuction operator. Thus, if there are
		// multiple expressions, f1, f2, .. fn, that have
		// been added to the tree uptil now, and the new 
		// expression is g, then, the evaluation
		// of the tree would give us the value of
		// g & f1 & f2 & .. & fn, which would tell us if
		// the new expression is consistent or not.

		root = new ASTNode('&', $1, root);
    }
    clear_types();

    // Add the symbols found in the current expression to the
    // global symbol table.
    symtab.insert(exp_symtab.begin(), exp_symtab.end());		

    // Evaluate expression here.
    check_validity();

 } ENDL
 | error ENDL { yyerrok; }
 | ENDL;


S:     T IMPLIES S {
    // cerr<<"Pushing: ->\n";
    ASTNode *nn = new ASTNode(IMPLIES, $1, $3);
    types.pop(); types.pop();
    types.push(nn);
    $$ = nn;
 }
| T {
    $$ = $1;
  };

T:     U '|' T {
    ASTNode *nn = new ASTNode('|', $1, $3);
    types.pop(); types.pop();
    types.push(nn);
    $$ = nn;
 }
| U {
    $$ = $1;
  };

U:     V '&' U {
    ASTNode *nn = new ASTNode('&', $1, $3);
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
    ASTNode *nn = new ASTNode('!', NULL, $2);
    types.pop();
    types.push(nn);
    $$ = nn;
 }
| STRING {
    // cerr<<"Pushing: "<<$1<<endl;
    ASTNode *nn = new ASTNode(STRING);
    nn->id = $1;
   	
    // Insert into the expression symbol table
    exp_symtab[$1] = 0;
    
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

bool
print_bit_string(std::vector<bool> &bit_string) {
    for (int i = 0; i < bit_string.size(); ++i) {
        cout<<bit_string[i]<<", ";
    }
    cout<<endl;
    return false;
}

int
main() {
    // all_combinations(10, print_bit_string);

    int ret = yyparse();
    return ret;
}

struct Evaluator {
    ASTNode *root;
    mutable symtab_t variables;

    Evaluator(ASTNode *_root, symtab_t _variables)
    : root(_root), variables(_variables)
    { }

    bool
    operator()(std::vector<bool> const& bit_string) const {
        int ctr = 0;
        for (symtab_t::iterator i = this->variables.begin(); i != this->variables.end(); ++i) {
            i->second = bit_string[ctr++];
        }

        return this->_eval(this->root);
    }

    bool
    _eval(ASTNode *n) const {
        assert(n);

        switch (n->type) {
        case '&':
            return _eval(n->left) && _eval(n->right);
        case '|':
            return _eval(n->left) || _eval(n->right);
        case IMPLIES:
            return !_eval(n->left) || _eval(n->right);
        case '!':
            return !_eval(n->right);
        case STRING:
            return this->variables[n->id];
        default:
            cerr<<"Invalid type in Evaluator::_eval\n";
            exit(-1);
        }
    }

};


static bool
evaluate(ASTNode *n) {
    Evaluator eval(root, symtab);

    bool val = all_combinations(symtab.size(), eval);
    printf("%s\n", val ? "Ok" : "No");
}



static void
check_validity() {
    // Check if the new expression is consistent with all
	// expressions entered till now and print a
    // message accordingly.

    evaluate(root);
}


void yyerror(const char *s) {
    // Clear the expression symbol table, so that it is reusable
    exp_symtab.clear();

    // Why do errors go to stdout?
    // 
    // Check: http://www.cs.sunysb.edu/~cram/cse504/Spring12/Homeworks/hw1.html
    // 
    // "All output from the program (including error messages!) should
    // go to standard output."
    // 
    if (yytext && yytext[0] == '\n') {
        fprintf(stdout, "Error on line %d, expected '.'\n", lno);
    }
    else {
        if (types.empty()) {
            fprintf(stdout, "Error on line %d, unexpected token '%s' at the beginning of the line\n", lno, yytext);
        }
        else {
            std::string lt = token_to_string(last_token(types.top()));
            fprintf(stdout, "Error on line %d, unexpected token '%s' after '%s'\n", lno, yytext, lt.c_str());
        }
    }
    // exit(-1);
}
