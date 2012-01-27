snazzle: lex.yy.c proptp.tab.c proptp.tab.h
	g++ proptp.tab.c lex.yy.c -lfl -o proptp

proptp.tab.c proptp.tab.h: proptp.y
	bison -d proptp.y

lex.yy.c: proptp.l proptp.tab.h
	flex proptp.l

clean:
	rm proptp lex.yy.c proptp.tab.h proptp.tab.c
