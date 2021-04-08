
%{
	#include <stdio.h>
	#include <ast.h>
	#include <symtab.h>
	void yyerror(const char *s);	
	extern "C" int yylex();
	extern Node * root; 
	

%}

%union{
	Node * node;
	int value;
	DeclarationSpecifiers * declaration_specifiers;
	Declaration * declaration;
	DeclarationList * declaration_list;
	Declarator * declarator;
	DeclaratorList * init_declarator_list;
	Pointer * pointer;
	DirectDeclarator * direct_declarator;
	Identifier * identifier;
	TypeQualifierList * type_qualifier_list;
	FunctionDefinition * function_definition;
	TypeSpecifier * type_specifier;
	Enumerator * enumerator;
	EnumeratorList * enumerator_list;
	StructDeclarationList * struct_declaration_list;
	StructDeclaration * struct_declaration;
	SpecifierQualifierList * specifier_qualifier_list;
	ParameterTypeList * parameter_type_list;
	ParameterDeclaration * parameter_declaration;
	DirectAbstractDeclarator * direct_abstract_declarator;
	AbstractDeclarator * abstract_declarator;
}



%token <node> '(' ')' '[' ']' '.' ',' '+' '-' '!' '&' '*' '~' '/' '%'
%token <node> '<' '>' '^' '|' ':' '?' '=' ';' '{' '}'

%token <node> CONSTANT STRING_LITERAL SIZEOF
%token <identifier> IDENTIFIER
%token <node> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <node> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <node> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <node> XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token <value> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <value> SIGNED UNSIGNED CHAR SHORT LONG INT FLOAT DOUBLE VOID
%token <value> CONST VOLATILE
%token <value> STRUCT UNION ENUM ELLIPSIS

%token <node> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN


%type <node> primary_expression postfix_expression expression argument_expression_list unary_expression cast_expression
%type <node> conditional_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_or_expression logical_and_expression
%type <node> assignment_expression constant_expression
%type <node> unary_operator assignment_operator
%type <declaration> declaration 
%type <init_declarator_list> init_declarator_list  
%type <declarator> init_declarator declarator
%type <declaration_specifiers> declaration_specifiers 
%type <value> storage_class_specifier 
%type <type_specifier> type_specifier struct_or_union_specifier enum_specifier
%type <value> type_qualifier

%type <pointer> pointer

%type <direct_declarator> direct_declarator

%type <type_qualifier_list> type_qualifier_list

%type <declaration_list> declaration_list 

%type <function_definition> function_definition function_declaration

%type <enumerator> enumerator

%type <enumerator_list> enumerator_list

%type <struct_declaration_list> struct_declaration_list

%type <value> struct_or_union  

%type <struct_declaration> struct_declaration

%type<specifier_qualifier_list> specifier_qualifier_list 

%type <init_declarator_list> struct_declarator_list 

%type <declarator> struct_declarator

%type <node> type_name initializer initializer_list
%type <parameter_type_list> parameter_type_list parameter_list
%type <parameter_declaration> parameter_declaration 
%type <abstract_declarator> abstract_declarator
%type <direct_abstract_declarator>  direct_abstract_declarator 


/*%type <node> identifier_list */
%type <node> statement labeled_statement compound_statement statement_list expression_statement selection_statement iteration_statement jump_statement

%type <node> translation_unit external_declaration 

%start translation_unit
%%

primary_expression
	: IDENTIFIER		{ $$ = $1; } 
	| CONSTANT		{ $$ = $1; } 
	| STRING_LITERAL	{ $$ = create_terminal("STRING LITERAL",NULL); }
	| '(' expression ')'	{ $$ = create_non_term("primary_expression", $2 ); } 
	;

postfix_expression
	: primary_expression	{ $$ = $1; } 
	| postfix_expression '[' expression ']'	{ $$ = create_non_term("[]", $1, $3); }
/*	| postfix_expression IDENTIFER '(' ')'	{ $$ = create_non_term("FUNCTION CALL", $1 ); } */
	| IDENTIFIER '(' ')'	{ $$ = create_non_term("FUNCTION CALL", $1 ); } 
/*	| postfix_expression '(' argument_expression_list ')'	{ create_non_term("FUNCTION CALL ARGS", $1, $3 ); } */
	| IDENTIFIER '(' argument_expression_list ')'	{ create_non_term( "FUNCTION CALL ARGS", $1, $3 ); }
	| postfix_expression '.' IDENTIFIER	{ $$ = create_non_term(".", $1, $3); } 
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = create_non_term("->", $1, $3); } 
	| postfix_expression INC_OP	{ $$ = create_non_term("POST INCREMENT", $1); } 
	| postfix_expression DEC_OP	{ $$ = create_non_term("POST DECREMENT", $1); } 
	;

argument_expression_list
	: assignment_expression					{ $$ = create_non_term("assignment_expression", $1); } 
	| argument_expression_list ',' assignment_expression	{ $$ = create_non_term("assignment_expression_list", $1, $3); } 
	;

unary_expression
	: postfix_expression			{ $$ = $1; } 
	| INC_OP unary_expression		{ $$ = create_non_term("PRE INCREMENT", $2); } 
	| DEC_OP unary_expression		{ $$ = create_non_term("PRE DECREMENT", $2); } 
	| unary_operator cast_expression	{ $1->add_child($2); $$ = $1; } 
	| SIZEOF unary_expression		{ $$ = create_non_term("SIZEOF unary_expr", $2); } 
	| SIZEOF '(' type_name ')'		{ $$ = create_non_term("SIZEOF type_name", $3); } 
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
	: unary_expression			{ $$ = $1; }
	| '(' type_name ')' cast_expression	{ $$ = create_non_term("cast_expression", $2, $4); }
	;

multiplicative_expression
	: cast_expression				{ $$ = $1; }
	| multiplicative_expression '*' cast_expression	{ $$ = create_non_term("*", $1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = create_non_term("/", $1, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = create_non_term("%", $1, $3); }
	;

additive_expression
	: multiplicative_expression				{ $$ = $1; }
	| additive_expression '+' multiplicative_expression	{ $$ = create_non_term("+", $1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = create_non_term("-", $1, $3); }
	;

shift_expression
	: additive_expression				{ $$ = $1; }
	| shift_expression LEFT_OP additive_expression	{ $$ = create_non_term(">>", $1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = create_non_term("<<", $1, $3); }
	;

relational_expression
	: shift_expression				{ $$ = $1; }
	| relational_expression '<' shift_expression	{ $$ = create_non_term("<", $1, $3); }
	| relational_expression '>' shift_expression	{ $$ = create_non_term(">", $1, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = create_non_term("<=", $1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = create_non_term(">=", $1, $3); }
	;

equality_expression
	: relational_expression					{ $$ = $1; }
	| equality_expression EQ_OP relational_expression	{ $$ = create_non_term("==", $1, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = create_non_term("!=", $1, $3); }
	;

and_expression
	: equality_expression				{ $$ = $1; }
	| and_expression '&' equality_expression	{ $$ = create_non_term("&", $1, $3); }
	;

exclusive_or_expression
	: and_expression				{ $$ = $1; }
	| exclusive_or_expression '^' and_expression	{ $$ = create_non_term("^", $1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression				{ $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = create_non_term("|", $1, $3); }
	;

logical_and_expression
	: inclusive_or_expression				{ $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = create_non_term("&&", $1, $3); }
	;

logical_or_expression
	: logical_and_expression				{ $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression	{ $$ = create_non_term("||", $1, $3); }
	;

conditional_expression
	: logical_or_expression							{ $$ = $1; }
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
	: assignment_expression			 { $$ = create_non_term("expression", $1); }
	| expression ',' assignment_expression	 { $$ = create_non_term("expression", $1, $3); }
	;

constant_expression
	: conditional_expression	 { $$ = $1; }
	;

declaration
	: declaration_specifiers ';'			  { $$ = new_declaration( $1, NULL );}
	| declaration_specifiers init_declarator_list ';' {$$ = new_declaration( $1, $2 );}
	;

/* These are types */
declaration_specifiers
	: storage_class_specifier				 { $$ = new_storage_class( $1 ); }
	| storage_class_specifier declaration_specifiers	 { $$ = add_storage_class( $2, $1 ); }
	| type_specifier					 { $$ = new_type_specifier( $1 ); }
	| type_specifier declaration_specifiers			 { $$ = add_type_specifier( $2, $1 ); }
	| type_qualifier					 { $$ = new_type_qualifier ( $1 );  }
	| type_qualifier declaration_specifiers			 { $$ = add_type_qualifier( $2, $1 ); }
	;

init_declarator_list
	: init_declarator				 { $$ = create_init_declarator_list( $1 ); }
	| init_declarator_list ',' init_declarator	 { $$ = add_to_init_declarator_list ( $1, $3 ); }
	;

init_declarator
	: declarator			 { $$ = $1; }
	| declarator '=' initializer	 { $$ = add_initializer_to_declarator( $1, $3 ); }
	;

storage_class_specifier
	:  TYPEDEF	{ $$ = TYPEDEF;  }  
/*	|  EXTERN	{ $$ = EXTERN;   } */ 
/*	|  STATIC	{ $$ = STATIC;   } */
/*	|  AUTO	 	{ $$ = AUTO; 	 } */
/*	|  REGISTER	{ $$ = REGISTER; } */
	;

type_specifier
	:  VOID				{ $$ = create_type_specifier(VOID); }
	|  CHAR				{ $$ = create_type_specifier(CHAR); }
	|  SHORT			{ $$ = create_type_specifier(SHORT); }
	|  INT				{ $$ = create_type_specifier(INT); }
	|  LONG				{ $$ = create_type_specifier(LONG); }
	|  FLOAT			{ $$ = create_type_specifier(FLOAT); }
	|  DOUBLE			{ $$ = create_type_specifier(DOUBLE); }
	|  SIGNED			{ $$ = create_type_specifier(SIGNED); }
	|  UNSIGNED			{ $$ = create_type_specifier(UNSIGNED); }
	|  struct_or_union_specifier 	{ $$ = $1; }
	|  enum_specifier		{ $$ = $1; }
	|  TYPE_NAME			{ $$ = create_type_specifier(TYPE_NAME); }
	;

struct_or_union_specifier
	:  struct_or_union IDENTIFIER '{' struct_declaration_list '}'	{ $$ = create_type_specifier( $1, $2, $4); }
	|  struct_or_union '{' struct_declaration_list '}'	 	{ $$ = create_type_specifier( $1, NULL, $3); }
	|  struct_or_union IDENTIFIER	 				{ $$ = create_type_specifier( $1, $2, (StructDeclarationList *) NULL); }
	;

struct_or_union
	:  STRUCT	 { $$ = STRUCT; }
	|  UNION	 { $$ = UNION; }
	;

struct_declaration_list
	:  struct_declaration	 			{ $$ = create_struct_declaration_list ( $1 ); }
	|  struct_declaration_list struct_declaration	{ $$ = add_to_struct_declaration_list ( $1, $2 ); }
	;

struct_declaration
	:  specifier_qualifier_list struct_declarator_list ';'	 { $$ = create_struct_declaration( $1, $2 ); }
	;

specifier_qualifier_list
	:  type_specifier specifier_qualifier_list	{ $$ = add_type_specifier_sq( $2, $1 ); }
	|  type_specifier	 			{ $$ = create_type_specifier_sq( $1 ); }
	|  type_qualifier specifier_qualifier_list	{ $$ = add_type_qualifier_sq( $2, $1 ); }
	|  type_qualifier	 			{ $$ = create_type_qualifier_sq( $1 ); }
	;

struct_declarator_list
	:  struct_declarator	 			 { $$ = create_init_declarator_list ( $1 ); }
	|  struct_declarator_list ',' struct_declarator	 { $$ = add_to_init_declarator_list( $1, $3 ); }
	;

struct_declarator
	:  declarator	 { $$ = $1; }
/*	|  ':' constant_expression	 { $$ = create_non_term(":", $2); }
	|  declarator ':' constant_expression	 { $$ = create_non_term(":", $1, $3); } */
	;

enum_specifier
	:  ENUM '{' enumerator_list '}'	 		{ $$ = create_type_specifier( ENUM, NULL, $3 ); }
	|  ENUM IDENTIFIER '{' enumerator_list '}'	{ $$ = create_type_specifier( ENUM, $2,   $4 ); }
	|  ENUM IDENTIFIER	 			{ $$ = create_type_specifier( ENUM, $2, (EnumeratorList *) NULL ); }
	;

enumerator_list
	:  enumerator	 			 { $$ = create_enumerator_list( $1 ); }
	|  enumerator_list ',' enumerator	 { $$ = add_to_enumerator_list( $1, $3 ); }
	;

enumerator
	:  IDENTIFIER	 			 { $$ = create_enumerator( $1, NULL ); }
	|  IDENTIFIER '=' constant_expression	 { $$ = create_enumerator( $1, $3 ); }
	;

type_qualifier
	:  CONST	 { $$ = CONST; }
	|  VOLATILE	 { $$ = VOLATILE; }
	;

declarator
	:  pointer direct_declarator	 { $$ = create_declarator( $1, $2 ); }
	|  direct_declarator	 	 { $$ = create_declarator( NULL, $1 ); }
	;

direct_declarator
	:  IDENTIFIER	 					 { $$ = create_dir_declarator_id( ID , $1 ); }
	|  '(' declarator ')'	 				 { $$ = create_dir_declarator_dec( DECLARATOR, $2 ); }
	/*|  direct_declarator '[' constant_expression ']'	 { $$ = create_dir_declarator_arr( ARRAY, $1, $3 ); }*/
	|  direct_declarator '[' CONSTANT ']'	 { $$ = create_dir_declarator_arr( ARRAY, $1, $3 ); }
	|  direct_declarator '[' ']'	 			 { $$ = create_dir_declarator_arr( ARRAY, $1, NULL ); }
	|  direct_declarator '(' parameter_type_list ')'	 { $$ = create_dir_declarator_fun( FUNCTION, $1, $3 ); }
	|  direct_declarator '(' ')'				 { $$ = create_dir_declarator_fun( FUNCTION, $1, NULL ); }
	;

pointer
	:  '*'	 				{ $$ = create_pointer(); }
	|  '*' type_qualifier_list	 	{ $$ = create_pointer( $2, NULL ); }
	|  '*' pointer	 			{ $$ = create_pointer( NULL, $2 ); }
	|  '*' type_qualifier_list pointer	{ $$ = create_pointer( $2, $3 ); }
	;

type_qualifier_list
	:  type_qualifier	 		{ $$ = create_type_qualifier_list( $1 ); }
	|  type_qualifier_list type_qualifier	{ $$ = add_to_type_qualifier_list( $1, $2 ); }
	;

parameter_type_list
	:  parameter_list	 	 { $$ = $1 ; }
	|  parameter_list ',' ELLIPSIS	 { $$ = add_ellipsis_to_list( $1 ); }
	;

parameter_list
	:  parameter_declaration	 		 { $$ = create_parameter_list( $1 ); }
	|  parameter_list ',' parameter_declaration	 { $$ = add_to_parameter_list( $1, $3 ); }
	;

parameter_declaration
	:  declaration_specifiers declarator	 	{ $$ = create_parameter_declaration( $1, $2, NULL ); } 
	|  declaration_specifiers abstract_declarator	{ $$ = create_parameter_declaration( $1, NULL, $2 ); } 
	|  declaration_specifiers	 		{ $$ = create_parameter_declaration( $1, NULL, NULL ); } 
	;
/*
identifier_list
	:  IDENTIFIER	 { $$ = $1; }
	|  identifier_list ',' IDENTIFIER	 { $$ = create_non_term("identifier_list", $1, $3); }
	;
*/
type_name
	:  specifier_qualifier_list	 { $$ = create_non_term("type_name", $1); }
	|  specifier_qualifier_list abstract_declarator	 { $$ = create_non_term("type_name", $1, $2); }
	;

abstract_declarator
	:  pointer	 			{ $$ = create_abstract_declarator( $1, NULL ); } 
	|  direct_abstract_declarator	 	{ $$ = create_abstract_declarator( NULL, $1 ); } 
	|  pointer direct_abstract_declarator	{ $$ = create_abstract_declarator( $1, $2 ); } 
	;

direct_abstract_declarator
	:  '(' abstract_declarator ')'					{ $$ = create_direct_abstract_declarator( ABSTRACT, $2 ); }
	|  '[' ']'							{ $$ = create_direct_abstract_declarator( SQUARE ); } 
	/*|  '[' constant_expression ']'	 				{ $$ = create_direct_abstract_declarator( SQUARE, NULL, $2 ); } */
	|  '[' CONSTANT ']'	 				{ $$ = create_direct_abstract_declarator( SQUARE, NULL, $2 ); } 
	|  direct_abstract_declarator '[' ']'				{ $$ = create_direct_abstract_declarator( SQUARE, $1 ); } 
	|  direct_abstract_declarator '[' constant_expression ']'	{ $$ = create_direct_abstract_declarator( SQUARE, $1, $3 ); } 
	|  '(' ')'	 						{ $$ = create_direct_abstract_declarator( ROUND ); } 
	|  '(' parameter_type_list ')'	 				{ $$ = create_direct_abstract_declarator( ROUND, NULL, $2 ); } 
	|  direct_abstract_declarator '(' ')'	 			{ $$ = create_direct_abstract_declarator( ROUND, $1 ); } 
	|  direct_abstract_declarator '(' parameter_type_list ')'	{ $$ = create_direct_abstract_declarator( ROUND, $1, $3 ); } 
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
	:  IDENTIFIER ':' statement	 { $$ = create_non_term("labeled_statement", $1, $3); }
	|  CASE constant_expression ':' statement	 { $$ = create_non_term("CASE", $2, $4); }
	|  DEFAULT ':' statement	 { $$ = create_non_term("DEFAULT", $2); }
	;

compound_statement
	:  '{' '}'	 				{ $$ = NULL; }
	|  '{' statement_list '}'	 		{ $$ = create_non_term("compound_statement", $2); }
	|  '{' declaration_list '}'	 		{ local_symbol_table.clear_current_level(); $$ = create_non_term("compound_statement", $2); }
	|  '{' declaration_list statement_list '}'	{ local_symbol_table.clear_current_level(); $$ = create_non_term("compound_statement", $2, $3); }
	;

declaration_list
	:  declaration	 		 { $$ = create_declaration_list( $1 ); }
	|  declaration_list declaration	 { $$ = add_to_declaration_list( $1, $2 );}
	;

statement_list
	:  statement			 { $$ = $1; }
	|  statement_list statement	 { $$ = create_non_term("statement_list", $1, $2); }
	;

expression_statement
	:  ';'			 { $$ = create_terminal("EMPTY_STMT",NULL); }
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
	:  GOTO IDENTIFIER ';'	 { $$ = create_non_term("GOTO", $2); }
	|  CONTINUE ';'	 { $$ = create_non_term("CONTINUE"); }
	|  BREAK ';'	 { $$ = create_non_term("BREAK"); }
	|  RETURN ';'	 { $$ = create_non_term("RETURN"); }
	|  RETURN expression ';'	 { $$ = create_non_term("RETURN", $2); }
	;

translation_unit
	:  external_declaration	 { root->add_child(create_non_term("external_declaration", $1)); }
	|  translation_unit external_declaration	 { root->add_child(create_non_term("external_declaration", $2)); }
	;

external_declaration
	:  function_definition	 { $$ = $1; }
	|  declaration	 { $$ = add_to_global_symbol_table( $1 ); }
	;

function_declaration
	: declaration_specifiers declarator { $$ = create_function_defintion($1, $2, NULL); }
	;

function_definition
	:  function_declaration compound_statement	 { $$ = $1; }
	;
/*	:  declaration_specifiers declarator compound_statement	 { $$ = create_function_defintion($1, $2, $3); }*/
/*	|  declarator compound_statement	 { $$ = create_non_term("function_definition", $2); }*/


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
