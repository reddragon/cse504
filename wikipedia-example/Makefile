clean:
	rm Main
	rm Lexer.c
	rm Lexer.h	
	rm Parser.tab.c
	rm Parser.h

lex:
	flex --outfile=Lexer.c --header-file=Lexer.h Lexer.l

bison:
	bison --defines=Parser.h Parser.y

hw:
	make lex
	make bison	
	gcc -o Main Main.c Lexer.c Parser.tab.c Expression.c 

