
%{
	#include <stdio.h>
	#include <ast.h>
	void yyerror(const char *s);	
	extern "C" int yylex();
	extern Node * root; 
	

%}

%union{
	Node * node;
}



%token <node> '(' ')' '[' ']' '.' ',' '+' '-' '!' '&' '*' '~' '/' '%'
%token <node> '<' '>' '^' '|' ':' '?' '=' ';' '{' '}'

%token <node> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token <node> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <node> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <node> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <node> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token <node> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <node> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <node> STRUCT UNION ENUM ELLIPSIS

%token <node> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN


%type <node> primary_expression postfix_expression expression argument_expression_list unary_expression cast_expression
%type <node> conditional_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_or_expression logical_and_expression
%type <node> assignment_expression constant_expression
%type <node> unary_operator assignment_operator
%type <node> declaration declaration_specifiers init_declarator_list  init_declarator 
%type <node> storage_class_specifier type_specifier 

%type <node> struct_or_union_specifier struct_or_union struct_declaration_list struct_declaration specifier_qualifier_list struct_declarator_list struct_declarator

%type <node> enum_specifier enumerator_list enumerator type_qualifier declarator direct_declarator pointer type_qualifier_list parameter_type_list parameter_list parameter_declaration identifier_list type_name abstract_declarator direct_abstract_declarator initializer initializer_list

%type <node> statement labeled_statement compound_statement declaration_list statement_list expression_statement selection_statement iteration_statement jump_statement

%type <node> translation_unit external_declaration function_definition 

%start translation_unit
%%

primary_expression
	: IDENTIFIER	{ $$ = create_terminal("IDENTIFIER", NULL); } 
	| CONSTANT	{ $$ = create_terminal("CONSTANT", NULL); } 
	| STRING_LITERAL	{ $$ = create_terminal("STRING_LITERAL", NULL); } 
	| '(' expression ')'	{ $$ = $2; } 
	;

postfix_expression
	: primary_expression	{ $$ = $1; } 
	| postfix_expression '[' expression ']'	{ $$ = create_non_term("[]", $1, $3); } 
	| postfix_expression '(' ')'	{ $$ = create_non_term("FUNCTION_CALL", $1); } 
	| postfix_expression '(' argument_expression_list ')'	{ $$ = create_non_term("FUNCTION_CALL_ARGS", $1, $3); } 
	| postfix_expression '.' IDENTIFIER	{ $$ = create_non_term(".", $1, create_terminal("IDENTIFIER",NULL)); } 
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = create_non_term("->", $1, create_terminal("IDENTIFIER",NULL)); } 
	| postfix_expression INC_OP	{ $$ = create_non_term("POST INCREMENT", $1); } 
	| postfix_expression DEC_OP	{ $$ = create_non_term("POST DECREMENT", $1); } 
	;

argument_expression_list
	: assignment_expression	{ $$ = create_non_term("assignment_expression", $1); } 
	| argument_expression_list ',' assignment_expression	{ $$ = create_non_term("assignment_expression_list", $1, $3); } 
	;

unary_expression
	: postfix_expression	{ $$ = $1; } 
	| INC_OP unary_expression	{ $$ = create_non_term("PRE INCREMENT", $2); } 
	| DEC_OP unary_expression	{ $$ = create_non_term("PRE DECREMENT", $2); } 
	| unary_operator cast_expression	{ $1->add_child($2); $$ = $1; } 
	| SIZEOF unary_expression	{ $$ = create_non_term("SIZEOF unary_expr", $2); } 
	| SIZEOF '(' type_name ')'	{ $$ = create_non_term("SIZEOF type_name", $3); } 
	;

unary_operator
	: '&'	{ $$ = create_non_term("UNARY &"); }
	| '*'	{ $$ = create_non_term("UNARY *"); }
	| '+'	{ $$ = create_non_term("UNARY +"); }
	| '-'	{ $$ = create_non_term("UNARY -"); }
	| '~'	{ $$ = create_non_term("UNARY ~"); }
	| '!'	{ $$ = create_non_term("UNARY !"); }
	;

cast_expression
	: unary_expression	{ $$ = $1; }
	| '(' type_name ')' cast_expression	{ $$ = create_non_term("cast_expression", $2, $4); }
	;

multiplicative_expression
	: cast_expression	{ $$ = $1; }
	| multiplicative_expression '*' cast_expression	{ $$ = create_non_term("*", $1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = create_non_term("/", $1, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = create_non_term("%", $1, $3); }
	;

additive_expression
	: multiplicative_expression	{ $$ = $1; }
	| additive_expression '+' multiplicative_expression	{ $$ = create_non_term("+", $1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = create_non_term("-", $1, $3); }
	;

shift_expression
	: additive_expression	{ $$ = $1; }
	| shift_expression LEFT_OP additive_expression	{ $$ = create_non_term(">>", $1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = create_non_term("<<", $1, $3); }
	;

relational_expression
	: shift_expression	{ $$ = $1; }
	| relational_expression '<' shift_expression	{ $$ = create_non_term("<", $1, $3); }
	| relational_expression '>' shift_expression	{ $$ = create_non_term(">", $1, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = create_non_term("<=", $1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = create_non_term(">=", $1, $3); }
	;

equality_expression
	: relational_expression	{ $$ = $1; }
	| equality_expression EQ_OP relational_expression	{ $$ = create_non_term("==", $1, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = create_non_term("!=", $1, $3); }
	;

and_expression
	: equality_expression	{ $$ = $1; }
	| and_expression '&' equality_expression	{ $$ = create_non_term("&", $1, $3); }
	;

exclusive_or_expression
	: and_expression	{ $$ = $1; }
	| exclusive_or_expression '^' and_expression	{ $$ = create_non_term("^", $1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression	{ $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = create_non_term("|", $1, $3); }
	;

logical_and_expression
	: inclusive_or_expression	{ $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = create_non_term("&&", $1, $3); }
	;

logical_or_expression
	: logical_and_expression	{ $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression	{ $$ = create_non_term("||", $1, $3); }
	;

conditional_expression
	: logical_or_expression	 { $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = create_non_term("?:", $1, $3, $5); }
	;

assignment_expression
	: conditional_expression					 { $$ = $1; }
	| unary_expression assignment_operator assignment_expression	 { $2->add_children($1,$3); $$ = $2;}
	;

assignment_operator
	: '='		 { $$ = create_non_term("="); }
	| MUL_ASSIGN	 { $$ = create_non_term("*="); }
	| DIV_ASSIGN	 { $$ = create_non_term("/="); }
	| MOD_ASSIGN	 { $$ = create_non_term("%="); }
	| ADD_ASSIGN	 { $$ = create_non_term("+="); }
	| SUB_ASSIGN	 { $$ = create_non_term("-="); }
	| LEFT_ASSIGN	 { $$ = create_non_term("<<="); }
	| RIGHT_ASSIGN	 { $$ = create_non_term(">>="); }
	| AND_ASSIGN	 { $$ = create_non_term("&="); }
	| XOR_ASSIGN	 { $$ = create_non_term("^="); }
	| OR_ASSIGN	 { $$ = create_non_term("|="); }
	;

expression
	: assignment_expression	 { $$ = create_non_term("expression", $1); }
	| expression ',' assignment_expression	 { $$ = create_non_term("expression", $1, $3); }
	;

constant_expression
	: conditional_expression	 { $$ = $1; }
	;

declaration
	: declaration_specifiers ';'	 { $$ = NULL; }
	| declaration_specifiers init_declarator_list ';' { $$ = create_non_term("initialized values", $2); }
	;

declaration_specifiers
	: storage_class_specifier	 { $$ = create_non_term("declaration_specifiers", $1); }
	| storage_class_specifier declaration_specifiers	 { $$ = create_non_term("declaration_specifiers", $1, $2); }
	| type_specifier	 { $$ = create_non_term("declaration_specifiers", $1); }
	| type_specifier declaration_specifiers	 { $$ = create_non_term("declaration_specifiers", $1, $2); }
	| type_qualifier	 { $$ = create_non_term("declaration_specifiers", $1); }
	| type_qualifier declaration_specifiers	 { $$ = create_non_term("declaration_specifiers", $1, $2); }
	;

init_declarator_list
	: init_declarator	 { $$ = $1; }
	| init_declarator_list ',' init_declarator	 { if( $1 == NULL ) $$ = $3; else if ( $3 == NULL ) $$ = $1; else $$ = create_non_term("init_declarator_list", $1, $3); }
	;

init_declarator
	: declarator	 { $$ = NULL; }
	| declarator '=' initializer	 { $$ = create_non_term("=", $1, $3); }
	;

storage_class_specifier
	:  TYPEDEF	 { $$ = create_terminal("TYPEDEF", NULL ); }
	|  EXTERN	 { $$ = create_terminal("EXTERN", NULL); }
	|  STATIC	 { $$ = create_terminal("STATIC", NULL); }
	|  AUTO	 { $$ = create_terminal("AUTO", NULL); }
	|  REGISTER	 { $$ = create_terminal("REGISTER", NULL); }
	;

type_specifier
	:  VOID	 	{ $$ = create_terminal("VOID", NULL); }
	|  CHAR	 	{ $$ = create_terminal("CHAR", NULL); }
	|  SHORT	{ $$ = create_terminal("SHORT", NULL); }
	|  INT	 	{ $$ = create_terminal("INT", NULL); }
	|  LONG	 	{ $$ = create_terminal("LONG", NULL); }
	|  FLOAT	{ $$ = create_terminal("FLOAT", NULL); }
	|  DOUBLE	{ $$ = create_terminal("DOUBLE", NULL); }
	|  SIGNED	{ $$ = create_terminal("SIGNED", NULL); }
	|  UNSIGNED	{ $$ = create_terminal("UNSIGNED", NULL); }
	|  struct_or_union_specifier	 { $$ = $1; }
	|  enum_specifier		{ $$ = $1; }
	|  TYPE_NAME	{ $$ = create_terminal("TYPE_NAME", NULL); }
	;

struct_or_union_specifier
	:  struct_or_union IDENTIFIER '{' struct_declaration_list '}'	 { $1->add_children(create_terminal("IDENTIFIER",NULL), $4); $$ = $1; }
	|  struct_or_union '{' struct_declaration_list '}'	 { $1->add_child($3); $$ = $1; }
	|  struct_or_union IDENTIFIER	 { $1->add_child(create_terminal("IDENTIFIER",NULL)); $$ = $1; }
	;

struct_or_union
	:  STRUCT	 { $$ = create_non_term("STRUCT"); }
	|  UNION	 { $$ = create_non_term("UNION"); }
	;

struct_declaration_list
	:  struct_declaration	 { $$ = create_non_term("struct_declaration", $1); }
	|  struct_declaration_list struct_declaration	 { $$ = create_non_term("struct_declaration_list", $1, $2); }
	;

struct_declaration
	:  specifier_qualifier_list struct_declarator_list ';'	 { $$ = create_non_term("struct_declaration", $1, $2); }
	;

specifier_qualifier_list
	:  type_specifier specifier_qualifier_list	 { $$ = create_non_term("specifier_qualifier_list", $1, $2); }
	|  type_specifier	 { $$ = create_non_term("type_specifier", $1); }
	|  type_qualifier specifier_qualifier_list	 { $$ = create_non_term("specifier_qualifier_list", $1, $2); }
	|  type_qualifier	 { $$ = create_non_term("type_qualifier", $1); }
	;

struct_declarator_list
	:  struct_declarator	 { $$ = create_non_term("struct_declarator", $1); }
	|  struct_declarator_list ',' struct_declarator	 { $$ = create_non_term("struct_declarator_list", $1, $3); }
	;

struct_declarator
	:  declarator	 { $$ = $1; }
	|  ':' constant_expression	 { $$ = create_non_term(":", $2); }
	|  declarator ':' constant_expression	 { $$ = create_non_term(":", $1, $3); }
	;

enum_specifier
	:  ENUM '{' enumerator_list '}'	 { $$ = create_non_term("ENUM", $3); }
	|  ENUM IDENTIFIER '{' enumerator_list '}'	 { $$ = create_non_term("ENUM",create_terminal("IDENTIFIER",NULL),  $4); }
	|  ENUM IDENTIFIER	 { $$ = create_non_term("ENUM", create_terminal("IDENTIFIER",NULL)); }
	;

enumerator_list
	:  enumerator	 { $$ = create_non_term("enumerator", $1); }
	|  enumerator_list ',' enumerator	 { $$ = create_non_term("enumerator_list", $1, $3); }
	;

enumerator
	:  IDENTIFIER	 { $$ = create_terminal("IDENTIFIER",NULL); }
	|  IDENTIFIER '=' constant_expression	 { $$ = create_non_term("=",  create_terminal("IDENTIFIER",NULL), $3); }
	;

type_qualifier
	:  CONST	 { $$ = create_terminal("CONST", NULL); }
	|  VOLATILE	 { $$ = create_terminal("VOLATILE", NULL); }
	;

declarator
	:  pointer direct_declarator	 { $$ = create_non_term("declarator", $1, $2); }
	|  direct_declarator	 { $$ = $1; }
	;

direct_declarator
	:  IDENTIFIER	 { $$ = create_terminal("IDENTIFIER", NULL); }
	|  '(' declarator ')'	 { $$ = create_non_term("direct_declarator", $2 ); }
	|  direct_declarator '[' constant_expression ']'	 { $$ = create_non_term("direct_declarator", $1, $3); }
	|  direct_declarator '[' ']'	 { $$ = create_non_term("direct_declarator", $1 ); }
	|  direct_declarator '(' parameter_type_list ')'	 { $$ = create_non_term("direct_declarator", $1, $3); }
	|  direct_declarator '(' identifier_list ')'	 { $$ = create_non_term("direct_declarator", $1, $3); }
	|  direct_declarator '(' ')'	 { $$ = create_non_term("direct_declarator_blah", $1); }
	;

pointer
	:  '*'	 { $$ = create_non_term("pointer"); }
	|  '*' type_qualifier_list	 { $$ = create_non_term("pointer", $2); }
	|  '*' pointer	 { $$ = create_non_term("pointer", $2); }
	|  '*' type_qualifier_list pointer	 { $$ = create_non_term("pointer", $2, $3); }
	;

type_qualifier_list
	:  type_qualifier	 { $$ = create_non_term("type_qualifier", $1); }
	|  type_qualifier_list type_qualifier	 { $$ = create_non_term("type_qualifier_list", $1, $2); }
	;

parameter_type_list
	:  parameter_list	 { $$ = create_non_term("parameter_type", $1); }
	|  parameter_list ',' ELLIPSIS	 { $$ = create_non_term("parameter_type_list", $1, $2, $3); }
	;

parameter_list
	:  parameter_declaration	 { $$ = create_non_term("parameter_declaration", $1); }
	|  parameter_list ',' parameter_declaration	 { $$ = create_non_term("parameter_list", $1, $2, $3); }
	;

parameter_declaration
	:  declaration_specifiers declarator	 { $$ = create_non_term("parameter_declaration", $1, $2); }
	|  declaration_specifiers abstract_declarator	 { $$ = create_non_term("parameter_declaration", $1, $2); }
	|  declaration_specifiers	 { $$ = create_non_term("parameter_declaration", $1); }
	;

identifier_list
	:  IDENTIFIER	 { $$ = create_terminal("IDENTIFIER",NULL); }
	|  identifier_list ',' IDENTIFIER	 { $$ = create_non_term("identifier_list", $1, create_terminal("IDENTIFIER",NULL)); }
	;

type_name
	:  specifier_qualifier_list	 { $$ = create_non_term("type_name", $1); }
	|  specifier_qualifier_list abstract_declarator	 { $$ = create_non_term("type_name", $1, $2); }
	;

abstract_declarator
	:  pointer	 { $$ = create_non_term("abstract_declarator", $1); }
	|  direct_abstract_declarator	 { $$ = create_non_term("abstract_declarator", $1); }
	|  pointer direct_abstract_declarator	 { $$ = create_non_term("abstract_declarator", $1, $2); }
	;

direct_abstract_declarator
	:  '(' abstract_declarator ')'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3); }
	|  '[' ']'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2); }
	|  '[' constant_expression ']'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3); }
	|  direct_abstract_declarator '[' ']'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3); }
	|  direct_abstract_declarator '[' constant_expression ']'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3, $4); }
	|  '(' ')'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2); }
	|  '(' parameter_type_list ')'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3); }
	|  direct_abstract_declarator '(' ')'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3); }
	|  direct_abstract_declarator '(' parameter_type_list ')'	 { $$ = create_non_term("direct_abstract_declarator", $1, $2, $3, $4); }
	;

initializer
	:  assignment_expression	 { $$ = $1; }
	|  '{' initializer_list '}'	 { $$ = create_non_term("initializer_list", $2); }
	|  '{' initializer_list ',' '}'	 { $$ = create_non_term("initializer_list", $2); }
	;

initializer_list
	:  initializer	 { $$ = create_non_term("initializer", $1); }
	|  initializer_list ',' initializer	 { $$ = create_non_term("initializer_list", $1, $3); }
	;

statement
	:  labeled_statement	 { $$ = $1; }
	|  compound_statement	 { $$ = $1; }
	|  expression_statement	 { $$ = create_non_term("expression_statement", $1); }
	|  selection_statement	 { $$ = create_non_term("selection_statement", $1); }
	|  iteration_statement	 { $$ = create_non_term("iteration_statement", $1); }
	|  jump_statement	 { $$ = create_non_term("jump_statement", $1); }
	;

labeled_statement
	:  IDENTIFIER ':' statement	 { $$ = create_non_term("labeled_statement", create_terminal("IDENTIFIER",NULL), $3); }
	|  CASE constant_expression ':' statement	 { $$ = create_non_term("CASE", $2, $4); }
	|  DEFAULT ':' statement	 { $$ = create_non_term("DEFAULT", $2); }
	;

compound_statement
	:  '{' '}'	 { $$ = NULL; }
	|  '{' statement_list '}'	 { $$ = create_non_term("compound_statement", $2); }
	|  '{' declaration_list '}'	 { $$ = create_non_term("compound_statement", $2); }
	|  '{' declaration_list statement_list '}'	 {  $$ = create_non_term("compound_statement", $2, $3); }
	;

declaration_list
	:  declaration	 { $$ = $1; }
	|  declaration_list declaration	 { $$ = create_non_term("declaration_list", $1, $2);}
	;

statement_list
	:  statement	 { $$ = $1; }
	|  statement_list statement	 { $$ = create_non_term("statement_list", $1, $2); }
	;

expression_statement
	:  ';'	 { $$ = create_terminal("EMPTY_STMT",NULL); }
	|  expression ';'	 { $$ = $1; }
	;

selection_statement
	:  IF '(' expression ')' statement	 { $$ = create_non_term("IF", $3, $5); }
	|  IF '(' expression ')' statement ELSE statement	 { $$ = create_non_term("IF ELSE", $3, $5, $7); }
	|  SWITCH '(' expression ')' statement	 { $$ = create_non_term("SWITCH", $3, $5); }
	;

iteration_statement
	:  WHILE '(' expression ')' statement	 { $$ = create_non_term("WHILE", $3, $5); }
	|  DO statement WHILE '(' expression ')' ';'	 { $$ = create_non_term("DO WHILE", $5, $2); }
	|  FOR '(' expression_statement expression_statement ')' statement	 { $$ = create_non_term("FOR", $3, $4, NULL, $6); }
	|  FOR '(' expression_statement expression_statement expression ')' statement	 { $$ = create_non_term("FOR", $3, $4, $5, $7); }
	;

jump_statement
	:  GOTO IDENTIFIER ';'	 { $$ = create_non_term("GOTO", create_terminal("IDENTIFIER",NULL)); }
	|  CONTINUE ';'	 { $$ = create_non_term("CONTINUE"); }
	|  BREAK ';'	 { $$ = create_non_term("BREAK"); }
	|  RETURN ';'	 { $$ = create_non_term("RETURN"); }
	|  RETURN expression ';'	 { $$ = create_non_term("RETURN", $2); }
	;

translation_unit
	:  external_declaration	 { root->add_child(create_non_term("external_declaration", $1)); }
	|  translation_unit external_declaration	 { root->add_child(create_non_term("translation_unit", $1, $2)); }
	;

external_declaration
	:  function_definition	 { $$ = $1; }
	|  declaration	 { $$ = $1; }
	;

function_definition
	:  declaration_specifiers declarator compound_statement	 { $$ = create_non_term("function_definition", $3); }
	|  declarator compound_statement	 { $$ = create_non_term("function_definition", $2); }
	;


%%
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(const char *s)
//char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
}
