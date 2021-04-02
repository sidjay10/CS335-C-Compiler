// symtab.cpp

#include <assert.h>
#include <ast.h>
#include <iostream>
#include <sstream>
#include <symtab.h>
#include <y.tab.h>

LocalSymbolTable local_symbol_table;
GlobalSymbolTable global_symbol_table;

//##############################################################################
//################################ POINTER #####################################
//##############################################################################

Pointer::Pointer() : Non_Terminal( "pointer" ){};

Pointer::Pointer( TypeQualifierList *type_list, Pointer *pointer_ )
    : Non_Terminal( "pointer" ), type_qualifier_list( type_list ),
      pointer( pointer_ ){};

Pointer *create_pointer() {
    Pointer *p = new Pointer();
    return p;
}

Pointer *create_pointer( TypeQualifierList *type_list, Pointer *pointer ) {
    Pointer *p = new Pointer( type_list, pointer );
    p->add_children( type_list, pointer );
    return p;
}

//##############################################################################
//########################### TYPE QUALIFIER LIST ##############################
//##############################################################################

TypeQualifierList ::TypeQualifierList()
    : Non_Terminal( "type_qualifier_list" ){};

TypeQualifierList ::~TypeQualifierList() { type_qualifier_list.clear(); };

void TypeQualifierList ::append_to_list( TYPE_QUALIFIER type ) {
    type_qualifier_list.push_back( type );
}

TypeQualifierList *create_type_qualifier_list( TYPE_QUALIFIER type ) {
    TypeQualifierList *tql = new TypeQualifierList();
    tql->append_to_list( type );
    return tql;
}

TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList *tql,
                                               TYPE_QUALIFIER type ) {
    tql->append_to_list( type );
    return tql;
}

//##############################################################################
//############################# DECLARATION ####################################
//##############################################################################

Declaration ::Declaration( DeclarationSpecifiers *declaration_specifiers_,
                           DeclaratorList *init_declarator_list_ )
    : Non_Terminal( "declaration" ),
      declaration_specifiers( declaration_specifiers_ ),
      init_declarator_list( init_declarator_list_ ){};

Declaration *new_declaration( DeclarationSpecifiers *declaration_specifiers,
                              DeclaratorList *init_declarator_list ) {
    Declaration *d =
        new Declaration( declaration_specifiers, init_declarator_list );
    d->add_children( declaration_specifiers, init_declarator_list );
    return d;
}

void Declaration::add_to_symbol_table( LocalSymbolTable &sym_tab ) {

    if ( init_declarator_list == nullptr )
        return;
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        for ( int i = 0; i < sym_tab.current_level; i++ ) {
            std::cout << "  ";
        }
        std::cout << ( *i )->id->value << " " << sym_tab.current_level << "\n";
    }
};

void Declaration::add_to_symbol_table( GlobalSymbolTable &sym_tab ) {

    if ( init_declarator_list == nullptr )
        return;
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        std::cout << ( *i )->id->value << "\n";
    }
};

//##############################################################################
//########################## DECLARATION SPECIFIERS ############################
//##############################################################################

DeclarationSpecifiers ::DeclarationSpecifiers()
    : Non_Terminal( "declaration_specifiers" ){};

DeclarationSpecifiers *new_storage_class( STORAGE_CLASS sc ) {
    DeclarationSpecifiers *ds = new DeclarationSpecifiers();
    ds->storage_class.push_back( sc );
    return ds;
}

DeclarationSpecifiers *new_type_specifier( TypeSpecifier *ts ) {
    DeclarationSpecifiers *ds = new DeclarationSpecifiers();
    ds->type_specifier.push_back( ts );
    ds->add_child( ts );
    return ds;
}

DeclarationSpecifiers *new_type_qualifier( TYPE_QUALIFIER tq ) {
    DeclarationSpecifiers *ds = new DeclarationSpecifiers();
    ds->type_qualifier.push_back( tq );
    return ds;
}

DeclarationSpecifiers *add_storage_class( DeclarationSpecifiers *ds,
                                          STORAGE_CLASS sc ) {
    ds->storage_class.push_back( sc );
    return ds;
}

DeclarationSpecifiers *add_type_specifier( DeclarationSpecifiers *ds,
                                           TypeSpecifier *ts ) {
    ds->type_specifier.push_back( ts );
    ds->add_child( ts );
    return ds;
}

DeclarationSpecifiers *add_type_qualifier( DeclarationSpecifiers *ds,
                                           TYPE_QUALIFIER tq ) {
    ds->type_qualifier.push_back( tq );
    return ds;
}

//##############################################################################
//########################### DECLARATION LIST #################################
//##############################################################################

DeclarationList::DeclarationList() : Non_Terminal( "declaration_list" ){};

DeclarationList *create_declaration_list( Declaration *declaration ) {
    DeclarationList *dl = new DeclarationList();
    dl->declarations.push_back( declaration );
    dl->add_child( declaration );
    local_symbol_table.increase_level();
    declaration->add_to_symbol_table( local_symbol_table );
    return dl;
}
DeclarationList *add_to_declaration_list( DeclarationList *declaration_list,
                                          Declaration *declaration ) {

    declaration_list->declarations.push_back( declaration );
    declaration_list->add_child( declaration );
    declaration->add_to_symbol_table( local_symbol_table );
    return declaration_list;
}

//##############################################################################
//############################# DECLARATOR LIST ################################
//##############################################################################

DeclaratorList ::DeclaratorList() : Non_Terminal( "init_declarator_list" ){};

DeclaratorList *create_init_declarator_list( Declarator *d ) {
    DeclaratorList *dl = new DeclaratorList();
    dl->declarator_list.push_back( d );
    dl->add_child( d );
    return dl;
}

DeclaratorList *add_to_init_declarator_list( DeclaratorList *dl,
                                             Declarator *d ) {
    dl->declarator_list.push_back( d );
    dl->add_child( d );
    return dl;
}

//##############################################################################
//################################# DECLARATOR #################################
//##############################################################################

Declarator ::Declarator( Pointer *p, DirectDeclarator *dd )
    : Non_Terminal( "declarator" ), pointer( p ), direct_declarator( dd ) {
    assert( dd != nullptr );
    assert( dd->id != nullptr );
    id = dd->id;
};

Declarator *create_declarator( Pointer *pointer,
                               DirectDeclarator *direct_declarator ) {
    Declarator *d = new Declarator( pointer, direct_declarator );
    d->add_children( pointer, direct_declarator );
    return d;
}

Declarator *add_initializer_to_declarator( Declarator *declarator, Node *ie ) {
    declarator->init_expr = ie;
    Identifier *id = new Identifier( declarator->id->value.c_str() );
    declarator->add_child( create_non_term( "=", id, ie ) );
    return declarator;
}

//##############################################################################
//############################ DIRECT DECLARATOR ###############################
//##############################################################################

DirectDeclarator ::DirectDeclarator()
    : Non_Terminal( "direct_declarator" ), type( ID ), id( nullptr ),
      declarator( nullptr ), direct_declarator( nullptr ),
      const_expr( nullptr ), params( nullptr ){};

DirectDeclarator *create_dir_declarator_id( DIRECT_DECLARATOR_TYPE type,
                                            Identifier *id ) {
    assert( type == ID );
    DirectDeclarator *dd = new DirectDeclarator();
    dd->type = type;
    assert( id != nullptr );
    dd->id = id;
    dd->add_child( id );
    return dd;
}

DirectDeclarator *create_dir_declarator_dec( DIRECT_DECLARATOR_TYPE type,
                                             Declarator *declarator ) {
    assert( type == DECLARATOR );
    DirectDeclarator *dd = new DirectDeclarator();
    dd->type = type;
    assert( declarator != nullptr );
    dd->declarator = declarator;
    assert( declarator->direct_declarator != nullptr );
    assert( declarator->direct_declarator->id != nullptr );
    dd->id = declarator->direct_declarator->id;
    dd->add_child( declarator );
    return dd;
}

DirectDeclarator *
create_dir_declarator_arr( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           Node *const_expr ) {
    assert( type == ARRAY );
    DirectDeclarator *dd = new DirectDeclarator();
    dd->name = "direct_declarator_array";
    dd->type = type;
    assert( direct_declarator != nullptr );
    dd->direct_declarator = direct_declarator;
    dd->const_expr = const_expr;
    assert( direct_declarator->id != nullptr );
    dd->id = direct_declarator->id;
    dd->add_children( direct_declarator, const_expr );
    return dd;
}

DirectDeclarator *
create_dir_declarator_fun( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator, Node *params ) {
    assert( type == FUNCTION );
    DirectDeclarator *dd = new DirectDeclarator();
    dd->name = "direct_declarator_function";
    dd->type = type;
    assert( direct_declarator != nullptr );
    dd->direct_declarator = direct_declarator;
    dd->params = params;
    dd->id = direct_declarator->id;
    assert( direct_declarator->id != nullptr );
    dd->add_children( direct_declarator, params );
    return dd;
}

//##############################################################################
//################################ IDENTIFIER ##################################
//##############################################################################

Identifier::Identifier( const char *name ) : Terminal( "IDENTIFIER", name ){};

//##############################################################################
//########################## FUNCTION DEFINITION ###############################
//##############################################################################

FunctionDefinition::FunctionDefinition(
    DeclarationSpecifiers *declaration_specifiers_, Declarator *declarator_,
    Node *compound_statement_ )
    : Non_Terminal( "function_definiton" ),
      declaration_specifiers( declaration_specifiers_ ),
      declarator( declarator_ ), compound_statement( compound_statement_ ){};

FunctionDefinition *
create_function_defintion( DeclarationSpecifiers *declaration_specifiers,
                           Declarator *declarator, Node *compound_statement ) {
    FunctionDefinition *fd = new FunctionDefinition(
        declaration_specifiers, declarator, compound_statement );
    fd->add_children( declaration_specifiers, declarator, compound_statement );
    global_symbol_table.add_symbol( declaration_specifiers, declarator );
    return fd;
}

//##############################################################################
//############################## TYPE SPECIFIER ################################
//##############################################################################

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER typ )
    : Non_Terminal( "type_specifier" ), type( typ ), id( nullptr ),
      struct_declaration_list( nullptr ), enumerator_list( nullptr ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              StructDeclarationList *struct_declaration_list_ )
    : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
      struct_declaration_list( struct_declaration_list_ ),
      enumerator_list( nullptr ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              EnumeratorList *enumerator_list_ )
    : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
      struct_declaration_list( nullptr ), enumerator_list( enumerator_list_ ){};

TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type ) {
    assert( type != UNION && type != STRUCT && type != ENUM );
    TypeSpecifier *ts = new TypeSpecifier( type );
    std::stringstream ss;
    ss << "type_specifier : ";
    switch ( type ) {
    case INT:
        ss << "INT";
        break;
    case VOID:
        ss << "VOID";
        break;
    case CHAR:
        ss << "CHAR";
        break;
    case SHORT:
        ss << "SHORT";
        break;
    case LONG:
        ss << "LONG";
        break;
    case FLOAT:
        ss << "FLOAT";
        break;
    case DOUBLE:
        ss << "DOUBLE";
        break;
    case SIGNED:
        ss << "SIGNED";
        break;
    case UNSIGNED:
        ss << "UNSIGNED";
        break;
    case TYPE_NAME:
        ss << "TYPE_NAME";
        break;
    default:
        assert( 0 );
    }
    ts->name = ss.str();
    return ts;
}

TypeSpecifier *
create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                       StructDeclarationList *struct_declaration_list ) {
    assert( type == UNION || type == STRUCT );
    TypeSpecifier *ts = new TypeSpecifier( type, id, struct_declaration_list );
    std::stringstream ss;
    ss << "type_specifier : ";
    switch ( type ) {
    case UNION:
        ss << "UNION";
        break;
    case STRUCT:
        ss << "STRUCT";
        break;
    default:
        assert( 0 );
    }
    ts->name = ss.str();
    ts->add_children( id, struct_declaration_list );
    return ts;
}

TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                                      EnumeratorList *enumerator_list ) {
    assert( type == ENUM );
    TypeSpecifier *ts = new TypeSpecifier( type, id, enumerator_list );
    std::stringstream ss;
    ss << "type_specifier : ENUM";
    ts->name = ss.str();
    ts->add_children( id, enumerator_list );
    return ts;
}

//##############################################################################
//######################### STRUCT DECLARATION LIST ############################
//##############################################################################

StructDeclarationList::StructDeclarationList()
    : Non_Terminal( "struct_declaration_list" ){};

StructDeclarationList *
create_struct_declaration_list( StructDeclaration *struct_declaration ) {

    StructDeclarationList *sdl = new StructDeclarationList();

    sdl->struct_declaration_list.push_back( struct_declaration );

    sdl->add_child( struct_declaration );

    return sdl;
}

StructDeclarationList *
add_to_struct_declaration_list( StructDeclarationList *struct_declaration_list,
                                StructDeclaration *struct_declaration ) {

    struct_declaration_list->struct_declaration_list.push_back(
        struct_declaration );

    struct_declaration_list->add_child( struct_declaration );

    return struct_declaration_list;
}

//##############################################################################
//############################ STRUCT DECLARATION ##############################
//##############################################################################

StructDeclaration::StructDeclaration( SpecifierQualifierList *sq_list_,
                                      DeclaratorList *declarator_list_ )
    : Non_Terminal( "struct_declaration" ), sq_list( sq_list_ ),
      declarator_list( declarator_list_ ){};

StructDeclaration *
create_struct_declaration( SpecifierQualifierList *sq_list,
                           DeclaratorList *struct_declarator_list ) {
    StructDeclaration *sd =
        new StructDeclaration( sq_list, struct_declarator_list );
    sd->add_children( sq_list, struct_declarator_list );
    return sd;
}

Declarator *verify_struct_declarator( Declarator *declarator ) {
    // TODO: Fill this in
    return declarator;
}

//##############################################################################
//######################### SPECIFIER QUALIFIER LIST ###########################
//##############################################################################

SpecifierQualifierList::SpecifierQualifierList()
    : Non_Terminal( "specifier_qualifier_list" ){};

SpecifierQualifierList *
create_type_specifier_sq( TypeSpecifier *type_specifier ) {
    SpecifierQualifierList *sql = new SpecifierQualifierList();
    sql->type_specifiers.push_back( type_specifier );
    sql->add_child( type_specifier );
    return sql;
}

SpecifierQualifierList *
create_type_qualifier_sq( TYPE_QUALIFIER type_qualifier ) {
    SpecifierQualifierList *sql = new SpecifierQualifierList();
    sql->type_qualifiers.push_back( type_qualifier );
    // sql->add_child(type_qualifier);
    return sql;
}

SpecifierQualifierList *add_type_specifier_sq( SpecifierQualifierList *sq_list,
                                               TypeSpecifier *type_specifier ) {
    sq_list->type_specifiers.push_back( type_specifier );
    sq_list->add_child( type_specifier );
    return sq_list;
}

SpecifierQualifierList *add_type_qualifier_sq( SpecifierQualifierList *sq_list,
                                               TYPE_QUALIFIER type_qualifier ) {
    sq_list->type_qualifiers.push_back( type_qualifier );
    // sq_list->add_child(type_qualifier);
    return sq_list;
}

//##############################################################################
//################################ ENUMERATOR ##################################
//##############################################################################

Enumerator::Enumerator( Identifier *id_, Node *const_expr )
    : Non_Terminal( "enumerator" ), id( id_ ), init_expr( const_expr ){};

Enumerator *create_enumerator( Identifier *id, Node *const_expr ) {
    Enumerator *e = new Enumerator( id, const_expr );
    e->add_children( id, const_expr );
    return e;
}

//##############################################################################
//############################## ENUMERATOR LIST ###############################
//##############################################################################

EnumeratorList::EnumeratorList() : Non_Terminal( "enumerator_list" ){};

EnumeratorList *create_enumerator_list( Enumerator *enumerator ) {
    EnumeratorList *el = new EnumeratorList();
    el->enumerator_list.push_back( enumerator );
    el->add_child( enumerator );
    return el;
}

EnumeratorList *add_to_enumerator_list( EnumeratorList *enumerator_list,
                                        Enumerator *enumerator ) {
    enumerator_list->enumerator_list.push_back( enumerator );
    enumerator_list->add_child( enumerator );
    return enumerator_list;
}

//##############################################################################
//############################ LOCAL SYMBOL TABLE ##############################
//##############################################################################

SymbolTable::SymbolTable(){};

LocalSymbolTable::LocalSymbolTable() : current_level( 0 ){};

void LocalSymbolTable::increase_level() {
    for ( int i = 0; i < current_level; i++ ) {
        std::cout << "  ";
    }
    std::cout << "{\n";
    current_level++;
}

void LocalSymbolTable::clear_current_level() {
    current_level--;
    for ( int i = 0; i < current_level; i++ ) {
        std::cout << "  ";
    }
    std::cout << "}\n";
}

//##############################################################################
//########################### GLOBAL SYMBOL TABLE ##############################
//##############################################################################

void GlobalSymbolTable::add_symbol(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    std::cout << declarator->id->value << "\n";
}

Node *add_to_global_symbol_table( Declaration *declaration ) {
    declaration->add_to_symbol_table( global_symbol_table );
    return declaration;
}

//##############################################################################
//########################### SYMBOL TABLE ENTRY  ##############################
//##############################################################################

// SymTabEntry::SymTabEntry ( std::string name, int level ) : name(name),
// level(level) {};

//##############################################################################
