#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <expression.h>
#include <symtab.h>
#include <vector>
#include <y.tab.h>
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>

extern int line_num;
extern int column;

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
            std::cerr << "ERROR at line " << line_num << "\n";
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
    P->add_child( a );

    return P;
}
Expression *create_primary_stringliteral( StringLiteral *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 3;
    P->Sval = a;
    P->type.typeIndex = PrimitiveTypes::U_CHAR_T;
    P->type.ptr_level = 1;

    P->name = "primary_expression";
    P->add_child( a );
    return P;
}
Expression *create_primary_expression( Expression *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 0;
    P->op1 = a;
    P->type = a->type;

    P->name = "primary_expression";
    P->add_child( a );

    return P;
}

// 2. ArguementExpressionList
Expression *create_argument_expr_assignement( Expression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op2 = ase;
    // ArguementExprList does not have any type as it is a composite entity
    P->name = "assignment_expression";
    P->add_child( ase );

    return P;
}
Expression *create_argument_expr_list( Expression *ae_list, Expression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->op1 = ae_list;
    P->op2 = ase;

    P->name = "assignment_expression_list";
    P->add_children( ae_list, ase );
    return P;
}

// PostFix Expression
Expression *create_postfix_expr_arr( Expression *pe, Expression *e ) {
    PostfixExpression *P = new PostfixExpression();
    if ( dynamic_cast<PostfixExpression *>( pe ) ) {
        P->pe = dynamic_cast<PostfixExpression *>( pe );
    } else
        P->pe = nullptr;
    P->exp = e;
    Types peT = defined_types[pe->type.typeIndex];

    if ( pe->type.ptr_level == 1 ) {
        if ( e->type.isInt() ) {
            P->type = pe->type;
            P->type.ptr_level = 0;
        } else {
            // Error
            std::cerr << "ERROR: Array subscript is not an integer\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    }

    P->name = "ARRAY ACCESS";
    P->add_children( pe, e );
    return P;
}

Expression *create_postfix_expr_voidfun( Identifier *fi ) {
    PostfixExpression *P = new PostfixExpression();
    // Lookup Function type from symbol table - should be void

    P->name = "FUNCTION CALL";
    P->add_child( fi );

    return P;
}

Expression *create_postfix_expr_fun( Identifier *fi, Expression *ae ) {
    PostfixExpression *P = new PostfixExpression();
    // Here, we need to check two things:
    // 1. whether ArguementExprList matches with Function signature from lookup
    // of symbol table table

    P->name = "FUNCTION CALL";
    P->add_children( fi, ae );

    return P;
}

Expression *create_postfix_expr_struct( std::string access_op, Expression *pe,
                                        Identifier *i ) {
    PostfixExpression *P = new PostfixExpression();
    Types peT = defined_types[pe->type.typeIndex];
    if ( access_op == "." ) {
        if ( peT.is_struct && pe->type.ptr_level == 0 ) {
            // whether i exists in Struct
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                std::cerr << "Error\n";
                std::cerr << "ERROR at line " << line_num << "\n";
            } else {
                P->type = *iType;
            }
        } else {
            std::cerr<<"Operand Error\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit(0);
        }
    } else if ( access_op == "->" ) {
        if ( peT.is_struct && pe->type.ptr_level == 1 ) {
            // whether i exists in Struct*
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                std::cerr << "Error\n";
                std::cerr << "ERROR at line " << line_num << "\n";
            } else {
                P->type = *iType;
            }
        } else {
            std::cerr<<"Operand Error\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit(0);
        }
    }

    P->name = access_op;
    P->add_children( pe, i );
    return P;
}

Expression *create_postfix_expr_ido( std::string op, Expression *pe ) {
    PostfixExpression *P = new PostfixExpression();
    if ( dynamic_cast<PostfixExpression *>( pe ) ) {
        P->pe = dynamic_cast<PostfixExpression *>( pe );
    } else {
        P->pe = nullptr;
    }
    P->op = op;
    if ( op == "++" || op == "--" ) {
        if ( pe->type.isInt() || pe->type.isFloat() || pe->type.isPointer() ) {
            P->type = pe->type;

        } else {
            // Error postfix operator
            std::cerr << "Postfix operator " << op
                      << " cannot be applied to type" << pe->type.get_name() << "\n";
                      std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        // This should not have reached
        std::cerr << "Parse error";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    if ( op == "++" )
        P->name = "POST INCREMENT";
    else
        P->name = "POST DECREMENT";

    P->add_child( pe );
    return P;
}

// Unary Expression
Expression *create_unary_expression_ue( std::string u_op, Expression *ue ) {
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
            std::cerr << "Prefix operator " << u_op << " cannot be applied to type " << ue->type.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit(0);
        }
    } else if ( u_op == "sizeof" ) {
        // SizeOf
        U->type.typeIndex = PrimitiveTypes::U_INT_T;
        U->type.ptr_level = 0;
        U->type.is_const = false;
    } else {
        // Raise Error
        std::cerr << "Error parsing Unary Expression.\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit(0);
    }
    U->name = 'unary_expression';
    U->add_child( ue );
    return U;
}

// & (int) (x)
// &(x) -> pointer value of x
Expression *create_unary_expression_cast( Node *n_op, Expression *ce ) {
    UnaryExpression *U = new UnaryExpression();
    Non_Terminal *nt_op = dynamic_cast<Non_Terminal *>( n_op );
    std::string u_op = nt_op->name;
    U->op = u_op;
    U->op1 = ce;
    Type ceT = ce->type;
    if ( u_op == "&" ) {
        // ce->op1 should be of type IDENTIFIER because we dont support function
        // pointers
        /// XXX:: TODO implement getPointerTypeIndex()
        U->type = ce->type;
        U->type.ptr_level++;
        // else{
        //     //Error cannot get reference to type casts
        //     std::cerr << "lvalue required as unary & operator";
        //     delete U;
        //     exit(0);
        // }
    } else if ( u_op == "*" ) {
        if ( ce->type.ptr_level > 0 ) {
            U->type = ce->type;
            U->type.ptr_level--;
        } else {
            // Error because of dereference of non-pointer type
            delete U;
            std::cerr << "Error : Invalid dereference of type " << ce->type.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit(0);
        }
    } else if ( u_op == "-" || u_op == "+" || u_op == "!" || u_op == "-") {
        if(ce->type.isIntorFloat()){
            U->type = ce->type;    
        }
        else{
            //Throw Error
            std::cerr << "Invalid use of unary operator " << u_op << " on type " << ce->type.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit(0);
        }
    } else {
        // Throw Error
        std::cerr << "Parse error, invalid unary operator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit(0);
    }

    U->name = "unary_expression";
    U->add_children( n_op, ce );
    return U;
}

Expression *create_unary_expression_typename( std::string u_op, Node *t_name ) {
    UnaryExpression *U = new UnaryExpression();
    /// XXX:TODO Handle

    Node *n_op = create_non_term( ( u_op ).c_str() );
    U->name = u_op;
    U->add_children( n_op, t_name );

    return U;
}

// Cast Expression
Expression *create_cast_expression_typename( Node *n, Expression *ce ) {
    // XXX: TODO Implement
    CastExpression *P = new CastExpression();
    P->op1 = ce;
    P->type = ce->type;

    P->name = "caste_expression";
    P->add_children( n, ce );
    return P;
}

// Multiplicative Expression
Expression *create_multiplicative_expression( std::string op, Expression *me,
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
            std::cerr << "ERROR at line " << line_num << "\n";
        }
    } else if ( op == "%" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = ceT;
            P->type.make_unsigned();
        } else {
            // Error
            std::cerr << "Invalid Operands to binary % having type "
                      << meT.get_name() << " and " << ceT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    }

    P->name = "multiplicative expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( me, n_op, ce );
    return P;
}

// TODO : Add Pointer Support
// Additive Expression
Expression *create_additive_expression( std::string op, Expression *ade,
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
    }else if( (meT.isPointer()&& adeT.isInt()) || (adeT.isPointer()&& meT.isInt())){
            P->type = meT.isPointer() ? meT: adeT;
    } 
    else if ( meT.isPointer() && ( ( meT.isFloat() && adeT.isInt() ) ||
                                   ( meT.isInt() && adeT.isFloat() ) ) ) {
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( meT.isFloat() && adeT.isFloat() ) {
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
    } else {
        // Error
        std::cerr << "Undefined operation " << op << " for operands of type " << meT.get_name()
                  << " and " << adeT.get_name() << "\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }

    P->name = "additive_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( ade, n_op, me );

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
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        // This should not happen
        std::cerr << "Incorrect shift expression. Something went wrong\n";
        exit( 0 );
    }

    P->name = "shift_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( se, n_op, ade );
    return P;
}

// Relation Expression

Expression *create_relational_expression( std::string op, Expression *re,
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
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.get_name() << " and "
                      << seT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect relation expression. Something went wrong\n";
        exit( 0 );
    }

    P->name = "relational_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( re, n_op, se );
    return P;
}

// Equality Expression
Expression *create_equality_expression( std::string op, Expression *eq,
                                        Expression *re ) {
    EqualityExpression *P = new EqualityExpression();
    P->op1 = eq;
    P->op2 = re;
    P->op = op;
    Type reT = re->type;
    Type eqT = eq->type;
    if ( op == "==" || op == "!=" ) {
        if ( ( reT.isInt() || reT.isFloat() ) &&
             ( eqT.isInt() || eqT.isFloat() ) ) {
            P->type = Type( U_CHAR_T, 0, 0 );
        }

        else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << reT.get_name() << " and "
                      << eqT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect equality expression. Something went wrong\n";
        exit( 0 );
    }

    P->name = "eqality_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( eq, n_op, re );
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
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect and_expression. Something went wrong\n";
        exit( 0 );
    }

    P->name = "and_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( an, n_op, eq );
    return P;
}

Expression *create_exclusive_or_expression( std::string op, Expression *ex,
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
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr
            << "Incorrect exclusive or expression. Something went wrong\n";
        exit( 0 );
    }

    P->name = "exclusive_or_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( ex, n_op, an );
    return P;
}

Expression *create_inclusive_or_expression( std::string op, Expression *ie,
                                            Expression *ex ) {
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
                std::cerr << "ERROR at line " << line_num << "\n";
                exit( 0 );
            }
        } else {
            std::cerr << "Incorrect inclusive or expression. Something "
                         "went wrong\n";
            exit( 0 );
        }
    }

    P->name = "inclusive_or_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( ie, n_op, ex );
    return P;
}

// Logical And
Expression *create_logical_and_expression( std::string op, Expression *la,
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
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ieT.get_name() << " and "
                      << laT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical and expression. Something went "
                     "wrong\n";
        exit( 0 );
    }

    P->name = "logical_and_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( la, n_op, ie );
    return P;
}

// Logical or
Expression *create_logical_or_expression( std::string op, Expression *lo,
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
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << laT.get_name() << " and "
                      << loT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else {
        std::cerr << "Incorrect logical or expression. Something went "
                     "wrong\n";
        exit( 0 );
    }

    P->name = "logical_or_expression";
    P->add_children( lo, la );
    return P;
}

// Conditional

Expression *create_conditional_expression( std::string op, Expression *lo,
                                           Expression *te, Expression *coe ) {
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
        } else if ( teT.typeIndex == coeT.typeIndex &&
                    teT.ptr_level == coeT.ptr_level ) {
            P->type = teT;
        } else {

            // ERROR:
            exit( 0 );
        }
    }

    P->name = "conditional_expression";
    P->add_children( lo, te, coe );
    return P;
}

// AssignmentExpression

Expression *create_assignment_expression( Expression *ue, Node *n_op,
                                          Expression *ase ) {
    AssignmentExpression *P = new AssignmentExpression();
    Non_Terminal *nt_op = dynamic_cast<Non_Terminal *>( n_op );
    std::string op = nt_op->name;
    P->op1 = ue;
    P->op2 = ase;
    P->op = op;
    Type ueT = ue->type;
    Type aseT = ase->type;
    if(ueT.is_const){
        std::cerr << "ERROR: Cannot assign to constant variable\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit(0);
    }
    if (op == "=") {
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                std::cout << "Warning: operation " << op << " between " << ueT.get_name() << " and " << aseT.get_name() << "\n";
                std::cout << "Warning on line " << line_num << "\n";
            }
            P->type = ueT;
        } else if ( ueT.ptr_level > 0 && aseT.ptr_level > 0 &&
                    ueT.ptr_level == aseT.ptr_level ) {
            /// meed the types of pointers
            if ( ueT.typeIndex == aseT.typeIndex ) {
                P->type = ueT;
            }
            else {
            std::cerr << "Undefined operation of " << op
                    << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0);
            }
        }
        else{
            std::cerr << "Undefined operation of " << op
                    << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0);
        }
    }    
    else if ( op == "*=" || op == "/="  ) {
            if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                std::cout << "Warning: operation " << op << " between "
                          << ueT.get_name() << " and " << aseT.get_name() << "\n";
                std::cout << "Warning on line " << line_num << "\n";
                }
                P->type = ueT;
            }else {
                std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
                std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
            }
    }
    else if (op == "+=" || op =="-="){
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
                if ( ueT.typeIndex != aseT.typeIndex ) {
                std::cout << "Warning: operation " << op << " between "
                          << ueT.get_name() << " and " << aseT.get_name() << "\n";
                std::cout << "Warning on line " << line_num << "\n";
                }
                P->type = ueT;
            }
            else if(ueT.isPointer() && aseT.isInt()){
                P->type = ueT;
            }
            else {
                std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
                std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else if ( op == "%=" ) {
        if ( ( ueT.isInt() && aseT.isInt() ) ) {
            P->type = ueT;
        } else {
            // Error
            std::cerr << "Invalid Operands " << op << "having type " << ueT.get_name()
                      << " and " << aseT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else if ( op == "<<=" || op == ">>=" ) {
        if ( ueT.isInt() && aseT.isInt() ) {
            P->type = ueT;
        } else {
            // Operands are not integer type
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << ase->name << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }
    } else if ( op == "&=" || op == "|=" && op == "^=" ) {
        if ( ueT.isInt() && aseT.isInt() ) {
            // int family
            P->type = ueT;
            if ( !( ueT.isUnsigned() && aseT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            std::cerr << "Undefined operation of " << op
                      << " on operands of type " << ueT.get_name() << " and "
                      << aseT.get_name() << "\n";
            std::cerr << "ERROR at line " << line_num << "\n";
            exit( 0 );
        }

    } else {
        std::cerr << "Incorrect logical or expression. Something went wrong\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }

    // std::cerr << "Finished assignment\n";
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

    P->name = "toplevel_expression";
    P->add_children( te, ase );
    return P;
}
