#ifndef __SCANNER_H__
#define __SCANNER_H__
typedef struct token_data_ {
  int column_num;
  int line_num;
  char *lexeme;
} TOKEN_DATA;

char *token_to_string(int);
#endif
