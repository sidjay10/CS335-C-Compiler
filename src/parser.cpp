#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>
#include <ast.h>
#include <symtab.h>
#include <expression.h>
#include <scanner.h>
#include <statement.h>
#include <y.tab.h>
#include <cmdline.h>

///////EXTERN GLOBALS//////
int error_flag;
std::stringstream tac_ss;
std::stringstream asm_ss;
std::stringstream dot_ss;
std::stringstream sym_ss;
///////////////////////////


extern FILE *yyin;
extern FILE *yyout;

Node * root = NULL;
TOKEN_DATA token_data;

int main(int argc, char *argv[]) {
	
	// Command Line Parsing and Options
	cmdline::parser cli;
	//Boolean Flags
	cli.add("dot-cfg", '\0', "Generate AST in dot format");
	cli.add("symtab", '\0', "Generate the Symbol Table in csv format");
	cli.add("tac", '\0', "Generate three address code intermediate");
	cli.add("all", 'v', "Verbose generation of all intermediate files/information");

	//Variable Flags
	cli.add<std::string>("output", 'o', "Output filename for the generated assembly.", false, "file.s");
	cli.add<std::string>("input", 'i', "Specify the input source file", true, "");

	//Run CLI parser
	cli.parse_check(argc, argv);
	
	//Initialize
	error_flag = 0;

	//Check if input file exists
	FILE *fh;
	if ((fh = fopen(cli.get<std::string>("input").c_str(), "r"))){
		yyin = fh;
	}
	else{
	 	std::cout << "Input file does not exist!";
	 	exit(0);
	}

	sym_ss << "Scope,Function Name, Symbol Name, Symbol Type, Symbol Level\n";
	std::stringstream ss;
	dot_ss << "digraph G {\n";
	dot_ss << "\tordering=out\n";

	root = create_non_term("translation_unit");

	setup_primitive_types();

	int abc = yyparse();
	root->dotify();
	
	assert(abc == 0);

	dot_ss << "}\n";

	//By this point, all output stringstreams are populated
	if(error_flag == 1){
		std::cout << "Compilation Failed\n";
		//Purge any files generated till now : No files have been output
		exit(0);
	}
	else{
		//Setup Output files as ofs
		//Dot file
		std::string file_name = cli.get<std::string>("output");
		std::string file_basename = file_name.substr(0, file_name.find_last_of("."));
		if(cli.exist("dot-cfg") || cli.exist("all")){
			std::ofstream dot_ofs;
			const std::string dot_fname = file_basename + ".dot";
			dot_ofs.open(dot_fname);
			dot_ofs << dot_ss.rdbuf();
			dot_ofs.close();
		}
		//Symbol Table file
		if(cli.exist("symtab") || cli.exist("all")){
			std::ofstream sym_ofs;
			const std::string sym_fname = file_basename + "_symtab.csv";
			sym_ofs.open(sym_fname);
			sym_ofs << sym_ss.rdbuf();
			sym_ofs.close();
		}
		//Three Address Code file
		if(cli.exist("tac") || cli.exist("all")){
			std::ofstream tac_ofs;
			const std::string tac_fname = file_basename + ".tac";
			tac_ofs.open(tac_fname);
			tac_ofs << tac_ss.rdbuf();
			tac_ofs.close();
		}
		
		//Generating ASM is mandatory
		std::ofstream asm_ofs;
		const std::string asm_fname = cli.get<std::string>("output");
		asm_ofs.open(asm_fname);
		asm_ofs << asm_ss.rdbuf();
		asm_ofs.close();
		//Additionally process the asm outfile for dead code pruning
		std::stringstream ssr1;
		ssr1 << "sed -i 's/^ASM:\ //' " << asm_fname;
		system(ssr1.str().c_str());
		std::stringstream ssr2;
		ssr2 << "sed -i '/^xxxx/d' " << asm_fname;
		system(ssr2.str().c_str());
	}

  return 0;
}

void file_writer(std::string s){
	dot_ss << s;
}
