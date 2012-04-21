#include "ParserParam.h"
#include "Parser.h"
#include "Lexer.h"
 
#include <stdio.h>
 
int yyparse(void *param);
 
static int initParserParam(SParserParam* param)
{
        int ret = 0;
 
        ret = yylex_init(&param->scanner);
        param->expression = NULL;
 
        return ret;
}
 
static int destroyParserParam(SParserParam* param)
{
        return yylex_destroy(param->scanner);
}
 
SExpression *getAST(const char *expr)
{
        SParserParam p;
        YY_BUFFER_STATE state;
 
        if ( initParserParam(&p) )
        {
                // couldn't initialize
                return NULL;
        }
 
        state = yy_scan_string(expr, p.scanner);
 
        if ( yyparse(&p) )
        {
                // error parsing
                return NULL;
        }
 
        yy_delete_buffer(state, p.scanner);
 
        destroyParserParam(&p);
 
        return p.expression;
}
 
int evaluate(SExpression *e)
{
        switch(e->type)
        {
                case eVALUE:
                        return e->value;
                case eMULTIPLY:
                        return evaluate(e->left) * evaluate(e->right);
                case ePLUS:
                        return evaluate(e->left) + evaluate(e->right);
                default:
                        // shouldn't be here
                        return 0;
        }
}
 
int main(void)
{
        SExpression *e = NULL;
        char test[]=" 4 + 2*10 + 3*( 5 + 1 )";
        int result = 0;
 
        e = getAST(test);
 
        result = evaluate(e);
 
        printf("Result of '%s' is %d\n", test, result);
 
        deleteExpression(e);
 
        return 0;
}
