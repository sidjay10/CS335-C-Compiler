# TOOLS
CC = gcc
LEX = flex
YACC = bison -y

# FLAGS
CFLAGS = -g -Wall
##LFLAGS
YFLAGS = -d

#This target can keep changing based on final binary required
TARGET = scanner

#DIRECTORIES
SRCDIR = src #Source code
INCDIR = inc #
TARGETDIR = bin #Contain final executable

#Define some standard files that we use
PATTERNS=src/patterns.l
GRAMMAR=src/grammar.y

all: $(TARGET)

scanner: grammar
	@mkdir -p $(TARGETDIR)

grammar:
	$(YACC) $(YFLAGS) $(GRAMMAR)

.PHONY: clean

clean:
	rm -rf $(TARGETDIR)
