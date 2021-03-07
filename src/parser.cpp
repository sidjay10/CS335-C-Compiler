#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>

#include <ast.h>
#include <scanner.h>
#include <y.tab.h>


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


	std::stringstream ss;
	ss << "digraph G {\n";
	ss << "\tordering=out\n";
	std::cout << ss.str();

	int abc = yyparse();
	
	assert(abc == 0);
	
	ss.str("");
	ss << "}\n";
	std::cout << ss.str();

  return 0;
}

