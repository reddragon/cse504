decaf-lexer: lex.yy.c decaf.tab.c decaf.tab.h
	g++ decaf.tab.c lex.yy.c -lfl -o decafch

debug: lex.yy.c decaf.tab.c decaf.tab.h
	g++ -D DEBUG decaf.tab.c lex.yy.c -lfl -o decafch

decaf.tab.c decaf.tab.h: decaf.y
	bison -d --report=all decaf.y

lex.yy.c: decaf.l decaf.tab.h
	flex decaf.l

tests: lex.yy.c decaf.tab.c decaf.tab.h
	make; ./tests.sh

clean:
	rm decafch lex.yy.c decaf.tab.c decaf.tab.h decaf.output > /dev/null 2>&1
