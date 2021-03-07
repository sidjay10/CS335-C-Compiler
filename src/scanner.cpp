#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <y.tab.h>

#include <scanner.h>
#include <ast.h>
//#define LEX

int yylex();

extern FILE *yyin;
extern FILE *yyout;

TOKEN_DATA token_data;

int main(int argc, char *argv[]) {
  FILE *fh;
  FILE *fo;
  if (argc == 2 && (fh = fopen(argv[1], "r")))
    yyin = fh;
  if (argc == 3 && (fo = fopen(argv[2], "w+")))
    yyout = fo;

  printf("Line#     Column#   Token               Lexeme                       "
         " \n");
  printf("====================================================================="
         "=\n");

#ifndef LEX
	yyparse();
#endif

#ifdef LEX
  while (1) {
    int token_type = yylex();
    char token_str[64];
    token_to_string(token_type, token_str);
    printf("%-9d %-9d %-19s %s\n", token_data.line_num, token_data.column_num,
           token_str, token_data.lexeme);
    free(token_data.lexeme);
  }
#endif
  return 0;
}

// Converts the token enum to a string
// Param1: yytokentype
// Param2 : pointer to string for output
void token_to_string(int token_type, char *str) {
  switch (token_type) {
  case AUTO:
    sprintf(str, "AUTO");
    break;
  case BREAK:
    sprintf(str, "BREAK");
    break;
  case CASE:
    sprintf(str, "CASE");
    break;
  case CHAR:
    sprintf(str, "CHAR");
    break;
  case CONST:
    sprintf(str, "CONST");
    break;
  case CONTINUE:
    sprintf(str, "CONTINUE");
    break;
  case DEFAULT:
    sprintf(str, "DEFAULT");
    break;
  case DO:
    sprintf(str, "DO");
    break;
  case DOUBLE:
    sprintf(str, "DOUBLE");
    break;
  case ELSE:
    sprintf(str, "ELSE");
    break;
  case ENUM:
    sprintf(str, "ENUM");
    break;
  case EXTERN:
    sprintf(str, "EXTERN");
    break;
  case FLOAT:
    sprintf(str, "FLOAT");
    break;
  case FOR:
    sprintf(str, "FOR");
    break;
  case GOTO:
    sprintf(str, "GOTO");
    break;
  case IF:
    sprintf(str, "IF");
    break;
  case INT:
    sprintf(str, "INT");
    break;
  case LONG:
    sprintf(str, "LONG");
    break;
  case REGISTER:
    sprintf(str, "REGISTER");
    break;
  case RETURN:
    sprintf(str, "RETURN");
    break;
  case SHORT:
    sprintf(str, "SHORT");
    break;
  case SIGNED:
    sprintf(str, "SIGNED");
    break;
  case SIZEOF:
    sprintf(str, "SIZEOF");
    break;
  case STATIC:
    sprintf(str, "STATIC");
    break;
  case STRUCT:
    sprintf(str, "STRUCT");
    break;
  case SWITCH:
    sprintf(str, "SWITCH");
    break;
  case TYPEDEF:
    sprintf(str, "TYPEDEF");
    break;
  case UNION:
    sprintf(str, "UNION");
    break;
  case UNSIGNED:
    sprintf(str, "UNSIGNED");
    break;
  case VOID:
    sprintf(str, "VOID");
    break;
  case VOLATILE:
    sprintf(str, "VOLATILE");
    break;
  case WHILE:
    sprintf(str, "WHILE");
    break;
  case IDENTIFIER:
    sprintf(str, "IDENTIFIER");
    break;
  case CONSTANT:
    sprintf(str, "CONSTANT");
    break;
  case STRING_LITERAL:
    sprintf(str, "STRING_LITERAL");
    break;
  case ELLIPSIS:
    sprintf(str, "ELLIPSIS");
    break;
  case RIGHT_ASSIGN:
    sprintf(str, "RIGHT_ASSIGN");
    break;
  case LEFT_ASSIGN:
    sprintf(str, "LEFT_ASSIGN");
    break;
  case ADD_ASSIGN:
    sprintf(str, "ADD_ASSIGN");
    break;
  case SUB_ASSIGN:
    sprintf(str, "SUB_ASSIGN");
    break;
  case MUL_ASSIGN:
    sprintf(str, "MUL_ASSIGN");
    break;
  case DIV_ASSIGN:
    sprintf(str, "DIV_ASSIGN");
    break;
  case MOD_ASSIGN:
    sprintf(str, "MOD_ASSIGN");
    break;
  case AND_ASSIGN:
    sprintf(str, "AND_ASSIGN");
    break;
  case XOR_ASSIGN:
    sprintf(str, "XOR_ASSIGN");
    break;
  case OR_ASSIGN:
    sprintf(str, "OR_ASSIGN");
    break;
  case RIGHT_OP:
    sprintf(str, "RIGHT_OP");
    break;
  case LEFT_OP:
    sprintf(str, "LEFT_OP");
    break;
  case INC_OP:
    sprintf(str, "INC_OP");
    break;
  case DEC_OP:
    sprintf(str, "DEC_OP");
    break;
  case PTR_OP:
    sprintf(str, "PTR_OP");
    break;
  case AND_OP:
    sprintf(str, "AND_OP");
    break;
  case OR_OP:
    sprintf(str, "OR_OP");
    break;
  case LE_OP:
    sprintf(str, "LE_OP");
    break;
  case GE_OP:
    sprintf(str, "GE_OP");
    break;
  case EQ_OP:
    sprintf(str, "EQ_OP");
    break;
  case NE_OP:
    sprintf(str, "NE_OP");
    break;
  case ';':
    sprintf(str, ";");
    break;
  case '{':
    sprintf(str, "{");
    break;
  case '}':
    sprintf(str, "}");
    break;
  case ',':
    sprintf(str, ",");
    break;
  case ':':
    sprintf(str, ":");
    break;
  case '=':
    sprintf(str, "=");
    break;
  case '(':
    sprintf(str, "(");
    break;
  case ')':
    sprintf(str, ")");
    break;
  case '[':
    sprintf(str, "[");
    break;
  case ']':
    sprintf(str, "]");
    break;
  case '.':
    sprintf(str, ".");
    break;
  case '&':
    sprintf(str, "&");
    break;
  case '!':
    sprintf(str, "!");
    break;
  case '~':
    sprintf(str, "~");
    break;
  case '-':
    sprintf(str, "-");
    break;
  case '+':
    sprintf(str, "+");
    break;
  case '*':
    sprintf(str, "*");
    break;
  case '/':
    sprintf(str, "/");
    break;
  case '%':
    sprintf(str, "%%");
    break;
  case '<':
    sprintf(str, "<");
    break;
  case '>':
    sprintf(str, ">");
    break;
  case '^':
    sprintf(str, "^");
    break;
  case '|':
    sprintf(str, "|");
    break;
  case '?':
    sprintf(str, "?");
    break;
  default:
    printf("NO TOKEN %d\n", token_type);
    assert(0);
    break;
  }
  return;
}
