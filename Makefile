# TOOLS
CC = gcc
CXX = g++
LEX = flex
YACC = bison -y

#This target can keep changing based on final binary required
#TARGET = scanner
#TARGET = parser
#TARGET = symtab
#TARGET = 3ac
TARGET = compiler

#DIRECTORIES
##Source code
SRCDIR = src
##Compiled source files
BUILDDIR = build
INCDIR = inc
##Contain final executable
TARGETDIR = bin

#Define some standard files that we use
PATTERNS=$(SRCDIR)/patterns.l
GRAMMAR=$(SRCDIR)/grammar.y

# FLAGS
CFLAGS = -g -Wall -D_CC
CXXFLAGS = -g -Wall -D_CXX
##LFLAGS
YFLAGS = -d
LDFLAGS = -lfl
INCFLAGS = $(addprefix -I, $(INCDIR))

all: $(TARGET)




# scanner: grammar patterns
# 	@mkdir -p $(TARGETDIR)
# 	$(CC) $(CFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c $(BUILDDIR)/y.tab.c $(SRCDIR)/scanner.c -o $(TARGETDIR)/scanner  

#parser: grammar patterns 
#	@mkdir -p $(TARGETDIR)
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c $(BUILDDIR)/y.tab.c $(SRCDIR)/parser.cpp $(SRCDIR)/ast.cpp -o $(TARGETDIR)/parser 

#symtab: grammar patterns 
#	@mkdir -p $(TARGETDIR)
#	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c $(BUILDDIR)/y.tab.c $(SRCDIR)/parser.cpp $(SRCDIR)/ast.cpp $(SRCDIR)/symtab.cpp $(SRCDIR)/expression.cpp -o $(TARGETDIR)/symtab 


compiler: grammar patterns symtab expression 3ac codegen
	@mkdir -p $(TARGETDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/patterns.o $(BUILDDIR)/grammar.o $(BUILDDIR)/parser.o $(BUILDDIR)/ast.o $(BUILDDIR)/symtab.o $(BUILDDIR)/expression.o $(BUILDDIR)/3ac.o $(BUILDDIR)/statement.o $(BUILDDIR)/codegen.o -o $(TARGETDIR)/compiler 

codegen: 
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/codegen.cpp  -o $(BUILDDIR)/codegen.o

3ac: 
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/3ac.cpp  -o $(BUILDDIR)/3ac.o
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/statement.cpp -o $(BUILDDIR)/statement.o

expression:
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/expression.cpp  -o $(BUILDDIR)/expression.o 

symtab:
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/y.tab.c -o $(BUILDDIR)/grammar.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c -o $(BUILDDIR)/patterns.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/ast.cpp -o $(BUILDDIR)/ast.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/parser.cpp -o $(BUILDDIR)/parser.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/symtab.cpp -o $(BUILDDIR)/symtab.o 

grammar:
	$(YACC) $(YFLAGS) $(GRAMMAR)
	@mkdir -p $(BUILDDIR)
	@mv y.tab.c $(BUILDDIR)/.
	@mv y.tab.h $(INCDIR)/.

patterns:
	$(LEX) $(PATTERNS)
	@mkdir -p $(BUILDDIR)
	@mv lex.yy.c $(BUILDDIR)/.

.PHONY: clean

clean:
	rm -rf $(BUILDDIR) $(TARGETDIR) $(INCDIR)/y.tab.h
