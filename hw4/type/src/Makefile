
COMMON_DIR = ..
include_dir = $(COMMON_DIR)/include
objects = TypeCheck.o Types.o driver.o Symbols.o AstSymbols.o Ast.o AstPrint.o Error.o lex.yy.o decaf.tab.o
CXX = g++
CXXFLAGS = -Wno-deprecated -g -I$(include_dir)
includes = $(include_dir)/Ast.hh $(include_dir)/AstSymbols.hh $(include_dir)/Symbols.hh

main:	$(objects) 
	$(CXX) $(CXXFLAGS) -o demo $(objects)

TypeCheck.o:	TypeCheck.cc $(includes)
Types.o:	Types.cc $(includes)
Error.o:	Error.cc $(includes)
Ast.o:		Ast.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstSymbols.o:	AstSymbols.cc  $(includes)
Symbols.o:	Symbols.cc $(include_dir)/Symbols.hh
decaf.tab.o:	decaf.tab.cc

lex.yy.o:	decaf.tab.hh lex.yy.c
	$(CXX) $(CXXFLAGS) -c lex.yy.c

decaf.tab.cc decaf.tab.hh:	decaf.yy
	bison -d decaf.yy
.PHONY :	clean
clean:
	-echo "Removing all object files!"
	-rm -f demo $(objects)
