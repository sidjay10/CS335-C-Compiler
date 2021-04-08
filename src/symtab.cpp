// symtab.cpp

#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <symtab.h>
#include <expression.h>
#include <vector>
#include <y.tab.h>
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>

LocalSymbolTable local_symbol_table;
GlobalSymbolTable global_symbol_table;
std::vector<Types> defined_types;
unsigned int anon_count = 0;
extern int line_num;

std::string primitive_type_name( PrimitiveTypes type ) {
    std::stringstream ss;
    switch ( type ) {
    case U_CHAR_T:
        ss << "unsigned char";
        break;
    case CHAR_T:
        ss << "char";
        break;
    case U_SHORT_T:
        ss << "unsigned short";
        break;
    case SHORT_T:
        ss << "short";
        break;
    case U_INT_T:
        ss << "unsigned int";
        break;
    case INT_T:
        ss << "int";
        break;
    case U_LONG_T:
        ss << "unsigned long";
        break;
    case LONG_T:
        ss << "long";
        break;
    case FLOAT_T:
        ss << "float";
        break;
    case DOUBLE_T:
        ss << "double";
        break;
    case LONG_DOUBLE_T:
        ss << "long double";
        break;
    case VOID_T:
        ss << "void";
        break;
    default:
        std::cerr << "Invalid Type" << type << "\n";
        assert( 0 );
    }
    return ss.str();
}
size_t primitive_type_size( PrimitiveTypes type ) {
    size_t size = 0;
    switch ( type ) {
    case U_CHAR_T:
        size = 1;
        break;
    case CHAR_T:
        size = 1;
        break;
    case U_SHORT_T:
        size = 2;
        break;
    case SHORT_T:
        size = 2;
        break;
    case U_INT_T:
        size = 4;
        break;
    case INT_T:
        size = 4;
        break;
    case U_LONG_T:
        size = 8;
        break;
    case LONG_T:
        size = 8;
        break;
    case FLOAT_T:
        size = 4;
        break;
    case DOUBLE_T:
        size = 8;
        break;
    case LONG_DOUBLE_T:
        size = 16;
        break;
    case VOID_T:
        size = 0;
        break;
    default:
        std::cerr << "Invalid Type";
        assert( 0 );
    }
    return size;
}

void setup_primitive_types() {

    for ( int i = 0; i <= VOID_T; i++ ) {
        Types t;
        t.index = i;
        t.name = primitive_type_name( (PrimitiveTypes)i );
        t.size = primitive_type_size( (PrimitiveTypes)i );
        t.is_primitive = true;
        t.is_union = false;
        t.is_struct = false;
        t.struct_definition = nullptr;
        defined_types.push_back( t );
    }
}

//##############################################################################
//################################## TYPE
//######################################
//##############################################################################

Type::Type() {
    typeIndex = -1;
    ptr_level = -1;
    is_const = false;
}

Type::Type( int idx, int p_lvl, bool is_con ) {
    typeIndex = idx;
    ptr_level = p_lvl;
    is_const = is_con;
}
bool Type::isPrimitive() {
    if ( typeIndex >= 0 && typeIndex <= 12 ) {
        return true;
    } else {
        return false;
    }
}

std::string Type::get_name() {
    std::stringstream ss;
    ss << defined_types[typeIndex].name;
    for ( int i = 0; i < ptr_level; i++ ) {
        ss << "*";
    }
    return ss.str();
}

int Type::get_size() {
    if ( ptr_level > 0 ) {
        return 8;
    } else {
        return defined_types[typeIndex].size;
    }
}

bool Type::isInt() {
    if ( typeIndex >= 0 && typeIndex <= 7 ) {
        if ( ptr_level == 0 ) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Type::isFloat() {
    if ( typeIndex >= 8 && typeIndex <= 10 && ptr_level == 0 )
        return true;
    else
        return false;
}
bool Type::isIntorFloat() {
    if ( typeIndex <= 10 && ptr_level == 0 )
        return true;
    else
        return false;
}

bool Type::isUnsigned() {
    if ( typeIndex == 0 || typeIndex == 2 || typeIndex == 4 ||
         typeIndex == 6 ) {
        return true;
    } else {
        return false;
    }
}

bool Type::isPointer() {
    if ( ptr_level ) {
        return true;
    } else {
        return false;
    }
}

void Type::make_signed() {
    if ( typeIndex == 0 || typeIndex == 2 || typeIndex == 4 ||
         typeIndex == 6 ) {
        typeIndex += 1;
    }
}

void Type::make_unsigned() {
    if ( typeIndex == 1 || typeIndex == 3 || typeIndex == 5 ||
         typeIndex == 7 ) {
        typeIndex -= 1;
    }
}

bool operator==( Type &obj1, Type &obj2 ) {
    return ( obj1.typeIndex == obj2.typeIndex &&
             obj1.ptr_level == obj2.ptr_level );
}

//##############################################################################
//################################## TYPES
//#####################################
//##############################################################################

int add_to_defined_types( Types *typ ) {
    for ( auto it = defined_types.begin(); it != defined_types.end(); it++ ) {
        if ( ( *it ).name == typ->name ) {
            return ( *it ).index;
        }
    }

    typ->index = defined_types.size();
    defined_types.push_back( *typ );
    return defined_types.size() - 1;
}

int get_type_index( std::string name ) {
    for ( auto it = defined_types.begin(); it != defined_types.end(); it++ ) {
        if ( ( *it ).name == name ) {
            return ( *it ).index;
        }
    }
    return -1;
}

void add_struct_defintion_to_type( int index,
                                   StructDefinition *struct_definition ) {
    defined_types[index].struct_definition = struct_definition;
    defined_types[index].size = struct_definition->get_size();
}

//##############################################################################
//############################ STRUCT DEFINITION
//###############################
//##############################################################################

StructDefinition::StructDefinition(){};

size_t StructDefinition::get_size() {
    size_t size = 0;
    for ( auto it = members.begin(); it != members.end(); it++ ) {
        // TODO: Implement This
        size += 8;
    }
    return size;
}

StructDefinition *create_struct_definition( int un_or_st,
                                            StructDeclarationList *sdl ) {
    StructDefinition *sd = new StructDefinition();
    sd->un_or_st = un_or_st;
    std::cout << "struct {\n";
    for ( auto it = sdl->struct_declaration_list.begin();
          it != sdl->struct_declaration_list.end(); it++ ) {
        // TODO: Complete this
        int type_index = ( *it )->sq_list->type_index;
        bool is_const = ( *it )->sq_list->is_const;
        //	    assert( type_index != -1 );

        std::vector<Declarator *> dl =
            ( *it )->declarator_list->declarator_list;

        for ( auto jt = dl.begin(); jt != dl.end(); jt++ ) {
            int pointer_level = ( *jt )->get_pointer_level();
            Type type( type_index, pointer_level, is_const );

            sd->members.insert( {( *jt )->id->value, type} );
            std::cout << "  " << ( *jt )->id->value << " " << type.get_name()
                      << "\n";
        }
    }
    std::cout << "}\n";
    return sd;
}

Type *StructDefinition::get_member( Identifier *id ) {
    auto it = members.find( id->value );
    if ( it == members.end() ) {
        return nullptr;
    } else {
        return &( it->second );
    }

    return nullptr;
}

//##############################################################################
//################################ POINTER
//#####################################
//##############################################################################
void is_Valid( TypeQualifierList *ts ) {

    for ( unsigned int i = 0; i < ts->type_qualifier_list.size(); i++ ) {
        if ( ts->type_qualifier_list.at( i ) == CONST ||
             ts->type_qualifier_list.at( i ) == VOLATILE ) {
        } else {
            std::cout << "Error in pointer type qualfier pointer\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            break;
        }
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
    // is_Valid( type_list );
    return p;
}

//##############################################################################
//########################### TYPE QUALIFIER LIST
//##############################
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

//##############################################################################
//############################# DECLARATION
//####################################
//##############################################################################

Declaration *new_declaration( DeclarationSpecifiers *declaration_specifiers,
                              DeclaratorList *init_declarator_list ) {
    Declaration *d =
        new Declaration( declaration_specifiers, init_declarator_list );
    d->add_children( declaration_specifiers, init_declarator_list );
    declaration_specifiers->create_type();
    if(declaration_specifiers->type_specifier.at(0)->type==VOID)
    {
        std::cerr << "Error in strorage class declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    return d;
}

void Declaration::add_to_symbol_table( LocalSymbolTable &sym_tab ) {
    if ( init_declarator_list == nullptr ) {
        return;
    }
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;
    int type_index = declaration_specifiers->type_index;
    bool is_const = declaration_specifiers->is_const;
    assert( type_index != -1 );

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        // for ( int i = 0; i < sym_tab.current_level; i++ ) {
        //    std::cout << "  ";
        //}
        int pointer_level = ( *i )->get_pointer_level();

        SymTabEntry *e = new SymTabEntry( ( *i )->id->value );
        e->type = Type( type_index, pointer_level, is_const );
        sym_tab.add_to_table( e );
        // CSV
        sym_tab.ss << "local," << sym_tab.function_name << ","
                   << ( *i )->id->value << "," << e->type.get_name() << ","
                   << sym_tab.current_level << "\n";
        // std::cout << ( *i )->id->value << " " << e->type.get_name() << " "
        //          << pointer_level << " : " << sym_tab.current_level << "\n";
    }
}

TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList *tql,
                                               TYPE_QUALIFIER type ) {
    tql->append_to_list( type );
    return tql;
}

//##############################################################################
//############################# DECLARATION
//####################################
//##############################################################################

Declaration ::Declaration( DeclarationSpecifiers *declaration_specifiers_,
                           DeclaratorList *init_declarator_list_ )
    : Non_Terminal( "declaration" ),
      declaration_specifiers( declaration_specifiers_ ),
      init_declarator_list( init_declarator_list_ ){};

void Declaration::add_to_symbol_table( GlobalSymbolTable &sym_tab ) {

    if ( init_declarator_list == nullptr )
        return;
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

    int type_index = declaration_specifiers->type_index;
    bool is_const = declaration_specifiers->is_const;
    assert( type_index != -1 );

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        int pointer_level = ( *i )->get_pointer_level();
        // std::cout << "G: " << ( *i )->id->value << "\n";

        SymTabEntry *e = new SymTabEntry( ( *i )->id->value );
        e->type = Type( type_index, pointer_level, is_const );
        sym_tab.add_to_table( e, true );
        sym_tab.ss << "global,"
                   << "-," << ( *i )->id->value << "," << e->type.get_name()
                   << ",0\n";
    }

    write_to_symtab_file( sym_tab.ss.str() );
    sym_tab.ss.clear();
    sym_tab.ss.str( std::string() );
}

//##############################################################################
//########################## DECLARATION SPECIFIERS
//############################
//##############################################################################

DeclarationSpecifiers ::DeclarationSpecifiers()
    : Non_Terminal( "declaration_specifiers" ), is_const( false ),
      type_index( -1 ){};

void DeclarationSpecifiers::create_type() {

    int err = 0;

    if ( storage_class.size() == 0 ) {
        ;
    } else if ( storage_class.size() == 1 &&
                storage_class.at( 0 ) == TYPEDEF ) {
        ;
    } else {
        err += 1;
    }

    std::vector<TYPE_SPECIFIER> ty;
    for ( unsigned int i = 0; i < type_specifier.size(); i++ )
        ty.push_back( type_specifier.at( i )->type );

    std::sort( ty.begin(), ty.end() );

    is_const = false;
    type_index = -1;
    if ( ty.size() == 3 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == LONG &&
             ty.at( 2 ) == LONG ) {
            type_index = U_LONG_T;
        } else {
            err += 2;
        }
    } else if ( ty.size() == 2 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == CHAR ) {
            type_index = U_CHAR_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == CHAR ) {
            type_index = CHAR_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == SHORT ) {
            type_index = U_SHORT_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == SHORT ) {
            type_index = SHORT_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == INT ) {
            type_index = U_INT_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == INT ) {
            type_index = INT_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == LONG ) {
            type_index = U_LONG_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == LONG ) {
            type_index = LONG_T;
            //        } else if ( ty.at( 0 ) == FLOAT && ty.at( 1 ) ==
            //        DOUBLE )
            //        {
            //            type_index = DOUBLE_T;
        } else
            err += 2;
    } else if ( ty.size() == 1 ) {
        if ( ty.at( 0 ) == FLOAT ) {
            type_index = FLOAT_T;
        } else if ( ty.at( 0 ) == DOUBLE ) {
            type_index = DOUBLE_T;

        } else if ( ty.at( 0 ) == INT ) {
            type_index = INT_T;

        } else if ( ty.at( 0 ) == SHORT ) {
            type_index = SHORT_T;

        } else if ( ty.at( 0 ) == CHAR ) {
            type_index = CHAR_T;

        }  else if ( ty.at( 0 ) == VOID ) {
            type_index = VOID_T;
        } else if ( ty.at( 0 ) == LONG ) {
            type_index = LONG_T;
        } else if ( ty.at( 0 ) == ENUM ) {
            std::cerr << "ENUM not supported\n";
            exit( 0 );

        } else if ( ty.at( 0 ) == STRUCT || ty.at( 0 ) == UNION ) {
            type_index = type_specifier.at( 0 )->type_index;
        } else {
            err += 2;
        }
    }
    for ( unsigned int i = 0; i < type_qualifier.size(); i++ ) {

        if ( type_qualifier.at( i ) == CONST ) {
            is_const = true;
        } else if ( type_qualifier.at( i ) == VOLATILE ) {
        } else {
            err += 4;
            break;
        }
    }
    if ( err & 1 ) {
        std::cerr << "Error in strorage class declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    if ( err & 2 ) {
        std::cerr << "Error in type specifier declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    if ( err & 4 ) {
        std::cerr << "Error in type qualifier declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
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
//########################### DECLARATION LIST
//#################################
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
//############################# DECLARATOR LIST
//################################
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
//################################# DECLARATOR
//#################################
//##############################################################################

Declarator ::Declarator( Pointer *p, DirectDeclarator *dd )
    : Non_Terminal( "declarator" ), pointer( p ), direct_declarator( dd ) {
    assert( dd != nullptr );
    assert( dd->id != nullptr );
    id = dd->id;
};

int Declarator::get_pointer_level() {
    Pointer *ptr = pointer;
    int count = 0;
    while ( ptr != NULL ) {
        count++;
        ptr = ptr->pointer;
    }
    return count;
}

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
//############################## STRUCT VERIFY
//################################
//##############################################################################

void verify_struct_declarator( StructDeclarationList *st ) {
    int err;
    if ( st != NULL ) {
        // std::cout<<st->struct_declaration_list.size();
        for ( unsigned int i = 0; i < st->struct_declaration_list.size();
              i++ ) {
            std::vector<TypeSpecifier *> ts =
                st->struct_declaration_list.at( i )->sq_list->type_specifiers;
            std::vector<TYPE_QUALIFIER> tq =
                st->struct_declaration_list.at( i )->sq_list->type_qualifiers;
            err = 0;
            std::vector<TYPE_SPECIFIER> ty;
            for ( unsigned int i = 0; i < ts.size(); i++ ) {
                ty.push_back( ts.at( i )->type );
            }
            std::sort( ty.begin(), ty.end() );

            if ( ty.size() == 3 ) {
                if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
                     ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG ) &&
                     ty.at( 2 ) == INT ) {
                } else
                    err += 2;
            } else if ( ty.size() == 2 ) {
                if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
                     ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG ||
                       ty.at( 1 ) == INT || ty.at( 1 ) == CHAR ) ) {
                } else if ( ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG ) &&
                            ty.at( 1 ) == INT ) {
                } else if ( ty.at( 0 ) == LONG && ty.at( 1 ) == DOUBLE ) {
                } else
                    err += 2;
            } else if ( ty.size() == 1 ) {
                if ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG ||
                     ty.at( 0 ) == INT || ty.at( 0 ) == CHAR ||
                     ty.at( 0 ) == FLOAT || ty.at( 0 ) == DOUBLE ||
                     ty.at( 0 ) == STRUCT || ty.at( 0 ) == UNION ||
                     ty.at( 0 ) == ENUM ) {
                } else {
                    err += 2;
                }
            }

            for ( unsigned int i = 0; i < tq.size(); i++ ) {
                if ( tq.at( i ) == CONST || tq.at( i ) == VOLATILE ) {
                } else {
                    err += 4;
                    break;
                }
            }
            if ( err & 2 ){
                std::cout << "Error in type specifier struct\n";
                std::cerr << "ERROR at line " << line_num << "\n";
                exit(0);
            }
            if ( err & 4 ){
                std::cout << "Error in type qualifier struct\n";
                std::cerr << "ERROR at line " << line_num << "\n";
                exit(0);
            }
        }
        // std::cout<<"done2 ";
    }
}
//##############################################################################
//############################ DIRECT DECLARATOR
//###############################
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
//######################## DIRECT ABSTRACT DECLARATOR
//##########################
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
//########################### ABSTRACT DECLARATOR
//##############################
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
//########################### PARAMETER DECLARATION
//############################
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
//############################ PARAMETER TYPE LIST
//#############################
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
//################################ IDENTIFIER
//##################################
//##############################################################################

Identifier::Identifier( const char *name ) : Terminal( "IDENTIFIER", name ){};

//##############################################################################
//########################## FUNCTION DEFINITION
//###############################
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
    declaration_specifiers->create_type();
    global_symbol_table.add_symbol( declaration_specifiers, declarator );
    local_symbol_table.add_function( declaration_specifiers, declarator );
    return fd;
}

FunctionDefinition *add_stmt_to_function_definition( FunctionDefinition *fd,
                                                     Node *compound_stmt ) {
    fd->compound_statement = compound_stmt;
    fd->add_child( compound_stmt );
    return fd;
}

//##############################################################################
//############################## TYPE SPECIFIER
//################################
//##############################################################################

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER typ )
    : Non_Terminal( "type_specifier" ), type( typ ), id( nullptr ),
      struct_declaration_list( nullptr ), enumerator_list( nullptr ),
      type_index( -1 ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              StructDeclarationList *struct_declaration_list_ )
    : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
      struct_declaration_list( struct_declaration_list_ ),
      enumerator_list( nullptr ), type_index( -1 ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              EnumeratorList *enumerator_list_ )
    : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
      struct_declaration_list( nullptr ), enumerator_list( enumerator_list_ ),
      type_index( -1 ){};

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

#if 0
void verify_struct_declarator( StructDeclarationList *st ) {
    int err;
    if ( st != NULL ) {
        // std::cout<<st->struct_declaration_list.size();
        for ( int i = 0; i < st->struct_declaration_list.size(); i++ ) {
            std::vector<TypeSpecifier *> ts =
                st->struct_declaration_list.at( i )->sq_list->type_specifiers;
            std::vector<TYPE_QUALIFIER> tq =
                st->struct_declaration_list.at( i )->sq_list->type_qualifiers;
            err = 0;
            std::vector<TYPE_SPECIFIER> ty;
            for ( int i = 0; i < ts.size(); i++ )
                ty.push_back( ts.at( i )->type );
            std::sort( ty.begin(), ty.end() );

            if ( ty.size() == 3 ) {
                if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
                     ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG ) &&
                     ty.at( 2 ) == INT ) {
                } else
                    err += 2;
            } else if ( ty.size() == 2 ) {
                if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
                     ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG ||
                       ty.at( 1 ) == INT || ty.at( 1 ) == CHAR ) ) {
                } else if ( ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG ) &&
                            ty.at( 1 ) == INT ) {
                } else if ( ty.at( 0 ) == LONG && ty.at( 1 ) == DOUBLE ) {
                } else
                    err += 2;
            } else if ( ty.size() == 1 )
                if ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG ||
                     ty.at( 0 ) == INT || ty.at( 0 ) == CHAR ||
                     ty.at( 0 ) == FLOAT || ty.at( 0 ) == DOUBLE ||
                     ty.at( 0 ) == STRUCT || ty.at( 0 ) == UNION ||
                     ty.at( 0 ) == ENUM) {
                } else
                    err += 2;

            for ( int i = 0; i < tq.size(); i++ ) {
                if ( tq.at( i ) == CONST || tq.at( i ) == VOLATILE ) {
                } else {
                    err += 4;
                    break;
                }
            }
            if ( err & 2 )
                std::cout << "Error in type specifier struct";
            if ( err & 4 )
                std::cout << "Error in type qualifier struct";
        }
        // std::cout<<"done2 ";
    }
}

#endif

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
    if ( struct_declaration_list != nullptr ) {
        verify_struct_declarator( struct_declaration_list );
        std::string struct_name;

        if ( id == nullptr ) {
            struct_name = "struct #anon_" + std::to_string( anon_count );
            anon_count++;
        } else {
            struct_name = "struct " + id->value;
        }

        Types *struct_type = new Types;
        struct_type->name = struct_name;
        struct_type->is_primitive = false;

        switch ( type ) {
        case UNION:
            struct_type->is_union = true;
            break;
        case STRUCT:
            struct_type->is_union = false;
            break;
        default:
            assert( 0 );
        }
        ts->type_index = add_to_defined_types( struct_type );
        StructDefinition *struct_definition =
            create_struct_definition( type, struct_declaration_list );
        add_struct_defintion_to_type( ts->type_index, struct_definition );

    } else {
        ts->type_index = get_type_index( "struct " + id->value );
    }

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
//######################### STRUCT DECLARATION LIST
//############################
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
//############################ STRUCT DECLARATION
//##############################
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
    sq_list->create_type();
    return sd;
}

//##############################################################################
//######################### SPECIFIER QUALIFIER LIST
//###########################
//##############################################################################

SpecifierQualifierList::SpecifierQualifierList()
    : Non_Terminal( "specifier_qualifier_list" ){};

void SpecifierQualifierList::create_type() {

    int err = 0;

    std::vector<TYPE_SPECIFIER> ty;
    for ( unsigned int i = 0; i < type_specifiers.size(); i++ )
        ty.push_back( type_specifiers.at( i )->type );

    std::sort( ty.begin(), ty.end() );

    is_const = false;
    type_index = -1;
    if ( ty.size() == 3 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == LONG &&
             ty.at( 2 ) == LONG ) {
            type_index = U_LONG_T;
        } else {
            err += 2;
        }
    } else if ( ty.size() == 2 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == CHAR ) {
            type_index = U_CHAR_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == CHAR ) {
            type_index = CHAR_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == SHORT ) {
            type_index = U_SHORT_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == SHORT ) {
            type_index = SHORT_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == INT ) {
            type_index = U_INT_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == INT ) {
            type_index = INT_T;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == LONG ) {
            type_index = U_LONG_T;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == LONG ) {
            type_index = LONG_T;
            //        } else if ( ty.at( 0 ) == FLOAT && ty.at( 1 ) ==
            //        DOUBLE )
            //        {
            //            type_index = DOUBLE_T;
        } else
            err += 2;
    } else if ( ty.size() == 1 ) {
        if ( ty.at( 0 ) == FLOAT ) {
            type_index = FLOAT_T;
        } else if ( ty.at( 0 ) == DOUBLE ) {
            type_index = DOUBLE_T;

        } else if ( ty.at( 0 ) == INT ) {
            type_index = INT_T;

        } else if ( ty.at( 0 ) == SHORT ) {
            type_index = SHORT_T;

        } else if ( ty.at( 0 ) == CHAR ) {
            type_index = CHAR_T;

        }  else if ( ty.at( 0 ) == VOID ) {
            type_index = VOID_T;
        } else if ( ty.at( 0 ) == LONG ) {
            type_index = LONG_T;
        } else if ( ty.at( 0 ) == ENUM ) {
            std::cerr << "ENUM not supported\n";
            exit( 0 );

        } else if ( ty.at( 0 ) == STRUCT || ty.at( 0 ) == UNION ) {
            type_index = type_specifiers.at( 0 )->type_index;
        } else {
            err += 2;
        }
    }
    for ( unsigned int i = 0; i < type_qualifiers.size(); i++ ) {

        if ( type_qualifiers.at( i ) == CONST ) {
            is_const = true;
        } else if ( type_qualifiers.at( i ) == VOLATILE ) {
        } else {
            err += 4;
            break;
        }
    }
    if ( err & 1 ) {
        std::cerr << "Error in strorage class declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    if ( err & 2 ) {
        std::cerr << "Error in type specifier declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    if ( err & 4 ) {
        std::cerr << "Error in type qualifier declarator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
}

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
//################################ ENUMERATOR
//##################################
//##############################################################################

Enumerator::Enumerator( Identifier *id_, Node *const_expr )
    : Non_Terminal( "enumerator" ), id( id_ ), init_expr( const_expr ){};

Enumerator *create_enumerator( Identifier *id, Node *const_expr ) {
    Enumerator *e = new Enumerator( id, const_expr );
    e->add_children( id, const_expr );
    return e;
}

//##############################################################################
//############################## ENUMERATOR LIST
//###############################
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
//############################ LOCAL SYMBOL TABLE
//##############################
//##############################################################################

SymbolTable::SymbolTable(){};

void SymbolTable::add_to_table( SymTabEntry * ) {}

SymTabEntry *SymbolTable::get_symbol_from_table( std::string name ) {
    return nullptr;
}

LocalSymbolTable::LocalSymbolTable() : current_level( 0 ){};

void LocalSymbolTable::increase_level() {
    for ( int i = 0; i < current_level; i++ ) {
        ; // std::cout << "  ";
    }
    // std::cout << "{\n";
    current_level++;
    write_to_symtab_file( ss.str() );
    ss.clear();
    ss.str( std::string() );
}

void LocalSymbolTable::clear_current_level() {

    for ( auto it = sym_table.begin(); it != sym_table.end(); it++ ) {
        if ( it->second.empty() ) {
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
        ; // std::cout << "  ";
    }
    // std::cout << "}\n";
    write_to_symtab_file( ss.str() );
    ss.clear();
    ss.str( std::string() );
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
            std::cout << "\nERROR: Redeclaration of symbol " << it->first
                      << " on line:" << line_num << " in the scope.\n";
            exit( 1 );
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
        if ( it->second.empty() ) {
            return nullptr;
        } else {
            return it->second.front();
        }
    }
}

void LocalSymbolTable::add_function(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    sym_table.clear();
    ss.clear();
    ss.str( std::string() );
    assert( declarator->direct_declarator->type == FUNCTION );
    function_name = declarator->id->value;
    // std::cout << "L: " << function_name << " " <<
    // declaration_specifiers->type_index;

    // Check whether the arguements are of the form ( )
    if ( declarator->direct_declarator->params == nullptr ) {
        // std::cout << "( )\n";
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
                // std::cout << "( void )\n";
                return;
            }
        }
    }
    // Parameters need to be added at level one, we avoid calling
    // increase_level and clear_from_level to avoid thier side effects

    current_level = 1;
    // std::cout << "( ";
    for ( auto it = param_list.begin(); it != param_list.end(); it++ ) {

        if ( ( *it )->declarator == nullptr ||
             ( *it )->declarator->id == nullptr ) {
            std::cout << "\nERROR: Arguement requires identifier on line:"
                      << line_num << "\n";
            exit( 1 );
        }

        ( *it )->declaration_specifiers->create_type();
        int type_index = ( *it )->declaration_specifiers->type_index;
        bool is_const = ( *it )->declaration_specifiers->is_const;
        int pointer_level = ( *it )->declarator->get_pointer_level();

        Type t( type_index, pointer_level, is_const );
        ss << "local," << function_name << "," << ( *it )->declarator->id->value
           << "," << t.get_name() << ","
           << "arg"
           << "\n";

        // std::cout << ( *it )->declarator->id->value << " " << type_index <<
        // ", ";
        SymTabEntry *symbol = new SymTabEntry( ( *it )->declarator->id->value );
        add_to_table( symbol );
    }
    current_level = 0;
    // std::cout << " )\n";
    write_to_symtab_file( ss.str() );
    ss.clear();
    ss.str( std::string() );
}

//##############################################################################
//########################### GLOBAL SYMBOL TABLE
//##############################
//##############################################################################

void GlobalSymbolTable::add_symbol(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    declaration_specifiers->create_type();
    int type_index = declaration_specifiers->type_index;
    bool is_const = declaration_specifiers->is_const;
    int pointer_level = declarator->get_pointer_level();

    SymTabEntry *e = new SymTabEntry( declarator->id->value );
    e->type = Type( type_index, pointer_level, is_const );
    add_to_table( e, false );
    ss << "global,"
       << "-," << declarator->id->value << ",fun:" << e->type.get_name()
       << ",0\n";

    write_to_symtab_file( ss.str() );
    ss.clear();
    ss.str( std::string() );

    // std::cout << "G: " << declarator->id->value << "\n";
}

SymTabEntry *GlobalSymbolTable::get_symbol_from_table( std::string name ) {
    auto it = sym_table.find( name );
    if ( it == sym_table.end() ) {
        return nullptr;
    } else {
        return it->second;
    }
}

void GlobalSymbolTable::add_to_table( SymTabEntry *symbol, bool redef ) {

    auto it = sym_table.find( symbol->name );
    if ( it == sym_table.end() ) {
        sym_table.insert( {symbol->name, symbol} );
    } else if ( it->second->type == symbol->type && redef ) {

        // Can't insert two symbols with same name at the same level
        std::cout << "\nERROR: Redeclaration of symbol " << it->first
                  << " on line:" << line_num << " in the scope.\n";
        exit( 1 );
    }
}

Node *add_to_global_symbol_table( Declaration *declaration ) {
    declaration->add_to_symbol_table( global_symbol_table );
    return declaration;
}

void write_to_symtab_file( std::string s ) { sym_file << s; }

//##############################################################################
//########################### SYMBOL TABLE ENTRY
//##############################
//##############################################################################

SymTabEntry::SymTabEntry( std::string name ) : name( name ){};

//##############################################################################
