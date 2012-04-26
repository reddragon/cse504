/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_BOOLEAN = 258,
     TOK_BREAK = 259,
     TOK_CLASS = 260,
     TOK_CONTINUE = 261,
     TOK_ELSE = 262,
     TOK_EXTENDS = 263,
     TOK_FALSE = 264,
     TOK_FLOAT = 265,
     TOK_FOR = 266,
     TOK_IF = 267,
     TOK_INT = 268,
     TOK_NEW = 269,
     TOK_NULL = 270,
     TOK_PRIVATE = 271,
     TOK_PUBLIC = 272,
     TOK_RETURN = 273,
     TOK_STATIC = 274,
     TOK_SUPER = 275,
     TOK_THIS = 276,
     TOK_TRUE = 277,
     TOK_VOID = 278,
     TOK_WHILE = 279,
     TOK_DO = 280,
     TOK_INT_CONST = 281,
     TOK_FLOAT_CONST = 282,
     TOK_STRING_CONST = 283,
     TOK_ID = 284,
     TOK_COMMA = 285,
     TOK_DOT = 286,
     TOK_SEMICOLON = 287,
     TOK_OPEN_SQ_BRACKET = 288,
     TOK_CLOSE_SQ_BRACKET = 289,
     TOK_OPEN_PAREN = 290,
     TOK_CLOSE_PAREN = 291,
     TOK_OPEN_BRACE = 292,
     TOK_CLOSE_BRACE = 293,
     TOK_PLUS = 294,
     TOK_MINUS = 295,
     TOK_MULTIPLY = 296,
     TOK_DIVIDE = 297,
     TOK_PLUS_PLUS = 298,
     TOK_MINUS_MINUS = 299,
     TOK_EQUAL = 300,
     TOK_AND = 301,
     TOK_OR = 302,
     TOK_NOT = 303,
     TOK_GREATER = 304,
     TOK_LESSER = 305,
     TOK_EQUAL_EQUAL = 306,
     TOK_NOT_EQUAL = 307,
     TOK_GREATER_OR_EQUAL = 308,
     TOK_LESSER_OR_EQUAL = 309,
     TOK_SYNTAX_ERROR = 310,
     TOK_RUNAWAY_STRING = 311
   };
#endif
/* Tokens.  */
#define TOK_BOOLEAN 258
#define TOK_BREAK 259
#define TOK_CLASS 260
#define TOK_CONTINUE 261
#define TOK_ELSE 262
#define TOK_EXTENDS 263
#define TOK_FALSE 264
#define TOK_FLOAT 265
#define TOK_FOR 266
#define TOK_IF 267
#define TOK_INT 268
#define TOK_NEW 269
#define TOK_NULL 270
#define TOK_PRIVATE 271
#define TOK_PUBLIC 272
#define TOK_RETURN 273
#define TOK_STATIC 274
#define TOK_SUPER 275
#define TOK_THIS 276
#define TOK_TRUE 277
#define TOK_VOID 278
#define TOK_WHILE 279
#define TOK_DO 280
#define TOK_INT_CONST 281
#define TOK_FLOAT_CONST 282
#define TOK_STRING_CONST 283
#define TOK_ID 284
#define TOK_COMMA 285
#define TOK_DOT 286
#define TOK_SEMICOLON 287
#define TOK_OPEN_SQ_BRACKET 288
#define TOK_CLOSE_SQ_BRACKET 289
#define TOK_OPEN_PAREN 290
#define TOK_CLOSE_PAREN 291
#define TOK_OPEN_BRACE 292
#define TOK_CLOSE_BRACE 293
#define TOK_PLUS 294
#define TOK_MINUS 295
#define TOK_MULTIPLY 296
#define TOK_DIVIDE 297
#define TOK_PLUS_PLUS 298
#define TOK_MINUS_MINUS 299
#define TOK_EQUAL 300
#define TOK_AND 301
#define TOK_OR 302
#define TOK_NOT 303
#define TOK_GREATER 304
#define TOK_LESSER 305
#define TOK_EQUAL_EQUAL 306
#define TOK_NOT_EQUAL 307
#define TOK_GREATER_OR_EQUAL 308
#define TOK_LESSER_OR_EQUAL 309
#define TOK_SYNTAX_ERROR 310
#define TOK_RUNAWAY_STRING 311




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 35 "decaf.yy"
{
        char* string_val;
        int   int_val;
        float float_val;
        char char_val;
	bool bool_val;
	Entity *entity;
	Type *type_val;
	Statement *statement;
	Expression *expression;
	list<Entity*> *entity_list;
	list<Statement *> *statement_list;
	list<Expression *> *expression_list;
	pair<bool, bool> *pair_val;
}
/* Line 1529 of yacc.c.  */
#line 177 "decaf.tab.hh"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

