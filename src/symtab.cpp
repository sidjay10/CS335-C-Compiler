// symtab.cpp

#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <symtab.h>
#include <vector>
#include <y.tab.h>
#include <algorithm>

LocalSymbolTable local_symbol_table;
GlobalSymbolTable global_symbol_table;

//##############################################################################
//################################ POINTER #####################################
//##############################################################################
void is_Valid(TypeQualifierList *ts){
     for(int i=0; i < ts->type_qualifier_list.size(); i++){
        if(ts->type_qualifier_list.at(i)==CONST||ts->type_qualifier_list.at(i)==VOLATILE) {}
        else {
            std::cout<<"Error in pointer type qualfier pointer";
            break;}
        }
}

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
    is_Valid(type_list);
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
    is_Valid(declaration_specifiers);
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
        SymTabEntry *e = new SymTabEntry( ( *i )->id->value );
        sym_tab.add_to_table( e );
        std::cout << ( *i )->id->value << " " << sym_tab.current_level << "\n";
    }
};

void Declaration::add_to_symbol_table( GlobalSymbolTable &sym_tab ) {

    if ( init_declarator_list == nullptr )
        return;
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        std::cout << "G: " << ( *i )->id->value << "\n";
    }
};

//##############################################################################
//########################## DECLARATION SPECIFIERS ############################
//##############################################################################

DeclarationSpecifiers ::DeclarationSpecifiers()
    : Non_Terminal( "declaration_specifiers" ){};


void is_Valid(DeclarationSpecifiers *ds){
    int err=0;
    if(ds->storage_class.size()==1){
        if(ds->storage_class.at(0)==TYPEDEF) err=0;
        else err+=1;
        }

    std::vector<int> ty;
    
    for(int i=0; i < ds->type_specifier.size(); i++)
    ty.push_back(ds->type_specifier.at(i)->type);
    std::sort(ty.begin(),ty.end());

    if(ty.size()==3){
        if ((ty.at(0)==UNSIGNED || ty.at(0)==SIGNED) && (ty.at(1)==SHORT || ty.at(1)==LONG) && ty.at(2)==INT) {}
        else err+=2;
        }
    else if (ty.size()==2){
        if ((ty.at(0)==UNSIGNED || ty.at(0)==SIGNED) && (ty.at(1)==SHORT || ty.at(1)==LONG || ty.at(1)==INT || ty.at(1)==CHAR)) {}
        else if ((ty.at(0)==SHORT || ty.at(0)==LONG) && ty.at(1)==INT) {}
        else if (ty.at(0)==LONG && ty.at(1)==DOUBLE) {}
        else err+=2;
        }
    else if (ty.size()==1) 
        if (ty.at(0)==SHORT || ty.at(0)==LONG || ty.at(0)==INT || ty.at(0)==CHAR || ty.at(0)==FLOAT||ty.at(0)==DOUBLE || ty.at(0)==STRUCT ||ty.at(0)==UNION || ty.at(0)==ENUM){}
    else err+=2;

    for(int i=0; i < ds->type_qualifier.size(); i++){
        if(ds->type_qualifier.at(i)==CONST||ds->type_qualifier.at(i)==VOLATILE) {}
        else {err+=4;break;}
        }
    //for(int i=0; i < ds->type_qualifier.size(); i++)
    //std::cout << ds->type_qualifier.at(i) << ' ';
    if (err&1) std::cout << "Error in strorage class declarator";
    if (err&2) std::cout << "Error in type specifier declarator";
    if (err&4) std::cout << "Error in type qualifier declarator";

}

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
                           DirectDeclarator *direct_declarator,
                           ParameterTypeList *params ) {
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
//######################## DIRECT ABSTRACT DECLARATOR ##########################
//##############################################################################

DirectAbstractDeclarator::DirectAbstractDeclarator()
    : Non_Terminal( "direct_abstract_declarator" ), type( ABSTRACT ),
      abstract_declarator( nullptr ), const_expr( nullptr ),
      direct_abstract_declarator( nullptr ), parameter_type_list( nullptr ){};

DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ ) {
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    dad->type = typ;
    return dad;
}

DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   AbstractDeclarator *abs ) {
    assert( typ == ABSTRACT );
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    dad->type = typ;
    dad->abstract_declarator = abs;
    dad->add_child( abs );
    return dad;
}

DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs ) {
    assert( typ == ROUND || typ == SQUARE );
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    dad->type = typ;
    dad->direct_abstract_declarator = dabs;
    dad->add_child( dabs );
    return dad;
}

DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs, Node *ce ) {
    assert( typ == SQUARE );
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    dad->type = typ;
    dad->direct_abstract_declarator = dabs;
    dad->const_expr = ce;
    dad->add_children( dabs, ce );
    return dad;
}
DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs,
                                   ParameterTypeList *param_list ) {

    assert( typ == ROUND );
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    dad->type = typ;
    dad->direct_abstract_declarator = dabs;
    dad->parameter_type_list = param_list;
    dad->add_children( dabs, param_list );
    return dad;
}

//##############################################################################
//########################### ABSTRACT DECLARATOR ##############################
//##############################################################################

AbstractDeclarator::AbstractDeclarator( Pointer *ptr,
                                        DirectAbstractDeclarator *dabs )
    : Non_Terminal( "abstract_declarator" ), pointer( ptr ),
      direct_abstract_declarator( dabs ){};

AbstractDeclarator *
create_abstract_declarator( Pointer *ptr, DirectAbstractDeclarator *dabs ) {
    AbstractDeclarator *abs = new AbstractDeclarator( ptr, dabs );
    abs->add_children( ptr, dabs );
    return abs;
}
//##############################################################################
//########################### PARAMETER DECLARATION ############################
//##############################################################################
ParameterDeclaration::ParameterDeclaration()
    : Non_Terminal( "parameter_declaration" ){};

ParameterDeclaration *create_parameter_declaration( DeclarationSpecifiers *ds,
                                                    Declarator *d,
                                                    AbstractDeclarator *ad ) {
    ParameterDeclaration *pd = new ParameterDeclaration();
    pd->declaration_specifiers = ds;
    pd->declarator = d;
    pd->abstract_declarator = ad;
    pd->add_children( ds, d, ad );
    return pd;
}
//##############################################################################
//############################ PARAMETER TYPE LIST #############################
//##############################################################################

ParameterTypeList::ParameterTypeList()
    : Non_Terminal( "parameter_type_list" ){};

ParameterTypeList *create_parameter_list( ParameterDeclaration *pd ) {
    ParameterTypeList *ptl = new ParameterTypeList();
    ptl->param_list.push_back( pd );
    ptl->add_child( pd );
    return ptl;
}

ParameterTypeList *add_to_parameter_list( ParameterTypeList *ptl,
                                          ParameterDeclaration *pd ) {
    ptl->param_list.push_back( pd );
    ptl->add_child( pd );
    return ptl;
}

ParameterTypeList *add_ellipsis_to_list( ParameterTypeList *ptl ) {
    ptl->has_ellipsis = true;
    ptl->add_child( create_terminal( "...", NULL ) );
    return ptl;
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
    local_symbol_table.add_function( declaration_specifiers, declarator );
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

void SymbolTable::add_to_table( SymTabEntry * ) {}

SymTabEntry *SymbolTable::get_symbol_from_table( std::string name ) {
    return nullptr;
}

LocalSymbolTable::LocalSymbolTable() : current_level( 0 ){};

void LocalSymbolTable::increase_level() {
    for ( int i = 0; i < current_level; i++ ) {
        std::cout << "  ";
    }
    std::cout << "{\n";
    current_level++;
}

void LocalSymbolTable::clear_current_level() {

    for ( auto it = sym_table.begin(); it != sym_table.end(); it++ ) {
        if (it->second.empty()){
            continue;
        }
        SymTabEntry *entry = ( it->second ).front();
        if ( !entry ) {
            continue;
        } else if ( entry->level == current_level ) {
            it->second.pop_front();
        }
    }
    current_level--;
    for ( int i = 0; i < current_level; i++ ) {
        std::cout << "  ";
    }
    std::cout << "}\n";
}

void LocalSymbolTable::add_to_table( SymTabEntry *symbol ) {

    auto it = sym_table.find( symbol->name );
    if ( it == sym_table.end() ) {
        std::deque<SymTabEntry *> &q = *new std::deque<SymTabEntry *>;
        symbol->level = current_level;
        q.push_front( symbol );
        sym_table.insert( {symbol->name, q} );
    } else {
        std::deque<SymTabEntry *> &q = it->second;
        if ( q.front() && ( q.front() )->level == current_level ) {
            // Can't insert two symbols with same name at the same level
            std::cerr << "ERROR: " << it->first << " " << symbol->name << "\n";
            assert( 0 );
        } else {
            symbol->level = current_level;
            q.push_front( symbol );
        }
    }
}

SymTabEntry *LocalSymbolTable::get_symbol_from_table( std::string name ) {
    auto it = sym_table.find( name );
    if ( it == sym_table.end() ) {
        return nullptr;
    } else {
        return it->second.front();
    }
}

void LocalSymbolTable::add_function(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    sym_table.clear();

    assert( declarator->direct_declarator->type == FUNCTION );
    function_name = declarator->id->value;
    std::cout << "L: " << function_name ;

    // Check whether the arguements are of the form ( )
    if ( declarator->direct_declarator->params == nullptr ) {
	std:: cout << "( )\n";
        return;
    }
    std::vector<ParameterDeclaration *> &param_list =
        declarator->direct_declarator->params->param_list;

    // Check whether the arguements are of the form ( void )
    if ( param_list.size() == 1 ) {
        auto it = param_list.begin();
        if ( ( *it )->declarator == nullptr &&
             ( *it )->abstract_declarator == nullptr ) {
            std::vector<TypeSpecifier *> &v =
                ( *it )->declaration_specifiers->type_specifier;
            if ( v.size() == 1 && ( *v.begin() )->type == VOID ) {
		std:: cout << "( void )\n";
                return;
            }
        }
    }
    // Parameters need to be added at level one, we avoid calling
    // increase_level and clear_from_level to avoid thier side effects

    current_level = 1;
	std::cout << "( ";
    for ( auto it = param_list.begin(); it != param_list.end(); it++ ) {

        if ( ( *it )->declarator == nullptr ||
             ( *it )->declarator->id == nullptr ) {
            std::cerr << "Declarator requires identifier\n";
            assert( 0 );
        }

        std::cout << ( *it )->declarator->id->value << ", ";
        SymTabEntry *symbol = new SymTabEntry( ( *it )->declarator->id->value );
        add_to_table( symbol );
    }
    current_level = 0;
	std::cout << " )\n";
}

//##############################################################################
//########################### GLOBAL SYMBOL TABLE ##############################
//##############################################################################

void GlobalSymbolTable::add_symbol(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    std::cout << "G: " << declarator->id->value << "\n";
}

Node *add_to_global_symbol_table( Declaration *declaration ) {
    declaration->add_to_symbol_table( global_symbol_table );
    return declaration;
}

//##############################################################################
//########################### SYMBOL TABLE ENTRY  ##############################
//##############################################################################

SymTabEntry::SymTabEntry( std::string name ) : name( name ){};

//##############################################################################
