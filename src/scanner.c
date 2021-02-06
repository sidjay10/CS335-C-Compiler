#include <stdio.h>
#include <stdlib.h>

int yylex();
int main(int argc, char* argv[]) {
    FILE *fh;
	FILE *fo;
    extern FILE *yyin;
    extern FILE *yyout;
    if (argc == 2 && (fh = fopen(argv[1], "r")))
        yyin = fh;
    if (argc == 3 && (fo= fopen(argv[2], "w+")))
        yyout = fo;
	while(1)
	{yylex();
	}
    return 0;
}  