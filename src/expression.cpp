#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <expression.h>
#include <symtab.h>
#include <vector>
#include <3ac.h>
#include <codegen.h>
#include <statement.h>
#include <y.tab.h>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <iterator>

extern unsigned int line_num;
extern unsigned int column;

Type INVALID_TYPE;

#define NUM_REG_ARGS 4

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

    P->res = new_3id( ste );
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
    P->res = new Address( a->value, CON );

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
//XXX: This needs to be changed
    P->res = new Address( a->value, CON );
    return P;
}

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

    if (  ! e->type.isInt() ) {
        // Error
        error_msg( "Array index must be of type integer", line_num );
        P->type = INVALID_TYPE;
        return P;
    }

    create_new_save_live();
	if ( pe->type.is_array ) {
		P->type = pe->type;
		P->type.ptr_level--;
		P->type.array_dim--;
		P->type.array_dims.erase( P->type.array_dims.begin() );
		P->type.is_const = false;
	
		Address * t1;
		if ( e->res->type == CON ) {
			unsigned long long off = std::stol(e->res->name)*P->type.get_size();
			t1 = new_3const( off, INT3 );
		} else {
			t1 = new_temp();
			emit( t1, "*", e->res, new_3const( P->type.get_size() , INT3 ));
		}
		
		P->res = new_mem();
		emit( P->res, "+", pe->res, t1 );
		
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

		Address * t1;
		if ( e->res->type == CON ) {
			unsigned long long off = std::stol(e->res->name)*P->type.get_size();
			t1 = new_3const(off, INT3);
		} else {
			t1 = new_temp();
			emit( t1, "*", e->res, new_3const( P->type.get_size() , INT3));
		}
		
		P->res = new_mem();


		if (pe->res->type == MEM ) {
			// Dereference the pointer
			Address * t2 = new_mem();
			emit(t2, "()", pe->res, nullptr);
			emit( P->res, "+", t2, t1 );

		} else {
			emit( P->res, "+", pe->res, t1 );
		}

	} else {
		error_msg( "Subscripted value is neither array nor pointer",
				   line_num );
		P->type = INVALID_TYPE;
	}

    P->add_children( pe, e );
    P->line_num = pe->line_num;
    P->column = pe->column;
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


    create_new_save_live();
	if ( P->type.isVoid() ) {
        create_new_call( nullptr, fi->value );
	} else {
		P->res = new_temp();
        create_new_call( P->res, fi->value );
	}

    return P;
}

Expression *create_postfix_expr_fun( Identifier *fi, ArgumentExprList *ae ) {
    PostfixExpression *P = new PostfixExpression();

    if ( fi->value == "printf" || fi->value == "scanf" ) {
	// XXX:
        /* Hack : To support printf for now */
        P->type = Type( INT_T, 0, true );
        P->add_children( fi, ae );
        P->res = new_temp();
        create_new_call( P->res, fi->value );
//        emit( P->res, "call", new_3id( fi->value ), new_3const( 0, INT3 ) );
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


	/* 	First NUM_REG_ARG Arguments got to registers *
		Rest go on to the stack in reverse order 	 */
	unsigned int arg_count = ste->type.num_args < NUM_REG_ARGS ? ste->type.num_args : NUM_REG_ARGS; 
	for ( unsigned int i = 0; i <  arg_count ; i++ ) {
		emit(nullptr, "arg" + std::to_string(i),  ae->args[i]->res, nullptr );
	}
    create_new_save_live();
	for ( unsigned int i = ste->type.num_args - 1; i >= NUM_REG_ARGS; i-- ) {
		emit(nullptr, "push" + std::to_string(i),  ae->args[i]->res, nullptr );
	}
	if ( P->type.isVoid() ) {
        create_new_call( nullptr, fi->value );
	} else {
		P->res = new_temp();
        create_new_call( P->res, fi->value );
	}

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
            if ( peT.struct_definition == nullptr ) {
                error_msg( i->value + " is not a member of " +
                               pe->type.get_name(),
                           i->line_num, i->column );
                P->type = INVALID_TYPE;
                return P;
            }
            // whether i exists in Struct
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                error_msg( i->value + " is not a member of " + peT.name,
                           i->line_num, i->column );
                P->type = INVALID_TYPE;
                return P;
            } else {
                P->type = *iType;
            }
        } else {
            error_msg( "Invalid operand . with type " + peT.name, i->line_num,
                       i->column );
            P->type = INVALID_TYPE;
            return P;
        }

    } else if ( access_op == "->" ) {
        if ( ( peT.is_struct || peT.is_union ) && pe->type.ptr_level == 1 ) {
            if ( peT.struct_definition == nullptr ) {
                error_msg( i->value + " is not a member of " + peT.name,
                           i->line_num, i->column );
                P->type = INVALID_TYPE;
                return P;
            }
            // whether i exists in Struct*
            Type *iType = peT.struct_definition->get_member( i );
            if ( iType == nullptr ) {
                // Error
                error_msg( i->value + " is not a member of " + peT.name,
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


// TODO: Implement 3AC for struct access

    P->name = access_op;
    P->add_children( pe, i );
    P->res = pe->res;
    assert( pe->res->type != CON );
    pe->res->name += ( access_op + i->value );
    return P;
}

Expression *create_postfix_expr_ido( Terminal *op, Expression *pe ) {

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

    P->op = op->name;

    if ( op->name == "++" )
        P->name = "POST INCREMENT";
    else
        P->name = "POST DECREMENT";

    std::string op_code = op->name.substr( 0, 1 );

    Address *inc_value;
    
    if (  op->name != "++" && op->name != "--" ) {
		std::cerr << "PANIC: Invalid operation " << op->name <<"\n";
		assert(0);
		return P;
    }
	if ( pe->type.is_const == true ) {
		error_msg( "Invalid operand " + op->name + " with constant type",
				   op->line_num, op->column );
		P->type = INVALID_TYPE;
		return P;
	}

	if ( pe->type.isPointer() ) {
		P->res = new_mem();
		P->type = pe->type;
		Type t = pe->type;
		t.ptr_level--;
		inc_value = new_3const( t.get_size() , INT3);
	} else if ( pe->type.isInt() ) {
		P->res = new_temp();
		P->type = pe->type;
		inc_value = new_3const( 1, INT3 );
	} else if ( pe->type.isFloat() ) {
		P->res = new_temp();
		P->type = pe->type;
		inc_value = new_3const( 1.0 , FLOAT3 );
	} else {
		// Error postfix operator
		error_msg( "Invalid operand " + op->name + " with type " +
					   pe->type.get_name(),
				   op->line_num, op->column );
		delete P->res;
		P->res = nullptr;
		P->type = INVALID_TYPE;
		return P;
	}

    P->add_child( pe );

    if ( pe->res->type == MEM ) {
        Address *t1 = new_temp();
        emit( P->res, "()", pe->res, nullptr );
        emit( t1, "=", P->res, nullptr);
        emit( t1, op_code, t1, inc_value );
        emit( pe->res, "()s", t1, nullptr );
    } else if ( pe->res->type == ID3 ) {
        emit( P->res, "=", pe->res, nullptr );
        emit( pe->res, op_code, pe->res, inc_value );
    } else {
		delete inc_value;
		delete P->res;
		P->res = nullptr;
		inc_value = nullptr;
        error_msg( "lvalue required as operand to" + op->name, op->line_num,
                   op->column + 1 );
        P->type = INVALID_TYPE;
        return P;
    }
    return P;
}

// Unary Expression
Expression *create_unary_expression( Terminal *op, Expression *ue ) {
    UnaryExpression *U = new UnaryExpression();
    U->op1 = ue;
    U->op = op->name;
    Type ueT = ue->type;
    if ( ueT.is_invalid() ) {
        U->type = INVALID_TYPE;
        return U;
    }
    std::string u_op = op->name;
    U->name = u_op;
    Address *inc_value = nullptr;

    if ( u_op == "++" || u_op == "--" ) {
        if ( ueT.is_const == true ) {
            error_msg( "Invalid operand " + u_op + " with constant type",
                       op->line_num, op->column );
            U->type = INVALID_TYPE;
            return U;
        }
        
		u_op = u_op.substr( 0, 1 );
		if ( ue->type.isPointer() ) {
            U->res = new_mem();
			U->type = ue->type;
			Type t = ue->type;
			t.ptr_level--;
			inc_value = new_3const( t.get_size() , INT3 );
		} else if ( ue->type.isInt() ) {
            U->res = new_temp();
			U->type = ue->type;
			inc_value = new_3const( 1 , INT3 );

		} else if ( ue->type.isFloat() ) {
            U->res = new_temp();
			U->type = ue->type;
			inc_value = new_3const( 1.0, FLOAT3 );
		} else {
			// Incorrect type throw error
			error_msg( "Invalid operand " + u_op + " with type " +
						   ue->type.get_name(),
					   op->line_num, op->column );
			delete U->res;
			U->res = nullptr;
			U->type = INVALID_TYPE;
			return U;
		}



        if ( ue->res->type == MEM ) {
			Address * t1 = new_temp();
            emit( t1, "()", ue->res, nullptr );
            emit( U->res, u_op, t1, inc_value );
            emit( ue->res, "()s", U->res, nullptr );
        } else if ( ue->res->type == ID3 ) {
            U->res = ue->res;
			ue->res->type = TEMP;
            emit( U->res, u_op, U->res, inc_value );
        } else {
			delete inc_value;
			inc_value = nullptr;
            error_msg( "lvalue required as unary " + op->name + " operand",
                       op->line_num, op->column );
            U->type = INVALID_TYPE;
            return U;
        }
    } else if ( u_op == "sizeof" ) {
        U->name = "sizeof";
        U->type = Type();
        U->type.typeIndex = PrimitiveTypes::U_LONG_T;
        U->type.ptr_level = 0;
        U->type.is_const = true;
        U->res = new_3const( ue->type.get_size() , INT3);
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
		// TODO : Fix this
        if ( ceT.is_function == true ) {
            error_msg( "lvalue required as unary & operand", n_op->line_num,
                       n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }

		U->type = ce->type;
		U->type.ptr_level++;
		U->type.is_pointer = true;
		if ( ce->res->type == MEM ) {
			U->res = ce->res;
			U->res->type = TEMP;
		} else if ( ce->res->type == ID3 ) {
			U->res = new_temp();
			emit ( U->res, "=", ce->res, nullptr);
		} else {
		   error_msg( "lvalue required as unary & operand", n_op->line_num,
				   n_op->column );
			U->type = INVALID_TYPE;
			return U; 
		}

    } else if ( u_op == "*" ) {
        if ( ce->type.ptr_level <= 0 || ce->type.is_array ) {
            // Error because of dereference of non-pointer type
            error_msg( "Cannot dereference type " + ceT.get_name(),
                       n_op->line_num, n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }

		U->type = ce->type;
		U->type.ptr_level--;
		if ( U->type.ptr_level == 0 ) {
			U->res = new_temp();
			U->type.is_pointer = false;
		} else {
			U->res = new_mem();
			U->type.is_pointer = true;
		}
		if ( ce->res->type == MEM || ce->res->type == ID3 ) {
			emit( U->res, "()", ce->res, nullptr );
		} else {
			error_msg( "lvalue required as unary * operand", n_op->line_num,
					   n_op->column );
			U->type = INVALID_TYPE;
			return U;
		}

    } else if ( u_op == "-" || u_op == "+" ) {
        


        if ( ! ce->type.isIntorFloat() ) {
            // Throw Error
            error_msg( "Invalid operand " + u_op + " on type " + ceT.get_name(),
                       n_op->line_num, n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }

		U->type = ce->type;
		U->type.make_signed();
		Address *t1;
		MEM_EMIT( ce, t1 );
		U->res = new_temp();
		Address * zero_value = nullptr;
		if ( ce->type.isInt() ) {
			zero_value = new_3const( 0, INT3 );
		} 
		else if ( ce->type.isFloat() ) {
			zero_value = new_3const( 0.0 , FLOAT3 );
		} else {
			// Should not come here
			assert(0);
		}

		emit( U->res, u_op, zero_value, t1 );

    } else if ( u_op == "!" ) {
        if (  ! ce->type.isInt() ) {
            // Throw Error
            error_msg( "Invalid operand " + u_op + " on type " + ceT.get_name(),
                       n_op->line_num, n_op->column );
            U->type = INVALID_TYPE;
            return U;
        }
            U->type = Type( U_CHAR_T, 0, 0 );
            Address *t1;
            MEM_EMIT( ce, t1 );
			U->truelist = ce->falselist;
			U->falselist = ce->truelist;
            BACKPATCH ( U );
			U->res = new_temp();
            emit( U->res, u_op, t1, nullptr );
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

Expression *create_unary_expression( Terminal *op, TypeName *t_name ) {
    UnaryExpression *U = new UnaryExpression();
    std::string u_op = op->name;
    U->name = u_op;
    U->add_children( op, t_name );
    U->type = Type( PrimitiveTypes::U_LONG_T, 0, true );
    U->res = new_3const( t_name->type.get_size() , INT3);

    return U;
}

// Cast Expression


//TODO: Implement 3AC
Expression *create_cast_expression_typename( TypeName *tn, Expression *ce ) {
    CastExpression *P = new CastExpression();
    P->op1 = ce;
    Type ceT = ce->type;
    Type tnT = tn->type;
    if ( ceT.is_invalid() || tnT.is_invalid() ) {
        P->type = INVALID_TYPE;
        return P;
    }
    if ( ce->type.isIntorFloat() && tn->type.isIntorFloat() ) {
        P->type = tn->type;
    } else if ( ce->type.ptr_level > 0 && tn->type.ptr_level > 0 ) {
        P->type = tn->type;
    } else {
        error_msg( "Undefined casting operation of " + ceT.get_name() +
                       " into " + tnT.get_name(),
                   line_num );
        P->type = INVALID_TYPE;
        return P;
    }
    P->name = "cast_expression";
    P->add_children( tn, ce );
    P->res = ce->res;
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

            if ( !meT.isUnsigned() && !ceT.isUnsigned() ) {
                ;
            } else if ( meT.isUnsigned() && ceT.isUnsigned() ) {
                op += "u";
            } else if ( !meT.isUnsigned() && ceT.isUnsigned() ) {
                meT.make_unsigned();
                op += "u";
            } else if ( meT.isUnsigned() && !ceT.isUnsigned() ) {
                ceT.make_unsigned();
                op += "u";
            }

            Address *t1, *t2;
            MEM_EMIT( me, t1 );
            MEM_EMIT( ce, t2 );
			P->res = new_temp();

            if ( meT.typeIndex < ceT.typeIndex ) {
                P->type = ceT;
                // TODO: extend the smaller type;
            } else if ( meT.typeIndex > ceT.typeIndex ) {
                P->type = meT;
                // TODO: extend the smaller type;
            } else {
                P->type = meT;
            }

            emit( P->res, op, t1, t2 );

        } else if ( ( meT.isFloat() && ceT.isInt() ) ||
                    ( meT.isInt() && ceT.isFloat() ) ) {

            P->type = meT.isFloat() ? meT : ceT;
            // TODO: Implement implicit cast to float
        }

        else if ( meT.isFloat() && ceT.isFloat() ) {

            op += "f";

            Address *t1, *t2;
            MEM_EMIT( me, t1 );
            MEM_EMIT( ce, t2 );
            SAVE_REGS ( P, t1, t2 );

            if ( meT.typeIndex < ceT.typeIndex ) {
                P->type = ceT;
                // TODO: Extend the smaller type
            } else if ( meT.typeIndex > ceT.typeIndex ) {
                P->type = meT;
                // TODO: Extend the smaller type
            } else {
                P->type = meT;
            }

            emit( P->res, op, t1, t2 );
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           meT.get_name() + " and " + ceT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
        }
    } else if ( op == "%" ) {

        if (  ! meT.isInt() || ! ceT.isInt() ) {
            // Error
            error_msg( "Invalid operands to " + op + " for operands of type " +
                           meT.get_name() + " and " + ceT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
        }

		P->type = ceT;
		P->type.make_unsigned();
		Address *t1, *t2;
		MEM_EMIT( me, t1 );
		MEM_EMIT( ce, t2 );
		P->res = new_temp();
		emit( P->res, op, t1, t2 );

    } else {
		assert(0);
	}

    P->name = "multiplicative expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( me, n_op, ce );
    return P;
}

// Additive Expression
// TODO: Implement 3AC for type casts
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
            if ( !adeT.isUnsigned() && !meT.isUnsigned() ) {
                ;
            } else if ( adeT.isUnsigned() && meT.isUnsigned() ) {
                op += "u";
            } else if ( !adeT.isUnsigned() && meT.isUnsigned() ) {
                adeT.make_unsigned();
                op += "u";
            } else if ( adeT.isUnsigned() && !meT.isUnsigned() ) {
                meT.make_unsigned();
                op += "u";
            }

            Address *t1, *t2;
            MEM_EMIT( ade, t1 );
            MEM_EMIT( me, t2 );
			P->res = new_temp();

            if ( adeT.typeIndex < meT.typeIndex ) {
                P->type = meT;
                // TODO: extend the smaller type;
            } else if ( adeT.typeIndex > meT.typeIndex ) {
                P->type = adeT;
                // TODO: extend the smaller type;
            } else {
                P->type = adeT;
            }

            emit( P->res, op, t1, t2 );

    } else if ( ( meT.isFloat() && adeT.isInt() ) ||
                ( meT.isInt() && adeT.isFloat() ) ) {
		// TODO: Implement 3AC
        P->type = meT.isFloat() ? meT : adeT;
    } else if ( meT.isFloat() && adeT.isFloat() ) {
		// TODO: Implement 3AC
        P->type = meT.typeIndex > adeT.typeIndex ? meT : adeT;
        op += "f";
    } else if ( adeT.isPointer() && meT.isInt() ) {
        P->type = adeT;
        Address *t1;
        //MEM_EMIT( ade, t1 );
        MEM_EMIT( me, t1 );
		P->res = new_mem();
        Type t = adeT;
        t.ptr_level--;
        op += "u";
		Address * t2 = new_temp();
        emit( t2, "*u", t1, new_3const( t.get_size() , INT3 ));
        emit( P->res, op, ade->res, t2 );
        P->name = "additive_expression";
        Node *n_op = create_non_term( ( op ).c_str() );
        P->add_children( ade, n_op, me );
        return P;
    } else if ( meT.isPointer() && adeT.isInt() ) {
        P->type = meT;
        Address *t1;
        MEM_EMIT( ade, t1 );
//      MEM_EMIT( me, t2 );
        Type t = meT;
        t.ptr_level--;
        op += "u";
		P->res = new_mem();
		Address * t2 = new_temp();
        emit( t2, "*u", t1, new_3const( t.get_size() , INT3 ));
        emit( P->res, op, me->res, t2 );
        P->name = "additive_expression";
        Node *n_op = create_non_term( ( op ).c_str() );
        P->add_children( ade, n_op, me );
        return P;
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

// TODO: Implement 3AC for type casts
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
        if ( adeT.isInt() && seT.isInt() ) {
            P->type = seT;
            if ( seT.isUnsigned() ) {
                op += "u";
            }
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
    Address *t1, *t2;
    MEM_EMIT( se, t1 );
    MEM_EMIT( ade, t2 );
    //SAVE_REGS( P, t1, t2 );
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
            if ( reT.isUnsigned() != seT.isUnsigned() ) {
                warning_msg( "Comparison " + op + " for operands of type " +
                                 reT.get_name() + " and " + seT.get_name(),
                             line_num );
            }
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
    Address *t1, *t2;
    MEM_EMIT( re, t1 );
    MEM_EMIT( se, t2 );
    //SAVE_REGS( P, t1, t2 );
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
        } else if ( ( reT.ptr_level == 0 && eqT.ptr_level == 0 ) &&
                    ( reT.isInt() || reT.isFloat() ) &&
                    ( eqT.isInt() || eqT.isFloat() ) ) {

            P->type = Type( U_CHAR_T, 0, 0 );

            if ( eqT.isUnsigned() != reT.isUnsigned() ) {
                warning_msg( "Comparison " + op + " for operands of type " +
                                 eqT.get_name() + " and " + reT.get_name(),
                             line_num );
            }
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
    Address *t1, *t2;
    MEM_EMIT( eq, t1 );
    MEM_EMIT( re, t2 );
    //SAVE_REGS( P, t1, t2);
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
            } else {
                op += "u";
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
    Address *t1, *t2;
    MEM_EMIT( an, t1 );
    MEM_EMIT( eq, t2 );
    //SAVE_REGS( P, t1, t2 );
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
            } else {
                op += "u";
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
    Address *t1, *t2;
    MEM_EMIT( ex, t1 );
    MEM_EMIT( an, t2 );
    //SAVE_REGS( P, t1, t2 );
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
                op += "u";
            }
        } else {
            // Error
            error_msg( "Undefined operation " + op + " for operands of type " +
                           ieT.get_name() + " and " + exT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            return P;
        }
    } else {
        std::cerr << "Incorrect inclusive or expression. Something "
                     "went wrong\n";
        exit( 0 );
    }

    P->name = "inclusive_or_expression";
    Node *n_op = create_non_term( ( op ).c_str() );
    P->add_children( ie, n_op, ex );
    Address *t1, *t2;
    MEM_EMIT( ie, t1 );
    MEM_EMIT( ex, t2 );
    //SAVE_REGS( P, t1, t2 );
	P->res = new_temp();
    emit( P->res, op, t1, t2 );
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
        if ( ieT.isInt() && laT.isInt() ) {
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
    P->res = ie->res;
    Address *t1, *t2;
    //MEM_EMIT( la, t1 );
    //MEM_EMIT( ie, t2 );
	//SAVE_REGS( P, t1, t2 );
    //emit( P->res, op, t1, t2 );
    
	append(P->truelist,ie->truelist);
    append(P->falselist,la->falselist);
    append(P->falselist,ie->falselist);
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

    if ( op == "||" ) {
        if ( loT.isInt() && laT.isInt() ) {
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
    P->res = la->res;
    Address *t1, *t2;
    //MEM_EMIT( lo, t1 );
    //MEM_EMIT( la, t2 );
	//SAVE_REGS( P, t1, t2 );
    //emit( P->res, op, t1, t2 );
    
	append(P->truelist,lo->truelist);
    append(P->truelist,la->truelist);
    append(P->falselist,la->falselist);

    return P;
}

// Conditional

// TODO : Implement 3AC for this

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
            error_msg( " Type mismatch in conditional expression for operands "
                       "of type " +
                           teT.get_name() + " and " + coeT.get_name(),
                       line_num );
            P->type = INVALID_TYPE;
            // ERROR:
            return P;
        }
    } else {
        error_msg( "Comparison expression is not an int", line_num );
        P->type = INVALID_TYPE;
        return P;
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
            // TODO: Implement safe cast
            //        if ( !safe_cast( ueT, aseT ) ) {
            if ( ueT.typeIndex != aseT.typeIndex ) {
                warning_msg( "Operation " + op + " between operands of type " +
                                 ueT.get_name() + " and " + aseT.get_name(),
                             n_op->line_num, n_op->column );
            }
            P->type = ueT;

        } else if ( ueT.ptr_level > 0 && aseT.ptr_level > 0 ) {
            if ( aseT.typeIndex != VOID_T && ( ueT.typeIndex != aseT.typeIndex || ueT.ptr_level != aseT.ptr_level ) ) {
                warning_msg( "Assigning pointer of type " + aseT.get_name() + " to " + ueT.get_name(), 
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

        if ( ue->res->type == MEM ) {
            P->res = ue->res;
            Address *t1;
            MEM_EMIT( ase, t1 );
            emit( ue->res, "()s", t1, nullptr );
            P->res->type = TEMP;
        } else if ( ue->res->type == ID3 ) {
            P->res = ue->res;
            Address *t1;
            MEM_EMIT( ase, t1 );
            emit( ue->res, "=", t1, nullptr );
            P->res->type = TEMP;
        } else {
            error_msg( "lvalue required as left operand of assignment",
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
        if ( ue->res->type == MEM ) {
            Address *t1 = new_temp();
            emit( t1, "()", ue->res, nullptr );
            emit( t1, op.substr( 0, 1 ), t1, ase->res );
            emit( ue->res, "()s", t1, nullptr );
            P->res = t1;

        } else if ( ue->res->type == ID3 ) {
            emit( ue->res, op.substr( 0, 1 ), ue->res, ase->res );
            P->res = ue->res;
			ue->res->type = TEMP;
        } else {
            error_msg( "lvalue required as left operand", n_op->line_num,
                       n_op->column );
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
// TODO: Add pointer support for these expressions
        if ( ue->res->type == MEM ) {
            Address *t1 = new_temp();
            emit( t1, "()", ue->res, nullptr );
            emit( t1, op.substr( 0, 1 ), t1, ase->res );
            emit( ue->res, "()s", t1, nullptr );
            P->res = t1;

        } else if ( ue->res->type == ID3 ) {
            emit( ue->res, op.substr( 0, 1 ), ue->res, ase->res );
            P->res = ue->res;
			P->res->type = TEMP;
        } else {
            error_msg( "lvalue required as left operand", n_op->line_num,
                       n_op->column );
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
        P->res = ue->res;
        emit( P->res, op.substr( 0, 1 ), ue->res, ase->res );
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
        if ( ue->res->type == MEM ) {
            Address *t1 = new_temp();
            emit( t1, "()", ue->res, nullptr );
            emit( t1, op.substr( 0, 2 ), t1, ase->res );
            emit( ue->res, "()s", t1, nullptr );
            P->res = t1;

        } else if ( ue->res->type == ID3 ) {
            emit( ue->res, op.substr( 0, 2 ), ue->res, ase->res );
            P->res = ue->res;
			P->res->type = TEMP;
        } else {
            error_msg( "lvalue required as left operand", n_op->line_num,
                       n_op->column );
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
        if ( ue->res->type == MEM ) {
            Address *t1 = new_temp();
            emit( t1, "()", ue->res, nullptr );
            emit( t1, op.substr( 0, 1 ), t1, ase->res );
            emit( ue->res, "()s", t1, nullptr );
            P->res = t1;

        } else if ( ue->res->type == ID3 ) {
            emit( ue->res, op.substr( 0, 1 ), ue->res, ase->res );
            P->res = ue->res;
			P->res->type = TEMP;
        } else {
            error_msg( "lvalue required as left operand", n_op->line_num,
                       n_op->column );
            P->type = INVALID_TYPE;
            return P;
        }

    } else {
        std::cerr << "Incorrect logical or expression. Something went wrong\n";
        std::cerr << "ERROR at line " << line_num << "\n";
        exit( 0 );
    }

    // std::cerr << "Finished assignment\n";
    P->add_children( ue, ase );
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

void Constant::negate(){
    value = '-'+value;
    switch(ConstantType.typeIndex){
        case U_CHAR_T:
            val.uc=-val.uc; break;
       case CHAR_T: 
			val.c = -val.c ; break;
        case U_INT_T: 
			val.ui = -val.ui ; break;
        case INT_T: 
			val.i = -val.i ; break;
        case U_LONG_T: 
			val.ul = -val.ul ; break;
        case LONG_T: 
			val.l = -val.l ; break;
        case FLOAT_T: 
			val.f = -val.f ; break;
        case DOUBLE_T: 
			val.d = -val.d ; break;
    }
}
Constant::Constant( const char *_name, const char *_value,
                    unsigned int _line_num, unsigned int _column )
    : Terminal( _name, _value, _line_num, _column ) {

    ConstantType = Type( -1, 0, false );
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
    } else if ( name == "CONSTANT CHAR" ) {
        if ( value[1] == '\\') {
            
            switch (value[2]) {
                case 'n' : val.c = '\n'; break;
                case 'r' : val.c = '\r'; break;
                case 't' : val.c = '\t'; break;
                case '\\' :val.c = '\\'; break;
                case '0' : val.c = '\0'; break;
                default:
                    error_msg("Invalid Escape Sequence",line_num,column);
            }
            ConstantType.typeIndex = PrimitiveTypes::CHAR_T;
            return;
        }

        ConstantType.typeIndex = PrimitiveTypes::CHAR_T;
        val.c = value[1];
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
