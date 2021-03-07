D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>

#include "scanner.h"
#include "ast.h"
#include "y.tab.h"

extern "C" int yylex();

extern TOKEN_DATA token_data;

void count();
void comment();
int main();
void parse();
int check_type();
int yylex();

int column = 1;
int line_num=1;
%}

%%
"/*"			{ comment(); }
"//"[^\n]*\n		{ line_num++;  }

"auto"			{ count(); yylval.node = create_terminal("AUTO", yytext); return(AUTO); }
"break"			{ count(); yylval.node = create_terminal("BREAK", yytext); return(BREAK); }
"case"			{ count(); yylval.node = create_terminal("CASE", yytext); return(CASE); }
"char"			{ count(); yylval.node = create_terminal("CHAR", yytext); return(CHAR); }
"const"			{ count(); yylval.node = create_terminal("CONST", yytext); return(CONST); }
"continue"		{ count(); yylval.node = create_terminal("CONTINUE", yytext); return(CONTINUE); }
"default"		{ count(); yylval.node = create_terminal("DEFAULT", yytext); return(DEFAULT); }
"do"			{ count(); yylval.node = create_terminal("DO", yytext); return(DO); }
"double"		{ count(); yylval.node = create_terminal("DOUBLE", yytext); return(DOUBLE); }
"else"			{ count(); yylval.node = create_terminal("ELSE", yytext); return(ELSE); }
"enum"			{ count(); yylval.node = create_terminal("ENUM", yytext); return(ENUM); }
"extern"		{ count(); yylval.node = create_terminal("EXTERN", yytext); return(EXTERN); }
"float"			{ count(); yylval.node = create_terminal("FLOAT", yytext); return(FLOAT); }
"for"			{ count(); yylval.node = create_terminal("FOR", yytext); return(FOR); }
"goto"			{ count(); yylval.node = create_terminal("GOTO", yytext); return(GOTO); }
"if"			{ count(); yylval.node = create_terminal("IF", yytext); return(IF); }
"int"			{ count(); yylval.node = create_terminal("INT", yytext); return(INT); }
"long"			{ count(); yylval.node = create_terminal("LONG", yytext); return(LONG); }
"register"		{ count(); yylval.node = create_terminal("REGISTER", yytext); return(REGISTER); }
"return"		{ count(); yylval.node = create_terminal("RETURN", yytext); return(RETURN); }
"short"			{ count(); yylval.node = create_terminal("SHORT", yytext); return(SHORT); }
"signed"		{ count(); yylval.node = create_terminal("SIGNED", yytext); return(SIGNED); }
"sizeof"		{ count(); yylval.node = create_terminal("SIZEOF", yytext); return(SIZEOF); }
"static"		{ count(); yylval.node = create_terminal("STATIC", yytext); return(STATIC); }
"struct"		{ count(); yylval.node = create_terminal("STRUCT", yytext); return(STRUCT); }
"switch"		{ count(); yylval.node = create_terminal("SWITCH", yytext); return(SWITCH); }
"typedef"		{ count(); yylval.node = create_terminal("TYPEDEF", yytext); return(TYPEDEF); }
"union"			{ count(); yylval.node = create_terminal("UNION", yytext); return(UNION); }
"unsigned"		{ count(); yylval.node = create_terminal("UNSIGNED", yytext); return(UNSIGNED); }
"void"			{ count(); yylval.node = create_terminal("VOID", yytext); return(VOID); }
"volatile"		{ count(); yylval.node = create_terminal("VOLATILE", yytext); return(VOLATILE); }
"while"			{ count(); yylval.node = create_terminal("WHILE", yytext); return(WHILE); }

{L}({L}|{D})*		{ count(); yylval.node = create_terminal("IDENTIFIER", yytext); return(check_type()); }

0[xX]{H}+{IS}?		{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }
0{D}+{IS}?		{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }
{D}+{IS}?		{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }

{D}+{E}{FS}?		{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); yylval.node = create_terminal("CONSTANT", yytext); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ count(); yylval.node = create_terminal("STRING_LITERAL", yytext); return(STRING_LITERAL); }

"..."			{ count(); yylval.node = create_terminal("ELLIPSIS", yytext); return(ELLIPSIS); }
">>="			{ count(); yylval.node = create_terminal("RIGHT_ASSIGN", yytext); return(RIGHT_ASSIGN); }
"<<="			{ count(); yylval.node = create_terminal("LEFT_ASSIGN", yytext); return(LEFT_ASSIGN); }
"+="			{ count(); yylval.node = create_terminal("ADD_ASSIGN", yytext); return(ADD_ASSIGN); }
"-="			{ count(); yylval.node = create_terminal("SUB_ASSIGN", yytext); return(SUB_ASSIGN); }
"*="			{ count(); yylval.node = create_terminal("MUL_ASSIGN", yytext); return(MUL_ASSIGN); }
"/="			{ count(); yylval.node = create_terminal("DIV_ASSIGN", yytext); return(DIV_ASSIGN); }
"%="			{ count(); yylval.node = create_terminal("MOD_ASSIGN", yytext); return(MOD_ASSIGN); }
"&="			{ count(); yylval.node = create_terminal("AND_ASSIGN", yytext); return(AND_ASSIGN); }
"^="			{ count(); yylval.node = create_terminal("XOR_ASSIGN", yytext); return(XOR_ASSIGN); }
"|="			{ count(); yylval.node = create_terminal("OR_ASSIGN", yytext); return(OR_ASSIGN); }
">>"			{ count(); yylval.node = create_terminal("RIGHT_OP", yytext); return(RIGHT_OP); }
"<<"			{ count(); yylval.node = create_terminal("LEFT_OP", yytext); return(LEFT_OP); }
"++"			{ count(); yylval.node = create_terminal("INC_OP", yytext); return(INC_OP); }
"--"			{ count(); yylval.node = create_terminal("DEC_OP", yytext); return(DEC_OP); }
"->"			{ count(); yylval.node = create_terminal("PTR_OP", yytext); return(PTR_OP); }
"&&"			{ count(); yylval.node = create_terminal("AND_OP", yytext); return(AND_OP); }
"||"			{ count(); yylval.node = create_terminal("OR_OP", yytext); return(OR_OP); }
"<="			{ count(); yylval.node = create_terminal("LE_OP", yytext); return(LE_OP); }
">="			{ count(); yylval.node = create_terminal("GE_OP", yytext); return(GE_OP); }
"=="			{ count(); yylval.node = create_terminal("EQ_OP", yytext); return(EQ_OP); }
"!="			{ count(); yylval.node = create_terminal("NE_OP", yytext); return(NE_OP); }
";"			{ count(); yylval.node = create_terminal(";", nullptr); return(';'); }
("{"|"<%")		{ count(); yylval.node = create_terminal("{", nullptr); return('{'); }
("}"|"%>")		{ count(); yylval.node = create_terminal("}", nullptr); return('}'); }
","			{ count(); yylval.node = create_terminal(",", nullptr); return(','); }
":"			{ count(); yylval.node = create_terminal(":", nullptr); return(':'); }
"="			{ count(); yylval.node = create_terminal("=", nullptr); return('='); }
"("			{ count(); yylval.node = create_terminal("(", nullptr); return('('); }
")"			{ count(); yylval.node = create_terminal(")", nullptr); return(')'); }
("["|"<:")		{ count(); yylval.node = create_terminal("[", nullptr); return('['); }
("]"|":>")		{ count(); yylval.node = create_terminal("]", nullptr); return(']'); }
"."			{ count(); yylval.node = create_terminal(".", nullptr); return('.'); }
"&"			{ count(); yylval.node = create_terminal("&", nullptr); return('&'); }
"!"			{ count(); yylval.node = create_terminal("!", nullptr); return('!'); }
"~"			{ count(); yylval.node = create_terminal("~", nullptr); return('~'); }
"-"			{ count(); yylval.node = create_terminal("-", nullptr); return('-'); }
"+"			{ count(); yylval.node = create_terminal("+", nullptr); return('+'); }
"*"			{ count(); yylval.node = create_terminal("*", nullptr); return('*'); }
"/"			{ count(); yylval.node = create_terminal("/", nullptr); return('/'); }
"%"			{ count(); yylval.node = create_terminal("%", nullptr); return('%'); }
"<"			{ count(); yylval.node = create_terminal("<", nullptr); return('<'); }
">"			{ count(); yylval.node = create_terminal(">", nullptr); return('>'); }
"^"			{ count(); yylval.node = create_terminal("^", nullptr); return('^'); }
"|"			{ count(); yylval.node = create_terminal("|", nullptr); return('|'); }
"?"			{ count(); yylval.node = create_terminal("?", nullptr); return('?'); }

[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }

%%


void comment()
{
	char c, c1;

loop:
	while ((c = yyinput()) != '*' && c != 0){
		if(c=='\n')
			line_num++; 
		continue;
	}
	if ((c1 = yyinput()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}
	
	if (c != 0){
		// do nothing
	}
		
}

void count()
{
	int i;
	token_data.column_num = column;
	token_data.line_num = line_num;
	token_data.lexeme = strdup(yytext);

	

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n')
			{column = 1;
			line_num++;}
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
	}
}

int check_type()
{
/*
 pseudo code --- this is what it should check

	if (yytext == type_name)
		return(TYPE_NAME);

	return(IDENTIFIER);
	it actually will only return IDENTIFIER
*/
	return(IDENTIFIER);
}


int yywrap()
{	
	return(1);
}

