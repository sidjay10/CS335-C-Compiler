#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <ast.h>
#include <deque>
#include <map>
#include <string>
#include <symtab.h>
#include <3ac.h>
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
    /* Change this later */
    int num_operands;
    // Expression( Types * type, int num_op );
	Address  * res;
  std::vector<GoTo*> truelist;
  std::vector<GoTo*> falselist;
//	ThreeAC * code;

#if 0
	Code
	NextList
	FalseList

#endif

    Expression() : Non_Terminal( "" ), res ( nullptr ){};
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
	std::vector <Expression * > args;

    ArgumentExprList() {};
};

// Grammar warppers for ArguementExpressionList
ArgumentExprList *create_argument_expr_assignement( Expression *ase );
ArgumentExprList *create_argument_expr_list( ArgumentExprList *ae_list, Expression *ase );
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
create_unary_expression( Terminal * op,
                            Expression *ue ); // INC_OP, DEC_OP, SIZEOF
Expression *create_unary_expression_cast( Node *n_op, Expression *ce );
Expression *create_unary_expression( Terminal *, TypeName *t_name );

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
    TypeName *tn,
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

union data{
    int i;
    unsigned int ui;
    long l;
    unsigned long ul;
    float f;
    double d;
    char c;
    unsigned char uc;
};
class Constant : public Terminal {
  public:
    Constant( const char *name ,const char* value, unsigned int line_num, unsigned int column);
    union data val;
    Type ConstantType;

    Type getConstantType() {
        return ConstantType;
    }
    void negate();
  
};

Constant* create_constant( const char *name ,const char* value, unsigned int line_num, unsigned int column);

class StringLiteral : public Terminal {
  public:
    StringLiteral( const char *name );
};

Expression *create_postfix_expr_arr( Expression *pe, Expression *exp );
Expression *create_postfix_expr_voidfun( Identifier *fi );
Expression *create_postfix_expr_fun( Identifier *fi, ArgumentExprList *ae );
Expression *create_postfix_expr_struct( std::string access_op, Expression *pe,
                                        Identifier *id );
Expression *create_postfix_expr_ido( Terminal * op, Expression *pe );
#endif
