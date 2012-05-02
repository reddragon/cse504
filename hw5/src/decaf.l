
%{
#include <iostream>
#include "Ast.hh"
#include "decaf.tab.hh"
// Prototypes to lexer functions
extern void yyerror (const char *error);
extern char *import_table[];
extern int import_index;
extern char *sourcefile;
FILE* infile;

int current_include_level = 0;
#define MAX_INCLUDE_LEVEL 10
int saved_lineno[MAX_INCLUDE_LEVEL];
char *saved_filename[MAX_INCLUDE_LEVEL];
%}

%option noyywrap
%option yylineno

%x IMPORT

letter	[A-Za-z]
digit   [0-9]
exponent ((E|e)("+"|"-")?({digit}+))
%%

"/*"(([^*]|(("*"+)[^*/]))*)("*"+)"/"		{
			;	/* skip C-style comments */
		}

"//"((.)*)\n    {
                        ;       /* skip single line (C++ style) comments */
                }

^"import"       {BEGIN(IMPORT);}
<IMPORT>({letter}|{digit}|"_"|"."|"/")+    {
        char* creamlib = getenv("CREAMLIB");
        if (creamlib == NULL)
           creamlib = strdup("");
        char *filename = (char *)malloc(strlen(creamlib) + strlen(yytext) + 2);
        strcpy(filename, yytext);
        infile = fopen(filename, "r");
        if (!infile) {
          // search for file in lib directory when specified
          if (strcmp(creamlib, "") != 0) {
              strcpy(filename, creamlib);
              strcat(filename, "/");
              strcat(filename, yytext);
              infile = fopen(filename, "r");
          } 
        } 
        if (infile)  {
	   if (current_include_level >= MAX_INCLUDE_LEVEL) {
	       yyerror("Include nesting level exceeds bounds");
	       yyterminate();
	   }
           saved_lineno[current_include_level] = yylineno;
           saved_filename[current_include_level] = sourcefile;
	   current_include_level++;
    	   import_table[import_index++] = strdup(filename);
           sourcefile = filename;
           yylineno = 1;
        } else 
           yyerror("Import file not found");
   }
<<EOF>>         {
		  yypop_buffer_state();
		  if (!YY_CURRENT_BUFFER || current_include_level < 0)
		    {
			yyterminate();
		    }
		  current_include_level--;
		  sourcefile = saved_filename[current_include_level];
		  yylineno = saved_lineno[current_include_level] + 1;
		  }

<IMPORT>.       {/* ignore other characters on import directive */}
<IMPORT>"\n"    {BEGIN(0);
        if (infile) {
	  yyin = infile;
	  yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));
        }
}
              

"boolean"	{
			return(TOK_BOOLEAN);
		}

"break"	{
			return(TOK_BREAK);
		}

"class"		{
			return(TOK_CLASS);
		}

"continue"		{
			return(TOK_CONTINUE);
		}

"do"		{
			return(TOK_DO);
		}

"else"		{
			return(TOK_ELSE);
		}

"extends"	{
			return(TOK_EXTENDS);
		}

"false"		{
			return(TOK_FALSE);
		}

"float"         {
                        return(TOK_FLOAT);
                }

"for"		{
			return(TOK_FOR);
		}

"if"		{
			return(TOK_IF);
		}

"int"		{
			return(TOK_INT);
		}

"native"	{
			return(TOK_NATIVE);
		}

"new"		{
			return(TOK_NEW);
		}

"null"		{
			return(TOK_NULL);
		}

"private"	{
			return(TOK_PRIVATE);
		}

"public"	{
			return(TOK_PUBLIC);
		}

"return"	{
			return(TOK_RETURN);
		}

"static"	{
			return(TOK_STATIC);
		}

"string"	{
			return(TOK_STRING);
		}

"super"		{
			return(TOK_SUPER);
		}

"this"		{
			return(TOK_THIS);
		}

"true"		{
			return(TOK_TRUE);
		}

"void"		{
			return(TOK_VOID);
		}

"while"		{
			return(TOK_WHILE);
		}

","		{
			return(TOK_COMMA);
		}

"."		{
			return(TOK_DOT);
		}

";"		{
			return(TOK_SEMICOLON);
		}

"["		{
			return(TOK_OPEN_SQ_BRACKET);
		}

"]"		{
			return(TOK_CLOSE_SQ_BRACKET);
		}

"("		{
			return(TOK_OPEN_PAREN);
		}

")"		{
			return(TOK_CLOSE_PAREN);
		}

"{"		{
			return(TOK_OPEN_BRACE);
		}

"}"		{
			return(TOK_CLOSE_BRACE);
		}

"+"		{
			return(TOK_PLUS);
		}

"-"		{
			return(TOK_MINUS);
		}

"*"		{
			return(TOK_MULTIPLY);
		}

"/"		{
			return(TOK_DIVIDE);
		}

"="		{
			return(TOK_EQUAL);
		}

"&&"		{
			return(TOK_AND);
		}

"||"		{
			return(TOK_OR);
		}

"!"		{
			return(TOK_NOT);
		}

"<"		{
			return(TOK_LESSER);
		}

">"		{
			return(TOK_GREATER);
		}

"=="		{
			return(TOK_EQUAL_EQUAL);
		}

"!="		{
			return(TOK_NOT_EQUAL);
		}

"<="		{
			return(TOK_LESSER_OR_EQUAL);
		}

">="		{
			return(TOK_GREATER_OR_EQUAL);
		}

"++"		{
			return(TOK_PLUS_PLUS);
		}

"--"		{
			return(TOK_MINUS_MINUS);
		}

{digit}+	{
                        yylval.int_val = atoi(yytext);
			return(TOK_INT_CONST);
		}

{digit}+"."{digit}+{exponent}?	|
{digit}+{exponent}	{
                        sscanf(yytext, "%g", &(yylval.float_val));
                        return(TOK_FLOAT_CONST);
		}

\"((\\.)|[^\n\\"])*\"	{
                        /* Strings */
                        yylval.string_val = (char *)malloc(yyleng+1);
			strncpy(yylval.string_val, yytext+1, yyleng-2);
			yylval.string_val[yyleng-2] = '\0';
// strip away the begin and end quotes
			return(TOK_STRING_CONST);
		}

\"((\\.)|[^\n\\"])*\n	{//"
                        /* Runaway string */
			yyerror("runaway string");
		}

{letter}({letter}|{digit}|"_")*	{
                        yylval.string_val = (char *)malloc(yyleng+1);
			strncpy(yylval.string_val, yytext, yyleng+1);
			return(TOK_ID);
		}

[ \t\n]+	{
			;	/* Ignore white spaces */
		}

.		{
			yyerror("unrecognized character");
		}
%%
