#include <assert.h>
#include <stdio.h>
#include <y.tab.h>

// Converts the token enum to a string
// Param1: yytokentype
// Param2 : pointer to string for output
void token_to_string(enum yytokentype t, char *str) {
  switch (t) {
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
    assert(0);
    break;
  }
}
