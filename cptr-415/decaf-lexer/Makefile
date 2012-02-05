decaf-lexer: lex.yy.c decaf.tab.c decaf.tab.h
	g++ decaf.tab.c lex.yy.c -lfl -o decaf-lexer

decaf.tab.c decaf.tab.h: decaf.y
	bison -d decaf.y
	
lex.yy.c: decaf.l decaf.tab.h
	flex decaf.l

clean:
	rm decaf-lexer lex.yy.c decaf.tab.c decaf.tab.h
