#ifndef __SCANNER_H__
#define __SCANNER_H__
typedef struct token_data_ {
  unsigned int column_num;
  unsigned int line_num;
  char *lexeme;
} TOKEN_DATA;

void token_to_string(int token_type, char * str);

#endif
