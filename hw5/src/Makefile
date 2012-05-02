
COMMON_DIR = ..
include_dir = $(COMMON_DIR)/include
objects = CodeGen.o TypeCheck.o Types.o driver.o Symbols.o AstSymbols.o Ast.o AstPrint.o Error.o decaf.tab.o lex.yy.o Allocate.o Headers.o
CXX = g++
CXXFLAGS = -g -I$(include_dir)
includes = $(include_dir)/Ast.hh $(include_dir)/AstSymbols.hh $(include_dir)/Symbols.hh

main:	$(objects) 
	$(CXX) $(CXXFLAGS) -o demo $(objects)

CodeGen.o:	CodeGen.cc $(includes)
Allocate.o:	Allocate.cc $(includes)
Headers.o:	Headers.cc $(includes)
TypeCheck.o:	TypeCheck.cc $(includes)
Types.o:	Types.cc $(includes)
Error.o:	Error.cc $(includes)
Ast.o:		Ast.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstSymbols.o:	AstSymbols.cc  $(includes)
Symbols.o:	Symbols.cc $(include_dir)/Symbols.hh

lex.yy.o:	decaf.tab.hh lex.yy.c
	$(CXX) $(CXXFLAGS) -c lex.yy.c
decaf.tab.hh decaf.tab.cc:
	bison -d decaf.yy
lex.yy.c:	decaf.l
	flex decaf.l
.PHONY :	clean
clean:
	-echo "Removing all object files!"
	-rm -f demo $(objects)
