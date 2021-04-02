#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "ast.h"
#include <map>
#include <string>

class SymTabEntry {
  public:
    std::string name;
    std::string type;
    int level;

    // TODO: This needs to be expanded
//    SymTabEntry( std::string name, int level );
};

class FuncEnt : public SymTabEntry {
  public:
    std::string args;

//    FuncEnt( std::string name, std::string type, std::string args );
};

class DeclarationSpecifiers;
class Declarator;

class SymbolTable {

  public:
    std::map<std::string, std::vector<SymTabEntry *>> sym_table;

    SymbolTable();

    void add_to_table( SymTabEntry *symbol );
    SymTabEntry *get_symbol_from_table( std::string name );
    void delete_from_table( SymTabEntry *symbol );
    void print_table();
};

class GlobalSymbolTable : public SymbolTable {
  public:
    void add_symbol( DeclarationSpecifiers *declaration_specifiers,
                     Declarator *declarator );
};

class LocalSymbolTable : public SymbolTable {
  public:
    std::string function_name;
    int current_level;
    void increase_level();
    void clear_current_level();
    void empty_table();
    LocalSymbolTable();
};

extern LocalSymbolTable local_symbol_table;
extern GlobalSymbolTable global_symbol_table;

typedef int TYPE_QUALIFIER;

class Identifier : public Terminal {
  public:
    Identifier( const char *name );
};

class TypeQualifierList : public Non_Terminal {
  public:
    std::vector<TYPE_QUALIFIER> type_qualifier_list;

    TypeQualifierList();

    void append_to_list( TYPE_QUALIFIER type );

    ~TypeQualifierList();
};

class Pointer : public Non_Terminal {
  public:
    TypeQualifierList *type_qualifier_list;
    Pointer *pointer;

    Pointer();
    Pointer( TypeQualifierList *type_qualifier_list, Pointer *pointer );
};

class DirectDeclarator;

class Declarator : public Non_Terminal {
  public:
    Identifier *id;
    Pointer *pointer;
    DirectDeclarator *direct_declarator;
    Node *init_expr;

    Declarator();
    Declarator( Pointer *p, DirectDeclarator *dd );
};

typedef enum direct_declartor_enum {
    ID,
    DECLARATOR,
    ARRAY,
    FUNCTION,
    NUM_DIRECT_DECLARATORS

} DIRECT_DECLARATOR_TYPE;

class DirectDeclarator : public Non_Terminal {

  public:
    DIRECT_DECLARATOR_TYPE type;

    Identifier *id;
    Declarator *declarator;
    DirectDeclarator *direct_declarator;

#if 0
		//TODO: Add these in
		Constant_Expression * const_expr;
		Parameter_Type_List * params;
#endif

    Node *const_expr;
    Node *params;

    DirectDeclarator();
};

class DeclaratorList : public Non_Terminal {
  public:
    std::vector<Declarator *> declarator_list;
    DeclaratorList();
};

typedef int STORAGE_CLASS;
class TypeSpecifier;

class DeclarationSpecifiers : public Non_Terminal {
  public:
    std::vector<STORAGE_CLASS> storage_class;
    std::vector<TypeSpecifier *> type_specifier;
    std::vector<TYPE_QUALIFIER> type_qualifier;

    DeclarationSpecifiers();
};

class Declaration : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    DeclaratorList *init_declarator_list;

    Declaration( DeclarationSpecifiers *declaration_specifiers_,
                 DeclaratorList *init_declarator_list_ );
    void add_to_symbol_table( LocalSymbolTable &sym_tab );
    void add_to_symbol_table( GlobalSymbolTable &sym_tab );
};

class DeclarationList : public Non_Terminal {
  public:
    std::vector<Declaration *> declarations;
    DeclarationList();

    void create_symbol_table_level();
};

class FunctionDefinition : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    Declarator *declarator;
    Node *compound_statement;

    FunctionDefinition( DeclarationSpecifiers *declaration_specifiers_,
                        Declarator *declarator_, Node *compound_statement_ );
};

class AbstractDeclarator;

typedef enum {
    ABSTRACT_DECLARATOR,
    // TODO: Fill this

} DIRECT_ABSTRACT_DECLARATOR_TYPE;

class DirectAbstractDeclarator : public Non_Terminal {
  public:
    DIRECT_ABSTRACT_DECLARATOR_TYPE type;
    AbstractDeclarator *abstract_declarator;
    Node *const_expr;
    DirectAbstractDeclarator *direct_abstract_declarator;
    // Parameter_Type_List *parameter_type_list;
};

class AbstractDeclarator : public Non_Terminal {
  public:
    Pointer *pointer;
    DirectAbstractDeclarator *direct_abstract_declarator;
};

class ParameterDeclaration : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    Declarator *declarator;
};

class SpecifierQualifierList : public Non_Terminal {
  public:
    std::vector<TypeSpecifier *> type_specifiers;
    std::vector<TYPE_QUALIFIER> type_qualifiers;

    SpecifierQualifierList();
};

class StructDeclaration : public Non_Terminal {
  public:
    SpecifierQualifierList *sq_list;
    DeclaratorList *declarator_list;

    StructDeclaration( SpecifierQualifierList *sq_list_,
                       DeclaratorList *declarator_list_ );
};

class StructDeclarationList : public Non_Terminal {
  public:
    std::vector<StructDeclaration *> struct_declaration_list;
    StructDeclarationList();
};

class Enumerator : public Non_Terminal {
  public:
    Identifier *id;
    Node *init_expr;
    Enumerator( Identifier *id_, Node *init_expr );
};

class EnumeratorList : public Non_Terminal {
  public:
    std::vector<Enumerator *> enumerator_list;
    EnumeratorList();
};

typedef int TYPE_SPECIFIER;

class TypeSpecifier : public Non_Terminal {
  public:
    TYPE_SPECIFIER type;
    Identifier *id;
    StructDeclarationList *struct_declaration_list;
    EnumeratorList *enumerator_list;

    TypeSpecifier( TYPE_SPECIFIER typ );
    TypeSpecifier( TYPE_SPECIFIER type, Identifier *Id,
                   StructDeclarationList *struct_declaration_list );
    TypeSpecifier( TYPE_SPECIFIER type, Identifier *Id,
                   EnumeratorList *enumerator_list );
};

class ParameterTypeList : public Non_Terminal {
  public:
    std::vector<ParameterDeclaration *> parameter_list;
    bool ellipsis;
};

Pointer *create_pointer();
Pointer *create_pointer( TypeQualifierList *type_list, Pointer *pointer );

TypeQualifierList *create_type_qualifier_list( TYPE_QUALIFIER type );
TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList *tql,
                                               TYPE_QUALIFIER type );

Node *add_to_global_symbol_table( Declaration *declaration );

Declaration *new_declaration( DeclarationSpecifiers *declaraion_specifiers,
                              DeclaratorList *init_declarator_list );

DeclarationSpecifiers *new_storage_class( STORAGE_CLASS sc );
DeclarationSpecifiers *new_type_specifier( TypeSpecifier *ts );
DeclarationSpecifiers *new_type_qualifier( TYPE_QUALIFIER tq );

DeclarationSpecifiers *add_storage_class( DeclarationSpecifiers *ds,
                                          STORAGE_CLASS sc );
DeclarationSpecifiers *add_type_specifier( DeclarationSpecifiers *ds,
                                           TypeSpecifier *ts );
DeclarationSpecifiers *add_type_qualifier( DeclarationSpecifiers *ds,
                                           TYPE_QUALIFIER tq );

DeclaratorList *create_init_declarator_list( Declarator *init_declarator );
DeclaratorList *
add_to_init_declarator_list( DeclaratorList *init_declarator_list,
                             Declarator *init_declarator );

Declarator *add_initializer_to_declarator( Declarator *declarator,
                                           Node *init_expr );

DirectDeclarator *create_dir_declarator_id( DIRECT_DECLARATOR_TYPE type,
                                            Identifier *id );
DirectDeclarator *create_dir_declarator_dec( DIRECT_DECLARATOR_TYPE type,
                                             Declarator *declarator );
DirectDeclarator *
create_dir_declarator_arr( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           Node *const_expr );
DirectDeclarator *
create_dir_declarator_fun( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator, Node *params );

Declarator *create_declarator( Pointer *pointer,
                               DirectDeclarator *direct_declarator );

DeclarationList *create_declaration_list( Declaration *declaraiton );
DeclarationList *add_to_declaration_list( DeclarationList *declaraiton_list,
                                          Declaration *Declaration );

FunctionDefinition *
create_function_defintion( DeclarationSpecifiers *declaration_specifiers,
                           Declarator *declarator, Node *compound_statement );

TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type );
TypeSpecifier *
create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                       StructDeclarationList *struct_declaration_list );
TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                                      EnumeratorList *enumerator_list );

StructDeclarationList *
create_struct_declaration_list( StructDeclaration *struct_declaration );
StructDeclarationList *
add_to_struct_declaration_list( StructDeclarationList *struct_declaration_list,
                                StructDeclaration *struct_declaration );

StructDeclaration *
create_struct_declaration( SpecifierQualifierList *sq_list,
                           DeclaratorList *struct_declarator_list );

SpecifierQualifierList *
create_type_specifier_sq( TypeSpecifier *type_specifier );
SpecifierQualifierList *
create_type_qualifier_sq( TYPE_QUALIFIER type_qualifier );
SpecifierQualifierList *add_type_specifier_sq( SpecifierQualifierList *sq_list,
                                               TypeSpecifier *type_specifier );
SpecifierQualifierList *add_type_qualifier_sq( SpecifierQualifierList *sq_list,
                                               TYPE_QUALIFIER type_qualifier );

Declarator *verify_struct_declarator( Declarator *declarator );

Enumerator *create_enumerator( Identifier *id, Node *const_expr );

EnumeratorList *create_enumerator_list( Enumerator *enumerator );
EnumeratorList *add_to_enumerator_list( EnumeratorList *enumerator_list,
                                        Enumerator *enumerator );

#endif
