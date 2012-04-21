
COMMON_DIR = ..
include_dir = $(COMMON_DIR)/include
objects = driver.o Symbols.o AstSymbols.o Ast.o AstPrint.o decaf.tab.o lex.yy.o 
CXX = g++
CXXFLAGS = -Wno-deprecated -g -I$(include_dir)
includes = $(include_dir)/Ast.hh $(include_dir)/AstSymbols.hh $(include_dir)/Symbols.hh

main:	$(objects) 
	$(CXX) $(CXXFLAGS) -o demo $(objects)

decaf.tab.hh decaf.tab.cc:	decaf.yy
	bison -d --report=all decaf.yy

decaf.tab.o:	decaf.tab.cc $(includes)
Ast.o:		Ast.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstPrint.o:	AstPrint.cc  $(includes)
AstSymbols.o:	AstSymbols.cc  $(includes)
Symbols.o:	Symbols.cc $(include_dir)/Symbols.hh

lex.yy.o:	decaf.tab.hh lex.yy.c
	$(CXX) $(CXXFLAGS) -c lex.yy.c

.PHONY :	clean
clean:
	-echo "Removing all object files!"
	-rm -f demo $(objects)
	-echo "Removing intermediate C files!"
	-rm -f decaf.tab.cc decaf.tab.hh
