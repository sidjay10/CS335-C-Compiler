SRCDIR=src
BINDIR=out/bin
INCDIR=inc
OUTDIR=out

CFLAGS = -Wall
CC = gcc

FLEX=flex
YACC=bison

.PHONY: all
all : 	yacc_files lex_files c_files

yacc_files : $(SRCDIR)/yacc.yy
	mkdir -p $(INCDIR)
	$(YACC) -d $(SRCDIR)/yacc.yy -o $(INCDIR)/y.tab.c
	rm $(INCDIR)/y.tab.c

lex_files : $(SRCDIR)/lex.l
	mkdir -p $(OUTDIR)
	$(FLEX) -o $(OUTDIR)/lex.yy.c $(SRCDIR)/lex.l 

c_files : $(INCDIR)/y.tab.h $(OUTDIR)/lex.yy.c
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -lfl $(OUTDIR)/lex.yy.c -o $(BINDIR)/lexer -I $(INCDIR)

clean : 
	rm -r $(OUTDIR);
	rm -r $(INCDIR);




	


