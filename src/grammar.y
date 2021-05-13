
%{
	#include <stdio.h>
	#include <iostream>
	#include <sstream>
	#include <ast.h>
	#include <symtab.h>
	#include <expression.h>
	#include <3ac.h>
	#include <statement.h>
	
	void yyerror(const char *s);	
	extern "C" int yylex();
	extern Node * root; 
	

%}

%union{
	Node * node;
	Terminal * terminal;
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
	Constant *constant;
	StringLiteral *string_literal;
	TopLevelExpression* top_level_expression;
	AssignmentExpression* assignment_expression;
	ArgumentExprList* argument_expression_list;
	PostfixExpression* postfix_expression;
	UnaryExpression* unary_expression;
	CastExpression* cast_expression;
	MultiplicativeExpression* multiplicative_expression;
	AdditiveExpression* additive_expression;
	ShiftExpression* shift_expression;
	RelationalExpression* relational_expression;
	EqualityExpression* equality_expression;
	AndExpression* and_expression;
	ExclusiveorExpression* exclusive_or_expression;
	InclusiveorExpression* inclusive_or_expression;
	Logical_andExpression* logical_and_expression;
	Logical_orExpression* logical_or_expression;
	ConditionalExpression* conditional_expression;
	Expression* expression;
	TypeName * type_name;
	Label * label; 
	GoTo  * _goto;
	Statement * statement;
}



%token <node> '(' ')' '[' ']' '.' ',' '+' '-' '!' '&' '*' '~' '/' '%'
%token <node> '<' '>' '^' '|' ':' '?' ';' '{' '}'
%token <terminal> '='

%token <terminal> SIZEOF
%token <identifier> IDENTIFIER
%token <constant> CONSTANT 
%token <string_literal> STRING_LITERAL
%type <expression> expression
%type <expression> assignment_expression
%type <expression> logical_and_expression
%type <expression> logical_or_expression 
%type <expression> unary_expression
%type <expression> cast_expression
%type <expression> relational_expression
%type <expression> shift_expression 
%type <expression> inclusive_or_expression 
%type <expression> exclusive_or_expression
%type <expression> additive_expression
%type <argument_expression_list> argument_expression_list
%type <expression> multiplicative_expression
%type <expression> and_expression
%type <expression> equality_expression  
%type <expression> conditional_expression
%type <expression> postfix_expression
%type <expression> primary_expression


%token <terminal> INC_OP DEC_OP
%token <node> PTR_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <node> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <node> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <node> XOR_ASSIGN OR_ASSIGN

%token <value> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <type_specifier> SIGNED UNSIGNED CHAR SHORT LONG INT FLOAT DOUBLE VOID TYPE_NAME 
%token <value> CONST VOLATILE
%token <value> STRUCT UNION ENUM ELLIPSIS

%token <node> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN


%type <type_name> type_name
%type <node> constant_expression
%type <node> unary_operator assignment_operator
%type <declaration> declaration 
%type <init_declarator_list> init_declarator_list  
%type <declarator> init_declarator declarator
%type <declaration_specifiers> declaration_specifiers 
%type <value> storage_class_specifier 
%type <type_specifier> type_specifier struct_or_union_specifier enum_specifier struct_or_union_id
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

/*%type <node> initializer initializer_list*/
%type <parameter_type_list> parameter_type_list parameter_list
%type <parameter_declaration> parameter_declaration 
%type <abstract_declarator> abstract_declarator
%type <direct_abstract_declarator>  direct_abstract_declarator 


/*%type <node> identifier_list */
%type <statement> labeled_statement compound_statement statement_list selection_statement iteration_statement jump_statement
%type <statement> statement
%type <expression> expression_statement 

%type <node> translation_unit external_declaration 

%type <label> M_LABEL
%type <_goto> M_GOTO M_FALSE

%type <constant> neg_constant

%start translation_unit
%%

primary_expression
	: IDENTIFIER		{ $$ = create_primary_identifier($1); } 
	| CONSTANT		{ $$ = create_primary_constant($1); }
	| STRING_LITERAL	{ $$ = create_primary_stringliteral($1); } 
	/*| '(' expression ')'	{ $$ = create_primary_expression($2); }*/
	| '(' expression ')'	{ $$ = $2; } 
	;

postfix_expression
	: primary_expression	{ $$ = $1; } 
	| postfix_expression '[' expression ']'	{ $$ = create_postfix_expr_arr( $1, $3 ); } 
/*	| postfix_expression '(' ')'	{ $$ = create_non_term("FUNCTION CALL", $1 ); } */
	| IDENTIFIER '(' ')'	{ $$ = create_postfix_expr_voidfun( $1 ); } 
	| IDENTIFIER '(' argument_expression_list ')'	{ $$ = create_postfix_expr_fun($1, $3); } 
/*	| postfix_expression '(' argument_expression_list ')'	{ create_non_term("FUNCTION CALL ARGS", $1, $3 ); } */
	| postfix_expression '.' IDENTIFIER	{ $$ = create_postfix_expr_struct(".", $1, $3); } 
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = create_postfix_expr_struct("->", $1, $3); } 
	| postfix_expression INC_OP	{ $$ = create_postfix_expr_ido( $2, $1); } 
	| postfix_expression DEC_OP	{ $$ = create_postfix_expr_ido( $2, $1); } 
	;

argument_expression_list
	: assignment_expression					{ $$ = create_argument_expr_assignement($1); } 
	| argument_expression_list ',' assignment_expression	{ $$ = create_argument_expr_list($1, $3); } 
	;

unary_expression
	: postfix_expression			{ $$ = $1; } 
	| INC_OP unary_expression		{ $$ = create_unary_expression( $1, $2 ); } 
	| DEC_OP unary_expression		{ $$ = create_unary_expression( $1, $2 ); } 
	| unary_operator cast_expression	{ $$ = create_unary_expression_cast( $1, $2 ); } 
	| SIZEOF unary_expression		{ $$ = create_unary_expression( $1, $2 ); } 
	| SIZEOF '(' type_name ')'		{ $$ = create_unary_expression( $1, $3); } 
	;

unary_operator
	: '&'	{ $$ = $1; }
	| '*'	{ $$ = $1; }
	| '+'	{ $$ = $1; }
	| '-'	{ $$ = $1; }
	| '~'	{ $$ = $1; }
	| '!'	{ $$ = $1; }
	;

cast_expression
	: unary_expression			{ $$ = $1; }
	| '(' type_name ')' cast_expression	{ $$ = create_cast_expression_typename($2, $4); }
	;

multiplicative_expression
	: cast_expression				{ $$ = $1; }
	| multiplicative_expression '*' cast_expression	{ $$ = create_multiplicative_expression("*", $1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = create_multiplicative_expression("/", $1, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = create_multiplicative_expression("%", $1, $3); }
	;

additive_expression
	: multiplicative_expression				{ $$ = $1; }
	| additive_expression '+' multiplicative_expression	{ $$ = create_additive_expression("+", $1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = create_additive_expression("-", $1, $3); }
	;

shift_expression
	: additive_expression				{ $$ = $1; }
	| shift_expression LEFT_OP additive_expression	{ $$ = create_shift_expression("<<", $1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = create_shift_expression(">>", $1, $3); }
	;

relational_expression
	: shift_expression				{ $$ = $1; }
	| relational_expression '<' shift_expression	{ $$ = create_relational_expression("<", $1, $3); }
	| relational_expression '>' shift_expression	{ $$ = create_relational_expression(">", $1, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = create_relational_expression("<=", $1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = create_relational_expression(">=", $1, $3); }
	;

equality_expression
	: relational_expression					{ $$ = $1; }
	| equality_expression EQ_OP relational_expression	{ $$ = create_equality_expression("==", $1, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = create_equality_expression("!=", $1, $3); }
	;

and_expression
	: equality_expression				{ $$ = $1; }
	| and_expression '&' equality_expression	{ $$ = create_and_expression("&", $1, $3); }
	;

exclusive_or_expression
	: and_expression				{ $$ = $1; }
	| exclusive_or_expression '^' and_expression	{ $$ = create_exclusive_or_expression("^", $1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression				{ $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = create_inclusive_or_expression("|", $1, $3); }
    ;

logical_and_expression
	: inclusive_or_expression				{ $$ = $1; }
	| logical_and_expression AND_OP { $1->falselist.push_back(create_new_goto_cond($1->res,false)); if( $1->truelist.size() != 0 ) { backpatch( $1->truelist, create_new_label()); } } inclusive_or_expression	{ $$ = create_logical_and_expression("&&", $1, $4); }
	;

logical_or_expression
	: logical_and_expression				{ $$ = $1; }
	| logical_or_expression OR_OP { $1->truelist.push_back(create_new_goto_cond($1->res,true)); if($1->falselist.size() != 0) { backpatch( $1->falselist, create_new_label() ); } }logical_and_expression	{ $$ = create_logical_or_expression("||", $1, $4); }
	;

conditional_expression
	: logical_or_expression							{ $$ = $1; }
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = create_conditional_expression("?:", $1, $3, $5); }
	;

assignment_expression
	: conditional_expression					 { $$ = $1; }
	| unary_expression assignment_operator assignment_expression	 { $$=create_assignment_expression($1,$2,$3); }
	;

assignment_operator
	: '='		 { $$ = $1; }
	| MUL_ASSIGN	 { $$ = $1; }
	| DIV_ASSIGN	 { $$ = $1; }
	| MOD_ASSIGN	 { $$ = $1; }
	| ADD_ASSIGN	 { $$ = $1; }
	| SUB_ASSIGN	 { $$ = $1; }
	| LEFT_ASSIGN	 { $$ = $1; }
	| RIGHT_ASSIGN	 { $$ = $1; }
	| AND_ASSIGN	 { $$ = $1; }
	| XOR_ASSIGN	 { $$ = $1; }
	| OR_ASSIGN	 { $$ = $1; }
	;

expression
	: assignment_expression			 { $$ = $1; }
	| expression ',' assignment_expression	 { $$ = create_toplevel_expression($1, $3); }
	;

/* Revisit this */ 
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
/*	| error						 { $$ = NULL; }*/
	| init_declarator_list ',' error ','		 { $$ = $1; }
	;

init_declarator
	: declarator			 { $$ = $1; }
	/*| declarator '=' initializer	 { $$ = add_initializer_to_declarator( $1, $2, $3 ); }*/
	| declarator '=' { error_msg("Invalid Initialisation of declaration",line_num,column); } assignment_expression	 {  $$ = $1; /* add_initializer_to_declarator( $1, $2, $3 );*/ }
	;

storage_class_specifier
	:  TYPEDEF	{ $$ = TYPEDEF;  }  
/*	|  EXTERN	{ $$ = EXTERN;   } */ 
/*	|  STATIC	{ $$ = STATIC;   } */
/*	|  AUTO	 	{ $$ = AUTO; 	 } */
/*	|  REGISTER	{ $$ = REGISTER; } */
	;

type_specifier
	:  VOID				{ $$ = $1; }
	|  CHAR				{ $$ = $1; }
	|  SHORT			{ $$ = $1; }
	|  INT				{ $$ = $1; }
	|  LONG				{ $$ = $1; }
	|  FLOAT			{ $$ = $1; }
	|  DOUBLE			{ $$ = $1; }
	|  SIGNED			{ $$ = $1; }
	|  UNSIGNED			{ $$ = $1; }
	|  struct_or_union_specifier 	{ $$ = $1; }
	|  enum_specifier		{ $$ = $1; }
	|  TYPE_NAME			{ $$ = $1; }
	;
/*
struct_or_union_specifier
	:  struct_or_union IDENTIFIER  _marker_ { } '{' struct_declaration_list '}'	{ $$ = create_type_specifier( $1, $2, $4); }
	|  struct_or_union '{' struct_declaration_list '}'	 	{ $$ = create_type_specifier( $1, NULL, $3); }
	|  struct_or_union IDENTIFIER	 				{ $$ = create_type_specifier( $1, $2, (StructDeclarationList *) NULL); }
	;

*/
struct_or_union_specifier
	:  struct_or_union_id  '{' struct_declaration_list '}'	{ $$ = add_struct_declaration( $1, $3); }
	|  struct_or_union_id 	 				{ $$ =  $1; }
	;

struct_or_union_id
	: struct_or_union IDENTIFIER	{ $$ = create_struct_type( $1, $2 ); }
/*	| struct_or_union 		{ $$ = create_struct_type( $1, NULL ); }*/
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
/*	|  VOLATILE	 { $$ = VOLATILE; }*/
	;

declarator
	:  pointer direct_declarator	 { $$ = create_declarator( $1, $2 ); }
	|  direct_declarator	 	 { $$ = create_declarator( NULL, $1 ); }
	;

direct_declarator
	:  IDENTIFIER	 					 { $$ = create_dir_declarator_id( ID , $1 ); }
	/*|  '(' declarator ')'	 				 { $$ = create_dir_declarator_dec( DECLARATOR, $2 ); }*/
	/*|  direct_declarator '[' constant_expression ']'	 { $$ = create_dir_declarator_arr( ARRAY, $1, $3 ); }*/
	|  direct_declarator '[' CONSTANT ']'	 		 { $$ = append_dir_declarator_arr( ARRAY, $1, $3 ); }
	|  direct_declarator '[' '-' CONSTANT ']'	         { error_msg("Array dimension must be a positive integer", $4->line_num, $4->column ); $$=NULL; }
	/*|  direct_declarator '[' ']'	 			 { $$ = create_dir_declarator_arr( ARRAY, $1, NULL ); }*/
	|  direct_declarator '(' parameter_type_list ')'	 { $$ = append_dir_declarator_fun( FUNCTION, $1, $3 ); }
	|  direct_declarator '(' ')'				 { $$ = append_dir_declarator_fun( FUNCTION, $1, NULL ); }
	;

pointer
	:  '*'	 				{ $$ = create_pointer(); }
	|  '*' type_qualifier_list	 	{ $$ = create_pointer( $2, NULL ); }
	|  '*' pointer	 			{ $$ = create_pointer( NULL, $2 ); }
	|  '*' type_qualifier_list pointer	{ $$ = create_pointer( $2, $3 ); }
	;

type_qualifier_list
	:  type_qualifier	 					{ $$ = create_type_qualifier_list( $1 ); }
	|  type_qualifier_list type_qualifier	{ $$ = add_to_type_qualifier_list( $1, $2 ); }
	;

parameter_type_list
	:  parameter_list	 	 		 { $$ = $1 ; }
	|  parameter_list ',' ELLIPSIS	 { $$ = add_ellipsis_to_list( $1 ); }
	;

parameter_list
	:  parameter_declaration	 		 { $$ = create_parameter_list( $1 ); }
	|  parameter_list ',' parameter_declaration	 { $$ = add_to_parameter_list( $1, $3 ); }
	;

parameter_declaration
	:  declaration_specifiers declarator	 		{ $$ = create_parameter_declaration( $1, $2, NULL ); } 
	|  declaration_specifiers abstract_declarator	{ $$ = create_parameter_declaration( $1, NULL, $2 ); } 
	|  declaration_specifiers	 					{ $$ = create_parameter_declaration( $1, NULL, NULL ); } 
	;
/*
identifier_list
	:  IDENTIFIER	 { $$ = $1; }
	|  identifier_list ',' IDENTIFIER	 { $$ = create_non_term("identifier_list", $1, $3); }
	;
*/
type_name
	:  specifier_qualifier_list	 		{ $$ = create_type_name( $1, NULL); }
	|  specifier_qualifier_list abstract_declarator	{ $$ = create_type_name( $1, $2); }
	;

abstract_declarator
	:  pointer	 			{ $$ = create_abstract_declarator( $1, NULL ); } 
	|  direct_abstract_declarator	 	{ $$ = create_abstract_declarator( NULL, $1 ); } 
	|  pointer direct_abstract_declarator	{ $$ = create_abstract_declarator( $1, $2 ); } 
	;

direct_abstract_declarator
	:  '[' ']'							{ $$ = create_direct_abstract_declarator( NULL ); } 
	|  '[' CONSTANT ']'	 					{ $$ = create_direct_abstract_declarator( $2 ); } 
	|  direct_abstract_declarator '[' ']'				{ $$ = append_direct_abstract_declarator( $1, NULL ); } 
	|  direct_abstract_declarator '[' CONSTANT ']'			{ $$ = append_direct_abstract_declarator( $1, $3 ); } 
	/*:  '(' abstract_declarator ')'					{ $$ = create_direct_abstract_declarator( ABSTRACT, $2 ); }*/
	/*|  '[' constant_expression ']'	 				{ $$ = create_direct_abstract_declarator( SQUARE, NULL, $2 ); } */
	/*|  direct_abstract_declarator '[' constant_expression ']'	{ $$ = create_direct_abstract_declarator( SQUARE, $1, $3 ); }*/ 
	/*|  '(' ')'	 						{ $$ = create_direct_abstract_declarator( ROUND ); } */
	/*|  '(' parameter_type_list ')'	 				{ $$ = create_direct_abstract_declarator( ROUND, NULL, $2 ); }*/ 
	/*|  direct_abstract_declarator '(' ')'	 			{ $$ = create_direct_abstract_declarator( ROUND, $1 ); } */
	/*|  direct_abstract_declarator '(' parameter_type_list ')'	{ $$ = create_direct_abstract_declarator( ROUND, $1, $3 ); } */ 
	;

/*initializer
	:  assignment_expression	 	 { $$ = $1; }
	|  '{' initializer_list '}'	 	 { $$ = create_non_term("initializer_list", $2); }
	|  '{' initializer_list ',' '}'	 { $$ = create_non_term("initializer_list", $2); }
	;

initializer_list
	:  initializer	 { $$ = create_non_term("initializer", $1); }
	|  initializer_list ',' ini$ = $2;tializer	 { $$ = create_non_term("initializer_list", $1, $3); }
	;
*/
statement
	:  labeled_statement	 { $$ = $1; }
	|  compound_statement	 { $$ = $1; }
	|  expression_statement	 { $$ = create_expression_statement($1); }
	|  selection_statement	 { $$ = $1; }
	|  iteration_statement	 { $$ = $1; }
	|  jump_statement	 { $$ = $1; }
	;

labeled_statement
	:  IDENTIFIER ':' M_LABEL statement	 		{ $$ = create_labeled_statement_iden($1, $3, $4); }
	|  CASE neg_constant ':' M_LABEL statement		{ $$ = create_labeled_statement_case($2,$4,$5); }
	|  DEFAULT ':' M_LABEL statement	 		{ $$ = create_labeled_statement_def($3,$4); }
	;

neg_constant
	: 	CONSTANT	{ $$ = $1; }
	|	'-' CONSTANT { $2->negate();  $$ = $2; }
	;

compound_statement
	: M_INC '{' '}'	 			                { local_symbol_table.clear_current_level(); $$ = NULL; }
	| M_INC '{' statement_list '}'	 		        { local_symbol_table.clear_current_level(); $$ = $3; }
	| M_INC '{' declaration_list '}'	 	        { local_symbol_table.clear_current_level(); $$ = NULL; }
	| M_INC '{' declaration_list statement_list '}'		{ local_symbol_table.clear_current_level(); $$ = $4; }
	;

declaration_list
	:  declaration	 		 		 { $$ = create_declaration_list( $1 ); }
	|  declaration_list declaration	 { $$ = add_to_declaration_list( $1, $2 );}
	;

statement_list
	:  statement			 	 { $$ = create_statement_list( $1 ); }
	|  error statement			 { $$ = create_statement_list( $2 ); }
	|  statement_list { if ( $1 != nullptr && $1->nextlist.size() != 0 ) { backpatch($1->nextlist,create_new_label()); } } statement	 { $$ = add_to_statement_list( $1, $3 ); }
	;

/* This is automatically an expression */
expression_statement
	:  ';'			 	 { $$ = NULL; }
	|  expression ';'	 { $$ = $1; }
	|  expression { error_msg("Missing ';'",prev_line_num,prev_column); } error  { $$ = $1; }
	;

selection_statement
	:  IF '(' expression M_FALSE ')' M_LABEL statement	 			{ $$ = create_selection_statement_if( $3, $4, $6, $7, NULL, NULL, NULL);}
	|  IF '(' expression M_FALSE ')' M_LABEL statement  ELSE M_GOTO M_LABEL statement	{ $$ = create_selection_statement_if( $3, $4, $6, $7, $9, $10, $11 );}
	|  SWITCH '(' expression { create_switch($3); } M_GOTO ')' statement M_GOTO			 { $$ = create_selection_statement_switch($3,$5,$7,$8); } 
	;

iteration_statement
	:  WHILE '(' M_LABEL expression M_FALSE ')' M_LABEL statement	 { $$ = create_iteration_statement_while( $3, $4, $5, $7, $8 ); }
	|  DO M_LABEL statement WHILE '(' M_LABEL expression M_FALSE ')' ';'	 { $$ = create_iteration_statement_do_while( $2, $3, $6, $7, $8 ); }
	|  FOR '(' expression_statement  M_LABEL expression_statement  
				{ if ( $5 != nullptr ) { $5->falselist.push_back(create_new_goto_cond($5->res,false)); } } 
			')' M_LABEL statement	 { $$ = create_iteration_statement_for( $3, $4, $5, $8, $9 ); }
	|  FOR '(' expression_statement M_LABEL expression_statement 
				{ if ( $5 != nullptr ) { $5->falselist.push_back(create_new_goto_cond($5->res,false)); }}
			 M_GOTO M_LABEL expression M_GOTO ')' M_LABEL statement	 { $$ = create_iteration_statement_for( $3, $4, $5, $7, $8, $9, $10, $12, $13 ); }
	;

jump_statement
	:  GOTO IDENTIFIER ';'	 { 	$$ = create_jump_statement_go_to( $2 ); }
	|  CONTINUE ';'	 		 { $$ = create_jump_statement( CONTINUE ); }
	|  BREAK ';'	 		 { $$ = create_jump_statement( BREAK ); }
	|  RETURN ';'	 		 { $$ = create_jump_statement( RETURN ); }
	|  RETURN expression ';'  { $$ = create_jump_statement_exp( $2 ); }
	|  GOTO IDENTIFIER { error_msg("Missing ';'",prev_line_num,prev_column);} error { $$ = NULL; }
	|  CONTINUE { error_msg("Missing ';'",prev_line_num,prev_column);} error { $$ = NULL; }
	|  BREAK { error_msg("Missing ';'",prev_line_num,prev_column);} error { $$ = NULL; }
	|  RETURN { error_msg("Missing ';'",prev_line_num,prev_column);} error { $$ = NULL; }
	|  RETURN  expression { error_msg("Missing ';'",prev_line_num,prev_column);} error { $$ = NULL; }
	;

translation_unit
	:  external_declaration	 { root->add_child(create_non_term("external_declaration", $1)); }
	|  translation_unit external_declaration	 { root->add_child(create_non_term("external_declaration", $2)); }
	|  error external_declaration	 { root->add_child(create_non_term("external_declaration", $2)); }
	|  translation_unit error external_declaration	 { root->add_child(create_non_term("external_declaration", $3)); }
	;

external_declaration
	:  function_definition	 { $$ = $1; }
	|  declaration	 { $$ = add_to_global_symbol_table( $1 ); }
	;

function_declaration
	: declaration_specifiers declarator { $$ = create_function_defintion($1, $2, NULL); }
	;

function_definition
	:  function_declaration compound_statement	 { $$ = add_stmt_to_function_definition( $1, $2 ); backpatch_fn( $2 );/*backpatch( $2->returnlist, create_new_label());*/ dump_and_reset_3ac(); }
	;
/*	:  declaration_specifiers declarator compound_statement	 { $$ = create_function_defintion($1, $2, $3); }*/
/*	|  declarator compound_statement	 { $$ = create_non_term("function_definition", $2); }*/

M_LABEL : %empty { $$ = create_new_label(); } ;
M_GOTO  : %empty { $$ = create_new_goto(); } ;
M_FALSE : %empty { $$ = create_new_goto_cond(nullptr, false); };
M_INC	: %empty { local_symbol_table.increase_level(); }


%%
#include <stdio.h>

extern char yytext[];
extern unsigned int column;
extern std::stringstream text;

void yyerror(const char *s)
//char *s;
{
	fflush(stdout);
	error_msg(std::string(s),prev_line_num,prev_column);
//	printf("Line %d:%d ERROR : %s\n",prev_line_num,prev_column,s);
//	std::cout << text.str(); 
//	printf("\n%*s\n", column, "^");
//	error_flag = 1;
}
