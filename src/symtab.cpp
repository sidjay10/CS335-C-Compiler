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
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>

LocalSymbolTable local_symbol_table;
GlobalSymbolTable global_symbol_table;

std::vector<Types> GlobalTypesMap;

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

StructDefinition *create_struct_definition( int un_or_st, StructDeclarationList *sdl ) {
    StructDefinition *sd = new StructDefinition();
    sd->un_or_st = un_or_st;
    std::cout << "struct {\n";
    for ( auto it = sdl->struct_declaration_list.begin();
          it != sdl->struct_declaration_list.end(); it++ ) {
        // TODO: Complete this

        std::vector<Declarator *> dl =
            ( *it )->declarator_list->declarator_list;

        for ( auto jt = dl.begin(); jt != dl.end(); jt++ ) {

            sd->members.insert(std::make_pair((*jt)->id->value, Type(-1,0,0)));
            std::cout << "  " << ( *jt )->id->value << "\n";
        }
    }
    std::cout << "}\n";
    return sd;
}

//##############################################################################
//############################# EXPRESSION #####################################
//##############################################################################

// Object creation Wrappers

// 1.Primary Expression
Expression *create_primary_identifier( Identifier *a ) {
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

    P->type = ste->type;
    P->name = "primary_expression";
    P->add_child(a);
    return P;
}

Expression *create_primary_constant( Constant *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 2;
    P->Cval = a;
    P->type = a->getConstantType();

    P->name = "primary_expression";
    P->add_child(a);

    return P;
}
Expression *create_primary_stringliteral( StringLiteral *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 3;
    P->Sval = a;
    P->type.typeIndex = PrimitiveTypes::CHAR_T;
    P->type.ptr_level = 1;

    P->name = "primary_expression";
    P->add_child(a);
    return P;
}
Expression *create_primary_expression( Expression *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 0;
    P->op1 = a;
    P->type = a->type;

    P->name = "primary_expression";
    P->add_child(a);

    return P;
}

// 2. ArguementExpressionList
Expression *create_argument_expr_assignement( Expression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op2 = ase;
    // ArguementExprList does not have any type as it is a composite entity
    P->name = "assignment_expression";
    P->add_child(ase);
    
    return P;
}
Expression *create_argument_expr_list( Expression *ae_list,
                                             Expression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op1 = ae_list;
    P->op2 = ase;
    
    P->name="assignment_expression_list";
    P->add_children(ae_list,ase);
    return P;
}

// PostFix Expression
Expression *create_postfix_expr_arr( Expression *pe,
                                            Expression *e ) {
    PostfixExpression *P = new PostfixExpression();
    if(dynamic_cast<PostfixExpression *>(pe)){
        P->pe = dynamic_cast<PostfixExpression *>(pe);
    }
    else
        P->pe = nullptr;
    P->exp = e;
    Types peT = GlobalTypesMap[pe->type.typeIndex];

    if ( pe->type.ptr_level == 1 ) {
        if ( e->type.isInt() ) {
            P->type = pe->type;
            P->type.ptr_level = 0;
        } else {
            // Error
            std::cerr << "Array subscript is not an integer";
            exit( 0 );
        }
    }

    P->name = "ARRAY ACCESS";
    P->add_children(pe, e);
    return P;
}

Expression *create_postfix_expr_voidfun( Identifier *fi ) {
    PostfixExpression *P = new PostfixExpression();
    // Lookup Function type from symbol table - should be void
       
    P->name = "FUNCTION CALL";
    P->add_child(fi);

    return P;
}

Expression *create_postfix_expr_fun( Identifier *fi,
                                            Expression *ae ) {
    PostfixExpression *P = new PostfixExpression();
    // Here, we need to check two things:
    // 1. whether ArguementExprList matches with Function signature from lookup
    // of symbol table table

    P->name = "FUNCTION CALL";
    P->add_children(fi,ae);

    return P;
}

Expression *create_postfix_expr_struct( std::string access_op,
                                               Expression *pe,
                                               Identifier *i ) {
    PostfixExpression *P = new PostfixExpression();
    Types peT = GlobalTypesMap[pe->type.typeIndex];
    if ( access_op == "." ) {
        if ( peT.is_struct && pe->type.ptr_level == 0 ) {
            // whether i exists in Struct
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                std::cerr << "Error";
            } else {
                P->type = *iType;
            }
        } else {
            std::cerr<<"Operand Error";exit(0);
        }
    } else if ( access_op == "->" ) {
        if ( peT.is_struct && pe->type.ptr_level == 1 ) {
            // whether i exists in Struct*
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                std::cerr << "Error";
            } else {
                P->type = *iType;
            }
        } else {
            std::cerr<<"Operand Error";exit(0);
        }
    }

    P->name = access_op;
    P->add_children(pe, i);
    return P;
}

Expression *create_postfix_expr_ido( std::string op,
                                            Expression *pe ) {
    PostfixExpression *P = new PostfixExpression();
    if(dynamic_cast<PostfixExpression *>(pe)){
        P->pe = dynamic_cast<PostfixExpression *>(pe);
    }
    else{
        P->pe = nullptr;
    }
    P->op = op;
    if ( op == "++" || op == "--" ) {
        if ( pe->type.isInt() || pe->type.isFloat() || pe->type.isPointer() ) {
            P->type = pe->type;

        } else {
            // Error postfix operator
            std::cerr << "Postfix operator " << op
                      << " cannot be applied to type";
            exit( 0 );
        }
    } else {
        // This should not have reached
        std::cerr << "Parse error";
        exit( 0 );
    }
    if(op == "++")
        P->name = "POST INCREMENT";
    else
        P->name = "POST DECREMENT";
    
    P->add_child(pe);
    return P;
}

// Unary Expression
Expression *create_unary_expression_ue( std::string u_op,
                                             Expression *ue ) {
    UnaryExpression *U = new UnaryExpression();
    U->op1 = ue;
    U->op = u_op;
    Type ueT = ue->type;
    if ( u_op == "++" || u_op == "--" ) {
        // INC_OP, DEC_OP
        if ( ueT.isInt() || ueT.isFloat() || ueT.isPointer() ) {
            U->type = ue->type;
        } else {
            // Incorrect type throw error
            delete U;
            std::cerr << "Prefix operator " << u_op << " cannot be applied to type " << ue->type.get_name();
            exit(0);
        }
    }
    else if(u_op == "sizeof"){
        //SizeOf
        U->type.typeIndex = PrimitiveTypes::U_INT_T;
        U->type.ptr_level = 0;
        U->type.is_const = false;
    }
    else{
        //Raise Error
        std::cerr << "Error parsing Unary Expression.\n";
        exit(0);
    }
    U->name = 'unary_expression';
    U->add_child(ue);
    return U;
}

// & (int) (x)
// &(x) -> pointer value of x
Expression *create_unary_expression_cast( Node *n_op,
                                               Expression *ce ) {
    UnaryExpression *U = new UnaryExpression();
    Non_Terminal * nt_op = dynamic_cast<Non_Terminal *>(n_op);
    std::string u_op = nt_op->name;
    U->op = u_op;
    U->op1 = ce;
    Type ceT = ce->type;
    if (u_op == "&"){
        // ce->op1 should be of type IDENTIFIER because we dont support function pointers
        ///XXX:: TODO implement getPointerTypeIndex()
        U->type = ce->type;
        U->type.ptr_level++;
        // else{
        //     //Error cannot get reference to type casts
        //     std::cerr << "lvalue required as unary & operator";
        //     delete U;
        //     exit(0);
        // }
    }
    else if(u_op == "*"){
        if(ce->type.ptr_level > 0){
            U->type = ce->type;
            U->type.ptr_level--;            
        }
        else{
            //Error because of dereference of non-pointer type
            delete U;
            std::cerr << "Error : Invalid dereference of type " << ce->type.get_name();
            exit(0);
        }
    } else if ( u_op == "-" || u_op == "+" || u_op == "!" || u_op == "-") {
        if(ce->type.isIntorFloat()){
            U->type = ce->type;    
        }
        else{
            //Throw Error
            std::cerr << "Invalid use of unary operator " << u_op << " on type " << ce->type.get_name();
            exit(0);
        }
    } else {
        // Throw Error
        std::cerr << "Parse error, invalid unary operator";
        exit(0);
    }
    
    U->name = "unary_expression";
    U->add_children(n_op, ce);
    return U;
}

Expression *create_unary_expression_typename(std::string u_op, Node *t_name){
    UnaryExpression *U = new UnaryExpression();
    ///XXX:TODO Handle
    
    Node * n_op = create_non_term((u_op).c_str());
    U->name = u_op;
    U->add_children(n_op, t_name);

    return U;
}

// Cast Expression
Expression *create_cast_expression_typename( Node *n,
                                                  Expression *ce ) {
    // XXX: TODO Implement
    CastExpression *P = new CastExpression();
    P->op1 = ce;
    P->type = ce->type;
    
    P->name="caste_expression";
    P->add_children(n,ce);
    return P;
}

// Multiplicative Expression
Expression *
create_multiplicative_expression( std::string op, Expression *me,
                                  Expression *ce ) {
    MultiplicativeExpression *P = new MultiplicativeExpression();
    P->op1 = me;
    P->op2 = ce;
    P->op = op;
    Type meT = me->type;
    Type ceT = ce->type;
    if ( op == "*" || op == "/" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = meT.typeIndex > ceT.typeIndex ? meT : ceT;
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
            P->type = meT.typeIndex > ceT.typeIndex ? meT : ceT;
        } else {
            std::cerr << "Undefined operation " << op
                      << " for operands of type " << meT.get_name() << " and "
                      << ceT.get_name() << "\n";
        }
    } else if ( op == "%" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = ceT;
            P->type.make_unsigned();
        } else {
            // Error
            std::cerr << "Invalid Operands to binary % having type "
                      << meT.get_name() << " and " << ceT.get_name() << "\n";
            exit( 0 );
        }
    }
    
    P->name="multiplicative expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(me, n_op, ce);
    return P;
}

// TODO : Add Pointer Support
// Additive Expression
Expression *create_additive_expression( std::string op,
                                                Expression *ade,
                                                Expression *me ) {
    AdditiveExpression *P = new AdditiveExpression();
    P->op1 = ade;
    P->op2 = me;
    P->op = op;
    Type adeT = ade->type;
    Type meT = me->type;
    if ( meT.isInt() && adeT.isInt() ) {
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
        if ( !( meT.isUnsigned() && adeT.isUnsigned() ) ) {
            // As a safety we upgrade unsigned type to corresponding signed type
            P->type.make_signed();
        }

    } else if ( ( meT.isFloat() && adeT.isInt() ) ||
                ( meT.isInt() && adeT.isFloat() ) ) {
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( meT.isPointer() && ( ( meT.isFloat() && adeT.isInt() ) ||
                                   ( meT.isInt() && adeT.isFloat() ) ) ) {
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( meT.isFloat() && adeT.isFloat() ) {
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
    } else {
        // Error
        std::cerr << "Undefined operation * for operands of type " << meT.get_name()
                  << " and " << adeT.get_name() << "\n";
        exit( 0 );
    }

    P->name="additive_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(ade, n_op, me);
    
    return P;

}
// Shift Expression

Expression *create_shift_expression( std::string op, Expression *se,
                                          Expression *ade ) {
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
                      << " on operands of type " << adeT.get_name() << " and "
                      << seT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        // This should not happen
        std::cerr << "Incorrect shift expression. Something went wrong\n";
        exit( 0 );
    }
    
    P->name="shift_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(se, n_op, ade);
    return P;
}

// Relation Expression

Expression *create_relational_expression( std::string op,
                                                    Expression *re,
                                                    Expression *se ) {
    RelationalExpression *P = new RelationalExpression();
    P->op1 = re;
    P->op2 = se;
    P->op = op;
    Type reT = re->type;
    Type seT = se->type;
    if ( op == "<=" || op == ">=" || op == ">" || op == "<" ) {
        if ( ( reT.isInt() || reT.isFloat() ) &&
             ( seT.isInt() || reT.isFloat() ) ) {
            P->type = Type( U_CHAR_T, 0,0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.get_name() << " and "
                      << seT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect relation expression. Something went wrong\n";
        exit( 0 );
    }
    
    P->name="relational_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(re,n_op, se);
    return P;
}

// Equality Expression
Expression *create_equality_expression( std::string op,
                                                Expression *eq,
                                                Expression *re ) {
    EqualityExpression *P = new EqualityExpression();
    P->op1 = eq;
    P->op2 = re;
    P->op = op;
    Type reT = re->type;
    Type eqT = eq->type;
    if (op == "==" || op == "!=" ) {
        if (( reT.isInt() || reT.isFloat() ) &&
             ( eqT.isInt() || eqT.isFloat() ) ) {
            P->type = Type( U_CHAR_T, 0,0 );
        }

        else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.get_name() << " and "
                      << eqT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect equality expression. Something went wrong\n";
        exit( 0 );
    }
    
    P->name="eqality_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(eq, n_op, re);
    return P;
}

// And Expression
Expression *create_and_expression( std::string op, Expression *an,
                                      Expression *eq ) {
    AndExpression *P = new AndExpression();
    P->op1 = an;
    P->op2 = eq;
    P->op = op;
    Type anT = an->type;
    Type eqT = eq->type;
    if ( op == "&" ) {
        if ( anT.isInt() && eqT.isInt() ) {
            P->type = anT.typeIndex > eqT.typeIndex ? anT : eqT;
            if ( !( anT.isUnsigned() && eqT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << anT.get_name() << " and "
                      << eqT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect and_expression. Something went wrong\n";
        exit( 0 );
    }
    
    P->name="and_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(an, n_op, eq);
    return P;
}

Expression *
create_exclusive_or_expression( std::string op, Expression *ex,
                                Expression *an ) {
    ExclusiveorExpression *P = new ExclusiveorExpression();
    P->op1 = ex;
    P->op2 = an;
    P->op = op;
    Type exT = ex->type;
    Type anT = an->type;
    if ( op == "^" ) {
        if ( anT.isInt() && exT.isInt() ) {
            P->type = anT.typeIndex > exT.typeIndex ? anT : exT;
            if ( !( anT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << exT.get_name() << " and "
                      << anT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr
            << "Incorrect exclusive or expression. Something went wrong\n";
        exit( 0 );
    }
    
    P->name="exclusive_or_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(ex, n_op, an);
    return P;
}

Expression* create_inclusive_or_expression( std::string op, Expression *ie, Expression *ex ) {
    InclusiveorExpression *P = new InclusiveorExpression();
    P->op1 = ie;
    P->op2 = ex;
    P->op = op;
    Type ieT = ie->type;
    Type exT = ex->type;
    if ( op == "|" ) {
        if ( ieT.isInt() && exT.isInt() ) {
            P->type = ieT.typeIndex > exT.typeIndex ? ieT : exT;
            if ( !( ieT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            } else {
                std::cerr << "Undefined operation of " << op
                          << " on operands of type " << ieT.get_name() << " and "
                          << exT.get_name() << "\n";
                exit( 0 );
            }
        } else {
            std::cerr << "Incorrect inclusive or expression. Something "
                         "went wrong\n";
            exit( 0 );
        }
    }

    P->name="inclusive_or_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(ie, n_op, ex);
    return P;
}

// Logical And
Expression *
create_logical_and_expression( std::string op, Expression *la,
                              Expression *ie ) {
    Logical_andExpression *P = new Logical_andExpression();
    P->op1 = la;
    P->op2 = ie;
    P->op = op;
    // To be confirmed
    Type laT = la->type;
    Type ieT = ie->type;
    if ( op == "&&" ) {
        if ( ieT.isIntorFloat() && laT.isIntorFloat() ) {
            P->type = Type(U_CHAR_T, 0,0);
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ieT.get_name() << " and "
                      << laT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical and expression. Something went "
                     "wrong\n";
        exit( 0 );
    }
    
    P->name="logical_and_expression";
    Node * n_op = create_non_term((op).c_str());
    P->add_children(la, n_op, ie);
    return P;
    
}

// Logical or
Expression *
create_logical_or_expression( std::string op, Expression *lo,
                              Expression *la ) {
    Logical_orExpression *P = new Logical_orExpression();
    P->op1 = lo;
    P->op2 = la;
    P->op = op;
    // To be confirmed
    Type loT = lo->type;
    Type laT = la->type;
    if ( op == "&&" ) {
        if ( loT.isIntorFloat() && laT.isIntorFloat() ) {
            P->type = Type( U_CHAR_T, 0,0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << laT.get_name() << " and "
                      << loT.get_name() << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical or expression. Something went "
                     "wrong\n";
        exit( 0 );
    }

    P->name="logical_or_expression";
    P->add_children(lo,la);
    return P;
}

// Conditional

Expression *
create_conditional_expression( std::string op, Expression *lo,
                               Expression *te,
                               Expression *coe ) {
    ConditionalExpression *P = new ConditionalExpression();
    P->op1 = lo;
    P->op2 = te;
    P->op3 = coe;
    P->op = op;
    Type loT = lo->type;
    Type teT = te->type;
    Type coeT = coe->type;
    if ( loT.isInt() ) {
        if ( teT.isIntorFloat() && coeT.isIntorFloat() ) {
            P->type = teT.typeIndex > coeT.typeIndex ? teT : coeT;
            if ( !( teT.isUnsigned() && coeT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding
                // signed type
                P->type.make_signed();
            }
        } else if ( teT.typeIndex == coeT.typeIndex && teT.ptr_level == coeT.ptr_level ) {
            P->type= teT;
        } else {

            // ERROR:
            exit( 0 );
        }
    }

    P->name="conditional_expression";
    P->add_children(lo,te,coe);
    return P;
}

// AssignmentExpression

Expression *create_assignment_expression( Expression *ue, Node *n_op, Expression *ase ) {
    AssignmentExpression *P = new AssignmentExpression();
    Non_Terminal * nt_op = dynamic_cast<Non_Terminal *>(n_op);
    std::string op = nt_op->name;
    P->op1 = ue;
    P->op2 = ase;
    P->op = op;
    Type ueT = ue->type;
    Type aseT = ase->type;
    if (op == "=") {
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                std::cout << "Warning: operation " << op << " between " << ueT.get_name() << " and " << aseT.get_name();
            }
            P->type = ueT;
        } 
        else if ( ueT.ptr_level > 0 && aseT.ptr_level > 0 && ueT.ptr_level == aseT.ptr_level ) {
            /// meed the types of pointers
            if ( ueT.typeIndex == aseT.typeIndex ) {
                P->type = ueT;
            }
            else {
            std::cerr << "Undefined operation of " << op
                    << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            exit( 0);
            }
        }
    }    
    else if ( op == "*=" || op == "/=" || op == "+=" || op == "-=" ) {
            if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
                if ( ueT.typeIndex != aseT.typeIndex ) {
                std::cout << "Warning: operation " << op << " between "
                          << ueT.get_name() << " and " << aseT.get_name() << "\n";
                }
                P->type = ueT;
            }else {
                std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            exit( 0 );
            }
    }
    else if ( op == "%=" ) {
        if( (ueT.isInt() && aseT.isInt())){
            P->type = ueT;
        } else {
            // Error
            std::cerr << "Invalid Operands " << op << "having type " << ueT.get_name()
                      << " and " << aseT.get_name() << "\n";
            exit( 0 );
        }
    }
    else if ( op == "<<=" || op == ">>=" ) {
        if ( ueT.isInt() && aseT.isInt()){
            P->type = ueT;
        }
        else {
            // Operands are not integer type
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << ase->name << "\n";
            exit( 0 );
        }
    }
    else if ( op == "&=" || op == "|=" && op == "^=" ) {
        if ( ueT.isInt() && aseT.isInt() ) {
            // int family
            P->type = ueT;
            if ( !( ueT.isUnsigned() && aseT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } 
        else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            exit( 0 );
        }
        
    } 
    else {
        std::cerr << "Incorrect logical or expression. Something went wrong\n";
        exit( 0 );
    }

    P->name="assignment_expression";
    P->add_children(ue, ase);
    return P;
}

// TopLevelExpression

Expression *create_toplevel_expression( Expression *te, Expression *ase ) {
    TopLevelExpression *P = new TopLevelExpression();
    P->op1 = te;
    P->op2 = ase;
    Type teT = te->type;
    Type aseT = ase->type;
    //////need TO DO LATER///////////
    
    P->name="toplevel_expression";
    P->add_children(te,ase);
    return P;
}

//##############################################################################
//################################ POINTER #####################################
//##############################################################################
    Pointer::Pointer() : Non_Terminal( "pointer" ){};

    Pointer::Pointer( TypeQualifierList * type_list, Pointer * pointer_ )
        : Non_Terminal( "pointer" ), type_qualifier_list( type_list ),
          pointer( pointer_ ){};

    Pointer *create_pointer() {
        Pointer *p = new Pointer();
        return p;
    }

    Pointer *create_pointer( TypeQualifierList * type_list,
                             Pointer * pointer ) {
        Pointer *p = new Pointer( type_list, pointer );
        p->add_children( type_list, pointer );

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

// Declaration ::Declaration(DeclarationSpecifiers *declaration_specifiers_, DeclaratorList *init_declarator_list_ )
//     : Non_Terminal( "declaration" ), declaration_specifiers( declaration_specifiers_ ), init_declarator_list( init_declarator_list_ ){};

int get_index(Type t){
    bool k=true;
    if (t.typeIndex==-1){
        auto it = find_if(GlobalTypesMap.begin(), GlobalTypesMap.end(), [&k](const Types & obj) {return obj.is_struct == k;});
        t.typeIndex=std::distance(it, GlobalTypesMap.begin());
    }
    else if(t.typeIndex==-2){
        auto it = find_if(GlobalTypesMap.begin(), GlobalTypesMap.end(), [&k](const Types & obj) {return obj.is_union == k;});
        t.typeIndex=std::distance(it, GlobalTypesMap.begin());
    }
    return t.typeIndex;
}
int set_index(DeclarationSpecifiers *ds)
{   
    ds->is_const=false;
    int err=0;
    if(ds->storage_class.size()==1){
        if(ds->storage_class.at(0)==TYPEDEF){
        }
        else {
            err+=1;
        }
    }

    std::vector<TYPE_SPECIFIER> ty;
    for(int i=0; i < ds->type_specifier.size(); i++){
        ty.push_back(ds->type_specifier.at(i)->type);
    }
    std::sort(ty.begin(),ty.end());
    Type t=Type(0,0, false);
    t.is_const=false;
    t.typeIndex=-1;
    for(int i=0; i < ty.size(); i++)
    std::cout << ty.at(i) << ' ';

    if(ty.size()==3){
        if (ty.at(0)==UNSIGNED  && ty.at(1)==INT && ty.at(2)==LONG) {t.typeIndex=8;}
        else if (ty.at(0)==SIGNED  && ty.at(1)==INT&& ty.at(2)==LONG) {t.typeIndex=9;}
        else err+=2;
        }
    else if (ty.size()==2){
        if (ty.at(0)==UNSIGNED && ty.at(1)==CHAR) {t.typeIndex=0;}
        else if (ty.at(0)==SIGNED && ty.at(1)==CHAR) {t.typeIndex=1;}
        else if (ty.at(0)==UNSIGNED && ty.at(1)==SHORT) {t.typeIndex=2;}
        else if (ty.at(0)==SIGNED && ty.at(1)==SHORT) {t.typeIndex=3;}
        else if (ty.at(0)==UNSIGNED && ty.at(1)==INT) {t.typeIndex=4;}
        else if (ty.at(0)==SIGNED && ty.at(1)==INT) {t.typeIndex=5;}
        else if (ty.at(0)==UNSIGNED && ty.at(1)==LONG) {t.typeIndex=6;}
        else if (ty.at(0)==SIGNED && ty.at(1)==LONG) {t.typeIndex=7;}
        else if (ty.at(0)==LONG && ty.at(1)==DOUBLE) {t.typeIndex=12;}
        else{
            err+=2;
        }
    }
    else if (ty.size()==1) {
        if(ty.at(0)==FLOAT){
            t.typeIndex=10;
        }
        else if(ty.at(0)==DOUBLE){
            t.typeIndex=11;
        }
        else if(ty.at(0)==CHAR){
            t.typeIndex=1;
        }
        else if(ty.at(0)==SHORT){
            t.typeIndex=3;
        }
        else if(ty.at(0)==INT){
            t.typeIndex=5;
        }
        else if(ty.at(0)==LONG){
            t.typeIndex=7;
        }
        else if(ty.at(0)==VOID){
            t.typeIndex=13;
        }
        else if (ty.at(0)==STRUCT){
            t.typeIndex=-1;
        }
        else if (ty.at(0)==UNION){
            t.typeIndex=-2;
        }
        else{
            err+=2;
        }
    }
    //Dangling else   
    for(int i=0; i < ds->type_qualifier.size(); i++){
        if(ds->type_qualifier.at(i)==CONST){
            t.is_const=true;
            ds->is_const=true;
        }
        else if(ds->type_qualifier.at(i)==VOLATILE){

        }
        else {err+=4;break;}
    }
    if(err&1){
        std::cerr << "Error in strorage class declarator";
        exit(0);
    }
    if(err&2){
        std::cerr << "Error in type specifier declarator";
        exit(0);
    }
    if(err&4){
        std::cerr << "Error in type qualifier declarator";
        exit(0);
    }
    //To take care:  pointer | size to be added.
    ds->type_index=get_index(t);
    return ds->type_index;
}




Declaration *new_declaration( DeclarationSpecifiers *declaration_specifiers, DeclaratorList *init_declarator_list ) {
    Declaration *d = new Declaration( declaration_specifiers, init_declarator_list );
    d->type=set_index(declaration_specifiers);
    return d;
}

void Declaration::add_to_symbol_table( LocalSymbolTable &sym_tab ) {
    if ( init_declarator_list == nullptr ){
        return;
    }
    std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

    for ( auto i = dec.begin(); i != dec.end(); i++ ) {
        for ( int i = 0; i < sym_tab.current_level; i++ ) {
            std::cout << "  ";
        }
        SymTabEntry *e = new SymTabEntry( ( *i )->id->value );
        sym_tab.add_to_table( e );
        std::cout << ( *i )->id->value << " " << sym_tab.current_level << "\n";
    }
}

    TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList * tql, TYPE_QUALIFIER type ) {
        tql->append_to_list( type );
        return tql;
    }

    //##############################################################################
    //############################# DECLARATION
    //####################################
    //##############################################################################

    Declaration ::Declaration( DeclarationSpecifiers * declaration_specifiers_, DeclaratorList * init_declarator_list_ )
        : Non_Terminal( "declaration" ), declaration_specifiers( declaration_specifiers_ ), init_declarator_list( init_declarator_list_ ){};


    void Declaration::add_to_symbol_table( GlobalSymbolTable & sym_tab ) {

        if ( init_declarator_list == nullptr )
            return;
        std::vector<Declarator *> &dec = init_declarator_list->declarator_list;

        for ( auto i = dec.begin(); i != dec.end(); i++ ) {
            std::cout << "G: " << ( *i )->id->value << "\n";
        }
    }

    //##############################################################################
    //########################## DECLARATION SPECIFIERS
    //############################
    //##############################################################################

    DeclarationSpecifiers ::DeclarationSpecifiers() : Non_Terminal( "declaration_specifiers" ){};

        DeclarationSpecifiers *new_storage_class( STORAGE_CLASS sc ) {
            DeclarationSpecifiers *ds = new DeclarationSpecifiers();
            ds->storage_class.push_back( sc );
            return ds;
        }

        DeclarationSpecifiers *new_type_specifier( TypeSpecifier * ts ) {
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

        DeclarationSpecifiers *add_storage_class( DeclarationSpecifiers * ds,
                                                  STORAGE_CLASS sc ) {
            ds->storage_class.push_back( sc );
            return ds;
        }

        DeclarationSpecifiers *add_type_specifier( DeclarationSpecifiers * ds,
                                                   TypeSpecifier * ts ) {
            ds->type_specifier.push_back( ts );
            ds->add_child( ts );
            return ds;
        }

        DeclarationSpecifiers *add_type_qualifier( DeclarationSpecifiers * ds,
                                                   TYPE_QUALIFIER tq ) {
            ds->type_qualifier.push_back( tq );
            return ds;
        }

        //##############################################################################
        //########################### DECLARATION LIST
        //#################################
        //##############################################################################

        DeclarationList::DeclarationList()
            : Non_Terminal( "declaration_list" ){};

        DeclarationList *create_declaration_list( Declaration * declaration ) {
            DeclarationList *dl = new DeclarationList();
            dl->declarations.push_back( declaration );
            dl->add_child( declaration );
            local_symbol_table.increase_level();
            declaration->add_to_symbol_table( local_symbol_table );
            return dl;
        }
        DeclarationList *add_to_declaration_list(
            DeclarationList * declaration_list, Declaration * declaration ) {

            declaration_list->declarations.push_back( declaration );
            declaration_list->add_child( declaration );
            declaration->add_to_symbol_table( local_symbol_table );
            return declaration_list;
        }

        //##############################################################################
        //############################# DECLARATOR LIST
        //################################
        //##############################################################################

        DeclaratorList ::DeclaratorList()
            : Non_Terminal( "init_declarator_list" ){};

        DeclaratorList *create_init_declarator_list( Declarator * d ) {
            DeclaratorList *dl = new DeclaratorList();
            dl->declarator_list.push_back( d );
            dl->add_child( d );
            return dl;
        }

        DeclaratorList *add_to_init_declarator_list( DeclaratorList * dl,
                                                     Declarator * d ) {
            dl->declarator_list.push_back( d );
            dl->add_child( d );
            return dl;
        }

        //##############################################################################
        //################################# DECLARATOR
        //#################################
        //##############################################################################

        Declarator ::Declarator( Pointer * p, DirectDeclarator * dd )
            : Non_Terminal( "declarator" ), pointer( p ),
              direct_declarator( dd ) {
            assert( dd != nullptr );
            assert( dd->id != nullptr );
            id = dd->id;
        };
        
        
        Declarator *create_declarator( Pointer * pointer,
                                       DirectDeclarator * direct_declarator ) {
            Declarator *d = new Declarator( pointer, direct_declarator );
            d->add_children( pointer, direct_declarator );
            return d;
        }

        Declarator *add_initializer_to_declarator( Declarator * declarator,
                                                   Node * ie ) {
            declarator->init_expr = ie;
            Identifier *id = new Identifier( declarator->id->value.c_str() );
            declarator->add_child( create_non_term( "=", id, ie ) );
            return declarator;
        }
        int Declarator::get_pointer_level(){
            Pointer *temp=pointer;
            int r=0;
            while(temp!=NULL){
                r+=1;
                temp=temp->pointer;
            }
            return r;
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
                                                    Identifier * id ) {
            assert( type == ID );
            DirectDeclarator *dd = new DirectDeclarator();
            dd->type = type;
            assert( id != nullptr );
            dd->id = id;
            dd->add_child( id );
            return dd;
        }

        DirectDeclarator *create_dir_declarator_dec(
            DIRECT_DECLARATOR_TYPE type, Declarator * declarator ) {
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

        DirectDeclarator *create_dir_declarator_arr(
            DIRECT_DECLARATOR_TYPE type, DirectDeclarator * direct_declarator,
            Node * const_expr ) {
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

        DirectDeclarator *create_dir_declarator_fun(
            DIRECT_DECLARATOR_TYPE type, DirectDeclarator * direct_declarator,
            ParameterTypeList * params ) {
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
              direct_abstract_declarator( nullptr ),
              parameter_type_list( nullptr ){};

        DirectAbstractDeclarator *create_direct_abstract_declarator(
            DIRECT_ABSTRACT_DECLARATOR_TYPE typ ) {
            DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
            dad->type = typ;
            return dad;
        }

        DirectAbstractDeclarator *create_direct_abstract_declarator(
            DIRECT_ABSTRACT_DECLARATOR_TYPE typ, AbstractDeclarator * abs ) {
            assert( typ == ABSTRACT );
            DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
            dad->type = typ;
            dad->abstract_declarator = abs;
            dad->add_child( abs );
            return dad;
        }

        DirectAbstractDeclarator *create_direct_abstract_declarator(
            DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
            DirectAbstractDeclarator * dabs ) {
            assert( typ == ROUND || typ == SQUARE );
            DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
            dad->type = typ;
            dad->direct_abstract_declarator = dabs;
            dad->add_child( dabs );
            return dad;
        }

        DirectAbstractDeclarator *create_direct_abstract_declarator(
            DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
            DirectAbstractDeclarator * dabs, Node * ce ) {
            assert( typ == SQUARE );
            DirectAbstractDeclarator *dad = new DirectAbstractDeclarator();
            dad->type = typ;
            dad->direct_abstract_declarator = dabs;
            dad->const_expr = ce;
            dad->add_children( dabs, ce );
            return dad;
        }
        DirectAbstractDeclarator *create_direct_abstract_declarator(
            DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
            DirectAbstractDeclarator * dabs, ParameterTypeList * param_list ) {

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

        ParameterDeclaration *create_parameter_declaration(
            DeclarationSpecifiers * ds, Declarator * d,
            AbstractDeclarator * ad ) {
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

        ParameterTypeList *create_parameter_list( ParameterDeclaration * pd ) {
            ParameterTypeList *ptl = new ParameterTypeList();
            ptl->param_list.push_back( pd );
            ptl->add_child( pd );
            return ptl;
        }

        ParameterTypeList *add_to_parameter_list( ParameterTypeList * ptl,
                                                  ParameterDeclaration * pd ) {
            ptl->param_list.push_back( pd );
            ptl->add_child( pd );
            return ptl;
        }

        ParameterTypeList *add_ellipsis_to_list( ParameterTypeList * ptl ) {
            ptl->has_ellipsis = true;
            ptl->add_child( create_terminal( "...", NULL ) );
            return ptl;
        }

        //##############################################################################
        //################################ IDENTIFIER
        //##################################
        //##############################################################################

        Identifier::Identifier( const char *name )
            : Terminal( "IDENTIFIER", name ){};

        //##############################################################################
        //########################## FUNCTION DEFINITION
        //###############################
        //##############################################################################

        FunctionDefinition::FunctionDefinition(
            DeclarationSpecifiers * declaration_specifiers_,
            Declarator * declarator_, Node * compound_statement_ )
            : Non_Terminal( "function_definiton" ),
              declaration_specifiers( declaration_specifiers_ ),
              declarator( declarator_ ),
              compound_statement( compound_statement_ ){};

        FunctionDefinition *create_function_defintion(
            DeclarationSpecifiers * declaration_specifiers,
            Declarator * declarator, Node * compound_statement ) {
            FunctionDefinition *fd = new FunctionDefinition(
                declaration_specifiers, declarator, compound_statement );
            fd->add_children( declaration_specifiers, declarator,
                              compound_statement );
            global_symbol_table.add_symbol( declaration_specifiers,
                                            declarator );
            local_symbol_table.add_function( declaration_specifiers,
                                             declarator );
            return fd;
        }

        //##############################################################################
        //############################## TYPE SPECIFIER
        //################################
        //##############################################################################

        TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER typ )
            : Non_Terminal( "type_specifier" ), type( typ ), id( nullptr ),
              struct_declaration_list( nullptr ), enumerator_list( nullptr ){};

        TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier * id_,
                                      StructDeclarationList *
                                          struct_declaration_list_ )
            : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
              struct_declaration_list( struct_declaration_list_ ),
              enumerator_list( nullptr ){};

        TypeSpecifier::TypeSpecifier( TYPE_SPECIFIER type_, Identifier * id_,
                                      EnumeratorList * enumerator_list_ )
            : Non_Terminal( "type_specifier" ), type( type_ ), id( id_ ),
              struct_declaration_list( nullptr ),
              enumerator_list( enumerator_list_ ){};

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
       void verify_struct_declarator( StructDeclarationList *st) {
            int err;
            if (st!=NULL){
                //std::cout<<st->struct_declaration_list.size();
                for(int i=0; i < st->struct_declaration_list.size(); i++){
                    std::vector<TypeSpecifier *> ts=st->struct_declaration_list.at(i)->sq_list->type_specifiers;
                    std::vector<TYPE_QUALIFIER> tq=st->struct_declaration_list.at(i)->sq_list->type_qualifiers;
                    err=0;
                    std::vector<TYPE_SPECIFIER> ty;
                    for(int i=0; i < ts.size(); i++)
                    ty.push_back(ts.at(i)->type);
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

                    for(int i=0; i < tq.size(); i++){
                        if(tq.at(i)==CONST||tq.at(i)==VOLATILE) {}
                        else {err+=4;break;}
                        }
            if (err&2) std::cout << "Error in type specifier struct";
            if (err&4) std::cout << "Error in type qualifier struct";
            }
        //std::cout<<"done2 ";
            }
        }
        TypeSpecifier *create_type_specifier(
            TYPE_SPECIFIER type, Identifier * id,
            StructDeclarationList * struct_declaration_list ) {
            assert( type == UNION || type == STRUCT );
            TypeSpecifier *ts =
                new TypeSpecifier( type, id, struct_declaration_list );
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

                // struct_type->pointer_level = 0;
            }

            return ts;
        }

        TypeSpecifier *create_type_specifier(
            TYPE_SPECIFIER type, Identifier * id,
            EnumeratorList * enumerator_list ) {
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

        StructDeclarationList *create_struct_declaration_list(
            StructDeclaration * struct_declaration ) {

            StructDeclarationList *sdl = new StructDeclarationList();

            sdl->struct_declaration_list.push_back( struct_declaration );

            sdl->add_child( struct_declaration );

            return sdl;
        }

        StructDeclarationList *add_to_struct_declaration_list(
            StructDeclarationList * struct_declaration_list,
            StructDeclaration * struct_declaration ) {

            struct_declaration_list->struct_declaration_list.push_back(
                struct_declaration );

            struct_declaration_list->add_child( struct_declaration );

            return struct_declaration_list;
        }

        //##############################################################################
        //############################ STRUCT DECLARATION
        //##############################
        //##############################################################################

        StructDeclaration::StructDeclaration( SpecifierQualifierList * sq_list_,
                                              DeclaratorList *
                                                  declarator_list_ )
            : Non_Terminal( "struct_declaration" ), sq_list( sq_list_ ),
              declarator_list( declarator_list_ ){};

        StructDeclaration *create_struct_declaration(
            SpecifierQualifierList * sq_list,
            DeclaratorList * struct_declarator_list ) {
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

        SpecifierQualifierList *create_type_specifier_sq( TypeSpecifier *
                                                          type_specifier ) {
            SpecifierQualifierList *sql = new SpecifierQualifierList();
            sql->type_specifiers.push_back( type_specifier );
            sql->add_child( type_specifier );
            return sql;
        }

        SpecifierQualifierList *create_type_qualifier_sq(
            TYPE_QUALIFIER type_qualifier ) {
            SpecifierQualifierList *sql = new SpecifierQualifierList();
            sql->type_qualifiers.push_back( type_qualifier );
            // sql->add_child(type_qualifier);
            return sql;
        }

        SpecifierQualifierList *add_type_specifier_sq(
            SpecifierQualifierList * sq_list, TypeSpecifier * type_specifier ) {
            sq_list->type_specifiers.push_back( type_specifier );
            sq_list->add_child( type_specifier );
            return sq_list;
        }

        SpecifierQualifierList *add_type_qualifier_sq(
            SpecifierQualifierList * sq_list, TYPE_QUALIFIER type_qualifier ) {
            sq_list->type_qualifiers.push_back( type_qualifier );
            // sq_list->add_child(type_qualifier);
            return sq_list;
        }

        //##############################################################################
        //################################ ENUMERATOR
        //##################################
        //##############################################################################

        Enumerator::Enumerator( Identifier * id_, Node * const_expr )
            : Non_Terminal( "enumerator" ), id( id_ ),
              init_expr( const_expr ){};

        Enumerator *create_enumerator( Identifier * id, Node * const_expr ) {
            Enumerator *e = new Enumerator( id, const_expr );
            e->add_children( id, const_expr );
            return e;
        }

        //##############################################################################
        //############################## ENUMERATOR LIST
        //###############################
        //##############################################################################

        EnumeratorList::EnumeratorList() : Non_Terminal( "enumerator_list" ){};

        EnumeratorList *create_enumerator_list( Enumerator * enumerator ) {
            EnumeratorList *el = new EnumeratorList();
            el->enumerator_list.push_back( enumerator );
            el->add_child( enumerator );
            return el;
        }

        EnumeratorList *add_to_enumerator_list(
            EnumeratorList * enumerator_list, Enumerator * enumerator ) {
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

        void LocalSymbolTable::add_to_table( SymTabEntry * symbol ) {

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
                    std::cerr << "ERROR: " << it->first << " " << symbol->name
                              << "\n";
                    assert( 0 );
                } else {
                    symbol->level = current_level;
                    q.push_front( symbol );
                }
            }
        }

        SymTabEntry *LocalSymbolTable::get_symbol_from_table(
            std::string name ) {
            auto it = sym_table.find( name );
            if ( it == sym_table.end() ) {
                return nullptr;
            } else {
                return it->second.front();
            }
        }

        void LocalSymbolTable::add_function( DeclarationSpecifiers *
                                                 declaration_specifiers,
                                             Declarator * declarator ) {

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
                SymTabEntry *symbol =
                    new SymTabEntry( ( *it )->declarator->id->value );
                add_to_table( symbol );
            }
            current_level = 0;
            std::cout << " )\n";
        }

        //##############################################################################
        //########################### GLOBAL SYMBOL TABLE
        //##############################
        //##############################################################################

        void GlobalSymbolTable::add_symbol( DeclarationSpecifiers *
                                                declaration_specifiers,
                                            Declarator * declarator ) {

            std::cout << "G: " << declarator->id->value << "\n";
        }

        SymTabEntry *GlobalSymbolTable::get_symbol_from_table(
            std::string name ) {
            auto it = sym_table.find( name );
            if ( it == sym_table.end() ) {
                return nullptr;
            } else {
                return it->second;
            }
        }

        Node *add_to_global_symbol_table( Declaration * declaration ) {
            declaration->add_to_symbol_table( global_symbol_table );
            return declaration;
        }

        //##############################################################################
        //########################### SYMBOL TABLE ENTRY
        //##############################
        //##############################################################################

        //##############################################################################