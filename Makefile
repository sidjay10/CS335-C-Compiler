# TOOLS
CC = gcc
CXX = g++
LEX = flex
YACC = bison -y

#This target can keep changing based on final binary required
#TARGET = scanner
#TARGET = parser
#TARGET = symtab
TARGET = 3ac

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

3ac: grammar patterns 
	@mkdir -p $(TARGETDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCFLAGS) $(BUILDDIR)/lex.yy.c $(BUILDDIR)/y.tab.c $(SRCDIR)/parser.cpp $(SRCDIR)/ast.cpp $(SRCDIR)/symtab.cpp $(SRCDIR)/expression.cpp $(SRCDIR)/3ac.cpp $(SRCDIR)/statement.cpp -o $(TARGETDIR)/3ac 

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
