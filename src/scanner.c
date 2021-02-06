#include <assert.h>
#include <scanner.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <y.tab.h>

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
  while (1) {
	int token_type = yylex();
    printf("yylex = %d %s %d %d\n", token_type,
           token_data.lexeme, token_data.line_num, token_data.column_num);
	free(token_data.lexeme);
  }
  return 0;
}

// Converts the token enum to a string
// Param1: yytokentype
// Param2 : pointer to string for output
char *token_to_string(int t) {
  switch (t) {
  case AUTO:
    return strdup("AUTO");
    break;
  case BREAK:
    return strdup("BREAK");
    break;
  case CASE:
    return strdup("CASE");
    break;
  case CHAR:
    return strdup("CHAR");
    break;
  case CONST:
    return strdup("CONST");
    break;
  case CONTINUE:
    return strdup("CONTINUE");
    break;
  case DEFAULT:
    return strdup("DEFAULT");
    break;
  case DO:
    return strdup("DO");
    break;
  case DOUBLE:
    return strdup("DOUBLE");
    break;
  case ELSE:
    return strdup("ELSE");
    break;
  case ENUM:
    return strdup("ENUM");
    break;
  case EXTERN:
    return strdup("EXTERN");
    break;
  case FLOAT:
    return strdup("FLOAT");
    break;
  case FOR:
    return strdup("FOR");
    break;
  case GOTO:
    return strdup("GOTO");
    break;
  case IF:
    return strdup("IF");
    break;
  case INT:
    return strdup("INT");
    break;
  case LONG:
    return strdup("LONG");
    break;
  case REGISTER:
    return strdup("REGISTER");
    break;
  case RETURN:
    return strdup("RETURN");
    break;
  case SHORT:
    return strdup("SHORT");
    break;
  case SIGNED:
    return strdup("SIGNED");
    break;
  case SIZEOF:
    return strdup("SIZEOF");
    break;
  case STATIC:
    return strdup("STATIC");
    break;
  case STRUCT:
    return strdup("STRUCT");
    break;
  case SWITCH:
    return strdup("SWITCH");
    break;
  case TYPEDEF:
    return strdup("TYPEDEF");
    break;
  case UNION:
    return strdup("UNION");
    break;
  case UNSIGNED:
    return strdup("UNSIGNED");
    break;
  case VOID:
    return strdup("VOID");
    break;
  case VOLATILE:
    return strdup("VOLATILE");
    break;
  case WHILE:
    return strdup("WHILE");
    break;
  case CONSTANT:
    return strdup("CONSTANT");
    break;
  case STRING_LITERAL:
    return strdup("STRING_LITERAL");
    break;
  case ELLIPSIS:
    return strdup("ELLIPSIS");
    break;
  case RIGHT_ASSIGN:
    return strdup("RIGHT_ASSIGN");
    break;
  case LEFT_ASSIGN:
    return strdup("LEFT_ASSIGN");
    break;
  case ADD_ASSIGN:
    return strdup("ADD_ASSIGN");
    break;
  case SUB_ASSIGN:
    return strdup("SUB_ASSIGN");
    break;
  case MUL_ASSIGN:
    return strdup("MUL_ASSIGN");
    break;
  case DIV_ASSIGN:
    return strdup("DIV_ASSIGN");
    break;
  case MOD_ASSIGN:
    return strdup("MOD_ASSIGN");
    break;
  case AND_ASSIGN:
    return strdup("AND_ASSIGN");
    break;
  case XOR_ASSIGN:
    return strdup("XOR_ASSIGN");
    break;
  case OR_ASSIGN:
    return strdup("OR_ASSIGN");
    break;
  case RIGHT_OP:
    return strdup("RIGHT_OP");
    break;
  case LEFT_OP:
    return strdup("LEFT_OP");
    break;
  case INC_OP:
    return strdup("INC_OP");
    break;
  case DEC_OP:
    return strdup("DEC_OP");
    break;
  case PTR_OP:
    return strdup("PTR_OP");
    break;
  case AND_OP:
    return strdup("AND_OP");
    break;
  case OR_OP:
    return strdup("OR_OP");
    break;
  case LE_OP:
    return strdup("LE_OP");
    break;
  case GE_OP:
    return strdup("GE_OP");
    break;
  case EQ_OP:
    return strdup("EQ_OP");
    break;
  case NE_OP:
    return strdup("NE_OP");
    break;
  case ';':
    return strdup(";");
    break;
  case '{':
    return strdup("{");
    break;
  case '}':
    return strdup("}");
    break;
  case ',':
    return strdup(",");
    break;
  case ':':
    return strdup(":");
    break;
  case '=':
    return strdup("=");
    break;
  case '(':
    return strdup("(");
    break;
  case ')':
    return strdup(")");
    break;
  case '[':
    return strdup("[");
    break;
  case ']':
    return strdup("]");
    break;
  case '.':
    return strdup(".");
    break;
  case '&':
    return strdup("&");
    break;
  case '!':
    return strdup("!");
    break;
  case '~':
    return strdup("~");
    break;
  case '-':
    return strdup("-");
    break;
  case '+':
    return strdup("+");
    break;
  case '*':
    return strdup("*");
    break;
  case '/':
    return strdup("/");
    break;
  case '%':
    return strdup("%%");
    break;
  case '<':
    return strdup("<");
    break;
  case '>':
    return strdup(">");
    break;
  case '^':
    return strdup("^");
    break;
  case '|':
    return strdup("|");
    break;
  case '?':
    return strdup("?");
    break;
  default:
    assert(0);
    break;
    return NULL;
  }
  assert(0);
  return NULL;
}
