clean:
	rm snazzle lex.yy.c snazzle.tab.h snazzle.tab.c

snazzle.tab.c snazzle.tab.h: snazzle.y
	bison -d snazzle.y

lex.yy.c: snazzle.l snazzle.tab.h
	flex snazzle.l

snazzle: lex.yy.c snazzle.tab.c snazzle.tab.h
	g++ snazzle.tab.c lex.yy.c -lfl -o snazzle
