#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include <ast.h>
#include <deque>
#include <map>
#include <string>

enum PrimitiveTypes {U_CHAR_T = 0, CHAR_T = 1, U_SHORT_T = 2, SHORT_T = 3, U_INT_T = 4, INT_T=5, U_LONG_T=6, LONG_T=7, U_INT_LONG_T=8, INT_LONG_T=9, FLOAT_T=10, DOUBLE_T=11, LONG_DOUBLE_T=12 VOID_T=13};

// All class declarations
class StructDefinition;
class AssignmentExpression;
class ParameterTypeList;
class CastExpression;


//SXXX:TODO Add all primitive types by default 

class Types {
  public:
    int sc;
    int tq;
	  int index; // For primitive types the order in GlobalTypeMap is same as enum

    /**
      typeFamily:
      0 = Integer types
      1 = Floating types
      2 = Pointer types : direct assignments allowed?
      3 = Other User Defined : No direct assignments allowed
    */
    int typeFamily;
    
    //Variable for pointers
    int pointerLevel;
    
	  //Variable for arrays
    /* ndarray = 0 => not an array, else it is level of array */
    int ndarray;
    size_t size;

	  /**
      0 = PrimitveType
      1 = Pointer
      2 = Array
      3 = Struct
      4 = Union -- not supported for now
     */
    int userDefined;

    //Variable for 
	  StructDefinition * struct_definition;

    // Types(int a, int b, int c,..){
    //   typFamily = a;
    //   ...
    // }
    Types(int tc,int sc, int tq,int ind, int typeF,int pL,int nd, size_t s, int uD) {
      tq=tq;sc=sc;
      index=ind;
      typeFamily=typeF;
      pointerLevel=pL;
      ndarray=nd;
      size=s;
      userDefined=uD;
    }

    bool isPrimitive(){
      if (typeFamily ==0 || typeFamily ==1){
        return true;
      }
      else{
        return false;
      }
    }

    bool isUserDefined(){
      if(userDefined != 0){
        return true;
      }
      else{
        return false;
      }
    }
    bool isArray(){
      if(ndarray!=0){
        return true;
      }
      else{
        return false;
      }
    }

    Types getArrayType(){
      Types t;
      return t;
    }

    // Returns an Index to the type's Pointer in Global Type Map
    //Used for reference '&' operator
    int getPointerTypeIndex(){
      Types t;
      return t.index;
    }

    //Used for dereference operator
    int getUnderlyingTypeIndex(){
      if(typeFamily != 2){
        //Ideally if it is 2 then it is a programming error
        std::cout << "Should not have happened";
        return index;
      }
      //Get the underlying type or create it
      Types t;
      return t.index;
  }

  bool isUnsigned(){
    if(index == 0 || index == 2 || index == 4 || index == 6){
      return true;
    }
    else{
      return false;
    }
  }
  bool operator==(const Types & obj2) const
    {
        if(this->tq == obj2.tq && this->sc == obj2.sc && this->typeFamily==obj2.typeFamily && this->pointerLevel == obj2.pointerLevel && this->ndarray==obj2.ndarray && this->userDefined==obj2.userDefined)
            return true;
        else
            return false;
    }
};

std::vector<Types> GlobalTypeMap;


class Type {
	std::string name;
	int typeIndex;
	int ptr_level;

	Type();	
	bool isInt();
	bool isFloat();
	bool isUnsigned();
	void make_signed();
	void make_unsigned();

};
int get_index(Types t)

class Expression : public Non_Terminal {
	public:
    	Type type;
	  /* Change this late */
	  int num_opearands;
	  // Expression( Types * type, int num_op );



    Types getType(){
      Types t; // Lookup using typeIndex 
      return t;
    }
  
  Expression();
};


//-------------------------------------------------
class PrimaryExpression : public Expression {
	/**
    isTerminal = 0 when ( Expression )
    isTerminal = 1 when IDENTIFIER
    isTerminal = 2 when CONSTANT
    isTerminal = 3 when STRING_LITERAL
  */
 public :
  int isTerminal;
  Identifier *Ival;
  StringLiteral *Sval;
  Constant *Cval;
  Expression *op1;

  PrimaryExpression();
  Types *evaluate_type();

};

//Grammar warppers for PrimaryExpression
PrimaryExpression *create_primary_identifier(Identifier *a);
PrimaryExpression *create_primary_constant(Constant *a);
PrimaryExpression *create_primary_stringliteral(StringLiteral *a);
PrimaryExpression *create_primary_expression(TopLevelExpression *a);



//-------------------------------------------------

class ArgumentExprList: public Expression{
  public :
  Expression *op1; // This will be null in case of root object
  Expression *op2;
  Types * evaluate_type(); // XXX: TODO: Do we need type checking here?
  ArgumentExprList();
};

//Grammar warppers for ArguementExpressionList
ArgumentExprList * create_argument_expr_assignement(AssignmentExpression *ase);
ArgumentExprList * create_argument_expr_list(ArgumentExprList *ae_list, AssignmentExpression *ase);



//-------------------------------------------------
class UnaryExpression: public Expression{
  public:
    Expression *op1;
    std::string op;
    Types * evaluate_type();

    UnaryExpression();

  /// XXX: TODO
  UnaryExpression(std::string u_op, Expression *e){
    if(u_op == "++"){
      //Set Something
        /// XXX : Should we type check in the constructor itself?
        // if(dynamic_cast<UnaryExpression *>(e) != nullptr)
    }
    else if(u_op == "--"){
      //Set something
    }
    else if(u_op == "sizeof"){
      //Set something
    }
    else{
      //ERROR
    }
    op1 = e;
  }

};

//Grammar warppers for UnaryExpression
UnaryExpression * create_unary_expression_ue(std::string u_op, UnaryExpression* ue); // INC_OP, DEC_OP, SIZEOF
UnaryExpression * create_unary_expression_cast(std::string u_op, CastExpression* ce); 
UnaryExpression * create_unary_expression_typename();


//-------------------------------------------------
class CastExpression : public Expression{
  public :
    Expression * op1;
    /**
      Index of Type casted to in GlobalTypes
      -1 if there is no casting
    */
    int typeCast; 
    Types * evaluate_type();
  CastExpression();
};
// NEED TO DO LATER
CastExpression * create_caste_expression_typename(CastExpression* ce); // type_name wala add krna hai


//-------------------------------------------------
class MultiplicativeExpression : public Expression{
  public :
  Expression * op1;
  Expression * op2;
  std::string op;
  Types * evaluate_type();
  MultiplicativeExpression();
};
MultiplicativeExpression * create_multiplicative_expression(std::string op,MultiplicativeExpression *me, CastExpression *ce);


//-------------------------------------------------
class AdditiveExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    AdditiveExpression();
};
AdditiveExpression * create_additive_expression(std::string op,AdditiveExpression * ade, MultiplicativeExpression *me);



//-------------------------------------------------
class ShiftExpression : public Expression{
  public  :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    ShiftExpression();
    
};

ShiftExpression * create_shift_expression(std::string op,ShiftExpression *se, AdditiveExpression *ade);



//-------------------------------------------------
class RelationalExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    RelationalExpression();
};
RelationalExpression * create_relational_expression(std::string op, RelationalExpression *re, ShiftExpression *se);


//-------------------------------------------------
class EqualityExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    EqualityExpression();
};
EqualityExpression * create_equality_expression(std::string op,EqualityExpression *eq, RelationalExpression *re);



//-------------------------------------------------
class AndExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    AndExpression();
}; 
AndExpression * create_and_expression(std::string op,AndExpression * an,EqualityExpression * eq);



//-------------------------------------------------
class ExclusiveorExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    ExclusiveorExpression();
};
ExclusiveorExpression * create_exclusive_or_expression(std::string op,ExclusiveorExpression * ex,AndExpression * an);



//-------------------------------------------------
class InclusiveorExpression : public Expression{
  public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    InclusiveorExpression();
};
InclusiveorExpression * create_inclusive_or_expression(std::string op,InclusiveorExpression * ie, ExclusiveorExpression *ex); 
 


//-------------------------------------------------
class Logical_andExpression : public Expression{
  public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    Logical_andExpression();
};
Logical_andExpression * creat_logical_and_expression(std::string op,Logical_andExpression * la, InclusiveorExpression *ie);



//-------------------------------------------------
class Logical_orExpression : public Expression{
    public :
    Expression *op1;
    Expression *op2;
    std::string op;
    Types * evaluate_type();
    Logical_orExpression();
};
Logical_orExpression * creat_logical_or_expression(std::string op,Logical_orExpression * lo, Logical_andExpression *la);



//-------------------------------------------------
class ConditionalExpression : public Expression{
  public:
    Expression *op1;
    Expression *op2;
    Expression *op3;
    std::string op;
    Types * evaluate_type();
    ConditionalExpression();
};
ConditionalExpression * create_conditional_expression(std::string op,Logical_orExpression *lo,TopLevelExpression * te, ConditionalExpression * coe);



//-------------------------------------------------
class AssignmentExpression : public Expression{
  public: 
    Expression *op1;
    Expression *op2;
    std::string op;
    Types *evaluate_type();
    AssignmentExpression();

};

AssignmentExpression * create_assignment_expression(std::string op,UnaryExpression * ue,  AssignmentExpression * ase); // can change string to node* later for assignment operator


//-------------------------------------------------
class TopLevelExpression : public Expression{
  public :
  Expression *op1;
  Expression *op2;
  Types * evaluate_type();
  TopLevelExpression();
}; 
TopLevelExpression * create_toplevel_expression(TopLevelExpression *te, AssignmentExpression *ase);


//-------------------------------------------------
class ConstantExpression : public Expression{
  public:
    Expression *op1;
    Types * evaluate_type();
};


//-------------------------------------------------
enum PostfixExpressionTypes {ARRAY,FUNCTION, STRUCT, INC, DEC };

class PostfixExpression : public Expression {
	public:
    PostfixExpressionTypes pe_type;
	  PostfixExpression * pe;
	  Expression *exp;
	  Identifier *id;
	  ArgumentExprList * ae_list;
	  PostfixExpression();
};

PostfixExpression* create_postfix_expr_arr(PostfixExpression * pe,  Expression * exp);
PostfixExpression* create_postfix_expr_voidfun();
PostfixExpression* create_postfix_expr_fun(PostfixExpression * pe,  ArgumentExprList * ae_list);
PostfixExpression* create_postfix_expr_struct(PostfixExpression * pe,  Identifier * id);
PostfixExpression* create_postfix_expr_pointer(PostfixExpression* pe, Identifier *id);
PostfixExpression *create_postfix_expr_ido(std::string op, PostfixExpression * pe);


class SymTabEntry {
  public:
    std::string name;
    int typeIndex;
    int level;

    // TODO: This needs to be expanded
    SymTabEntry( std::string name );
};

class FuncEnt : public SymTabEntry {
  public:
    std::string args;

    FuncEnt( std::string name, std::string type, std::string args );
};

class DeclarationSpecifiers;
class Declarator;

class SymbolTable {

  public:
    SymbolTable();

    virtual SymTabEntry *get_symbol_from_table( std::string name );
    void delete_from_table( SymTabEntry *symbol );
    void print_table();
    virtual void add_to_table( SymTabEntry *symbol );
};

class GlobalSymbolTable : public SymbolTable {
  public:
    std::map<std::string, SymTabEntry *> sym_table;
    void add_symbol( DeclarationSpecifiers *declaration_specifiers,
                     Declarator *declarator );
};

class LocalSymbolTable : public SymbolTable {
  public:
    std::map<std::string, std::deque<SymTabEntry *> &> sym_table;
    std::string function_name;
    int current_level;
    void increase_level();
    void clear_current_level();
    void empty_table();
    LocalSymbolTable();
    void add_to_table( SymTabEntry *symbol );
    SymTabEntry *get_symbol_from_table( std::string name );
    void add_function( DeclarationSpecifiers *declaration_specifiers,
                       Declarator *declarator );
};

extern LocalSymbolTable local_symbol_table;
extern GlobalSymbolTable global_symbol_table;

typedef int TYPE_QUALIFIER;

class Identifier : public Terminal {
  public:
    Identifier( const char *name );
    
    
};

class Constant : public Terminal {
  public:
    Constant (const char *name);

    int getConstantType(){
      int length = value.length(); 
      if(name == "CONSTANT HEX" || name == "CONSTANT INT"){
          int islong=0,isunsigned=0;
          for(int i=0;i<length;i++){
            if(value[i]=='l' || value[i]=='L')
              islong=1;
            if(value[i]=='u' || value[i]=='U')
              isunsigned=1;
            if(islong && isunsigned)
              return UL_INT_T;
          }
          if(islong)
            return L_INT_T;
          if(isunsigned)
            return U_INT_T;
          return INT_T;
        //loop over value to get unsigned etc and return typeIndex
      }
      else if(name == "CONSTANT FLOAT"){
        int isfloat=0;
        for(int i=0;i<length;i++){
          if(value[i]=='f' || value[i]=='F')
            return FLOAT_T;
        }
        return DOUBLE_T;
        //loop over value to get float
      }
      else if(name == "CONSANT EXP"){
        //loop over value to get if long or double
        int islong=0;
        for(int i=0;i<length;i++){
          if(value[i]=='f' || value[i]=='F')
            return FLOAT_T;
        }
        return LONG_T;
      }
      else{
        return 0;
      }
    }
};

class StringLiteral : public Terminal {
  public:
    StringLiteral(const char *name);
};

class TypeQualifierList : public Non_Terminal {
  public:
    std::vector<TYPE_QUALIFIER> type_qualifier_list;

    TypeQualifierList();

    void append_to_list( TYPE_QUALIFIER type );

    ~TypeQualifierList();
};

TypeQualifierList *create_type_qualifier_list( TYPE_QUALIFIER type );
TypeQualifierList *add_to_type_qualifier_list( TypeQualifierList *tql,
                                               TYPE_QUALIFIER type );

class Pointer : public Non_Terminal {
  public:
    TypeQualifierList *type_qualifier_list;
    Pointer *pointer;

    Pointer();
    Pointer( TypeQualifierList *type_qualifier_list, Pointer *pointer );
};

Pointer *create_pointer();
Pointer *create_pointer( TypeQualifierList *type_list, Pointer *pointer );

class DirectDeclarator;

class Declarator : public Non_Terminal {
  public:
    Identifier *id;
    Pointer *pointer;
    DirectDeclarator *direct_declarator;
    Node *init_expr;

    Declarator();
    Declarator( Pointer *p, DirectDeclarator *dd );
};
Declarator *add_initializer_to_declarator( Declarator *declarator,
                                           Node *init_expr );

Declarator *create_declarator( Pointer *pointer,
                               DirectDeclarator *direct_declarator );

Declarator *verify_struct_declarator( Declarator *declarator );

typedef enum direct_declartor_enum {
    ID,
    DECLARATOR,
    ARRAY,
    FUNCTION,
    NUM_DIRECT_DECLARATORS

} DIRECT_DECLARATOR_TYPE;

class DirectDeclarator : public Non_Terminal {

  public:
    DIRECT_DECLARATOR_TYPE type;

    Identifier *id;
    Declarator *declarator;
    DirectDeclarator *direct_declarator;

#if 0
		//TODO: Add these in
		Constant_Expression * const_expr;
#endif

    Node *const_expr;
    ParameterTypeList *params;
    DirectDeclarator();
};

DirectDeclarator *create_dir_declarator_id( DIRECT_DECLARATOR_TYPE type,
                                            Identifier *id );
DirectDeclarator *create_dir_declarator_dec( DIRECT_DECLARATOR_TYPE type,
                                             Declarator *declarator );
DirectDeclarator *
create_dir_declarator_arr( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           Node *const_expr );
DirectDeclarator *
create_dir_declarator_fun( DIRECT_DECLARATOR_TYPE type,
                           DirectDeclarator *direct_declarator,
                           ParameterTypeList *params );

class DeclaratorList : public Non_Terminal {
  public:
    std::vector<Declarator *> declarator_list;
    DeclaratorList();
};

DeclaratorList *create_init_declarator_list( Declarator *init_declarator );
DeclaratorList *
add_to_init_declarator_list( DeclaratorList *init_declarator_list,
                             Declarator *init_declarator );

typedef int STORAGE_CLASS;
class TypeSpecifier;

class DeclarationSpecifiers : public Non_Terminal {
  public:
    std::vector<STORAGE_CLASS> storage_class;
    std::vector<TypeSpecifier *> type_specifier;
    std::vector<TYPE_QUALIFIER> type_qualifier;

    int isValid(); // Type Checking

    DeclarationSpecifiers();
};

DeclarationSpecifiers *new_storage_class( STORAGE_CLASS sc );
DeclarationSpecifiers *new_type_specifier( TypeSpecifier *ts );
DeclarationSpecifiers *new_type_qualifier( TYPE_QUALIFIER tq );

DeclarationSpecifiers *add_storage_class( DeclarationSpecifiers *ds,
                                          STORAGE_CLASS sc );
DeclarationSpecifiers *add_type_specifier( DeclarationSpecifiers *ds,
                                           TypeSpecifier *ts );
DeclarationSpecifiers *add_type_qualifier( DeclarationSpecifiers *ds,
                                           TYPE_QUALIFIER tq );

class Declaration : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    DeclaratorList *init_declarator_list;
    int type;

    Declaration( DeclarationSpecifiers *declaration_specifiers_,
                 DeclaratorList *init_declarator_list_ );
    void add_to_symbol_table( LocalSymbolTable &sym_tab );
    void add_to_symbol_table( GlobalSymbolTable &sym_tab );
};

Declaration *new_declaration( DeclarationSpecifiers *declaraion_specifiers,
                              DeclaratorList *init_declarator_list );

class DeclarationList : public Non_Terminal {
  public:
    std::vector<Declaration *> declarations;
    DeclarationList();

    void create_symbol_table_level();
};

DeclarationList *create_declaration_list( Declaration *declaraiton );
DeclarationList *add_to_declaration_list( DeclarationList *declaraiton_list,
                                          Declaration *Declaration );

class FunctionDefinition : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    Declarator *declarator;
    Node *compound_statement;

    FunctionDefinition( DeclarationSpecifiers *declaration_specifiers_,
                        Declarator *declarator_, Node *compound_statement_ );
};

FunctionDefinition *
create_function_defintion( DeclarationSpecifiers *declaration_specifiers,
                           Declarator *declarator, Node *compound_statement );

class AbstractDeclarator;

typedef enum {
    ABSTRACT,
    SQUARE,
    ROUND
    // TODO: Fill this

} DIRECT_ABSTRACT_DECLARATOR_TYPE;

class DirectAbstractDeclarator : public Non_Terminal {
  public:
    DIRECT_ABSTRACT_DECLARATOR_TYPE type;Type
    AbstractDeclarator *abstract_declarator;
    Node *const_expr;
    DirectAbstractDeclarator *direct_abstract_declarator;
    ParameterTypeList *parameter_type_list;
    DirectAbstractDeclarator();
};

DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ );
DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   AbstractDeclarator *abs );
DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs );
DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs, Node *ce );
DirectAbstractDeclarator *
create_direct_abstract_declarator( DIRECT_ABSTRACT_DECLARATOR_TYPE typ,
                                   DirectAbstractDeclarator *dabs,
                                   ParameterTypeList *param_list );

class AbstractDeclarator : public Non_Terminal {
  public:
    Pointer *pointer;
    DirectAbstractDeclarator *direct_abstract_declarator;
    AbstractDeclarator( Pointer *ptr, DirectAbstractDeclarator *dabs );
};

AbstractDeclarator *
create_abstract_declarator( Pointer *pointer, DirectAbstractDeclarator *dabs );

class ParameterDeclaration : public Non_Terminal {
  public:
    DeclarationSpecifiers *declaration_specifiers;
    Declarator *declarator;
    AbstractDeclarator *abstract_declarator;
    ParameterDeclaration();
};

ParameterDeclaration *create_parameter_declaration( DeclarationSpecifiers *ds,
                                                    Declarator *d,
                                                    AbstractDeclarator *ad );

class ParameterTypeList : public Non_Terminal {
  public:
    std::vector<ParameterDeclaration *> param_list;
    bool has_ellipsis;
    ParameterTypeList();
};

ParameterTypeList *create_parameter_list( ParameterDeclaration *pd );
ParameterTypeList *add_to_parameter_list( ParameterTypeList *ptl,
                                          ParameterDeclaration *pd );
ParameterTypeList *add_ellipsis_to_list( ParameterTypeList *ptl );

class SpecifierQualifierList : public Non_Terminal {
  public:
    std::vector<TypeSpecifier *> type_specifiers;
    std::vector<TYPE_QUALIFIER> type_qualifiers;

    SpecifierQualifierList();
};

SpecifierQualifierList *
create_type_specifier_sq( TypeSpecifier *type_specifier );
SpecifierQualifierList *
create_type_qualifier_sq( TYPE_QUALIFIER type_qualifier );
SpecifierQualifierList *add_type_specifier_sq( SpecifierQualifierList *sq_list,
                                               TypeSpecifier *type_specifier );
SpecifierQualifierList *add_type_qualifier_sq( SpecifierQualifierList *sq_list,
                                               TYPE_QUALIFIER type_qualifier );

class StructDeclaration : public Non_Terminal {
  public:
    SpecifierQualifierList *sq_list;
    DeclaratorList *declarator_list;

    StructDeclaration( SpecifierQualifierList *sq_list_,
                       DeclaratorList *declarator_list_ );
};

StructDeclaration *
create_struct_declaration( SpecifierQualifierList *sq_list,
                           DeclaratorList *struct_declarator_list );

class StructDeclarationList : public Non_Terminal {
  public:
    std::vector<StructDeclaration *> struct_declaration_list;
    StructDeclarationList();
};

StructDeclarationList *
create_struct_declaration_list( StructDeclaration *struct_declaration );
StructDeclarationList *
add_to_struct_declaration_list( StructDeclarationList *struct_declaration_list,
                                StructDeclaration *struct_declaration );

class Enumerator : public Non_Terminal {
  public:
    Identifier *id;
    Node *init_expr;
    Enumerator( Identifier *id_, Node *init_expr );
};

Enumerator *create_enumerator( Identifier *id, Node *const_expr );

class EnumeratorList : public Non_Terminal {
  public:
    std::vector<Enumerator *> enumerator_list;
    EnumeratorList();
};

EnumeratorList *create_enumerator_list( Enumerator *enumerator );
EnumeratorList *add_to_enumerator_list( EnumeratorList *enumerator_list,
                                        Enumerator *enumerator );

typedef int TYPE_SPECIFIER;

class TypeSpecifier : public Non_Terminal {
  public:
    TYPE_SPECIFIER type;
    Identifier *id;
    StructDeclarationList *struct_declaration_list;
    EnumeratorList *enumerator_list;

    TypeSpecifier( TYPE_SPECIFIER typ );
    TypeSpecifier( TYPE_SPECIFIER type, Identifier *Id,
                   StructDeclarationList *struct_declaration_list );
    TypeSpecifier( TYPE_SPECIFIER type, Identifier *Id,
                   EnumeratorList *enumerator_list );
};

TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type );
TypeSpecifier *
create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                       StructDeclarationList *struct_declaration_list );
TypeSpecifier *create_type_specifier( TYPE_SPECIFIER type, Identifier *id,
                                      EnumeratorList *enumerator_list );

Node *add_to_global_symbol_table( Declaration *declaration );

#endif
