#include <stdio.h>
#include <stdlib.h>
int column = 0;
int line_num=1;
extern char* yytext;
extern int unput();

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			{column = 0;
			line_num++;}
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	//ECHO;
	printf("yytext = %s\t",yytext);
	printf("column = %d\t",column);
	printf("line = %d\n",line_num);

}

void comment()
{
	char c, c1;
    extern int line_num;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}
	if (c1=='\n')
		line_num++;
		
	if (c != 0)
		putchar(c1);
}

int yywrap()
{	exit(0);
	return(1);
}



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