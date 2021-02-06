# TOOLS
CC = gcc
LEX = flex
YACC = bison -y

# FLAGS
CFLAGS = -g -Wall
##LFLAGS
YFLAGS = -d
LDFLAGS = -lfl

#This target can keep changing based on final binary required
TARGET = scanner

#DIRECTORIES
##Source code
SRCDIR = src
INCDIR = inc
##Compiled source files
BUILDDIR = build
##Contain final executable
TARGETDIR = bin

#Define some standard files that we use
PATTERNS=$(SRCDIR)/patterns.l
GRAMMAR=$(SRCDIR)/grammar.y

all: $(TARGET)

scanner: grammar patterns
	@mkdir -p $(TARGETDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(BUILDDIR)/lex.yy.c $(BUILDDIR)/y.tab.c $(SRCDIR)/scanner.c -o $(TARGETDIR)/scanner  

grammar:
	$(YACC) $(YFLAGS) $(GRAMMAR)
	@mkdir -p $(BUILDDIR)
	@mv y.tab.* $(BUILDDIR)/.

patterns:
	$(LEX) $(PATTERNS)
	@mkdir -p $(BUILDDIR)
	@mv lex.yy.c $(BUILDDIR)/.

.PHONY: clean

clean:
	rm -rf $(BUILDDIR) $(TARGETDIR)