D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "scanner.h"
#include "ast.h"
#include "symtab.h"
#include "expression.h"
#include "statement.h"
#include "y.tab.h"


extern "C" int yylex();

extern TOKEN_DATA token_data;

void count();
void count_wspace();
void comment();
int main();
void parse();
int check_type();
int yylex();

unsigned int column = 1;
unsigned int line_num=1;
unsigned int prev_column = 0;
unsigned int prev_line_num=0;
bool wspace = false;

std::stringstream text;
std::vector < std::string > code;
%}

%%
"/*"			{ wspace = true; comment(); }
"//"[^\n]*\n	{ wspace = true; code.push_back(text.str()); text.clear(); text.str(std::string()); column=1; line_num++;  }

"auto"			{ count(); return(AUTO); }
"break"			{ count(); return(BREAK); }
"case"			{ count(); return(CASE); }
"char"			{ yylval.type_specifier = create_type_specifier(CHAR, line_num, column); count(); return(CHAR); }
"const"			{ count(); return(CONST); }
"continue"		{ count(); return(CONTINUE); }
"default"		{ count(); return(DEFAULT); }
"do"			{ count(); return(DO); }
"double"		{ yylval.type_specifier = create_type_specifier(DOUBLE, line_num, column); count(); return(DOUBLE); }
"else"			{ count(); return(ELSE); }
"enum"			{ count(); return(ENUM); }
"extern"		{ count(); return(EXTERN); }
"float"			{ yylval.type_specifier = create_type_specifier(FLOAT, line_num, column); count(); return(FLOAT); }
"for"			{ count(); return(FOR); }
"goto"			{ count(); return(GOTO); }
"if"			{ count(); return(IF); }
"int"			{ yylval.type_specifier = create_type_specifier(INT, line_num, column); count(); return(INT); }
"long"			{ yylval.type_specifier = create_type_specifier(LONG, line_num, column); count(); return(LONG); }
"register"		{ count(); return(REGISTER); }
"return"		{ count(); return(RETURN); }
"short"			{ yylval.type_specifier = create_type_specifier(SHORT, line_num, column); count(); return(SHORT); }
"signed"		{ yylval.type_specifier = create_type_specifier(SIGNED, line_num, column); count(); return(SIGNED); }
"sizeof"		{ yylval.terminal= create_terminal("sizeof",NULL, line_num, column ); count(); return(SIZEOF); }
"static"		{ count(); return(STATIC); }
"struct"		{ count(); return(STRUCT); }
"switch"		{ count(); return(SWITCH); }
"typedef"		{ count(); return(TYPEDEF); }
"union"			{ count(); return(UNION); }
"unsigned"		{ yylval.type_specifier = create_type_specifier(UNSIGNED, line_num, column); count(); return(UNSIGNED); }
"void"			{ yylval.terminal = create_type_specifier(VOID, line_num, column); count(); return(VOID); }
"bool"			{ yylval.terminal = create_type_specifier(CHAR, line_num, column); count(); return(CHAR); }
"volatile"		{ count(); return(VOLATILE); }
"while"			{ count(); return(WHILE); }

{L}({L}|{D})*		{  int type = check_type(); count(); return(type); }

0[xX]{H}+{IS}?		{  yylval.constant = create_constant("CONSTANT HEX", yytext, line_num, column); count(); return(CONSTANT); }
0{D}+{IS}?		{  yylval.constant = create_constant("CONSTANT INT", yytext, line_num, column); count(); return(CONSTANT); }
{D}+{IS}?		{  yylval.constant = create_constant("CONSTANT INT", yytext, line_num, column); count(); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{  count(); yylval.constant = create_constant("CONSTANT CHAR", yytext, line_num, prev_column); return(CONSTANT); }

{D}+{E}{FS}?		{  yylval.constant = create_constant("CONSTANT EXP", yytext, line_num, column); count(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{  yylval.constant = create_constant("CONSTANT FLOAT", yytext, line_num, column); count(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{  yylval.constant = create_constant("CONSTANT FLOAT", yytext, line_num, column); count(); return(CONSTANT); }

L?\"(\\.|[^\\"])*\"	{ yylval.terminal = create_terminal("STRING LITERAL", yytext, line_num, column); count(); return(STRING_LITERAL); }

"..."			{ count(); return(ELLIPSIS); }
">>="			{ yylval.terminal= create_terminal(">>=",NULL, line_num, column ); count(); return(RIGHT_ASSIGN); }
"<<="			{ yylval.terminal= create_terminal("<<=",NULL, line_num, column ); count(); return(LEFT_ASSIGN); }
"+="			{ yylval.terminal= create_terminal("+=",NULL, line_num, column ); count(); return(ADD_ASSIGN); }
"-="			{ yylval.terminal= create_terminal("-=",NULL, line_num, column ); count(); return(SUB_ASSIGN); }
"*="			{ yylval.terminal= create_terminal("*=",NULL, line_num, column ); count(); return(MUL_ASSIGN); }
"/="			{ yylval.terminal= create_terminal("/=",NULL, line_num, column ); count(); return(DIV_ASSIGN); }
"%="			{ yylval.terminal= create_terminal("%=",NULL, line_num, column ); count(); return(MOD_ASSIGN); }
"&="			{ yylval.terminal= create_terminal("&=",NULL, line_num, column ); count(); return(AND_ASSIGN); }
"^="			{ yylval.terminal= create_terminal("^=",NULL, line_num, column ); count(); return(XOR_ASSIGN); }
"|="			{ yylval.terminal= create_terminal("|=",NULL, line_num, column ); count(); return(OR_ASSIGN); }
">>"			{ yylval.terminal= create_terminal(">>",NULL, line_num, column ); count(); return(RIGHT_OP); }
"<<"			{ count(); return(LEFT_OP); }
"++"			{ yylval.terminal= create_terminal("++",NULL, line_num, column ); count(); return(INC_OP); }
"--"			{ yylval.terminal= create_terminal("--",NULL, line_num, column ); count(); return(DEC_OP); }
"->"			{ count(); return(PTR_OP); }
"&&"			{ count(); return(AND_OP); }
"||"			{ count(); return(OR_OP); }
"<="			{ count(); return(LE_OP); }
">="			{ count(); return(GE_OP); }
"=="			{ count(); return(EQ_OP); }
"!="			{ count(); return(NE_OP); }
";"			{ count(); return(';'); }
("{"|"<%")		{ count(); return('{'); }
("}"|"%>")		{ count(); return('}'); }
","			{ count(); return(','); }
":"			{ count(); return(':'); }
"="			{ yylval.terminal= create_terminal("=",NULL, line_num, column ); count(); return('='); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
("["|"<:")		{ count(); return('['); }
("]"|":>")		{ count(); return(']'); }
"."			{ count(); return('.'); }
"&"			{ yylval.terminal= create_terminal("&",NULL, line_num, column ); count(); return('&'); }
"!"			{ yylval.terminal= create_terminal("!",NULL, line_num, column ); count(); return('!'); }
"~"			{ yylval.terminal= create_terminal("~",NULL, line_num, column ); count(); return('~'); }
"-"			{ yylval.terminal= create_terminal("-",NULL, line_num, column ); count(); return('-'); }
"+"			{ yylval.terminal= create_terminal("+",NULL, line_num, column ); count(); return('+'); }
"*"			{ yylval.terminal= create_terminal("*",NULL, line_num, column ); count(); return('*'); }
"/"			{ count(); return('/'); }
"%"			{ count(); return('%'); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
"^"			{ count(); return('^'); }
"|"			{ count(); return('|'); }
"?"			{ count(); return('?'); }

[ \t\v\n\f]		{ count_wspace(); }
.			{ /* ignore bad characters */ }

%%


void comment()
{
	char c, c1;

loop:
	while ((c = yyinput()) != '*' && c != 0){
		if(c=='\n') {
			code.push_back(text.str());
			text.clear(); text.str(std::string());
			line_num++;
		} 
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
	if ( !wspace ) {
		prev_column = column;
		prev_line_num = line_num;
	}
	wspace = false;
	token_data.lexeme = strdup(yytext);

	

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n')
			{
				column = 1;
				code.push_back(text.str());
				text.clear(); text.str(std::string());
	

			line_num++;}
		else if (yytext[i] == '\t') {
			column += 8 - (column % 8) ;
			text << yytext[i];
		} else {
			column++; 

	text << yytext[i];
	}
	}
}

void count_wspace()
{
	int i;
	if ( !wspace ) {
		prev_column = column;
		prev_line_num = line_num;
	}
	wspace = true;

	for (i = 0; yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n') {
				column = 1;
				line_num++;
				code.push_back(text.str());
				text.clear(); text.str(std::string());
		}
		else if (yytext[i] == '\t') {
			column += 8 - (column % 8) ;
			text << yytext[i];
		} else {
			column++; 
			text << yytext[i];
		}
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


	if( std::string(yytext) == "NULL") {
		yylval.constant = create_constant("CONSTANT HEX", "0x0", line_num, column);
		yylval.constant->ConstantType = Type(VOID_T, 1, true);
		yylval.constant->ConstantType.is_pointer = true;
		return(CONSTANT);
	}
	if( std::string(yytext) == "true") {
		yylval.constant = create_constant("CONSTANT INT", "1", line_num, column);
		yylval.constant->ConstantType = Type(CHAR_T, 0, true);
		return(CONSTANT);
	} else if( std::string(yytext) == "false") {
		yylval.constant = create_constant("CONSTANT INT", "0", line_num, column);
		yylval.constant->ConstantType = Type(CHAR_T, 0, true);
		return(CONSTANT);
	}

	Identifier * id = new Identifier(yytext, line_num, column);
	yylval.identifier = id;
	return(IDENTIFIER);
}


int yywrap()
{	
	return(1);
}

