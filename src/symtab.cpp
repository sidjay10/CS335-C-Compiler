// symtab.cpp

#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <symtab.h>
#include <vector>
#include <y.tab.h>

LocalSymbolTable local_symbol_table;
GlobalSymbolTable global_symbol_table;

std::vector<Types *> defined_types;

unsigned int anon_count = 0;
//##############################################################################
//############################ STRUCT DEFINITION ###############################
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

        std::vector<Declarator *> &dl =
            ( *it )->declarator_list->declarator_list;

        for ( auto jt = dl.begin(); jt != dl.end(); jt++ ) {

            sd->members.insert( {( *jt )->id->value, nullptr} );
            std::cout << "  " << ( *jt )->id->value << "\n";
        }
    }
    std::cout << "}\n";
    return sd;
}

//##############################################################################
//############################# EXPRESSION #####################################
//##############################################################################

// Object creation Wrapper

// 1.Primary Expression
PrimaryExpression *create_primary_identifier( Identifier *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 1;
    P->Ival = a;
    SymTabEntry *ste = local_symbol_table.get_symbol_from_table( a->value );
    if ( ste == nullptr ) {
        ste = global_symbol_table.get_symbol_from_table( a->value );
        if ( ste == nullptr ) {
            // Error
            std::cerr << "Undefined symbol " << a->value << "\n";
            exit( 0 );
        }
    }
    P->typeIndex = ste->typeIndex;
    P->Cval = nullptr;
    P->Sval = nullptr;
    P->op1 = nullptr;

    return P;
}

PrimaryExpression *create_primary_constant( Constant *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 2;
    P->Ival = nullptr;
    P->Cval = a;
    P->typeIndex = a->getConstantType();
    P->Sval = nullptr;
    P->op1 = nullptr;

    return P;
}
PrimaryExpression *create_primary_stringliteral( StringLiteral *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 3;
    P->Ival = nullptr;
    P->Cval = nullptr;
    P->Sval = a;
    P->typeIndex = GlobalTypeMap[PrimitiveTypes::CHAR_T].getPointerTypeIndex();
    P->op1 = nullptr;

    return P;
}
PrimaryExpression *create_primary_expression( TopLevelExpression *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 0;
    P->Ival = nullptr;
    P->Cval = nullptr;
    P->Sval = nullptr;
    P->op1 = a;
    P->typeIndex = a->typeIndex;

    return P;
}

// 2. ArguementExpressionList
ArgumentExprList *
create_argument_expr_assignement( AssignmentExpression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op1 = nullptr;
    P->op2 = ase;
    // ArguementExprList does not have any type as it is a composite entity

    return P;
}
ArgumentExprList *create_argument_expr_list( ArgumentExprList *ae_list,
                                             AssignmentExpression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op1 = ae_list;
    P->op2 = ase;

    return P;
}

// PostFix Expression
PostfixExpression *create_postfix_expr_arr( PostfixExpression *pe,
                                            Expression *e ) {
    PostfixExpression *P = new PostfixExpression();
    P->pe_type = PostfixExpressionTypes::ARRAY;
    Types peT = pe->getType();
    Types eT = e->getType();
    if ( peT.isArrayType() && eT.typeFamily == 0 ) {
        /// XXX: TODO API to get ArrayType
        P->typeIndex = peT.getArrayType().index;
    } else {
        // Tried to access a non-array
        std::cerr << "Subscripted value " << pe->name
                  << " is neither array nor pointer"
    }
    // Set others to null

    return P;
}

PostfixExpression *create_postfix_expr_voidfun( Identifier *fi ) {
    PostfixExpression *P = new PostfixExpression();
    P->pe_type = PostfixExpressionTypes::FUNCTION;
    // Lookup Function type from symbol table - should be void

    return P;
}

PostfixExpression *create_postfix_expr_fun( Identifier *fi,
                                            ArgumentExprList *ae ) {
    PostfixExpression *P = new PostfixExpression();
    P->pe_type = PostfixExpressionTypes::FUNCTION;
    // Here, we need to check two things:
    // 1. whether ArguementExprList matches with Function signature from lookup
    // of symbol table table

    return P;
}

PostfixExpression *create_postfix_expr_struct( std::string access_op,
                                               PostfixExpression *pe,
                                               Identifier *i ) {
    PostfixExpression *P = new PostfixExpression();
    // Struct lookup - use struct array to check if Identifier i is present in
    // struct
    Types peT = pe->getType();
    if ( access_op == "." ) {
        // if(check if peT is struct)
        // if(i is present in struct)
        // P->typeIndex = i->typeIndex;
        // else
        // Error
        // std::cerr << "Element" << i->value << " not present in struct"
    } else if ( access_op == "->" ) {
        // if(check if peT is struct pointer)
        // if(i is present in struct)
    }
    // else
    // Error
    // std::cerr << "Expression is not struct"

    return P;
}
// Unary Expression

UnaryExpression *create_unary_expression_ue( std::string u_op,
                                             UnaryExpression *ue ) {
    UnaryExpression *U = new UnaryExpression();
    U->op1 = ue;
    U->op = u_op;
    Types ueT = ue->getType();
    if ( u_op == "++" || u_op == "--" ) {
        // INC_OP, DEC_OP
        if ( ueT.isPrimitive() ) {
            U->typeIndex = ue->typeIndex;
        } else {
            // Incorrect type throw error
            delete U;
            exit( 0 );
        }
    } else if ( u_op == "sizeof" ) {
        // SizeOf
        U->typeIndex = PrimitiveTypes::U_INT_T;
    } else {
        // Raise Error
    }

    return U;
}

// & (int) (x)
// &(x) -> pointer value of x
UnaryExpression *create_unary_expression_cast( std::string u_op,
                                               CastExpression *ce ) {
    UnaryExpression *U = new UnaryExpression();
    U->op = u_op;
    U->op1 = ce;
    Types ceT = ce->getType();
    if ( u_op == "&" ) {
        // ce->op1 should be of type IDENTIFIER because we dont support function
        // pointers
        if ( ce->typeCast == -1 ) {
            /// XXX:: TODO implement getPointerTypeIndex()
            U->op1->typeIndex = ceT.getPointerTypeIndex();
        } else {
            // Error cannot get reference to type casts
            std::cerr << "lvalue required as unary & operator";
            delete U;
            exit( 0 );
        }
    } else if ( u_op == "*" ) {
        if ( ceT.typeFamily == 2 ) {
            /// XXX:: TODO implement getUnderlyingTypeIndex()
            U->op1->typeIndex = ceT.getUnderlyingTypeIndex();
        } else {
            // Error because of dereference of non-pointer type
            delete U;
            std::cerr << "Error : Invalid dereference of type";
            exit( 0 );
        }
    } else if ( u_op == "-" ) {

    } else if ( u_op == "+" ) {

    } else if ( u_op == "!" ) {

    } else if ( u_op == "~" ) {

    } else {
        // Throw Error
    }
}

// Cast Expression
CastExpression *create_caste_expression_typename( Node *n,
                                                  CastExpression *ce ) {
    // XXX: TODO Implement
    CastExpression *P = new CastExpression();
    P->op1 = ce;

    return P;
}

// Multiplicative Expression
MultiplicativeExpression *
create_multiplicative_expression( std::string op, MultiplicativeExpression *me,
                                  CastExpression *ce ) {
    MultiplicativeExpression *P = new MultiplicativeExpression();
    P->op1 = me;
    P->op2 = ce;
    P->op = op;
    Type meT = me->type;
    Type ceT = ce->type;
    if ( op == "*" || op == "/" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = meT > ceT ? meT : ceT;
            if ( !( meT.isUnsigned() && ceT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else if ( ( meT.isFloat() && ceT.isInt() ) ||
                    ( meT.isInt() && ceT.isFloat() ) ) {

            P->type = meT.isFloat() ? meT : ceT;
        }

        else if ( meT.isFloat() && ceT.isFloat() ) {
            P->type = meT > ceT ? meT : ceT;
        } else {
            std::cerr << "Undefined operation " << op
                      << " for operands of type " << meT.name << " and "
                      << ceT.name << "\n";
        }
    } else if ( op == "%" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = ceT;
            P->type.make_unsigned();
        } else {
            // Error
            std::cerr << "Invalid Operands to binary % having type " << meT.name
                      << " and " << ceT.name << "\n";
            exit( 0 );
        }
    }
}

// TODO : Add Pointer Support
// Additive Expression
AdditiveExpression *create_additive_expression( std::string op,
                                                AdditiveExpression *ade,
                                                MultiplicativeExpression *me ) {
    AdditiveExpression *P = new AdditiveExpression();
    P->op1 = ade;
    P->op2 = me;
    P->op = op;
    Type adeT = ade.type;
    Type meT = me.type;
    if ( meT.isInt() && adeT.isInt() ) {
        P->type = meT > adeT ? meT : adeT;
        if ( !( meT.isUnsigned() && adeT.isUnsigned() ) ) {
            // As a safety we upgrade unsigned type to corresponding signed type
            P->make_signed();
        }

    } else if ( ( meT.isFloat() && ceT.isInt() ) ||
                ( meT.isInt() && ceT.isFloat() ) ) {
        P->type = meT.isFloat() ? meT : ceT;
    }

    else if ( meT.isFloat() && ceT.isFloat() ) {
        P->type = meT > ceT ? meT : ceT;
    } else {
        // Error
        std::cerr << "Undefined operation " << op << " for operands of type "
                  << meT.name << " and " << adeT.name << "\n";
        exit( 0 );
    }
    return P;
}
// Shift Expression

ShiftExpression *create_shift_expression( std::string op, ShiftExpression *se,
                                          AdditiveExpression *ade ) {
    ShiftExpression *P = new ShiftExpression();
    P->op1 = se;
    P->op2 = ade;
    P->op = op;

    Type adeT = ade->type;
    Type seT = se->type;

    if ( op == "<<" || op == ">>" ) {
        if ( adeT.isInt() == 0 && seT.isInt() == 0 ) {
            P->type = seT;
        } else {
            // Operands are not integer type
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << adeT.name << " and "
                      << seT.name << "\n";
            exit( 0 );
        }
    } else {
        // This should not happen
        std::cerr << "Incorrect shift expression. Something went wrong\n";
        exit( 0 );
    }
}

// Relation Expression

RelationalExpression *create_relational_expression( std::string op,
                                                    RelationalExpression *re,
                                                    ShiftExpression *se ) {
    RelationalExpression *P = new RelationalExpression();
    P->op1 = re;
    P->op2 = se;
    P->op = op;
    Type reT = re->type;
    Type seT = se->type;
    if ( op == "<=" || op == ">=" || op == ">" || op == "<" ) {
        if ( ( reT.isInt() || reT.isFloat() ) &&
             ( seT.isInt() || reT.isFloat() ) ) {
            P->type = Type( "bool", U_CHAR_T, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.name << " and "
                      << seT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect relation expression. Something went wrong\n";
        exit( 0 );
    }
}

// Equality Expression
EqualityExpression *create_equality_expression( std::string op,
                                                EqualityExpression *eq,
                                                RelationalExpression *re ) {
    EqualityExpression *P = new EqualityExpression();
    P->op1 = eq;
    P->op2 = re;
    P->op = op;
    Type reT = re->type;
    Type eqT = eq->type;
    if ( op == "==" || op == "!=" ) {
        if ( ( reT.isInt() || reT.isFloat() ) &&
             ( seT.isInt() || reT.isFloat() ) ) {
            P->type = Type( "bool", U_CHAR_T, 0 );
        }

        else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.name << " and "
                      << eqT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect equality expression. Something went wrong\n";
        exit( 0 );
    }
}

// And Expression
AndExpression *create_and_expression( std::string op, AndExpression *an,
                                      EqualityExpression *eq ) {
    AndExpression *P = new AndExpression();
    P->op1 = an;
    P->op2 = eq;
    P->op = op;
    Type anT = an->type;
    Type eqT = eq->type;
    if ( op == "&" ) {
        if ( anT.isInt() && eqT.isInt() ) {
            P->type = anT > eqT ? anT : eqT;
            if ( !( anT.isUnsigned() && eeT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << anT.name << " and "
                      << eqT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect and_expression. Something went wrong\n";
        exit( 0 );
    }
}

// Exclusiveor
ExclusiveorExpression *
create_exclusive_or_expression( std::string op, ExclusiveorExpression *ex,
                                AndExpression *an ) {
    ExclusiveorExpression *P = new ExclusiveorExpression();
    P->op1 = ex;
    P->op2 = an;
    P->op = op;
    Type exT = ex->type;
    Type anT = an->type;
    if ( op == "^" ) {
        if ( anT.isInt() && exT.isInt() ) {
            P->typeIndex = anT.index > exT.index ? anT.index : exT.index;
            if ( !( anT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
		P->type.make_signed();
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << exT.name << " and "
                      << anT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr
            << "Incorrect exclusive or expression. Something went wrong\n";
        exit( 0 );
    }
}

// Inclusiveor
InclusiveorExpression *
create_inclusive_or_expression( std::string op, InclusiveorExpression *ie,
                                ExclusiveorExpression *ex ) {
    InclusiveorExpression *P = new InclusiveorExpression();
    P->op1 = ie;
    P->op2 = ex;
    P->op = op;
    Type ieT = ie->type;
    Type exT = ex->type;
    if ( op == "|" ) {
        if ( ieT.isInt() && exT.isInt() ) {
		P->type = ieT > exT ? ieT : exT ;
            if ( !( ieT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
		P->type.make_signed();
                } else {
                    std::cerr << "Undefined operation of " << op
                              << " on operands of type " << ieT.name << " and "
                              << exT.name << "\n";
                    exit( 0 );
                }
            } else {
                std::cerr << "Incorrect inclusive or expression. Something "
                             "went wrong\n";
                exit( 0 );
            }
        }
    }
}

// Logical And

Logical_andExpression *
creat_logical_and_expression( std::string op, Logical_andExpression *la,
                              InclusiveorExpression *ie ) {
    Logical_andExpression *P = new Logical_andExpression();
    P->op1 = la;
    P->op2 = ie;
    P->op = op;
    // To be confirmed
    Type laT = la->type;
    Type ieT = ie->type;
    if ( op == "&&" ) {
        if ( ieT.isIntOrFloat() && laT.isIntOrFloat() ) {
            P->type = Type( "bool", U_CHAR_T, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ieT.name << " and "
                      << exT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical and expression. Something went "
                     "wrong\n";
        exit( 0 );
    }
}

// Logical or
Logical_orExpression *create_logical_or_expression( std::string op,
                                                   Logical_orExpression *lo,
                                                   Logical_andExpression *la ) {
    Logical_orExpression *P = new Logical_orExpression();
    P->op1 = lo;
    P->op2 = la;
    P->op = op;
    // To be confirmed
    Type laT = la->type;
    Type ieT = ie->type;
    if ( op == "&&" ) {
        if ( ieT.isIntOrFloat() && laT.isIntOrFloat() ) {
            P->type = Type( "bool", U_CHAR_T, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << laT.name << " and "
                      << exT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical or expression. Something went "
                     "wrong\n";
        exit( 0 );
    }
}

// Conditional

ConditionalExpression *
create_conditional_expression( std::string op, Logical_orExpression *lo,
                               TopLevelExpression *te,
                               ConditionalExpression *coe ) {
    ConditionalExpression *P = new ConditionalExpression();
    P->op1 = lo;
    P->op2 = te;
    P->op3 = coe;
    P->op = op;
    Types loT = lo->getType();
    Types teT = te->getType();
    Types coeT = coe->getType();
    if ( loT.typeFamily == 0 ) {
        if ( teT.typeFamily < 2 && coeT.typeFamily < 2 ) {
            P->typeIndex = teT.index > coeT.index ? teT.index : coeT.index;
            if ( !( tetT.isUnsigned() && coeT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding
                // signed type
		P->type.make_signed();
            }
        } else if ( tet.type == coet.type ) {
                P->typeIndex = tet.type;
        }
	else {
		
		//ERROR: 
		exit( 0 );
	}
    }
}

// AssignmentExpression

AssignmentExpression *
create_assignment_expression( std::string op, UnaryExpression *ue,
                              AssignmentExpression *ase ) {
    AssignmentExpression *P = new AssignmentExpression();
    P->op1 = ue;
    P->op2 = ase;
    P->op = op;
    Types ueT = ue->getType();
    Types aseT = ase->getType();
    if ( op == "=" ) {
        if ( ueT.typeFamily < 2 && aseT.typeFamily < 2 ) {
            // int and/or flot
            if ( ueT.index != aseT.index ) {
                std::cout << "Warning: operation " << op << " between "
                          << ueT.name << " and " << aseT.name;
            }
            P->typeIndex = ueT.index;
        } else if ( ueT.typeFamily == aseT.typeFamily && ueT.typeFamily == 2 ) {
            /// meed the types of pointers
            if ( ueT.index == aseT.index ) {
                P->typeIndex = ueT.index;
            } else {
                std::cerr << "Undefined operation of " << op
                          << " on operands of type " << ueT.name << " and "
                          << aseT.name << "\n";
                exit( 0 );
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.name << " and "
                      << aseT.name << "\n";
            exit( 0 );
        }
    } else if ( op == "*=" || op == "/=" || op == "+=" || op == "-=" ) {
        if ( ueT.isPrimitive() && aseT.isPrimitive() ) {
            // int and/or flot
            if ( ueT.index != aseT.index ) {
                std::cout << "Warning: operation " << op << " between "
                          << ueT.name << " and " << aseT.name << "\n";
            }
            P->typeIndex = ueT.index;
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.name << " and "
                      << aseT.name << "\n";
            exit( 0 );
        }
    } else if ( op == "%=" ) {
        if ( ueT.typeFamily == 0 && aseT.typeFamily == 0 ) {
            P->typeIndex = PrimitiveTypes::U_INT_T;
        } else {
            // Error
            std::cerr << "Invalid Operands " << op << "having type " << ueT.name
                      << " and " << ceT.name << "\n";
            exit( 0 );
        }
    } else if ( op == "<<=" || op == ">>=" ) {
        if ( ueT.typeFamily == 0 && aseT.typeFamily == 0 ) {
            P->typeIndex = ueT.index;
        } else {
            // Operands are not integer type
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << adeT.name << " and "
                      << se->name << "\n";
            exit( 0 );
        }
    } else if ( op == "&=" || op == "|=" && op == "^=" ) {
        if ( ueT.typeFamily == 0 && aseT.typeFamily == 0 ) {
            // int family
            P->typeIndex = ueT.index > aseT.index ? ueT.index : aseT.index;
            if ( !( anT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding
                // signed type
                if ( P->typeIndex == PrimitiveTypes::U_CHAR_T ) {
                    P->typeIndex = PrimitiveTypes::CHAR_T;
                } else if ( P->typeIndex == PrimitiveTypes::U_SHORT_T ) {
                    P->typeIndex = PrimitiveTypes::SHORT_T;
                } else if ( P->typeIndex == PrimitiveTypes::U_INT_T ) {
                    P->typeIndex = PrimitiveTypes::INT_T;
                } else if ( P->typeIndex == PrimitiveTypes::UL_INT_T ) {
                    P->typeIndex = PrimitiveTypes::L_INT_T;
                }
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << exT.name << " and "
                      << anT.name << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical or expression. Something went "
                     "wrong\n";
        exit( 0 );
    }
}

// TopLevelExpression

TopLevelExpression *create_toplevel_expression( TopLevelExpression *te,
                                                AssignmentExpression *ase ) {
    TopLevelExpression *P = new TopLevelExpression();
    P->op1 = te;
    P->op2 = ase;
    Types tet = te->getType();
    Types aset = ase->getType();
    //////need TO DO LATER///////////
}

//##############################################################################
//################################ POINTER #####################################
//##############################################################################
void is_Valid( TypeQualifierList *ts ) {
    for ( unsigned int i = 0; i < ts->type_qualifier_list.size(); i++ ) {
        if ( ts->type_qualifier_list.at( i ) == CONST ||
             ts->type_qualifier_list.at( i ) == VOLATILE ) {
        } else {
            std::cout << "Error in pointer type qualfier pointer";
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
    is_Valid( type_list );
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

TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList *tql,
                                               TYPE_QUALIFIER type ) {
    tql->append_to_list( type );
    return tql;
}

//##############################################################################
//############################# DECLARATION
//####################################
//##############################################################################
int get_index( Types t ) {
    std::vector<Types>::iterator it;
    it = std::find( GlobalTypeMap.begin(), GlobalTypeMap.end(), t );
    if ( it != GlobalTypeMap.end() )
        return std::distance( GlobalTypeMap.begin(), it ) else {
            GlobalTypeMap.push_back( t );
            return GlobalTypeMap.size() - 1;
        }
}

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

    int tc = 0, sc = 0, tq = 0, ind = 0, typeF = 0, pL = 0, nd = 0, s = 0,
        uD = 0;
    int err = 0;
    if ( declaration_specifiers->storage_class.size() == 1 ) {
        if ( declaration_specifiers->storage_class.at( 0 ) == TYPEDEF )
            sc = 1;
        else
            err += 1;
    }

    std::vector<TYPE_SPECIFIER> ty;
    for ( int i = 0; i < declaration_specifiers->type_specifier.size(); i++ )
        ty.push_back( declaration_specifiers->type_specifier.at( i )->type );

    std::sort( ty.begin(), ty.end() );

    if ( ty.size() == 3 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == INT &&
             ty.at( 2 ) == LONG ) {
            tc = 8;
            ind = 8;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == INT &&
                    ty.at( 2 ) == LONG ) {
            tc = 9;
            ind = 9;
        } else
            err += 2;
    } else if ( ty.size() == 2 ) {
        if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == CHAR ) {
            tc = 0;
            ind = 0;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == CHAR ) {
            tc = 1;
            ind = 1;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == SHORT ) {
            tc = 2;
            ind = 2;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == SHORT ) {
            tc = 3;
            ind = 3;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == INT ) {
            tc = 4;
            ind = 4;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == INT ) {
            tc = 5;
            ind = 5;
        } else if ( ty.at( 0 ) == UNSIGNED && ty.at( 1 ) == LONG ) {
            tc = 6;
            ind = 6;
        } else if ( ty.at( 0 ) == SIGNED && ty.at( 1 ) == LONG ) {
            tc = 7;
            ind = 7;
        } else if ( ty.at( 0 ) == FLOAT && ty.at( 1 ) == DOUBLE ) {
            tc = 12;
            ind = 12;
            typeF = 1;
        } else
            err += 2;
    } else if ( ty.size() == 1 ) {
        if ( ty.at( 0 ) == FLOAT ) {
            tc = 10;
            ind = 10;
            typeF = 1;
        } else if ( ty.at( 0 ) == DOUBLE ) {
            tc = 11;
            ind = 11;
            typeF = 1;
        } else
            err += 2;
    }
    for ( int i = 0; i < declaration_specifiers->type_qualifier.size(); i++ ) {
        if ( declaration_specifiers->type_qualifier.at( i ) == CONST ) {
            tq += 1;
        } else if ( declaration_specifiers->type_qualifier.at( i ) ==
                    VOLATILE ) {
            tq += 2;
        } else {
            err += 4;
            break;
        }
    }
    if ( err & 1 ) {
        std::cerr << "Error in strorage class declarator";
        exit( 0 );
    }
    if ( err & 2 ) {
        std::cerr << "Error in type specifier declarator";
        exit( 0 );
    }
    if ( err & 4 ) {
        std::cerr << "Error in type qualifier declarator";
        exit( 0 );
    }
    // To take care: const, pointer, struct , enum, | size to be added.
    Types t = new Types( tc, sc, tq, ind, typeF, pL, nd, s, uD );
    d->type = get_index( t );
    is_Valid( declaration_specifiers );
    return d;
}

void Declaration::add_to_symbol_table( LocalSymbolTable &sym_tab ) {

    if ( init_declarator_list == nullptr )
        return;
    std::vector < Declarator * > &dec = init_declarator_list->declarator_list;

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
//########################## DECLARATION SPECIFIERS
//############################
//##############################################################################

DeclarationSpecifiers ::DeclarationSpecifiers()
    : Non_Terminal( "declaration_specifiers" ){};

void is_Valid( DeclarationSpecifiers *ds ) {
    int err = 0;
    if ( ds->storage_class.size() == 1 ) {
        if ( ds->storage_class.at( 0 ) == TYPEDEF ) {
            err = 0;
        } else {
            err += 1;
        }
    }

    std::vector<TYPE_SPECIFIER> ty;

    for ( unsigned int i = 0; i < ds->type_specifier.size(); i++ ) {
        ty.push_back( ds->type_specifier.at( i )->type );
    }
    std::sort( ty.begin(), ty.end() );

    if ( ty.size() == 3 ) {
        if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
             ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG ) &&
             ty.at( 2 ) == INT ) {
        } else {
            err += 2;
        }
    } else if ( ty.size() == 2 ) {
        if ( ( ty.at( 0 ) == UNSIGNED || ty.at( 0 ) == SIGNED ) &&
             ( ty.at( 1 ) == SHORT || ty.at( 1 ) == LONG || ty.at( 1 ) == INT ||
               ty.at( 1 ) == CHAR ) ) {
        } else if ( ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG ) &&
                    ty.at( 1 ) == INT ) {
        } else if ( ty.at( 0 ) == LONG && ty.at( 1 ) == DOUBLE ) {
        } else {
            err += 2;
        }
    } else if ( ty.size() == 1 ) {
        if ( ty.at( 0 ) == SHORT || ty.at( 0 ) == LONG || ty.at( 0 ) == INT ||
             ty.at( 0 ) == CHAR || ty.at( 0 ) == FLOAT ||
             ty.at( 0 ) == DOUBLE || ty.at( 0 ) == STRUCT ||
             ty.at( 0 ) == UNION || ty.at( 0 ) == ENUM ) {
        } else {
            err += 2;
        }
    }
    for ( unsigned int i = 0; i < ds->type_qualifier.size(); i++ ) {
        if ( ds->type_qualifier.at( i ) == CONST ||
             ds->type_qualifier.at( i ) == VOLATILE ) {
        } else {
            err += 4;
            break;
        }
    }
    // for(int i=0; i < ds->type_qualifier.size(); i++)
    // std::cout << ds->type_qualifier.at(i) << ' ';
    if ( err & 1 ) {
        {
            std::cout << "Error in strorage class declarator\n";
        }
        if ( err & 2 ) {
            std::cout << "Error in type specifier declarator\n";
        }
        if ( err & 4 ) {
            std::cout << "Error in type qualifier declarator\n";
        }
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
            if ( err & 2 )
                std::cout << "Error in type specifier struct";
            if ( err & 4 )
                std::cout << "Error in type qualifier struct";
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
    global_symbol_table.add_symbol( declaration_specifiers, declarator );
    local_symbol_table.add_function( declaration_specifiers, declarator );
    return fd;
}

//##############################################################################
//############################## TYPE SPECIFIER
//################################
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
    if ( struct_declaration_list != nullptr ) {
        verify_struct_declarator( struct_declaration_list );
        std::string struct_name;

        if ( id == nullptr ) {
            struct_name = "anon" + std::to_string( anon_count );
            anon_count++;
        } else {
            struct_name = id->value;
        }

        Types *struct_type = new Types;
        struct_type->name = struct_name;
        struct_type->is_primitive = false;
        struct_type->pointer_level = 0;

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
        struct_type->struct_definition =
            create_struct_definition( type, struct_declaration_list );

        struct_type->size = struct_type->struct_definition->get_size();

        struct_type->pointer_level = 0;
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
    return sd;
}

//##############################################################################
//######################### SPECIFIER QUALIFIER LIST
//###########################
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
        std::cout << "  ";
    }
    std::cout << "{\n";
    current_level++;
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
    std::cout << "L: " << function_name;

    // Check whether the arguements are of the form ( )
    if ( declarator->direct_declarator->params == nullptr ) {
        std::cout << "( )\n";
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
                std::cout << "( void )\n";
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
//########################### GLOBAL SYMBOL TABLE
//##############################
//##############################################################################

void GlobalSymbolTable::add_symbol(
    DeclarationSpecifiers *declaration_specifiers, Declarator *declarator ) {

    std::cout << "G: " << declarator->id->value << "\n";
}

SymTabEntry *GlobalSymbolTable::get_symbol_from_table( std::string name ) {
    auto it = sym_table.find( name );
    if ( it == sym_table.end() ) {
        return nullptr;
    } else {
        return it->second;
    }
}

Node *add_to_global_symbol_table( Declaration *declaration ) {
    declaration->add_to_symbol_table( global_symbol_table );
    return declaration;
}

//##############################################################################
//########################### SYMBOL TABLE ENTRY
//##############################
//##############################################################################

SymTabEntry::SymTabEntry( std::string name ) : name( name ){};

//##############################################################################
