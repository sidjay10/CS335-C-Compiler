#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <ast.h>
#include <deque>
#include <map>
#include <string>
#include <symtab.h>

// All class declarations
class StructDefinition;
class AssignmentExpression;
class ParameterTypeList;
class CastExpression;
class Constant;
class StringLiteral;
class TopLevelExpression;

class Expression : public Non_Terminal {
  public:
    Type type;
    /* Change this late */
    int num_opearands;
    // Expression( Types * type, int num_op );

    Expression() : Non_Terminal( "" ){};
};

// Expression::Expression(){}

//-------------------------------------------------
class PrimaryExpression : public Expression {
    /**
isTerminal = 0 when ( Expression )
isTerminal = 1 when IDENTIFIER
isTerminal = 2 when CONSTANT
isTerminal = 3 when STRING_LITERAL
*/
  public:
    int isTerminal;
    Identifier *Ival;
    StringLiteral *Sval;
    Constant *Cval;
    Expression *op1;

    PrimaryExpression() {
        isTerminal = 0;
        Ival = nullptr;
        Sval = nullptr;
        Cval = nullptr;
        op1 = nullptr;
    }
};

// Grammar warppers for PrimaryExpression
Expression *create_primary_identifier( Identifier *a );
Expression *create_primary_constant( Constant *a );
Expression *create_primary_stringliteral( StringLiteral *a );
Expression *create_primary_expression( Expression *a );

//-------------------------------------------------

class ArgumentExprList : public Expression {
  public:
    Expression *op1; // This will be null in case of root object
    Expression *op2;

    ArgumentExprList() {
        op1 = nullptr;
        op2 = nullptr;
    };
};

// Grammar warppers for ArguementExpressionList
Expression *create_argument_expr_assignement( Expression *ase );
Expression *create_argument_expr_list( Expression *ae_list, Expression *ase );

//-------------------------------------------------
class UnaryExpression : public Expression {
  public:
    Expression *op1;
    std::string op;

    UnaryExpression() {
        op1 = nullptr;
        op = "";
    }
};

// Grammar warppers for UnaryExpression
Expression *
create_unary_expression_ue( std::string u_op,
                            Expression *ue ); // INC_OP, DEC_OP, SIZEOF
Expression *create_unary_expression_cast( Node *n_op, Expression *ce );
Expression *create_unary_expression_typename( std::string u_op, Node *t_name );

//-------------------------------------------------
class CastExpression : public Expression {
  public:
    Expression *op1;
    /**
      Index of Type casted to in GlobalTypes
      -1 if there is no casting
    */
    int typeCast;

    CastExpression() {
        op1 = nullptr;
        typeCast = -1;
    };
};
// NEED TO DO LATER
Expression *create_cast_expression_typename(
    Node *n,
    Expression *ce ); // type_name wala add krna hai

//-------------------------------------------------
class MultiplicativeExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    MultiplicativeExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_multiplicative_expression( std::string op, Expression *me,
                                              Expression *ce );

//-------------------------------------------------
class AdditiveExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    AdditiveExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_additive_expression( std::string op, Expression *ade,
                                        Expression *me );

//-------------------------------------------------
class ShiftExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    ShiftExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};

Expression *create_shift_expression( std::string op, Expression *se,
                                     Expression *ade );

//-------------------------------------------------
class RelationalExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    RelationalExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_relational_expression( std::string op, Expression *re,
                                          Expression *se );

//-------------------------------------------------
class EqualityExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    EqualityExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_equality_expression( std::string op, Expression *eq,
                                        Expression *re );

//-------------------------------------------------
class AndExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    AndExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_and_expression( std::string op, Expression *an,
                                   Expression *eq );

//-------------------------------------------------
class ExclusiveorExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    ExclusiveorExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_exclusive_or_expression( std::string op, Expression *ex,
                                            Expression *an );

//-------------------------------------------------
class InclusiveorExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    InclusiveorExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    }
};
Expression *create_inclusive_or_expression( std::string op, Expression *ie,
                                            Expression *ex );

//-------------------------------------------------
class Logical_andExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    Logical_andExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_logical_and_expression( std::string op, Expression *la,
                                           Expression *ie );

//-------------------------------------------------
class Logical_orExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    Logical_orExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    };
};
Expression *create_logical_or_expression( std::string op, Expression *lo,
                                          Expression *la );

//-------------------------------------------------
class ConditionalExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    Expression *op3;
    std::string op;

    ConditionalExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op3 = nullptr;
        op = "";
    };
};
Expression *create_conditional_expression( std::string op, Expression *lo,
                                           Expression *te, Expression *coe );

//-------------------------------------------------
class AssignmentExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;
    std::string op;

    AssignmentExpression() {
        op1 = nullptr;
        op2 = nullptr;
        op = "";
    }
};

Expression *create_assignment_expression(
    Expression *ue, Node *n_op,
    Expression
        *ase ); // can change string to node* later for assignment operator

//-------------------------------------------------
class TopLevelExpression : public Expression {
  public:
    Expression *op1;
    Expression *op2;

    TopLevelExpression() {
        op1 = nullptr;
        op2 = nullptr;
    };
};
Expression *create_toplevel_expression( Expression *te, Expression *ase );

//-------------------------------------------------
class ConstantExpression : public Expression {
  public:
    Expression *op1;
};

//-------------------------------------------------
class PostfixExpression : public Expression {
  public:
    PostfixExpression *pe;
    Expression *exp;
    Identifier *id;
    ArgumentExprList *ae_list;
    std::string op;

    PostfixExpression() {
        pe = nullptr;
        exp = nullptr;
        id = nullptr;
        ae_list = nullptr;
        op = "";
    };
};

class Constant : public Terminal {
  public:
    Constant( const char *name );

    Type getConstantType() {
        Type retT( 2, 0, false );
        int length = value.length();
        if ( name == "CONSTANT HEX" || name == "CONSTANT INT" ) {
          
            int islong = 0, isunsigned = 0;
            for ( int i = 0; i < length; i++ ) {
                if ( value[i] == 'l' || value[i] == 'L' )
                    islong = 1;
                if ( value[i] == 'u' || value[i] == 'U' )
                    isunsigned = 1;
                if ( islong && isunsigned ){
                    retT.typeIndex = PrimitiveTypes::U_LONG_T;
                    return retT;
                }
            }
            if ( islong ) {
                retT.typeIndex = PrimitiveTypes::LONG_T;
                return retT;
            }
            if ( isunsigned ) {
                retT.typeIndex = PrimitiveTypes::U_INT_T;
                return retT;
            }
            retT.typeIndex = PrimitiveTypes::INT_T;
            return retT;
            // loop over value to get unsigned etc and return typeIndex
        } else if ( name == "CONSTANT FLOAT" ) {
            
            int isfloat = 0;
            for ( int i = 0; i < length; i++ ) {
                
                if ( value[i] == 'f' || value[i] == 'F') {
                    retT.typeIndex = PrimitiveTypes::FLOAT_T;
                    return retT;
                }
            }
            
            retT.typeIndex = PrimitiveTypes::DOUBLE_T;

            return retT;
            // loop over value to get float
        } else if ( name == "CONSANT EXP" ) {
            // loop over value to get if long or double
            int islong = 0;
            for ( int i = 0; i < length; i++ ) {
                if ( value[i] == 'f' || value[i] == 'F' ) {
                    retT.typeIndex = PrimitiveTypes::FLOAT_T;
                    return retT;
                }
            }
            retT.typeIndex = PrimitiveTypes::LONG_T;
            return retT;
        } else {
            return retT;
        }
    }
};

class StringLiteral : public Terminal {
  public:
    StringLiteral( const char *name );
};

Expression *create_postfix_expr_arr( Expression *pe, Expression *exp );
Expression *create_postfix_expr_voidfun( Identifier *fi );
Expression *create_postfix_expr_fun( Identifier *fi, Expression *ae );
Expression *create_postfix_expr_struct( std::string access_op, Expression *pe,
                                        Identifier *id );
Expression *create_postfix_expr_ido( std::string op, Expression *pe );
#endif
