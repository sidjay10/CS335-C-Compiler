
%{
	#include <stdio.h>
	#include <ast.h>
	void yyerror(const char *s);	
	extern "C" int yylex();
	

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
	: IDENTIFIER	{ $$ = create_non_term("primary_expression", $1); } 
	| CONSTANT	{ $$ = create_non_term("primary_expression", $1); } 
	| STRING_LITERAL	{ $$ = create_non_term("primary_expression", $1); } 
	| '(' expression ')'	{ $$ = create_non_term("primary_expression", $1, $2, $3); } 
	;

postfix_expression
	: primary_expression	{ $$ = create_non_term("postfix_expression", $1); } 
	| postfix_expression '[' expression ']'	{ $$ = create_non_term("postfix_expression", $1, $2, $3, $4); } 
	| postfix_expression '(' ')'	{ $$ = create_non_term("postfix_expression", $1, $2, $3); } 
	| postfix_expression '(' argument_expression_list ')'	{ $$ = create_non_term("postfix_expression", $1, $2, $3, $4); } 
	| postfix_expression '.' IDENTIFIER	{ $$ = create_non_term("postfix_expression", $1, $2, $3); } 
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = create_non_term("postfix_expression", $1, $2, $3); } 
	| postfix_expression INC_OP	{ $$ = create_non_term("postfix_expression", $1, $2); } 
	| postfix_expression DEC_OP	{ $$ = create_non_term("postfix_expression", $1, $2); } 
	;

argument_expression_list
	: assignment_expression	{ $$ = create_non_term("assignment_expression_list", $1); } 
	| argument_expression_list ',' assignment_expression	{ $$ = create_non_term("assignment_expression_list", $1, $2, $3); } 
	;

unary_expression
	: postfix_expression	{ $$ = create_non_term("unary_expression", $1); } 
	| INC_OP unary_expression	{ $$ = create_non_term("unary_expression", $1, $2); } 
	| DEC_OP unary_expression	{ $$ = create_non_term("unary_expression", $1, $2); } 
	| unary_operator cast_expression	{ $$ = create_non_term("unary_expression", $1, $2); } 
	| SIZEOF unary_expression	{ $$ = create_non_term("unary_expression", $1, $2); } 
	| SIZEOF '(' type_name ')'	{ $$ = create_non_term("unary_expression", $1, $2, $3, $4); } 
	;

unary_operator
	: '&'	{ $$ = create_non_term("unary_operator", $1); }
	| '*'	{ $$ = create_non_term("unary_operator", $1); }
	| '+'	{ $$ = create_non_term("unary_operator", $1); }
	| '-'	{ $$ = create_non_term("unary_operator", $1); }
	| '~'	{ $$ = create_non_term("unary_operator", $1); }
	| '!'	{ $$ = create_non_term("unary_operator", $1); }
	;

cast_expression
	: unary_expression	{ $$ = create_non_term("cast_expression", $1); }
	| '(' type_name ')' cast_expression	{ $$ = create_non_term("cast_expression", $1, $2, $3); }
	;

multiplicative_expression
	: cast_expression	{ $$ = create_non_term("multiplicative_expression", $1); }
	| multiplicative_expression '*' cast_expression	{ $$ = create_non_term("multiplicative_expression", $1, $2, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = create_non_term("multiplicative_expression", $1, $2, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = create_non_term("multiplicative_expression", $1, $2, $3); }
	;

additive_expression
	: multiplicative_expression	{ $$ = create_non_term("additive_expression", $1); }
	| additive_expression '+' multiplicative_expression	{ $$ = create_non_term("additive_expression", $1, $2, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = create_non_term("additive_expression", $1, $2, $3); }
	;

shift_expression
	: additive_expression	{ $$ = create_non_term("shift_expression", $1); }
	| shift_expression LEFT_OP additive_expression	{ $$ = create_non_term("shift_expression", $1, $2, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = create_non_term("shift_expression", $1, $2, $3); }
	;

relational_expression
	: shift_expression	{ $$ = create_non_term("relational_expression", $1); }
	| relational_expression '<' shift_expression	{ $$ = create_non_term("relational_expression", $1, $2, $3); }
	| relational_expression '>' shift_expression	{ $$ = create_non_term("relational_expression", $1, $2, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = create_non_term("relational_expression", $1, $2, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = create_non_term("relational_expression", $1, $2, $3); }
	;

equality_expression
	: relational_expression	{ $$ = create_non_term("equality_expression", $1); }
	| equality_expression EQ_OP relational_expression	{ $$ = create_non_term("equality_expression", $1, $2, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = create_non_term("equality_expression", $1, $2, $3); }
	;

and_expression
	: equality_expression	{ $$ = create_non_term("and_expression", $1); }
	| and_expression '&' equality_expression	{ $$ = create_non_term("and_expression", $1, $2, $3); }
	;

exclusive_or_expression
	: and_expression	{ $$ = create_non_term("exclusive_or_expression", $1); }
	| exclusive_or_expression '^' and_expression	{ $$ = create_non_term("exclusive_or_expression", $1, $2, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression	{ $$ = create_non_term("inclusive_or_expression", $1); }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = create_non_term("inclusive_or_expression", $1, $2, $3); }
	;

logical_and_expression
	: inclusive_or_expression	{ $$ = create_non_term("logical_and_expression", $1); }
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = create_non_term("logical_and_expression", $1, $2, $3); }
	;

logical_or_expression
	: logical_and_expression	{ $$ = create_non_term("logical_or_expression", $1); }
	| logical_or_expression OR_OP logical_and_expression	{ $$ = create_non_term("logical_or_expression", $1, $2, $3); }
	;

conditional_expression
	: logical_or_expression	 { $$ = create_non_term("conditional_expression", $1); }
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = create_non_term("conditional_expression", $1, $2, $3, $4, $5); }
	;

assignment_expression
	: conditional_expression					 { $$ = create_non_term("assignment_expression", $1); }
	| unary_expression assignment_operator assignment_expression	 { $$ = create_non_term("assignment_expression", $1, $2, $3); }
	;

assignment_operator
	: '='		 { $$ = create_non_term("assignment_operator", $1); }
	| MUL_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| DIV_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| MOD_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| ADD_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| SUB_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| LEFT_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| RIGHT_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| AND_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| XOR_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	| OR_ASSIGN	 { $$ = create_non_term("assignment_operator", $1); }
	;

expression
	: assignment_expression	 { $$ = create_non_term("expression", $1); }
	| expression ',' assignment_expression	 { $$ = create_non_term("expression", $1, $2, $3); }
	;

constant_expression
	: conditional_expression	 { $$ = create_non_term("constant_expression", $1); }
	;

declaration
	: declaration_specifiers ';'	 { $$ = create_non_term("declaration", $1, $2); }
	| declaration_specifiers init_declarator_list ';' { $$ = create_non_term("declaration", $1, $2, $3); }
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
	: init_declarator	 { $$ = create_non_term("init_declarator_list", $1); }
	| init_declarator_list ',' init_declarator	 { $$ = create_non_term("init_declarator_list", $1, $2, $3); }
	;

init_declarator
	: declarator	 { $$ = create_non_term("init_declarator", $1); }
	| declarator '=' initializer	 { $$ = create_non_term("init_declarator", $1, $2, $3); }
	;

storage_class_specifier
	:  TYPEDEF	 { $$ = create_non_term("storage_class_specifier", $1); }
	|  EXTERN	 { $$ = create_non_term("storage_class_specifier", $1); }
	|  STATIC	 { $$ = create_non_term("storage_class_specifier", $1); }
	|  AUTO	 { $$ = create_non_term("storage_class_specifier", $1); }
	|  REGISTER	 { $$ = create_non_term("storage_class_specifier", $1); }
	;

type_specifier
	:  VOID	 { $$ = create_non_term("type_specifier", $1); }
	|  CHAR	 { $$ = create_non_term("type_specifier", $1); }
	|  SHORT	 { $$ = create_non_term("type_specifier", $1); }
	|  INT	 { $$ = create_non_term("type_specifier", $1); }
	|  LONG	 { $$ = create_non_term("type_specifier", $1); }
	|  FLOAT	 { $$ = create_non_term("type_specifier", $1); }
	|  DOUBLE	 { $$ = create_non_term("type_specifier", $1); }
	|  SIGNED	 { $$ = create_non_term("type_specifier", $1); }
	|  UNSIGNED	 { $$ = create_non_term("type_specifier", $1); }
	|  struct_or_union_specifier	 { $$ = create_non_term("type_specifier", $1); }
	|  enum_specifier	 { $$ = create_non_term("type_specifier", $1); }
	|  TYPE_NAME	 { $$ = create_non_term("type_specifier", $1); }
	;

struct_or_union_specifier
	:  struct_or_union IDENTIFIER '{' struct_declaration_list '}'	 { $$ = create_non_term("struct_or_union_specifier", $1, $2, $3, $4, $5); }
	|  struct_or_union '{' struct_declaration_list '}'	 { $$ = create_non_term("struct_or_union_specifier", $1, $2, $3, $4); }
	|  struct_or_union IDENTIFIER	 { $$ = create_non_term("struct_or_union_specifier", $1, $2); }
	;

struct_or_union
	:  STRUCT	 { $$ = create_non_term("struct_or_union", $1); }
	|  UNION	 { $$ = create_non_term("struct_or_union", $1); }
	;

struct_declaration_list
	:  struct_declaration	 { $$ = create_non_term("struct_declaration_list", $1); }
	|  struct_declaration_list struct_declaration	 { $$ = create_non_term("struct_declaration_list", $1, $2); }
	;

struct_declaration
	:  specifier_qualifier_list struct_declarator_list ';'	 { $$ = create_non_term("struct_declaration", $1, $2, $3); }
	;

specifier_qualifier_list
	:  type_specifier specifier_qualifier_list	 { $$ = create_non_term("specifier_qualifier_list", $1, $2); }
	|  type_specifier	 { $$ = create_non_term("specifier_qualifier_list", $1); }
	|  type_qualifier specifier_qualifier_list	 { $$ = create_non_term("specifier_qualifier_list", $1, $2); }
	|  type_qualifier	 { $$ = create_non_term("specifier_qualifier_list", $1); }
	;

struct_declarator_list
	:  struct_declarator	 { $$ = create_non_term("struct_declarator_list", $1); }
	|  struct_declarator_list ',' struct_declarator	 { $$ = create_non_term("struct_declarator_list", $1, $2, $3); }
	;

struct_declarator
	:  declarator	 { $$ = create_non_term("struct_declarator", $1); }
	|  ':' constant_expression	 { $$ = create_non_term("struct_declarator", $1, $2); }
	|  declarator ':' constant_expression	 { $$ = create_non_term("struct_declarator", $1, $2, $3); }
	;

enum_specifier
	:  ENUM '{' enumerator_list '}'	 { $$ = create_non_term("enum_specifier", $1, $2, $3, $4); }
	|  ENUM IDENTIFIER '{' enumerator_list '}'	 { $$ = create_non_term("enum_specifier", $1, $2, $3, $4, $5); }
	|  ENUM IDENTIFIER	 { $$ = create_non_term("enum_specifier", $1, $2); }
	;

enumerator_list
	:  enumerator	 { $$ = create_non_term("enumerator_list", $1); }
	|  enumerator_list ',' enumerator	 { $$ = create_non_term("enumerator_list", $1, $2, $3); }
	;

enumerator
	:  IDENTIFIER	 { $$ = create_non_term("enumerator", $1); }
	|  IDENTIFIER '=' constant_expression	 { $$ = create_non_term("enumerator", $1, $2, $3); }
	;

type_qualifier
	:  CONST	 { $$ = create_non_term("type_qualifier", $1); }
	|  VOLATILE	 { $$ = create_non_term("type_qualifier", $1); }
	;

declarator
	:  pointer direct_declarator	 { $$ = create_non_term("declarator", $1, $2); }
	|  direct_declarator	 { $$ = create_non_term("declarator", $1); }
	;

direct_declarator
	:  IDENTIFIER	 { $$ = create_non_term("direct_declarator", $1); }
	|  '(' declarator ')'	 { $$ = create_non_term("direct_declarator", $1, $2, $3); }
	|  direct_declarator '[' constant_expression ']'	 { $$ = create_non_term("direct_declarator", $1, $2, $3, $4); }
	|  direct_declarator '[' ']'	 { $$ = create_non_term("direct_declarator", $1, $2, $3); }
	|  direct_declarator '(' parameter_type_list ')'	 { $$ = create_non_term("direct_declarator", $1, $2, $3, $4); }
	|  direct_declarator '(' identifier_list ')'	 { $$ = create_non_term("direct_declarator", $1, $2, $3, $4); }
	|  direct_declarator '(' ')'	 { $$ = create_non_term("direct_declarator", $1, $2, $3); }
	;

pointer
	:  '*'	 { $$ = create_non_term("pointer", $1); }
	|  '*' type_qualifier_list	 { $$ = create_non_term("pointer", $1, $2); }
	|  '*' pointer	 { $$ = create_non_term("pointer", $1, $2); }
	|  '*' type_qualifier_list pointer	 { $$ = create_non_term("pointer", $1, $2, $3); }
	;

type_qualifier_list
	:  type_qualifier	 { $$ = create_non_term("type_qualifier_list", $1); }
	|  type_qualifier_list type_qualifier	 { $$ = create_non_term("type_qualifier_list", $1, $2); }
	;

parameter_type_list
	:  parameter_list	 { $$ = create_non_term("parameter_type_list", $1); }
	|  parameter_list ',' ELLIPSIS	 { $$ = create_non_term("parameter_type_list", $1, $2, $3); }
	;

parameter_list
	:  parameter_declaration	 { $$ = create_non_term("parameter_list", $1); }
	|  parameter_list ',' parameter_declaration	 { $$ = create_non_term("parameter_list", $1, $2, $3); }
	;

parameter_declaration
	:  declaration_specifiers declarator	 { $$ = create_non_term("parameter_declaration", $1, $2); }
	|  declaration_specifiers abstract_declarator	 { $$ = create_non_term("parameter_declaration", $1, $2); }
	|  declaration_specifiers	 { $$ = create_non_term("parameter_declaration", $1); }
	;

identifier_list
	:  IDENTIFIER	 { $$ = create_non_term("identifier_list", $1); }
	|  identifier_list ',' IDENTIFIER	 { $$ = create_non_term("identifier_list", $1, $2, $3); }
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
	:  assignment_expression	 { $$ = create_non_term("initializer", $1); }
	|  '{' initializer_list '}'	 { $$ = create_non_term("initializer", $1, $2, $3); }
	|  '{' initializer_list ',' '}'	 { $$ = create_non_term("initializer", $1, $2, $3, $4); }
	;

initializer_list
	:  initializer	 { $$ = create_non_term("initializer_list", $1); }
	|  initializer_list ',' initializer	 { $$ = create_non_term("initializer_list", $1, $2, $3); }
	;

statement
	:  labeled_statement	 { $$ = create_non_term("statement", $1); }
	|  compound_statement	 { $$ = create_non_term("statement", $1); }
	|  expression_statement	 { $$ = create_non_term("statement", $1); }
	|  selection_statement	 { $$ = create_non_term("statement", $1); }
	|  iteration_statement	 { $$ = create_non_term("statement", $1); }
	|  jump_statement	 { $$ = create_non_term("statement", $1); }
	;

labeled_statement
	:  IDENTIFIER ':' statement	 { $$ = create_non_term("labeled_statement", $1, $2); }
	|  CASE constant_expression ':' statement	 { $$ = create_non_term("labeled_statement", $1, $2, $3); }
	|  DEFAULT ':' statement	 { $$ = create_non_term("labeled_statement", $1, $2); }
	;

compound_statement
	:  '{' '}'	 { $$ = create_non_term("compound_statement", $1, $2); }
	|  '{' statement_list '}'	 { $$ = create_non_term("compound_statement", $1, $2, $3); }
	|  '{' declaration_list '}'	 { $$ = create_non_term("compound_statement", $1, $2, $3); }
	|  '{' declaration_list statement_list '}'	 { $$ = create_non_term("compound_statement", $1, $2, $3, $4); }
	;

declaration_list
	:  declaration	 { $$ = create_non_term("declaration_list", $1); }
	|  declaration_list declaration	 { $$ = create_non_term("declaration_list", $1, $2); }
	;

statement_list
	:  statement	 { $$ = create_non_term("statement_list", $1); }
	|  statement_list statement	 { $$ = create_non_term("statement_list", $1, $2); }
	;

expression_statement
	:  ';'	 { $$ = create_non_term("expression_statement", $1); }
	|  expression ';'	 { $$ = create_non_term("expression_statement", $1, $2); }
	;

selection_statement
	:  IF '(' expression ')' statement	 { $$ = create_non_term("selection_statement", $1, $2, $3, $4, $5); }
	|  IF '(' expression ')' statement ELSE statement	 { $$ = create_non_term("selection_statement", $1, $2, $3, $4, $5, $6, $7); }
	|  SWITCH '(' expression ')' statement	 { $$ = create_non_term("selection_statement", $1, $2, $3, $4, $5); }
	;

iteration_statement
	:  WHILE '(' expression ')' statement	 { $$ = create_non_term("iteration_statement", $1, $2, $3, $4, $5); }
	|  DO statement WHILE '(' expression ')' ';'	 { $$ = create_non_term("iteration_statement", $1, $2, $3, $4, $5, $6, $7); }
	|  FOR '(' expression_statement expression_statement ')' statement	 { $$ = create_non_term("iteration_statement", $1, $2, $3, $4, $5, $6); }
	|  FOR '(' expression_statement expression_statement expression ')' statement	 { $$ = create_non_term("iteration_statement", $1, $2, $3, $4, $5, $6, $7); }
	;

jump_statement
	:  GOTO IDENTIFIER ';'	 { $$ = create_non_term("jump_statement", $1, $2, $3); }
	|  CONTINUE ';'	 { $$ = create_non_term("jump_statement", $1, $2); }
	|  BREAK ';'	 { $$ = create_non_term("jump_statement", $1, $2); }
	|  RETURN ';'	 { $$ = create_non_term("jump_statement", $1, $2); }
	|  RETURN expression ';'	 { $$ = create_non_term("jump_statement", $1, $2, $3); }
	;

translation_unit
	:  external_declaration	 { $$ = create_non_term("translation_unit", $1); }
	|  translation_unit external_declaration	 { $$ = create_non_term("translation_unit", $1, $2); }
	;

external_declaration
	:  function_definition	 { $$ = create_non_term("external_declaration", $1); }
	|  declaration	 { $$ = create_non_term("external_declaration", $1); }
	;

function_definition
	:  declaration_specifiers declarator declaration_list compound_statement	 { $$ = create_non_term("function_definition", $1, $2, $3, $4); }
	|  declaration_specifiers declarator compound_statement	 { $$ = create_non_term("function_definition", $1, $2, $3); }
	|  declarator declaration_list compound_statement	 { $$ = create_non_term("function_definition", $1, $2, $3); }
	|  declarator compound_statement	 { $$ = create_non_term("function_definition", $1, $2); }
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
