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
#include <string>
#include <iterator>

extern unsigned int line_num;
extern unsigned int column;

Type INVALID_TYPE;

//##############################################################################
//############################# EXPRESSION #####################################
//##############################################################################

// Object creation Wrappers

// 1.Primary Expression
Expression *create_primary_identifier( Identifier *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 1;
    P->Ival = a;
    P->name = "primary_expression";

    SymTabEntry *ste = local_symbol_table.get_symbol_from_table( a->value );
    if ( ste == nullptr ) {
        ste = global_symbol_table.get_symbol_from_table( a->value );
        if ( ste == nullptr ) {
            // Error
            error_msg( "Undeclared symbol " + a->value, a->line_num,
                       a->column );
            P->type = INVALID_TYPE;
            return P;
        }
    }
    P->type = ste->type;
    P->add_child( a );
    P->line_num = a->line_num;
    P->column = a->column;

    return P;
}

Expression *create_primary_constant( Constant *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 2;
    P->Cval = a;
    P->type = a->getConstantType();
    P->line_num = a->line_num;
    P->column = a->column;

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
    P->line_num = a->line_num;
    P->column = a->column;

    P->name = "primary_expression";
    P->add_child( a );
    return P;
}

#if 0
Expression *create_primary_expression( Expression *a ) {
    PrimaryExpression *P = new PrimaryExpression();
    P->isTerminal = 0;
    P->op1 = a;
    P->type = a->type;

    P->name = "primary_expression";
    P->add_child( a );

    return P;
}

#endif

// 2. ArgumentExpressionList
ArgumentExprList *create_argument_expr_assignement( Expression *ase ) {
    ArgumentExprList *P = new ArgumentExprList();
    P->args.push_back( ase );
    // ArgumentExprList does not have any type as it is a composite entity
    P->name = "arguments";
    P->add_child( ase );

    return P;
}
ArgumentExprList *create_argument_expr_list( ArgumentExprList *P,
                                             Expression *ase ) {

    P->args.push_back( ase );
    // ArgumentExprList does not have any type as it is a composite entity
    P->name = "arguments";
    P->add_child( ase );
    return P;
}

// PostFix Expression
Expression *create_postfix_expr_arr( Expression *pe, Expression *e ) {
    PostfixExpression *P = new PostfixExpression();
    if ( dynamic_cast<PostfixExpression *>( pe ) ) {
        P->pe = dynamic_cast<PostfixExpression *>( pe );
    } else {
        P->pe = nullptr;
    }
    P->exp = e;
    P->name = "ARRAY ACCESS";
    if ( pe->type.is_invalid() || e->type.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }
    P->add_children( pe, e );
    P->line_num = pe->line_num;
    P->column = pe->column;

    if ( e->type.isInt() ) {
        if ( pe->type.is_array ) {
            P->type = pe->type;
            P->type.ptr_level--;
            P->type.array_dim--;
            P->type.array_dims.erase( P->type.array_dims.begin() );
		P->type.is_const = false;
            if ( P->type.ptr_level == 0 ) {
                P->type.is_pointer = false;
            }
            if ( P->type.array_dim == 0 ) {
                P->type.is_array = 0;
            }
        } else if ( pe->type.is_pointer ) {
            P->type = pe->type;
            P->type.ptr_level--;
		P->type.is_const = false;
            if ( P->type.ptr_level == 0 ) {
                P->type.is_pointer = false;
            }

        } else {
            error_msg( "Subscripted value is neither array nor pointer",
                       line_num );
            P->type = INVALID_TYPE;
        }
    } else {
        // Error
        error_msg( "Array index must be of type integer", line_num );
        P->type = INVALID_TYPE;
        return P;
    }

    return P;
}

Expression *create_postfix_expr_voidfun( Identifier *fi ) {
    PostfixExpression *P = new PostfixExpression();
    // Lookup Function type from symbol table - should be void

    SymTabEntry *ste = global_symbol_table.get_symbol_from_table( fi->value );
    if ( ste == nullptr ) {
        // Error
        error_msg( "Undeclared symbol " + fi->value, fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( !ste->type.is_function ) {
        // Error
        error_msg( "Called object '" + fi->value + "' is not a function",
                   fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( ste->type.num_args != 0 ) {
        // Error
        error_msg( "Too few arguments to function '" + fi->value + "'",
                   fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    }

    P->name = "FUNCTION CALL";
    P->add_child( fi );
    P->line_num = fi->line_num;
    P->column = fi->column;

    P->type = ste->type;
    P->type.is_function = false;
    P->type.num_args = 0;
    P->type.arg_types.clear();

    return P;
}

Expression *create_postfix_expr_fun( Identifier *fi, ArgumentExprList *ae ) {
    PostfixExpression *P = new PostfixExpression();
    // Here, we need to check two things:
    // 1. whether ArgumentExprList matches with Function signature from lookup
    // of symbol table table


	if ( fi->value == "printf"  || fi->value == "scanf" ) {
/* Hack : To support printf for now */ 
		P->type = Type(INT_T,0,true);
		P->add_children( fi, ae);
		return P;
	}

    SymTabEntry *ste = global_symbol_table.get_symbol_from_table( fi->value );
    if ( ste == nullptr ) {
        // Error
        error_msg( "Undeclared symbol " + fi->value, fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( !ste->type.is_function ) {
        // Error
        error_msg( "Called object '" + fi->value + "' is not a function",
                   fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( ste->type.num_args > ae->args.size() ) {
        // Error
        error_msg( "Too few arguments to function '" + fi->value +
                       "'. Expected " + std::to_string( ste->type.num_args ) +
                       ", got " + std::to_string( ae->args.size() ),
                   fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( ste->type.num_args < ae->args.size() ) {
        // Error
        error_msg( "Too many arguments to function '" + fi->value +
                       "'. Expected " + std::to_string( ste->type.num_args ) +
                       ", got " + std::to_string( ae->args.size() ),
                   fi->line_num, fi->column );
        P->type = INVALID_TYPE;
        return P;
    } else if ( ste->type.num_args == ae->args.size() ) {
        for ( unsigned int i = 0; i < ste->type.num_args; i++ ) {
            if ( ae->args[i]->type.is_invalid() ) {
                P->type = INVALID_TYPE;
                return P;
            }

            if ( !( ste->type.arg_types[i] == ae->args[i]->type ) ) {
                error_msg( "Type mismatch at argument " + std::to_string( i ) +
                               " of function '" + fi->value + "'. Expected " +
                               ste->type.arg_types[i].get_name() + ", got " +
                               ae->args[i]->type.get_name(),
                           fi->line_num, fi->column );
                P->type = INVALID_TYPE;
                return P;
            }
        }
    }

    P->name = "FUNCTION CALL";
    P->add_children( fi, ae );
    P->line_num = fi->line_num;
    P->column = fi->column;
    P->type = ste->type;
    P->type.is_function = false;
    P->type.num_args = 0;
    P->type.arg_types.clear();

    return P;
}

Expression *create_postfix_expr_struct( std::string access_op, Expression *pe,
                                        Identifier *i ) {
    PostfixExpression *P = new PostfixExpression();

    if ( pe->type.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    Types peT = defined_types[pe->type.typeIndex];
    if ( access_op == "." ) {
        if ( ( peT.is_struct || peT.is_union ) && pe->type.ptr_level == 0 ) {
            // whether i exists in Struct
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                error_msg( i->value + " is not a member of " +
                               pe->type.get_name(),
                           i->line_num, i->column );
                P->type = INVALID_TYPE;
                return P;
            } else {
                P->type = *iType;
            }
        } else {
            error_msg( "Invalid operand . with type " + pe->type.get_name(),
                       i->line_num, i->column );
            P->type = INVALID_TYPE;
            return P;
        }

    } else if ( access_op == "->" ) {
        if ( ( peT.is_struct || peT.is_union ) && pe->type.ptr_level == 1 ) {
            // whether i exists in Struct*
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                error_msg( i->value + " is not a member of " +
                               peT.name,
                           i->line_num, i->column );
                P->type = INVALID_TYPE;
                return P;
            } else {
                P->type = *iType;
            }
        } else {
            error_msg( "Invalid operand -> with type " + pe->type.get_name(),
                       i->line_num, i->column );
            P->type = INVALID_TYPE;
            return P;
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

    if ( pe->type.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    P->op = op;
    if ( op == "++" || op == "--" ) {
        if ( pe->type.is_const == false ) {
            if ( pe->type.isInt() || pe->type.isFloat() ||
                 pe->type.isPointer() ) {
                P->type = pe->type;

            } else {
                // Error postfix operator
                error_msg( "Invalid operand " + op + " with type " +
                               pe->type.get_name(),
                           line_num );
                P->type = INVALID_TYPE;
                return P;
            }
        } else {
            error_msg( "Invalid operand " + op + " with constant type",
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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
    if ( ueT.is_invalid() ) {
        U->type = INVALID_TYPE;
        return U;
    }
    if ( u_op == "++" || u_op == "--" ) {
        if ( ueT.is_const == false ) {
            // INC_OP, DEC_OP
            if ( ueT.isInt() ) {
                U->name = u_op + ":" + ue->type.get_name();
                U->type = ue->type;

            } else if ( ueT.isFloat() ) {

                U->name = u_op + ":" + ue->type.get_name();
                U->type = ue->type;

            } else if ( ueT.isPointer() ) {
                U->name = u_op + ":" + ue->type.get_name();
                U->type = ue->type;
            } else {
                // Incorrect type throw error
                error_msg( "Invalid operand " + u_op + " with type " +
                               ue->type.get_name(),
                           line_num );
                U->type = INVALID_TYPE;
                return U;
            }
        } else {
            error_msg( "Invalid operand " + u_op + " with constant type",
                       line_num );
            U->type = INVALID_TYPE;
            return U;
        }
    } else if ( u_op == "sizeof" ) {
        // SizeOf
        // TODO: Add implementation of sizeof, should return a constant
        U->name = "sizeof";
        U->type = Type();
        U->type.typeIndex = PrimitiveTypes::U_INT_T;
        U->type.ptr_level = 0;
        U->type.is_const = true;
    } else {
        // Raise Error
        std::cerr << "Error parsing Unary Expression.\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }
    U->add_child( ue );
    return U;
}

// & (int) (x)
// &(x) -> pointer value of x
Expression *create_unary_expression_cast( Node *n_op, Expression *ce ) {
    UnaryExpression *U = new UnaryExpression();
    Terminal *t_op = dynamic_cast<Terminal *>( n_op );
    std::string u_op = t_op->name;
    U->op = u_op;
    U->op1 = ce;
    Type ceT = ce->type;

    if ( ceT.is_invalid() ) {
        U->type = INVALID_TYPE;
        return U;
    }

    if ( u_op == "&" ) {
        // ce->op1 should be of type IDENTIFIER because we dont support function
        // pointers
        /// XXX:: TODO implement getPointerTypeIndex()
        if ( ceT.is_function == false ) {
            U->type = ce->type;
            U->type.ptr_level++;
		U->type.is_pointer = true;
        } else {
            error_msg( "lvalue required as unary & operand", n_op->line_num,
                       n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }
    } else if ( u_op == "*" ) {
        if ( ce->type.ptr_level > 0 && !ce->type.is_array ) {
            U->type = ce->type;
            U->type.ptr_level--;
		if ( U->type.ptr_level == 0 ) {
			U->type.is_pointer = false;
		} else {
			U->type.is_pointer = true;
		}
        } else {
            // Error because of dereference of non-pointer type
            error_msg( "Cannot dereference type " + ceT.get_name(),
                       n_op->line_num, n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }
    } else if ( u_op == "-" || u_op == "+" || u_op == "!" || u_op == "-" ) {
        if ( ce->type.isIntorFloat() ) {
            U->type = ce->type;
        } else {
            // Throw Error
            error_msg( "Invalid operand " + u_op + " on type " + ceT.get_name(),
                       n_op->line_num, n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }
    } else {
        // Throw Error
        std::cerr << "Parse error, invalid unary operator\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
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

    P->name = "cast_expression";
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
    if ( meT.is_invalid() || ceT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }
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

            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           meT.get_name() + " and " + ceT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "%" ) {
        if ( meT.isInt() && ceT.isInt() ) {
            P->type = ceT;
            P->type.make_unsigned();
        } else {
            // Error
            error_msg( "Invalid operands to " + op + " for operands of type " +
                           meT.get_name() + " and " + ceT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( meT.is_invalid() || adeT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( meT.isInt() && adeT.isInt() ) {
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
        if ( !( meT.isUnsigned() && adeT.isUnsigned() ) ) {
            // As a safety we upgrade unsigned type to corresponding signed type
            P->type.make_signed();
        }

    } else if ( ( meT.isFloat() && adeT.isInt() ) ||
                ( meT.isInt() && adeT.isFloat() ) ) {
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( ( meT.isPointer() && adeT.isInt() ) ||
                ( adeT.isPointer() && meT.isInt() ) ) {
        P->type = meT.isPointer() ? meT : adeT;
    } else if ( meT.isPointer() && ( ( meT.isFloat() && adeT.isInt() ) ||
                                     ( meT.isInt() && adeT.isFloat() ) ) ) {
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( meT.isFloat() && adeT.isFloat() ) {
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
    } else {
        // Error
        error_msg( "Undefined operation " + op + " for operands of type " +
                       meT.get_name() + " and " + adeT.get_name(),
                   line_num );
        P->type = INVALID_TYPE;
        return P;
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

    if ( seT.is_invalid() || adeT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "<<" || op == ">>" ) {
        if ( adeT.isInt() == 0 && seT.isInt() == 0 ) {
            P->type = seT;
        } else {
            // Operands are not integer type
            error_msg( "Undefined operation " + op + " for operands of type " +
                           seT.get_name() + " and " + adeT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( reT.is_invalid() || seT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "<=" || op == ">=" || op == ">" || op == "<" ) {
        if ( ( reT.isInt() || reT.isFloat() ) &&
             ( seT.isInt() || reT.isFloat() ) ) {
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           reT.get_name() + " and " + seT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( reT.is_invalid() || eqT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "==" || op == "!=" ) {
	if ( reT.ptr_level > 0 && eqT.ptr_level > 0 ) {
            P->type = Type( U_CHAR_T, 0, 0 );
	}
        else if ( ( reT.ptr_level == 0 && eqT.ptr_level == 0 ) && ( reT.isInt() || reT.isFloat() ) &&
             ( eqT.isInt() || eqT.isFloat() ) ) {
            P->type = Type( U_CHAR_T, 0, 0 );
        }

        else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           eqT.get_name() + " and " + reT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( anT.is_invalid() || eqT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "&" ) {
        if ( anT.isInt() && eqT.isInt() ) {
            P->type = anT.typeIndex > eqT.typeIndex ? anT : eqT;
            if ( !( anT.isUnsigned() && eqT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           anT.get_name() + " and " + eqT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( exT.is_invalid() || anT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "^" ) {
        if ( anT.isInt() && exT.isInt() ) {
            P->type = anT.typeIndex > exT.typeIndex ? anT : exT;
            if ( !( anT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           exT.get_name() + " and " + anT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( ieT.is_invalid() || exT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "|" ) {
        if ( ieT.isInt() && exT.isInt() ) {
            P->type = ieT.typeIndex > exT.typeIndex ? ieT : exT;
            if ( !( ieT.isUnsigned() && exT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            } else {
                // Error
                error_msg( "Undefined operation " + op +
                               " for operands of type " + ieT.get_name() +
                               " and " + exT.get_name(),
                           line_num );
                P->type = INVALID_TYPE;
                return P;
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

    if ( laT.is_invalid() || ieT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "&&" ) {
        if ( ieT.isIntorFloat() && laT.isIntorFloat() ) {
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           laT.get_name() + " and " + ieT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( loT.is_invalid() || laT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    if ( op == "&&" ) {
        if ( loT.isIntorFloat() && laT.isIntorFloat() ) {
            P->type = Type( U_CHAR_T, 0, 0 );
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           loT.get_name() + " and " + laT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
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

    if ( loT.is_invalid() || teT.is_invalid() || coeT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

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
    Terminal *t_op = dynamic_cast<Terminal *>( n_op );
    std::string op = t_op->name;
    P->op1 = ue;
    P->op2 = ase;
    P->op = op;
    P->name = "assignment_expression";
    P->add_children( ue, ase );

    Type ueT = ue->type;
    Type aseT = ase->type;

    if ( ueT.is_invalid() || aseT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }
    if ( ueT.is_const ) {
        error_msg( "Invalid assignment to constant expression", n_op->line_num,
                   n_op->column );
        P->type = INVALID_TYPE;
        return P;
    }
    if ( op == "=" ) {
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                warning_msg( "Operation " + op + " between operands of type " +
                                 ueT.get_name() + " and " + aseT.get_name(),
                             n_op->line_num, n_op->column );
            }
            P->type = ueT;
        } else if ( ueT.ptr_level > 0 && aseT.ptr_level > 0 &&
                    ueT.ptr_level == aseT.ptr_level ) {
            /// meed the types of pointers
            if ( ueT.typeIndex == aseT.typeIndex ) {
                P->type = ueT;
            } else {
                error_msg( "Undefined operation " + op +
                               " for operands of type " + ueT.get_name() +
                               " and " + aseT.get_name(),
                           n_op->line_num, n_op->column );
                P->type = INVALID_TYPE;
                return P;
            }
        } else {
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "*=" || op == "/=" ) {
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                warning_msg( "Operation " + op + " between operands of type " +
                                 ueT.get_name() + " and " + aseT.get_name(),
                             n_op->line_num, n_op->column );
            }
            P->type = ueT;
        } else {
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "+=" || op == "-=" ) {
        if ( ( ueT.isIntorFloat() ) && ( aseT.isIntorFloat() ) ) {
            // int and/or flot
            if ( ueT.typeIndex != aseT.typeIndex ) {
                warning_msg( "Operation " + op + " between operands of type " +
                                 ueT.get_name() + " and " + aseT.get_name(),
                             n_op->line_num, n_op->column );
            }
            P->type = ueT;
        } else if ( ueT.isPointer() && aseT.isInt() ) {
            P->type = ueT;
        } else {
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "%=" ) {
        if ( ( ueT.isInt() && aseT.isInt() ) ) {
            P->type = ueT;
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "<<=" || op == ">>=" ) {
        if ( ueT.isInt() && aseT.isInt() ) {
            P->type = ueT;
        } else {
            // Operands are not integer type
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "&=" || op == "|=" || op == "^=" ) {
        if ( ueT.isInt() && aseT.isInt() ) {
            // int family
            P->type = ueT;
            if ( !( ueT.isUnsigned() && aseT.isUnsigned() ) ) {
                // As a safety we upgrade unsigned type to corresponding signed
                // type
                P->type.make_signed();
            }
        } else {
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ueT.get_name() + " and " + aseT.get_name(),
                       n_op->line_num, n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }

    } else {
        std::cerr << "Incorrect logical or expression. Something went wrong\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }

    // std::cerr << "Finished assignment\n";
    return P;
}

// TopLevelExpression

Expression *create_toplevel_expression( Expression *te, Expression *ase ) {
    TopLevelExpression *P = new TopLevelExpression();
    P->op1 = te;
    P->op2 = ase;
    Type teT = te->type;
    Type aseT = ase->type;

    if ( teT.is_invalid() || aseT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }

    //////need TO DO LATER///////////

    P->name = "toplevel_expression";
    P->add_children( te, ase );
    return P;
}
unsigned stou( std::string const &str, size_t *idx = 0, int base = 10 ) {
    unsigned long result = std::stoul( str, idx, base );
    if ( result > std::numeric_limits<unsigned>::max() ) {
        throw std::out_of_range( "stou" );
    }
    return result;
}
Constant::Constant( const char *_name, const char *_value,
                    unsigned int _line_num, unsigned int _column )
    : Terminal( _name, _value, _line_num, _column ) {

    ConstantType = Type( 2, 0, false );
    int length = value.length();
    if ( name == "CONSTANT HEX" || name == "CONSTANT INT" ) {

        int islong = 0, isunsigned = 0, digitend = length;
        for ( int i = 0; i < length; i++ ) {
            if ( value[i] == 'l' || value[i] == 'L' ) {
                islong = 1;
                digitend = i < digitend ? i : digitend;
            }
            if ( value[i] == 'u' || value[i] == 'U' ) {
                isunsigned = 1;
                digitend = i < digitend ? i : digitend;
            }
            if ( islong && isunsigned ) {
                ConstantType.typeIndex = PrimitiveTypes::U_LONG_T;
                if ( name == "CONSTANT HEX" ) {
                    val.ul = std::stoul( value, nullptr, 16 );
                } else {
                    val.ul = std::stoul( value );
                }
                return;
                // TODO//
            }
        }
        if ( islong ) {
            ConstantType.typeIndex = PrimitiveTypes::LONG_T;
            if ( name == "CONSTANT HEX" ) {
                val.l = std::stol( value, nullptr, 16 );
            } else {
                val.l = std::stol( value );
            }
            return;
        } else if ( isunsigned ) {
            ConstantType.typeIndex = PrimitiveTypes::U_INT_T;
            if ( name == "CONSTANT HEX" ) {
                val.ui = stou( value, nullptr, 16 );
            } else {
                val.ui = stou( value );
            }
            return;
        } else {
            if ( name == "CONSTANT HEX" ) {
                val.i = std::stoi( value, nullptr, 16 );
            } else {
                val.i = std::stoi( value );
            }
            ConstantType.typeIndex = PrimitiveTypes::INT_T;
            return;
        }
        // TODO return type

        // return retT;
        // loop over value to get unsigned etc and return typeIndex
    } else if ( name == "CONSTANT FLOAT" ) {

        int isfloat = 0;
        for ( int i = 0; i < length; i++ ) {

            if ( value[i] == 'f' || value[i] == 'F' ) {
                ConstantType.typeIndex = PrimitiveTypes::FLOAT_T;
                val.f = std::stof( value );
                return;
            }
        }
        ConstantType.typeIndex = PrimitiveTypes::DOUBLE_T;
        val.d = std::stod( value );
        return;
        // loop over value to get float
    } else if ( name == "CONSANT EXP" ) {
        // loop over value to get if long or double
        int islong = 0;
        for ( int i = 0; i < length; i++ ) {
            if ( value[i] == 'f' || value[i] == 'F' ) {
                ConstantType.typeIndex = PrimitiveTypes::FLOAT_T;
                val.f = std::stof( value );
                return;
            }
        }
        ConstantType.typeIndex = PrimitiveTypes::LONG_T;
        val.l = std::stol( value );
        return;
    } else {
        // ------TODO---------
        // return retT;
    }
}

Constant *create_constant( const char *name, const char *value,
                           unsigned int line_num, unsigned int column ) {
    Constant *con = new Constant( name, value, line_num, column );
    return con;
}
