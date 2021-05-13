# TOOLS
CC = gcc
CXX = g++
LEX = flex
YACC = bison -y
LD = ld

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


.PHONY: compiler
compiler: $(BUILDDIR)/symtab.o $(BUILDDIR)/expression.o $(BUILDDIR)/3ac.o $(BUILDDIR)/codegen.o
	@mkdir -p $(TARGETDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/symtab.o $(BUILDDIR)/expression.o $(BUILDDIR)/3ac.o $(BUILDDIR)/codegen.o -o $(TARGETDIR)/compiler 

$(BUILDDIR)/codegen.o: $(INCDIR)/codegen.h $(SRCDIR)/codegen.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/codegen.cpp  -o $(BUILDDIR)/codegen.o

$(BUILDDIR)/3ac.o: $(INCDIR)/3ac.h $(INCDIR)/statement.h $(SRCDIR)/3ac.cpp $(SRCDIR)/statement.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/statement.cpp -o $(BUILDDIR)/statement.o
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS)  $(SRCDIR)/3ac.cpp -o $(BUILDDIR)/3ac_.o
	$(LD) -Ur $(BUILDDIR)/statement.o $(BUILDDIR)/3ac_.o -o $(BUILDDIR)/3ac.o

$(BUILDDIR)/expression.o: $(INCDIR)/expression.h $(SRCDIR)/expression.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/expression.cpp  -o $(BUILDDIR)/expression.o 

$(BUILDDIR)/symtab.o: patterns grammar $(INCDIR)/ast.h $(INCDIR)/symtab.h $(SRCDIR)/ast.cpp $(SRCDIR)/parser.cpp $(SRCDIR)/symtab.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/y.tab.c -o $(BUILDDIR)/grammar.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c -o $(BUILDDIR)/patterns.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/ast.cpp -o $(BUILDDIR)/ast.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/parser.cpp -o $(BUILDDIR)/parser.o 
	$(CXX) -c $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(SRCDIR)/symtab.cpp -o $(BUILDDIR)/symtab_.o 
	$(LD) -Ur $(BUILDDIR)/grammar.o $(BUILDDIR)/patterns.o $(BUILDDIR)/ast.o $(BUILDDIR)/parser.o $(BUILDDIR)/symtab_.o -o $(BUILDDIR)/symtab.o

.PHONY: grammar
grammar: $(GRAMMAR)
	$(YACC) $(YFLAGS) $(GRAMMAR)
	@mkdir -p $(BUILDDIR)
	@mv y.tab.c $(BUILDDIR)/.
	@mv y.tab.h $(INCDIR)/.

.PHONY: patterns
patterns: $(PATTERNS)
	$(LEX) $(PATTERNS)
	@mkdir -p $(BUILDDIR)
	@mv lex.yy.c $(BUILDDIR)/.

.PHONY: clean clear

clean:
	rm -rf $(BUILDDIR) $(TARGETDIR) $(INCDIR)/y.tab.h

clear:
	rm -rf *.s *.dot *.tac *.csv
