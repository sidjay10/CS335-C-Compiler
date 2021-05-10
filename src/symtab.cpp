// symtab.cpp

#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <symtab.h>
#include <expression.h>
#include <statement.h>
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
extern unsigned int line_num;

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
//################################## TYPE ######################################
//##############################################################################

Type::Type() {
    typeIndex = -1;
    ptr_level = -1;
    is_const = false;

    is_pointer = false;
    is_array = false;
    array_dim = 0;

    is_function = false;
    num_args = 0;
}

Type::Type( int idx, int p_lvl, bool is_con ) {
    typeIndex = idx;
    is_const = is_con;

    ptr_level = p_lvl;
    is_pointer = ptr_level > 0 ? true : false;

    is_array = false;
    array_dim = 0;

    is_function = false;
    num_args = 0;
    is_defined = false;
}
bool Type::isPrimitive() {
    if ( typeIndex >= 0 && typeIndex <= VOID_T ) {
        return true;
    } else {
        return false;
    }
}

std::string Type::get_name() {
    std::stringstream ss;
    ss << defined_types[typeIndex].name;

    if ( is_array ) {
        ss << " ";
        for ( unsigned int i = 0; i < array_dim; i++ ) {
            if ( array_dims[i] != 0 ) {
                ss << "[" << array_dims[i] << "]";
            } else {
                ss << "[]";
            }
        }
    } else if ( is_pointer ) {

        for ( int i = 0; i < ptr_level; i++ ) {
            ss << "*";
        }
    } else if ( is_function ) {
        if ( num_args == 0 ) {
            ss << "( )";
        } else if ( num_args == 1 ) {
            ss << "( ";
            for ( auto it = arg_types.begin(); it != arg_types.end(); it++ ) {
                ss << ( *it ).get_name();
            }
            ss << " )";
        } else {
            ss << "( ";
            auto it = arg_types.begin();
            ss << ( *it ).get_name();
            for ( auto it = arg_types.begin() + 1; it != arg_types.end();
                  it++ ) {
                ss << ", " << ( *it ).get_name();
            }
            ss << " )";
        }
    }
    return ss.str();
}

size_t Type::get_size() {
    if ( is_array ) {
        unsigned int p = 1;
        for ( unsigned int i = 0; i < array_dim; i++ ) {
            p *= array_dims[i];
        }
        size_t sz;
        if ( isPrimitive() ) {
            sz = defined_types[typeIndex].size;
        } else {
            if ( defined_types[typeIndex].struct_definition != nullptr ) {
                sz = defined_types[typeIndex].struct_definition->get_size();
            } else {
                error_msg( "Size of " + defined_types[typeIndex].name +
                               " isn't known",
                           line_num );
                exit( 0 );
            }
        }
        return sz * p;

    } else if ( ptr_level > 0 || is_function ) {
        return WORD_SIZE;
    } else if ( !isPrimitive() ) {
        if ( defined_types[typeIndex].struct_definition != nullptr ) {
            return defined_types[typeIndex].struct_definition->get_size();
        } else {
            error_msg( "Size of " + defined_types[typeIndex].name +
                           " isn't known",
                       line_num );
            exit( 0 );
        }
    }

    else {
        return defined_types[typeIndex].size;
    }
}

bool Type::isInt() {
    if ( typeIndex >= U_CHAR_T && typeIndex <= LONG_T ) {
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

bool Type::isVoid() {
    if ( typeIndex == VOID_T ) {
        if ( ptr_level == 0 || is_array ) {
            return true;
        }
    }
    return false;
}

bool Type::is_invalid() {

    if ( typeIndex == -1 ) {
        return true;
    }

    return false;
}

bool Type::is_ea() {
	
	if ( is_array ) {
		return true;
	} else if ( !isPrimitive() && ptr_level == 0 ) {
		return true;
	} else {
		return false;
	}

    return false;
}

bool operator!=( Type &obj1, Type &obj2 ) {
	return !(obj1 == obj2);
}
bool operator==( Type &obj1, Type &obj2 ) {

    if ( obj1.typeIndex != obj2.typeIndex ) {
        return false;
    } else if ( obj1.is_array == true && obj2.is_array == true ) {
        if ( obj1.array_dim != obj2.array_dim ) {
            return false;
        } else {
            for ( unsigned int i = 0; i < obj1.array_dim; i++ ) {
                if ( obj1.array_dims[i] == 0 || obj2.array_dims[i] == 0 ||
                     obj1.array_dims[i] == obj2.array_dims[i] ) {
                    continue;
                } else {
                    return false;
                }
            }

            return true;
        }
    } else if ( obj1.is_array != obj2.is_array ) {
        if ( obj1.ptr_level == 1 && obj2.ptr_level == 1 ) {
            return true;
        } else {
            return false;
        }
    } else if ( obj1.is_pointer == true && obj2.is_pointer == true ) {
        return obj1.ptr_level == obj2.ptr_level;

    } else if ( obj1.is_pointer != obj2.is_pointer ) {
        return false;
    } else if ( obj1.is_function == true && obj2.is_function == true ) {
        if ( obj1.num_args != obj2.num_args ) {
            return false;
        } else {
            for ( unsigned int i = 0; i < obj1.num_args; i++ ) {
                if ( obj1.arg_types[i] == obj2.arg_types[i] ) {
                    continue;
                } else {
                    return false;
                }
            }

            return true;
        }

    } else if ( obj1.is_function != obj2.is_function ) {
        return false;
    }

    else {
        return true;
    }

    return false;
}

//##############################################################################
//################################## TYPES #####################################
//##############################################################################

Types::Types()
    : index( -1 ), name( "" ), size( 0 ), is_primitive( true ),
      is_struct( false ), is_union( false ), struct_definition( nullptr ){};

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
//############################ STRUCT DEFINITION ###############################
//##############################################################################

StructDefinition::StructDefinition() : recursive( 0 ){};

size_t StructDefinition::get_size() {
    if ( recursive == 1 ) {
        std::cout << "ERROR : Cannot recursively define struct/union type\n";
        exit( 0 );
    }

    recursive = 1;
    size_t size = 0;
    for ( auto it = members.begin(); it != members.end(); it++ ) {
        size_t sz = it->second.get_size();
        if ( sz % WORD_SIZE != 0 ) {
            sz += ( WORD_SIZE - ( sz % WORD_SIZE ) );
        }
        if ( un_or_st == UNION ) {
            size = size < sz ? sz : size;
        } else {
            size += sz;
        }
    }
    assert( size != 0 );
    std::cout << size << "\n";
    recursive = 0;
    return size;
}

StructDefinition *create_struct_definition( int un_or_st,
                                            StructDeclarationList *sdl ) {
    StructDefinition *sd = new StructDefinition();
    sd->un_or_st = un_or_st;
    std::cout << "struct {\n";
    for ( auto it = sdl->struct_declaration_list.begin();
          it != sdl->struct_declaration_list.end(); it++ ) {
        int type_index = ( *it )->sq_list->type_index;
        bool is_const = ( *it )->sq_list->is_const;
        assert( type_index != -1 );
        if ( type_index == -2 ) {
            continue;
        }

        std::vector<Declarator *> dl =
            ( *it )->declarator_list->declarator_list;

        for ( auto jt = dl.begin(); jt != dl.end(); jt++ ) {
            int pointer_level = ( *jt )->get_pointer_level();
            DirectDeclarator *dd = ( *jt )->direct_declarator;
            Type type( type_index, pointer_level, is_const );

            if ( dd->type == ID ) {
                ;
            } else if ( dd->type == ARRAY ) {
                type = Type( type_index, dd->array_dims.size(), true );
                type.is_array = true;
                type.is_pointer = true;
                type.array_dim = dd->array_dims.size();
                type.array_dims = dd->array_dims;
            }

            else if ( dd->type == FUNCTION ) {
                error_msg( "Function cannot be member of struct/union",
                           ( *jt )->id->line_num, ( *jt )->id->column );
                continue;
            }
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
//################################ POINTER #####################################
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

Declaration *new_declaration( DeclarationSpecifiers *declaration_specifiers,
                              DeclaratorList *init_declarator_list ) {
    Declaration *d =
        new Declaration( declaration_specifiers, init_declarator_list );
    d->add_children( declaration_specifiers, init_declarator_list );
    declaration_specifiers->create_type();
    return d;
}

void Declaration::add_to_symbol_table( LocalSymbolTable &sym_tab ) {
    if ( init_declarator_list == nullptr ) {
        /* Nothing to add to the symbol table */
        return;
    }
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;
    int type_index = declaration_specifiers->type_index;
    bool is_const = declaration_specifiers->is_const;
    assert( type_index != -1 );
    if ( type_index == -2 ) {
        /* Error in type index, don't add any thing to the symbol table */
        return;
    }

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
	PrimaryExpression *P = nullptr;
        int pointer_level = ( *i )->get_pointer_level();

        SymTabEntry *e = new SymTabEntry(
            ( *i )->id->value, ( *i )->id->line_num, ( *i )->id->column );
        DirectDeclarator *dd = ( *i )->direct_declarator;

        if ( dd->type == ID ) {
            e->type = Type( type_index, pointer_level, is_const );
            if ( ( *i )->init_expr != nullptr ) {
                P = new PrimaryExpression();
                P->isTerminal = 1;
                Identifier *a = new Identifier( "" );
                *a = *( *i )->id;

                P->Ival = a;
                P->name = "primary_expression";
                P->type = Type( type_index, pointer_level, false );
                P->add_child( a );
                P->line_num = a->line_num;
                P->column = a->column;
		
		size_t size = e->type.get_size();
		e->offset = sym_tab.offset;
		sym_tab.offset = sym_tab.offset + size + ( size % WORD_SIZE );
		sym_tab.reqd_size = sym_tab.offset > sym_tab.reqd_size ? sym_tab.offset : sym_tab.reqd_size;
		sym_tab.add_to_table( e, ( *i )->id , false );
		
		P->res = new_3id( e );
                Expression *ae = create_assignment_expression(
                    P, ( *i )->eq, ( *i )->init_expr );
                add_child( ae );
		continue;
            }
        } else if ( dd->type == ARRAY ) {
            e->type = Type( type_index, dd->array_dims.size(), true );
            e->type.is_array = true;
            e->type.is_pointer = true;
            e->type.array_dim = dd->array_dims.size();
            e->type.array_dims = dd->array_dims;
        }

        else if ( dd->type == FUNCTION ) {

            e->type = Type( type_index, 0, true );
            e->type.is_function = true;
            e->type.is_pointer =
                false; /* We don't implement function pointers */
            if ( dd->params != nullptr ) {
                e->type.num_args = dd->params->param_list.size();

                bool fun_args_valid = true;
                for ( auto j = dd->params->param_list.begin();
                      j != dd->params->param_list.end(); j++ ) {
                    if ( ( *j )->type.is_invalid() ) {
                        fun_args_valid = false;
                        break;
                    } else {
                        e->type.arg_types.push_back( ( *j )->type );
                    }
                }

                if ( !fun_args_valid ) {
                    delete e;
                    continue;
                }
            } else {
                e->type.num_args = 0;
            }

        } else {
            std::cerr << "INVALID TYPE: " << dd->type << "\n";
            assert( 0 );
        }

        if ( e->type.isVoid() ) {
            error_msg( "Invalid type void for variable " + ( *i )->id->value,
                       declaration_specifiers->type_specifier[0]->line_num,
                       declaration_specifiers->type_specifier[0]->column );
            continue;
        }
        size_t size = e->type.get_size();
        e->offset = sym_tab.offset;
        sym_tab.offset = sym_tab.offset + size + ( size % WORD_SIZE );
        sym_tab.reqd_size = sym_tab.offset > sym_tab.reqd_size ? sym_tab.offset : sym_tab.reqd_size;
        sym_tab.add_to_table( e, ( *i )->id , false );
    }
}

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
    if ( type_index == -2 ) {
        return;
    }

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        int pointer_level = ( *i )->get_pointer_level();

        SymTabEntry *e = new SymTabEntry(
            ( *i )->id->value, ( *i )->id->line_num, ( *i )->id->column );
        DirectDeclarator *dd = ( *i )->direct_declarator;

        if ( dd->type == ID ) {
            e->type = Type( type_index, pointer_level, is_const );
            if ( ( *i )->init_expr != nullptr ) {
                PrimaryExpression *P = new PrimaryExpression();
                P->isTerminal = 1;
                Identifier *a = new Identifier( "" );
                *a = *( *i )->id;

                P->Ival = a;
                P->name = "primary_expression";
                P->type = Type( type_index, pointer_level, false );
                P->add_child( a );
                P->line_num = a->line_num;
                P->column = a->column;
//FIXME: Intialised global data;
		//P->res = new Address ( a->value, ID3);
                //Expression *ae = create_assignment_expression(
                //    P, ( *i )->eq, ( *i )->init_expr );
                //add_child( ae );
            }
        } else if ( dd->type == ARRAY ) {
            e->type = Type( type_index, dd->array_dims.size(), true );
            e->type.is_array = true;
            e->type.is_pointer = true;
            e->type.array_dim = dd->array_dims.size();
            e->type.array_dims = dd->array_dims;
        } else if ( dd->type == FUNCTION ) {

            e->type = Type( type_index, 0, true );
            e->type.is_function = true;
            e->type.is_pointer =
                false; /* We don't implement function pointers */
            if ( dd->params != nullptr ) {
                e->type.num_args = dd->params->param_list.size();
                bool fun_args_valid = true;
                for ( auto j = dd->params->param_list.begin();
                      j != dd->params->param_list.end(); j++ ) {
                    if ( ( *j )->type.is_invalid() ) {
                        fun_args_valid = false;
                        break;
                    } else {
                        e->type.arg_types.push_back( ( *j )->type );
                    }
                }

                if ( !fun_args_valid ) {
                    delete e;
                    continue;
                }
            } else {
                e->type.num_args = 0;
            }
        } else {
            std::cerr << "INVALID TYPE : " << dd->type << "\n";
            assert( 0 );
        }

        if ( e->type.isVoid() ) {
            error_msg( "Invalid type void for variable " + ( *i )->id->value,
                       declaration_specifiers->type_specifier[0]->line_num,
                       declaration_specifiers->type_specifier[0]->column );
            continue;
        }

        sym_tab.add_to_table( e, true, ( *i )->id );
        sym_tab.ss << "global,"
                   << "-," << ( *i )->id->value << "," << e->type.get_name()
                   << ",0\n";
    }

    write_to_symtab_file( sym_tab.ss.str() );
    sym_tab.ss.clear();
    sym_tab.ss.str( std::string() );
}

void Declaration::dotify() {

    std::vector<Node *> &c = children;

    bool dotify_ = false;
    for ( auto it = c.begin(); it != c.end(); it++ ) {
        if ( dynamic_cast<AssignmentExpression *>( *it ) ) {
            dotify_ = true;
            break;
        }
    }
    if ( dotify_ ) {
        if ( is_printed ) {
            is_printed = 0;
            std::stringstream ss;
            ss << "\t" << id << " [label=\"" << name << "\"];\n";
            for ( auto it = c.begin(); it != c.end(); it++ ) {
                if ( dynamic_cast<AssignmentExpression *>( *it ) ) {
                    ss << "\t" << id << " -> " << ( *it )->id << ";\n";
                }
            }
            file_writer( ss.str() );

            for ( auto it = c.begin(); it != c.end(); it++ ) {
                if ( dynamic_cast<AssignmentExpression *>( *it ) ) {
                    ( *it )->dotify();
                }
            }
        }
    }
}

//##############################################################################
//########################## DECLARATION SPECIFIERS ############################
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

        } else if ( ty.at( 0 ) == VOID ) {
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
        error_msg( "Invalid type", ( type_specifier.back() )->line_num,
                   ( type_specifier.back() )->column );
        type_index = -2;
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

    if ( declaration == nullptr ) {
        return declaration_list;
    }
    if ( declaration_list == nullptr ) {

        return create_declaration_list( declaration );
    }
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

    if ( d == nullptr ) {
        return nullptr;
    }
    DeclaratorList *dl = new DeclaratorList();
    dl->declarator_list.push_back( d );
    dl->add_child( d );
    return dl;
}

DeclaratorList *add_to_init_declarator_list( DeclaratorList *dl,
                                             Declarator *d ) {

    if ( d == nullptr ) {
        return dl;
    }

    if ( dl == nullptr ) {
        dl = new DeclaratorList();
    }
    dl->declarator_list.push_back( d );
    dl->add_child( d );
    return dl;
}

//##############################################################################
//################################# DECLARATOR
//#################################
//##############################################################################

Declarator ::Declarator( Pointer *p, DirectDeclarator *dd )
    : Non_Terminal( "declarator" ), pointer( p ), direct_declarator( dd ),
      init_expr( nullptr ), eq( nullptr ) {
    if ( dd == nullptr ) {
        id = nullptr;
    } else {
        assert( dd->id != nullptr );
        id = dd->id;
    }
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
#if 0
void Declarator::dotify() {
	if(is_printed){
		is_printed = 0;
		std::stringstream ss;
		if ( init_expr != nullptr ) {
		ss << "\t" << get_id() << " [label=\"" << name << "\"];\n";
			ss << "\t" << get_id() << " -> " << init_expr->id << ";\n";
		file_writer(ss.str());
	
		init_expr->dotify();
}
	}

};
#endif

Declarator *create_declarator( Pointer *pointer,
                               DirectDeclarator *direct_declarator ) {
    if ( direct_declarator == NULL ) {
        return NULL;
    }
    Declarator *d = new Declarator( pointer, direct_declarator );
    d->add_children( pointer, direct_declarator );
    return d;
}

Declarator *add_initializer_to_declarator( Declarator *declarator, Terminal *eq,
                                           Expression *ie ) {

    if ( declarator == nullptr ) {
        return NULL;
    }
    if ( declarator->direct_declarator->type == ARRAY ) {
        error_msg( "Unsupported initialization of array declaration",
                   eq->line_num, eq->column );
        return declarator;
    } else if ( declarator->direct_declarator->type == FUNCTION ) {
        error_msg( "Invalid initialization of function declaration",
                   eq->line_num, eq->column );
        return declarator;
    } else if ( declarator->direct_declarator->type == ID ) {
        declarator->eq = eq;
        declarator->init_expr = ie;
        declarator->add_child( ie );
        return declarator;
    } else {
        std::cerr << "INVALID TYPE: " << declarator->direct_declarator->type
                  << "\n";
        assert( 0 );
    }
    return NULL;
}
//##############################################################################
//############################## STRUCT VERIFY
//################################
//##############################################################################

int verify_struct_declarator( StructDeclarationList *st ) {
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
            if ( err & 2 ) {
                error_msg( "Invalid type", ( ts.back() )->line_num,
                           ( ts.back() )->column );
                return -1;
            }
            if ( err & 4 ) {
                std::cout << "Error in type qualifier struct\n";
                std::cerr << "ERROR at line " << line_num << "\n";
                exit( 0 );
                return -2;
            }
        }
    }
    return 0;
}
//##############################################################################
//############################ DIRECT DECLARATOR ###############################
//##############################################################################

DirectDeclarator ::DirectDeclarator()
    : Non_Terminal( "direct_declarator" ), type( ID ), id( nullptr ),
      params( nullptr ){};

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

#if 0
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
#endif

DirectDeclarator *
append_dir_declarator_arr( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           Constant *const_ ) {
    if ( direct_declarator == nullptr ) {
        return nullptr;
    }
    assert( type == ARRAY );
    assert( direct_declarator->id != nullptr );
    assert( direct_declarator->type == ARRAY || direct_declarator->type == ID );
    // DirectDeclarator *dd = new DirectDeclarator();
    direct_declarator->name = "direct_declarator_array";
    direct_declarator->type = ARRAY;
    assert( const_ != nullptr );
    Type const_type = const_->getConstantType();
    long dim = -1;
    switch ( const_type.typeIndex ) {
    case U_CHAR_T:
    case U_SHORT_T:
    case U_INT_T:
        direct_declarator->array_dims.push_back( const_->val.ui );
        break;
    case U_LONG_T:
        direct_declarator->array_dims.push_back( const_->val.ul );
        break;
    case CHAR_T:
    case SHORT_T:
    case INT_T:
        dim = const_->val.i;
        break;
    case LONG_T:
        dim = const_->val.l;
        break;
    default:
        error_msg( "Array Dimensions must be positive integral constant",
                   const_->line_num, const_->column );
    }

    if ( const_type.isUnsigned() == false && dim <= 0 ) {
        error_msg( "Array Dimensions must be positive integral constant",
                   const_->line_num, const_->column );
    } else {
        direct_declarator->array_dims.push_back( dim );
    }

    direct_declarator->add_child( const_ );
    return direct_declarator;
}

DirectDeclarator *
append_dir_declarator_fun( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           ParameterTypeList *params ) {
    assert( type == FUNCTION );
    assert( direct_declarator != nullptr );
    assert( direct_declarator->id != nullptr );
    assert( direct_declarator->type == ID );
    direct_declarator->name = "direct_declarator_function";
    direct_declarator->type = FUNCTION;
    direct_declarator->params = params;
    direct_declarator->add_child( params );
    return direct_declarator;
}

//##############################################################################
//######################## DIRECT ABSTRACT DECLARATOR ##########################
//##############################################################################

DirectAbstractDeclarator::DirectAbstractDeclarator()
    : Non_Terminal( "direct_abstract_declarator" ){};

DirectAbstractDeclarator *
create_direct_abstract_declarator( Constant *_const ) {
    DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
    if ( _const == nullptr ) {
        dad->array_dims.push_back( 0 );
    } else {
        Type const_type = _const->getConstantType();
        long dim = -1;
        switch ( const_type.typeIndex ) {
        case U_CHAR_T:
        case U_SHORT_T:
        case U_INT_T:
            dad->array_dims.push_back( _const->val.ui );
            break;
        case U_LONG_T:
            dad->array_dims.push_back( _const->val.ul );
            break;
        case CHAR_T:
        case SHORT_T:
        case INT_T:
            dim = _const->val.i;
            break;
        case LONG_T:
            dim = _const->val.l;
            break;
        default:
            error_msg( "Array Dimensions must be positive integral constant",
                       _const->line_num, _const->column );
        }

        if ( const_type.isUnsigned() == false && dim <= 0 ) {
            error_msg( "Array Dimensions must be positive integral constant",
                       _const->line_num, _const->column );
        } else {
            dad->array_dims.push_back( dim );
        }
    }
    return dad;
}

DirectAbstractDeclarator *
append_direct_abstract_declarator( DirectAbstractDeclarator *dad,
                                   Constant *_const ) {
    if ( _const == nullptr ) {
        dad->array_dims.push_back( 0 );
    } else {
        Type const_type = _const->getConstantType();
        long dim = -1;
        switch ( const_type.typeIndex ) {
        case U_CHAR_T:
        case U_SHORT_T:
        case U_INT_T:
            dad->array_dims.push_back( _const->val.ui );
            break;
        case U_LONG_T:
            dad->array_dims.push_back( _const->val.ul );
            break;
        case CHAR_T:
        case SHORT_T:
        case INT_T:
            dim = _const->val.i;
            break;
        case LONG_T:
            dim = _const->val.l;
            break;
        default:
            error_msg( "Array Dimensions must be positive integral constant",
                       _const->line_num, _const->column );
        }

        if ( const_type.isUnsigned() == false && dim <= 0 ) {
            error_msg( "Array Dimensions must be positive integral constant",
                       _const->line_num, _const->column );
        } else {
            dad->array_dims.push_back( dim );
        }
    }
    return dad;
}

#if 0
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

#endif

//##############################################################################
//########################### ABSTRACT DECLARATOR ##############################
//##############################################################################

AbstractDeclarator::AbstractDeclarator( Pointer *ptr,
                                        DirectAbstractDeclarator *dabs )
    : Non_Terminal( "abstract_declarator" ), pointer( ptr ),
      direct_abstract_declarator( dabs ){};

int AbstractDeclarator::get_pointer_level() {
    Pointer *ptr = pointer;
    int count = 0;
    while ( ptr != NULL ) {
        count++;
        ptr = ptr->pointer;
    }
    return count;
}

AbstractDeclarator *
create_abstract_declarator( Pointer *ptr, DirectAbstractDeclarator *dabs ) {
    AbstractDeclarator *abs = new AbstractDeclarator( ptr, dabs );
    abs->add_children( ptr, dabs );
    return abs;
}

//##############################################################################
//############################### TYPE NAME  ###################################
//##############################################################################

TypeName::TypeName() : Non_Terminal( "type_name" ){};

TypeName *create_type_name( SpecifierQualifierList *sq_list,
                            AbstractDeclarator *abstract_declarator ) {
    TypeName *type_name = new TypeName();
    type_name->sq_list = sq_list;
    type_name->abstract_declarator = abstract_declarator;
    sq_list->create_type();

    assert( sq_list->type_index != -1 );
    if ( sq_list->type_index == -2 ) {
        type_name->type = Type();
    }
    if ( abstract_declarator == nullptr ) {
        type_name->type = Type( sq_list->type_index, 0, false );

    }

    else {
        DirectAbstractDeclarator *dd =
            abstract_declarator->direct_abstract_declarator;

        if ( dd != nullptr && dd->array_dims.size() != 0 ) {
            type_name->type =
                Type( sq_list->type_index, dd->array_dims.size(), true );
            type_name->type.is_array = true;
            type_name->type.is_pointer = true;
            type_name->type.array_dim = dd->array_dims.size();
            type_name->type.array_dims = dd->array_dims;
        }

        else {
            int pointer_level = abstract_declarator->get_pointer_level();
            type_name->type = Type( sq_list->type_index, pointer_level, false );
        }
    }

    return type_name;
}

//##############################################################################
//########################### PARAMETER DECLARATION ############################
//##############################################################################
ParameterDeclaration::ParameterDeclaration()
    : Non_Terminal( "parameter_declaration" ){};

void ParameterDeclaration::create_type() {
    declaration_specifiers->create_type();
    int type_index = declaration_specifiers->type_index;
    bool is_const = declaration_specifiers->is_const;
    assert( type_index != -1 );
    if ( type_index == -2 ) {
        return;
    }

    int pointer_level = 0;
    if ( declarator != nullptr ) {
        pointer_level = declarator->get_pointer_level();
        DirectDeclarator *dd = declarator->direct_declarator;

        if ( dd->type == ID ) {
            type = Type( type_index, pointer_level, is_const );
        } else if ( dd->type == ARRAY ) {
            type = Type( type_index, dd->array_dims.size(), true );
            type.is_array = true;
            type.is_pointer = true;
            type.array_dim = dd->array_dims.size();
            type.array_dims = dd->array_dims;
        }

        else if ( dd->type == FUNCTION ) {
            error_msg( "Formal Argument can't be of type function",
                       dd->id->line_num, dd->id->column );
            type = Type();
            // type = INVALID_TYPE;
        }

        else {
            std::cerr << "INVALID TYPE: " << dd->type << "\n";
            assert( 0 );
        }
    } else if ( abstract_declarator != nullptr ) {
        DirectAbstractDeclarator *dd =
            abstract_declarator->direct_abstract_declarator;

        if ( dd->array_dims.size() != 0 ) {
            type = Type( type_index, dd->array_dims.size(), true );
            type.is_array = true;
            type.is_pointer = true;
            type.array_dim = dd->array_dims.size();
            type.array_dims = dd->array_dims;
        }

        else {
            pointer_level = abstract_declarator->get_pointer_level();
            type = Type( type_index, pointer_level, is_const );
        }
    } else {
        type = Type( type_index, 0, is_const );
        type.is_array = false;
        type.is_pointer = false;
    }
};

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
    pd->create_type();
    ptl->param_list.push_back( pd );
    ptl->add_child( pd );
    return ptl;
}

ParameterTypeList *add_to_parameter_list( ParameterTypeList *ptl,
                                          ParameterDeclaration *pd ) {
    pd->create_type();
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
Identifier::Identifier( const char *name )
    : Terminal( "IDENTIFIER", name, 0, 0 ){};

Identifier::Identifier( const char *name, unsigned int _line_num,
                        unsigned int _column )
    : Terminal( "IDENTIFIER", name, _line_num, _column ){};

//##############################################################################
//########################## FUNCTION DEFINITION ###############################
//##############################################################################

FunctionDefinition::FunctionDefinition(
    DeclarationSpecifiers *declaration_specifiers_, Declarator *declarator_,
    Node *compound_statement_ )
    : Non_Terminal( "function_definiton" ),
      declaration_specifiers( declaration_specifiers_ ),
      declarator( declarator_ ), compound_statement( compound_statement_ ),
      error( 0 ){};

FunctionDefinition *
create_function_defintion( DeclarationSpecifiers *declaration_specifiers,
                           Declarator *declarator, Node *compound_statement ) {
    FunctionDefinition *fd = new FunctionDefinition(
        declaration_specifiers, declarator, compound_statement );
    fd->add_children( declaration_specifiers, declarator, compound_statement );
    global_symbol_table.add_symbol( declaration_specifiers, declarator,
                                    &( fd->error ) );
    local_symbol_table.add_function( declaration_specifiers, declarator,
                                     &( fd->error ) );
    return fd;
}

FunctionDefinition *add_stmt_to_function_definition( FunctionDefinition *fd,
                                                     Node *compound_stmt ) {
    fd->compound_statement = compound_stmt;
    fd->add_child( compound_stmt );
    if ( fd->error == 0 && compound_stmt != nullptr ) {
        SymTabEntry *gte = global_symbol_table.get_symbol_from_table(
            fd->declarator->id->value );
        gte->type.is_defined = true;
    }
    return fd;
}

//##############################################################################
//############################## TYPE SPECIFIER ################################
//##############################################################################

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER typ, unsigned int _line_num,
                              unsigned int _column )
    : Terminal( "type_specifier", NULL, _line_num, _column ), type( typ ),
      id( nullptr ), struct_declaration_list( nullptr ),
      enumerator_list( nullptr ), type_index( -1 ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              StructDeclarationList *struct_declaration_list_ )
    : Terminal( "type_specifier", NULL ), type( type_ ), id( id_ ),
      struct_declaration_list( struct_declaration_list_ ),
      enumerator_list( nullptr ), type_index( -1 ){};

TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier *id_,
                              EnumeratorList *enumerator_list_ )
    : Terminal( "type_specifier", NULL ), type( type_ ), id( id_ ),
      struct_declaration_list( nullptr ), enumerator_list( enumerator_list_ ),
      type_index( -1 ){};

TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type,
                                      unsigned int line_num,
                                      unsigned int column ) {
    assert( type != UNION && type != STRUCT && type != ENUM );
    TypeSpecifier *ts = new TypeSpecifier( type, line_num, column );
    std::stringstream ss;
    // ss << "type_specifier : ";
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

TypeSpecifier *create_struct_type( TYPE_SPECIFIER type, Identifier *id ) {
    assert( type == UNION || type == STRUCT );
    TypeSpecifier *ts =
        new TypeSpecifier( type, id, (StructDeclarationList *)NULL );
    std::stringstream ss;
    std::string name_;

    switch ( type ) {
    case UNION:
        ss << "UNION";
        name_ = "union";
        break;
    case STRUCT:
        ss << "STRUCT";
        name_ = "struct";
        break;
    default:
        assert( 0 );
    }
    ss << " " << id->value;
    ts->name = "type_specifier : " + ss.str();

    Types *struct_type = new Types;
    struct_type->name = name_ + " " + id->value;
    struct_type->is_primitive = false;

    switch ( type ) {
    case UNION:
        struct_type->is_union = true;
        struct_type->is_struct = false;
        break;
    case STRUCT:
        struct_type->is_union = false;
        struct_type->is_struct = true;
        break;
    default:
        assert( 0 );
    }
    ts->type_index = add_to_defined_types( struct_type );

    return ts;
}

TypeSpecifier *
add_struct_declaration( TypeSpecifier *ts,
                        StructDeclarationList *struct_declaration_list ) {
    if ( struct_declaration_list != nullptr ) {
        int err = verify_struct_declarator( struct_declaration_list );
        if ( err == 0 ) {
            StructDefinition *struct_definition =
                create_struct_definition( ts->type, struct_declaration_list );
            add_struct_defintion_to_type( ts->type_index, struct_definition );
        }
        delete struct_declaration_list;
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
    sq_list->create_type();
    return sd;
}

//##############################################################################
//######################### SPECIFIER QUALIFIER LIST ###########################
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

        } else if ( ty.at( 0 ) == VOID ) {
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
        error_msg( "Invalid type", ( type_specifiers.back() )->line_num,
                   ( type_specifiers.back() )->column );
        type_index = -2;
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

SymbolTable::SymbolTable() : symbol_id(0) {};

void SymbolTable::add_to_table( SymTabEntry * ) { assert( 0 ); }

SymTabEntry *SymbolTable::get_symbol_from_table( std::string name ) {
    return nullptr;
}

LocalSymbolTable::LocalSymbolTable() : current_level( 0 ), offset(0), reqd_size(0), return_type(INVALID_TYPE) {};

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
            //Reduce the offset to the min offset in this level;
            offset = offset > entry->offset ? entry->offset : offset;
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

void LocalSymbolTable::add_to_table( SymTabEntry *symbol, Identifier *id , bool is_arg ) {

    auto it = sym_table.find( symbol->name );
    if ( it == sym_table.end() ) {
        std::deque<SymTabEntry *> &q = *new std::deque<SymTabEntry *>;
        symbol->level = current_level;
	if ( is_arg ) {
		symbol->id = (FUN_ARG_MASK | symbol_id++);
	} else {
		symbol->id = (LOCAL_SYM_MASK | symbol_id++);
        }
	q.push_front( symbol );
        sym_table.insert( {symbol->name, q} );
        // CSV
        ss << "local," << function_name << "," << id->value << ","
           << symbol->type.get_name() << "," << symbol->offset << "\n";
    } else {
        std::deque<SymTabEntry *> &q = it->second;
        if ( q.front() && ( q.front() )->level == current_level ) {
            // Can't insert two symbols with same name at the same level
            error_msg( "Redeclaration of symbol " + it->first +
                           " in this scope",
                       id->line_num, id->column );
        } else {
            symbol->level = current_level;
			symbol->id = (LOCAL_SYM_MASK | symbol_id++);
			//MSB 0 -> LOCAL_SYMBOL 
            q.push_front( symbol );
            // CSV
            ss << "local," << function_name << "," << id->value << ","
               << symbol->type.get_name() << "," << symbol->offset << "\n";
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
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator,
    int *error ) {

    sym_table.clear();
	symbol_id = 0;
	offset = 0;
    ss.clear();
    ss.str( std::string() );
    if ( *error == -1 ) {
        return;
    }
    assert( declarator->direct_declarator->type == FUNCTION );
    function_name = declarator->id->value;
    SymTabEntry *gte =
        global_symbol_table.get_symbol_from_table( function_name );

    if ( gte == nullptr ) {
        *error = -1;
        return;
    }


	return_type = gte->type;
	return_type.is_function = false;
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

        int type_index = ( *it )->declaration_specifiers->type_index;
        assert( type_index != -1 );
        if ( type_index == -2 ) {
            continue;
        }

        TypeSpecifier *ts =
            *( ( *it )->declaration_specifiers->type_specifier.begin() );

        if ( ( *it )->declarator == nullptr ||
             ( *it )->declarator->id == nullptr ) {
            error_msg( "Formal argument requires identifier", ts->line_num,
                       ts->column + ts->name.size() );
            continue;
        }

        //( *it )->declaration_specifiers->create_type();
        // bool is_const = ( *it )->declaration_specifiers->is_const;
        // int pointer_level = ( *it )->declarator->get_pointer_level();
        Type t = ( *it )->type;
        if ( t.isVoid() ) {
            error_msg( "Invalid type void for argument " +
                           ( *it )->declarator->id->value,
                       ts->line_num, ts->column );
            continue;
        }

        SymTabEntry *symbol = new SymTabEntry(
            ( *it )->declarator->id->value, ( *it )->declarator->id->line_num,
            ( *it )->declarator->id->column );

        symbol->type = t;
        add_to_table( symbol, ( *it )->declarator->id , true );

    }
    current_level = 0;
    write_to_symtab_file( ss.str() );
    ss.clear();
    ss.str( std::string() );
}

//##############################################################################
//########################### GLOBAL SYMBOL TABLE ##############################
//##############################################################################

void GlobalSymbolTable::add_symbol(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator,
    int *error ) {

    declaration_specifiers->create_type();
    int type_index = declaration_specifiers->type_index;
    //bool is_const = declaration_specifiers->is_const;
    int pointer_level = declarator->get_pointer_level();

    assert( type_index != -1 );
    if ( type_index == -2 ) {
        *error = -1;
        return;
    }
    assert( declarator->direct_declarator->type == FUNCTION );

    SymTabEntry *e =
        new SymTabEntry( declarator->id->value, declarator->id->line_num,
                         declarator->id->column );

    e->type = Type( type_index, pointer_level, true );
    e->type.is_function = true;
    e->type.is_pointer = false; /* We don't implement function pointers */
    DirectDeclarator *dd = declarator->direct_declarator;
    if ( dd->params != nullptr ) {
        e->type.num_args = dd->params->param_list.size();
        bool fun_args_valid = true;
        for ( auto j = dd->params->param_list.begin();
              j != dd->params->param_list.end(); j++ ) {
            if ( ( *j )->type.is_invalid() ) {
                fun_args_valid = false;
                break;
            } else {
                e->type.arg_types.push_back( ( *j )->type );
            }
        }

        if ( !fun_args_valid ) {
            delete e;
            *error = -1;
            return;
        }

    } else {

        e->type.num_args = 0;
    }

	e->id = (GLOBAL_SYM_MASK | symbol_id++);
	//MSB 1 -> GLOBAL_SYMBOL 
    add_to_table( e, true, declarator->id );
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

void GlobalSymbolTable::add_to_table( SymTabEntry *symbol, bool redef,
                                      Identifier *id ) {

    auto it = sym_table.find( symbol->name );
    if ( it == sym_table.end() ) {
		symbol->id = (GLOBAL_SYM_MASK | symbol_id++);
		//MSB 1 -> GLOBAL_SYMBOL 
        sym_table.insert( {symbol->name, symbol} );
    } else if ( !( it->second->type == symbol->type ) ) {

        // Can't insert two symbols with same name at the same level
        error_msg( "Conflicting types for " + it->first, id->line_num,
                   id->column );
//        std::cout << "\tPrevious declaration of " << it->first << " at "
//                  << it->second->line_num << ":" << it->second->column << "\n";
//        if ( it->second->line_num == ( code.size() + 1 ) ) {
//            std::cout << "\t" << text.str();
//        } else {
//            std::cout << "\t" << code[it->second->line_num - 1];
//        }
//        printf( "\n\t%*s\n", it->second->column, "^" );
    }
}

Node *add_to_global_symbol_table( Declaration *declaration ) {
    declaration->add_to_symbol_table( global_symbol_table );
    return declaration;
}

void write_to_symtab_file( std::string s ) { sym_file << s; }

//##############################################################################
//########################### SYMBOL TABLE ENTRY ##############################
//##############################################################################

SymTabEntry::SymTabEntry( std::string _name, unsigned int _line_num,
                          unsigned int _column )
    : name( _name ), line_num( _line_num ), column( _column ), offset(0) {};

//##############################################################################
//############################## ERROR MESSAGE #################################
//##############################################################################

void error_msg( std::string str, unsigned int line_num, unsigned int column ) {

    std::cout << "\nLine: " << line_num << ":" << column << " ERROR: " << str << "\n";
    if ( line_num == ( code.size() + 1 ) ) {

        std::cout << "\t" << text.str();
    } else {
        std::cout << "\t" << code[line_num - 1];
    }

    printf( "\n\t%*s\n", column, "^" );
}

void error_msg( std::string str, unsigned int line_num ) {

    std::cout << "\nLine: " << line_num << " ERROR: " << str << "\n";
    if ( line_num == ( code.size() + 1 ) ) {

        std::cout << "\t" << text.str();
    } else {
        std::cout << "\t" << code[line_num - 1];
    }
    std::cout << "\n";

    // printf( "\n\t%*s\n", column, "^" );
}

void warning_msg( std::string str, unsigned int line_num,
                  unsigned int column ) {

    std::cout << "\nLine: "
              << line_num << ":" << column << " WARNING: " << str << "\n";
    if ( line_num == ( code.size() + 1 ) ) {

        std::cout << "\t" << text.str();
    } else {
        std::cout << "\t" << code[line_num - 1];
    }

    printf( "\n\t%*s\n", column, "^" );
}

void warning_msg( std::string str, unsigned int line_num ) {

    std::cout << "\nLine: " << line_num << " WARNING: " << str << "\n";
    if ( line_num == ( code.size() + 1 ) ) {

        std::cout << "\t" << text.str();
    } else {
        std::cout << "\t" << code[line_num - 1];
    }
    std::cout << "\n";

    // printf( "\n\t%*s\n", column, "^" );
}

//##############################################################################
