/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "src/grammar.y"

	#include <stdio.h>
	#include <iostream>
	#include <sstream>
	#include <ast.h>
	#include <symtab.h>
	#include <expression.h>
	#include <3ac.h>
	#include <statement.h>
	
	void yyerror(const char *s);	
	extern "C" int yylex();
	extern Node * root; 
	


#line 87 "grammar.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SIZEOF = 258,
    IDENTIFIER = 259,
    CONSTANT = 260,
    STRING_LITERAL = 261,
    INC_OP = 262,
    DEC_OP = 263,
    PTR_OP = 264,
    LEFT_OP = 265,
    RIGHT_OP = 266,
    LE_OP = 267,
    GE_OP = 268,
    EQ_OP = 269,
    NE_OP = 270,
    AND_OP = 271,
    OR_OP = 272,
    MUL_ASSIGN = 273,
    DIV_ASSIGN = 274,
    MOD_ASSIGN = 275,
    ADD_ASSIGN = 276,
    SUB_ASSIGN = 277,
    LEFT_ASSIGN = 278,
    RIGHT_ASSIGN = 279,
    AND_ASSIGN = 280,
    XOR_ASSIGN = 281,
    OR_ASSIGN = 282,
    TYPEDEF = 283,
    EXTERN = 284,
    STATIC = 285,
    AUTO = 286,
    REGISTER = 287,
    SIGNED = 288,
    UNSIGNED = 289,
    CHAR = 290,
    SHORT = 291,
    LONG = 292,
    INT = 293,
    FLOAT = 294,
    DOUBLE = 295,
    VOID = 296,
    TYPE_NAME = 297,
    CONST = 298,
    VOLATILE = 299,
    STRUCT = 300,
    UNION = 301,
    ENUM = 302,
    ELLIPSIS = 303,
    CASE = 304,
    DEFAULT = 305,
    IF = 306,
    ELSE = 307,
    SWITCH = 308,
    WHILE = 309,
    DO = 310,
    FOR = 311,
    GOTO = 312,
    CONTINUE = 313,
    BREAK = 314,
    RETURN = 315
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "src/grammar.y"

	Node * node;
	Terminal * terminal;
	int value;
	DeclarationSpecifiers * declaration_specifiers;
	Declaration * declaration;
	DeclarationList * declaration_list;
	Declarator * declarator;
	DeclaratorList * init_declarator_list;
	Pointer * pointer;
	DirectDeclarator * direct_declarator;
	Identifier * identifier;
	TypeQualifierList * type_qualifier_list;
	FunctionDefinition * function_definition;
	TypeSpecifier * type_specifier;
	Enumerator * enumerator;
	EnumeratorList * enumerator_list;
	StructDeclarationList * struct_declaration_list;
	StructDeclaration * struct_declaration;
	SpecifierQualifierList * specifier_qualifier_list;
	ParameterTypeList * parameter_type_list;
	ParameterDeclaration * parameter_declaration;
	DirectAbstractDeclarator * direct_abstract_declarator;
	AbstractDeclarator * abstract_declarator;
	Constant *constant;
	StringLiteral *string_literal;
	TopLevelExpression* top_level_expression;
	AssignmentExpression* assignment_expression;
	ArgumentExprList* argument_expression_list;
	PostfixExpression* postfix_expression;
	UnaryExpression* unary_expression;
	CastExpression* cast_expression;
	MultiplicativeExpression* multiplicative_expression;
	AdditiveExpression* additive_expression;
	ShiftExpression* shift_expression;
	RelationalExpression* relational_expression;
	EqualityExpression* equality_expression;
	AndExpression* and_expression;
	ExclusiveorExpression* exclusive_or_expression;
	InclusiveorExpression* inclusive_or_expression;
	Logical_andExpression* logical_and_expression;
	Logical_orExpression* logical_or_expression;
	ConditionalExpression* conditional_expression;
	Expression* expression;
	TypeName * type_name;
	Label * label; 
	GoTo  * _goto;
	Statement * statement;

#line 247 "grammar.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);





#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  45
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   762

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  193
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  324

#define YYUNDEFTOK  2
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    11,     2,     2,     2,    16,    12,     2,
       3,     4,    13,     9,     8,    10,     7,    15,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    21,    23,
      17,    26,    18,    22,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     5,     2,     6,    19,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    24,    20,    25,    14,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   171,   171,   172,   173,   175,   179,   180,   182,   183,
     185,   186,   187,   188,   192,   193,   197,   198,   199,   200,
     201,   202,   206,   207,   208,   209,   210,   211,   215,   216,
     220,   221,   222,   223,   227,   228,   229,   233,   234,   235,
     239,   240,   241,   242,   243,   247,   248,   249,   253,   254,
     258,   259,   263,   264,   268,   269,   273,   274,   278,   279,
     283,   284,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   302,   303,   308,   312,   313,   318,   319,
     320,   321,   322,   323,   327,   328,   329,   330,   334,   336,
     340,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   370,   371,   375,   380,   381,   385,   386,
     390,   394,   395,   396,   397,   401,   402,   406,   412,   413,
     414,   418,   419,   423,   424,   428,   433,   434,   438,   441,
     442,   444,   445,   449,   450,   451,   452,   456,   457,   461,
     462,   466,   467,   471,   472,   473,   482,   483,   487,   488,
     489,   493,   494,   495,   496,   518,   519,   520,   521,   522,
     523,   527,   528,   529,   533,   534,   535,   536,   540,   541,
     545,   546,   551,   552,   556,   557,   558,   562,   563,   564,
     565,   569,   570,   571,   572,   573,   577,   578,   582,   583,
     587,   591,   596,   597
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'('", "')'", "'['", "']'", "'.'", "','",
  "'+'", "'-'", "'!'", "'&'", "'*'", "'~'", "'/'", "'%'", "'<'", "'>'",
  "'^'", "'|'", "':'", "'?'", "';'", "'{'", "'}'", "'='", "SIZEOF",
  "IDENTIFIER", "CONSTANT", "STRING_LITERAL", "INC_OP", "DEC_OP", "PTR_OP",
  "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP",
  "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN",
  "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN",
  "OR_ASSIGN", "TYPEDEF", "EXTERN", "STATIC", "AUTO", "REGISTER", "SIGNED",
  "UNSIGNED", "CHAR", "SHORT", "LONG", "INT", "FLOAT", "DOUBLE", "VOID",
  "TYPE_NAME", "CONST", "VOLATILE", "STRUCT", "UNION", "ENUM", "ELLIPSIS",
  "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO",
  "CONTINUE", "BREAK", "RETURN", "$accept", "primary_expression",
  "postfix_expression", "argument_expression_list", "unary_expression",
  "unary_operator", "cast_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_or_expression", "conditional_expression",
  "assignment_expression", "assignment_operator", "expression",
  "constant_expression", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "struct_or_union_specifier", "struct_or_union_id",
  "struct_or_union", "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "declarator", "direct_declarator", "pointer",
  "type_qualifier_list", "parameter_type_list", "parameter_list",
  "parameter_declaration", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "statement", "labeled_statement",
  "compound_statement", "declaration_list", "statement_list",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "translation_unit", "external_declaration",
  "function_declaration", "function_definition", "M_LABEL", "M_GOTO", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,    40,    41,    91,    93,    46,    44,    43,
      45,    33,    38,    42,   126,    47,    37,    60,    62,    94,
     124,    58,    63,    59,   123,   125,    61,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   312,   313,   314,   315
};
# endif

#define YYPACT_NINF (-208)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-191)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     691,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,    56,  -208,    77,   691,   691,
    -208,    53,     6,  -208,   691,   203,  -208,    79,  -208,   117,
     142,  -208,     9,  -208,  -208,    74,  -208,     3,   137,   146,
    -208,  -208,   411,  -208,  -208,  -208,  -208,   219,  -208,   150,
      23,  -208,   117,  -208,  -208,     9,    29,  -208,   652,   541,
      10,   137,   411,   280,  -208,     4,   411,   525,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,   664,    78,  -208,  -208,
     694,   694,   652,   172,   198,   210,   214,  -208,   218,   178,
     200,   201,   168,  -208,    21,   404,   652,  -208,   173,   195,
     116,    55,   169,   213,   222,   232,   216,    44,  -208,  -208,
     130,  -208,    77,  -208,  -208,  -208,   301,   383,  -208,  -208,
    -208,  -208,   652,   117,  -208,    61,  -208,  -208,  -208,  -208,
     227,   251,  -208,  -208,    33,   253,   250,  -208,   230,   269,
    -208,  -208,  -208,   136,  -208,  -208,  -208,   133,    28,   283,
     525,  -208,   610,   495,   652,  -208,  -208,  -208,  -208,   270,
     495,   652,   652,  -208,   495,   622,   272,  -208,  -208,  -208,
     141,   652,   278,  -208,  -208,   279,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,  -208,  -208,  -208,  -208,   652,  -208,   652,
     652,   652,   652,   652,   652,   652,   652,   652,   652,   652,
     652,   652,   652,   652,   652,   652,   652,   652,   652,  -208,
    -208,  -208,   413,  -208,  -208,  -208,  -208,  -208,     7,  -208,
      40,  -208,   303,  -208,   675,   310,  -208,     4,  -208,  -208,
     304,  -208,   652,   313,  -208,   157,  -208,  -208,   495,  -208,
     159,   164,   652,   240,  -208,  -208,  -208,   184,  -208,  -208,
    -208,  -208,  -208,  -208,   173,   173,   195,   195,   116,   116,
     116,   116,    55,    55,   169,   213,   222,   232,    81,   216,
    -208,  -208,  -208,   314,   303,    59,  -208,  -208,  -208,  -208,
    -208,  -208,  -208,   652,  -208,  -208,   495,   165,   316,   622,
    -208,   652,  -208,  -208,   315,  -208,   495,  -208,  -208,  -208,
     318,  -208,  -208,   247,   495,   652,  -208,  -208,  -208,  -208,
     179,   495,   652,  -208,   311,  -208,   319,   495,  -208,   331,
    -208,  -208,   495,  -208
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    90,    98,    99,    92,    93,    95,    94,    96,    97,
      91,   102,   125,   106,   107,     0,   189,     0,    78,    80,
     100,   104,     0,   101,    82,     0,   186,     0,   188,     0,
     120,    86,   133,    76,   128,     0,    84,    88,   127,     0,
      79,    81,     0,   105,    83,     1,   187,     0,   191,   123,
       0,   121,     0,   137,   135,   134,     0,    77,     0,     0,
       0,   126,   112,     0,   108,     0,   114,     0,    24,    25,
      27,    22,    23,    26,   172,   164,     0,     2,     3,     4,
       0,     0,     0,     0,     0,     0,     0,   192,     0,     0,
       0,     0,     0,     6,    16,    28,     0,    30,    34,    37,
      40,    45,    48,    50,    52,    54,    56,    58,    60,    73,
       0,   168,     0,   170,   155,   156,     0,     0,   157,   158,
     159,   160,     0,     0,   118,     0,   138,   136,    87,    85,
      88,     2,    89,   132,   145,     0,   139,   141,     0,     0,
     111,   103,   109,     0,   115,   117,   113,     0,   146,     0,
       0,    20,     0,     0,     0,    17,    18,    28,    75,     0,
       0,     0,     0,   192,     0,     0,     0,   182,   183,   184,
       0,     0,     0,    12,    13,     0,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,     0,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
     166,   169,     0,   165,   171,   124,   122,   119,     0,   143,
     148,   144,   149,   131,     0,     0,   129,     0,   110,     5,
     148,   147,     0,     0,     8,     0,    14,   161,     0,   163,
       0,     0,     0,     0,   192,   181,   185,     0,    10,    11,
      61,    31,    32,    33,    35,    36,    38,    39,    41,    42,
      43,    44,    46,    47,    49,    51,    53,    55,     0,    57,
      74,   167,   151,     0,   150,     0,   140,   142,   130,   116,
      29,    21,     9,     0,   162,   192,     0,     0,     0,     0,
       7,     0,   152,   153,     0,    15,     0,   176,   192,   192,
     193,    59,   154,   174,     0,     0,   192,   192,   193,   177,
       0,     0,     0,   192,     0,   179,   193,     0,   178,     0,
     175,   192,     0,   180
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -208,  -208,  -208,  -208,   -75,  -208,   -78,    18,    24,    38,
      39,   134,   145,   148,   149,   162,  -208,   -79,   -56,  -208,
     -67,   233,   -37,   -10,  -208,   300,  -208,    -7,  -208,  -208,
    -208,  -208,   294,   -43,  -208,   160,  -208,   321,   254,     8,
      -1,   -35,    12,  -208,  -208,  -208,   166,   238,   241,  -122,
    -102,  -208,   364,  -208,   282,  -154,  -208,  -208,  -208,  -208,
     374,  -208,  -208,  -151,  -207
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    93,    94,   235,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   187,
     110,   159,    16,    17,    35,    36,    18,    19,    20,    21,
      22,    63,    64,    65,   143,   144,    23,    50,    51,    24,
     130,    38,    39,    55,   135,   136,   137,   149,   221,   222,
     113,   114,   115,   116,   117,   118,   119,   120,   121,    25,
      26,    27,    28,   164,   307
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     147,   151,   132,   158,    61,   155,   156,   157,    40,    41,
     111,   244,   242,   272,    44,   214,    37,    32,   188,   140,
     138,   157,    32,   146,   148,   170,   171,  -190,   172,    58,
     128,   123,    34,   218,    43,    62,   273,   112,   218,   139,
      53,    32,    32,   158,    54,   218,    32,   157,   124,   134,
      66,   237,   173,   174,   175,    62,    62,    34,   239,    62,
      62,    34,   243,   126,   145,   293,   206,   127,    34,   123,
      66,    66,   196,   197,    66,    66,    12,    42,    31,   211,
      29,   152,    56,   147,    30,   207,   217,   147,   294,   208,
      32,   198,   199,   289,   240,   241,   236,    57,   274,   153,
      33,   313,   291,    47,   247,    34,   112,   148,   274,   319,
     214,   251,   252,   253,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   250,   157,   219,   296,   300,   284,   229,   208,   268,
      59,   208,    60,    62,   227,    49,   220,   304,   305,   208,
     194,   195,   270,   209,   280,   311,   312,   157,    66,   228,
     230,   282,   317,   285,   246,   283,    52,   208,   286,   298,
     322,    67,   208,   208,    34,   287,   122,    68,    69,    70,
      71,    72,    73,   314,   297,    61,   189,   208,   190,   191,
     290,   169,   208,   160,   303,    76,   131,    78,    79,    80,
      81,   161,   309,    45,   192,   193,   166,   200,   201,   315,
     254,   255,   301,   162,   134,   320,   157,   163,   256,   257,
     323,   165,    67,   167,   168,   202,   145,   295,    68,    69,
      70,    71,    72,    73,   258,   259,   260,   261,   310,   262,
     263,   203,    74,    47,    75,   316,    76,    77,    78,    79,
      80,    81,   204,    58,   152,     1,   205,   223,   224,   225,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     1,    13,    14,    15,   226,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,   232,    13,    14,
      15,   238,    82,    83,    84,   245,    85,    86,    87,    88,
      89,    90,    91,    92,    67,   141,   248,   249,   275,   218,
      68,    69,    70,    71,    72,    73,   278,   281,   288,   299,
     292,   302,   306,   308,    74,    47,   210,   208,    76,    77,
      78,    79,    80,    81,   318,   321,   264,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,   265,    13,
      14,    15,   266,     1,   267,   215,   129,   142,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,   269,
      13,    14,    15,   125,    82,    83,    84,   216,    85,    86,
      87,    88,    89,    90,    91,    92,    67,   279,   233,   231,
     277,    48,    68,    69,    70,    71,    72,    73,   212,    46,
       0,     0,     0,     0,     0,     0,    74,    47,   213,     0,
      76,    77,    78,    79,    80,    81,    67,     0,     0,     0,
       0,     0,    68,    69,    70,    71,    72,    73,     0,     0,
     176,     0,     0,     0,     0,     0,    74,    47,   271,     0,
      76,    77,    78,    79,    80,    81,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    82,    83,    84,     0,
      85,    86,    87,    88,    89,    90,    91,    92,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,    14,    15,     0,     0,     0,    82,    83,    84,     0,
      85,    86,    87,    88,    89,    90,    91,    92,    67,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
       0,     0,     0,     0,     0,     0,     0,     0,    74,    47,
       0,     0,    76,    77,    78,    79,    80,    81,    67,     0,
       0,     0,     0,     0,    68,    69,    70,    71,    72,    73,
       0,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,    76,   131,    78,    79,    80,    81,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    82,    83,
      84,     0,    85,    86,    87,    88,    89,    90,    91,    92,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     1,    13,    14,    15,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,    14,    15,    67,   234,     0,     0,     0,     0,    68,
      69,    70,    71,    72,    73,    67,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,    73,    76,   131,    78,
      79,    80,    81,     0,     0,    74,     0,     0,     0,    76,
     131,    78,    79,    80,    81,    67,     0,     0,     0,     0,
       0,    68,    69,    70,    71,    72,    73,   150,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,    73,    76,
     131,    78,    79,    80,    81,     0,     0,     0,     0,     0,
       0,    76,   131,    78,    79,    80,    81,   154,     0,     0,
       0,     0,     0,    68,    69,    70,    71,    72,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    76,   131,    78,    79,    80,    81,     1,     0,     0,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     1,    13,    14,    15,   276,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     0,
      13,    14,    15
};

static const yytype_int16 yycheck[] =
{
      67,    76,    58,    82,    39,    80,    81,    82,    18,    19,
      47,   165,   163,     6,    24,   117,    17,    13,    96,    62,
      10,    96,    13,    66,    67,    92,     5,    24,     7,    26,
       1,     8,    28,     5,    28,    42,    29,    47,     5,    29,
      32,    13,    13,   122,    32,     5,    13,   122,    25,    59,
      42,   153,    31,    32,    33,    62,    63,    28,   160,    66,
      67,    28,   164,    55,    65,     6,    22,    55,    28,     8,
      62,    63,    17,    18,    66,    67,    67,    24,     1,   116,
      24,     3,     8,   150,    28,    41,    25,   154,    29,     8,
      13,    36,    37,   244,   161,   162,   152,    23,   220,    21,
      23,   308,    21,    24,   171,    28,   116,   150,   230,   316,
     212,   189,   190,   191,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   187,   207,   134,   285,   289,   238,     4,     8,   206,
       3,     8,     5,   150,     8,    28,   134,   298,   299,     8,
      34,    35,   208,    23,   232,   306,   307,   232,   150,    23,
     148,     4,   313,     4,    23,     8,    24,     8,     4,     4,
     321,     3,     8,     8,    28,   242,    26,     9,    10,    11,
      12,    13,    14,     4,   286,   220,    13,     8,    15,    16,
       6,    23,     8,    21,   296,    27,    28,    29,    30,    31,
      32,     3,   304,     0,     9,    10,    28,    38,    39,   311,
     192,   193,   291,     3,   224,   317,   291,     3,   194,   195,
     322,     3,     3,    23,    23,    12,   227,   283,     9,    10,
      11,    12,    13,    14,   196,   197,   198,   199,   305,   200,
     201,    19,    23,    24,    25,   312,    27,    28,    29,    30,
      31,    32,    20,    26,     3,    52,    40,     4,     8,    29,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    52,    69,    70,    71,     6,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,     4,    69,    70,
      71,    21,    73,    74,    75,    23,    77,    78,    79,    80,
      81,    82,    83,    84,     3,    25,    28,    28,     5,     5,
       9,    10,    11,    12,    13,    14,     6,     4,    78,     3,
       6,     6,     4,    76,    23,    24,    25,     8,    27,    28,
      29,    30,    31,    32,    23,     4,   202,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,   203,    69,
      70,    71,   204,    52,   205,   122,    56,    63,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,   207,
      69,    70,    71,    52,    73,    74,    75,   123,    77,    78,
      79,    80,    81,    82,    83,    84,     3,   227,   150,   148,
     224,    27,     9,    10,    11,    12,    13,    14,   116,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,     3,    -1,    -1,    -1,
      -1,    -1,     9,    10,    11,    12,    13,    14,    -1,    -1,
      26,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    -1,
      27,    28,    29,    30,    31,    32,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    -1,    -1,    73,    74,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,     3,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,    24,
      -1,    -1,    27,    28,    29,    30,    31,    32,     3,    -1,
      -1,    -1,    -1,    -1,     9,    10,    11,    12,    13,    14,
      -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
      -1,    -1,    27,    28,    29,    30,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    52,    69,    70,    71,    -1,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,     3,     4,    -1,    -1,    -1,    -1,     9,
      10,    11,    12,    13,    14,     3,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,    27,    28,    29,
      30,    31,    32,    -1,    -1,    23,    -1,    -1,    -1,    27,
      28,    29,    30,    31,    32,     3,    -1,    -1,    -1,    -1,
      -1,     9,    10,    11,    12,    13,    14,     3,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    27,
      28,    29,    30,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    30,    31,    32,     3,    -1,    -1,
      -1,    -1,    -1,     9,    10,    11,    12,    13,    14,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    27,    28,    29,    30,    31,    32,    52,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    52,    69,    70,    71,    72,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    52,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    69,    70,    71,   107,   108,   111,   112,
     113,   114,   115,   121,   124,   144,   145,   146,   147,    24,
      28,     1,    13,    23,    28,   109,   110,   125,   126,   127,
     108,   108,    24,    28,   108,     0,   145,    24,   137,    28,
     122,   123,    24,   124,   127,   128,     8,    23,    26,     3,
       5,   126,   112,   116,   117,   118,   124,     3,     9,    10,
      11,    12,    13,    14,    23,    25,    27,    28,    29,    30,
      31,    32,    73,    74,    75,    77,    78,    79,    80,    81,
      82,    83,    84,    86,    87,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     105,   107,   108,   135,   136,   137,   138,   139,   140,   141,
     142,   143,    26,     8,    25,   122,   124,   127,     1,   110,
     125,    28,   103,     4,   108,   129,   130,   131,    10,    29,
     118,    25,   117,   119,   120,   125,   118,   105,   118,   132,
       3,    89,     3,    21,     3,    89,    89,    89,   102,   106,
      21,     3,     3,     3,   148,     3,    28,    23,    23,    23,
     105,     5,     7,    31,    32,    33,    26,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,   104,    91,    13,
      15,    16,     9,    10,    34,    35,    17,    18,    36,    37,
      38,    39,    12,    19,    20,    40,    22,    41,     8,    23,
      25,   107,   139,    25,   135,   106,   123,    25,     5,   125,
     127,   133,   134,     4,     8,    29,     6,     8,    23,     4,
     127,   133,     4,   132,     4,    88,   103,   135,    21,   135,
     105,   105,   148,   135,   140,    23,    23,   105,    28,    28,
     103,    91,    91,    91,    92,    92,    93,    93,    94,    94,
      94,    94,    95,    95,    96,    97,    98,    99,   105,   100,
     103,    25,     6,    29,   134,     5,    72,   131,     6,   120,
      91,     4,     4,     8,   135,     4,     4,   105,    78,   148,
       6,    21,     6,     6,    29,   103,   148,   135,     4,     3,
     140,   102,     6,   135,   148,   148,     4,   149,    76,   135,
     105,   148,   148,   149,     4,   135,   105,   148,    23,   149,
     135,     4,   148,   135
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    86,    86,    87,    87,    87,    87,
      87,    87,    87,    87,    88,    88,    89,    89,    89,    89,
      89,    89,    90,    90,    90,    90,    90,    90,    91,    91,
      92,    92,    92,    92,    93,    93,    93,    94,    94,    94,
      95,    95,    95,    95,    95,    96,    96,    96,    97,    97,
      98,    98,    99,    99,   100,   100,   101,   101,   102,   102,
     103,   103,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   105,   105,   106,   107,   107,   108,   108,
     108,   108,   108,   108,   109,   109,   109,   109,   110,   110,
     111,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   114,   115,   115,   116,   116,
     117,   118,   118,   118,   118,   119,   119,   120,   121,   121,
     121,   122,   122,   123,   123,   124,   125,   125,   126,   126,
     126,   126,   126,   127,   127,   127,   127,   128,   128,   129,
     129,   130,   130,   131,   131,   131,   132,   132,   133,   133,
     133,   134,   134,   134,   134,   135,   135,   135,   135,   135,
     135,   136,   136,   136,   137,   137,   137,   137,   138,   138,
     139,   139,   140,   140,   141,   141,   141,   142,   142,   142,
     142,   143,   143,   143,   143,   143,   144,   144,   145,   145,
     146,   147,   148,   149
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     4,     3,     4,
       3,     3,     2,     2,     1,     3,     1,     2,     2,     2,
       2,     4,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     2,     3,     1,     2,
       1,     2,     1,     2,     1,     3,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     2,     1,     1,     1,     2,
       3,     2,     1,     2,     1,     1,     3,     1,     4,     5,
       2,     1,     3,     1,     3,     1,     2,     1,     1,     4,
       5,     4,     3,     1,     2,     2,     3,     1,     2,     1,
       3,     1,     3,     2,     2,     1,     1,     2,     1,     1,
       2,     2,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     3,     4,     3,     2,     3,     3,     4,     1,     2,
       1,     2,     1,     2,     6,    10,     5,     7,     9,     8,
      12,     3,     2,     2,     2,     3,     1,     2,     1,     1,
       2,     2,     0,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 171 "src/grammar.y"
                                { (yyval.expression) = create_primary_identifier((yyvsp[0].identifier)); }
#line 1771 "grammar.tab.c"
    break;

  case 3:
#line 172 "src/grammar.y"
                                { (yyval.expression) = create_primary_constant((yyvsp[0].constant)); }
#line 1777 "grammar.tab.c"
    break;

  case 4:
#line 173 "src/grammar.y"
                                { (yyval.expression) = create_primary_stringliteral((yyvsp[0].string_literal)); }
#line 1783 "grammar.tab.c"
    break;

  case 5:
#line 175 "src/grammar.y"
                                { (yyval.expression) = (yyvsp[-1].expression); }
#line 1789 "grammar.tab.c"
    break;

  case 6:
#line 179 "src/grammar.y"
                                { (yyval.expression) = (yyvsp[0].expression); }
#line 1795 "grammar.tab.c"
    break;

  case 7:
#line 180 "src/grammar.y"
                                                { (yyval.expression) = create_postfix_expr_arr( (yyvsp[-3].expression), (yyvsp[-1].expression) ); }
#line 1801 "grammar.tab.c"
    break;

  case 8:
#line 182 "src/grammar.y"
                                { (yyval.expression) = create_postfix_expr_voidfun( (yyvsp[-2].identifier) ); }
#line 1807 "grammar.tab.c"
    break;

  case 9:
#line 183 "src/grammar.y"
                                                        { (yyval.expression) = create_postfix_expr_fun((yyvsp[-3].identifier), (yyvsp[-1].argument_expression_list)); }
#line 1813 "grammar.tab.c"
    break;

  case 10:
#line 185 "src/grammar.y"
                                                { (yyval.expression) = create_postfix_expr_struct(".", (yyvsp[-2].expression), (yyvsp[0].identifier)); }
#line 1819 "grammar.tab.c"
    break;

  case 11:
#line 186 "src/grammar.y"
                                                { (yyval.expression) = create_postfix_expr_struct("->", (yyvsp[-2].expression), (yyvsp[0].identifier)); }
#line 1825 "grammar.tab.c"
    break;

  case 12:
#line 187 "src/grammar.y"
                                        { (yyval.expression) = create_postfix_expr_ido( (yyvsp[0].terminal), (yyvsp[-1].expression)); }
#line 1831 "grammar.tab.c"
    break;

  case 13:
#line 188 "src/grammar.y"
                                        { (yyval.expression) = create_postfix_expr_ido( (yyvsp[0].terminal), (yyvsp[-1].expression)); }
#line 1837 "grammar.tab.c"
    break;

  case 14:
#line 192 "src/grammar.y"
                                                                { (yyval.argument_expression_list) = create_argument_expr_assignement((yyvsp[0].expression)); }
#line 1843 "grammar.tab.c"
    break;

  case 15:
#line 193 "src/grammar.y"
                                                                { (yyval.argument_expression_list) = create_argument_expr_list((yyvsp[-2].argument_expression_list), (yyvsp[0].expression)); }
#line 1849 "grammar.tab.c"
    break;

  case 16:
#line 197 "src/grammar.y"
                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 1855 "grammar.tab.c"
    break;

  case 17:
#line 198 "src/grammar.y"
                                                { (yyval.expression) = create_unary_expression( (yyvsp[-1].terminal), (yyvsp[0].expression) ); }
#line 1861 "grammar.tab.c"
    break;

  case 18:
#line 199 "src/grammar.y"
                                                { (yyval.expression) = create_unary_expression( (yyvsp[-1].terminal), (yyvsp[0].expression) ); }
#line 1867 "grammar.tab.c"
    break;

  case 19:
#line 200 "src/grammar.y"
                                                { (yyval.expression) = create_unary_expression_cast( (yyvsp[-1].node), (yyvsp[0].expression) ); }
#line 1873 "grammar.tab.c"
    break;

  case 20:
#line 201 "src/grammar.y"
                                                { (yyval.expression) = create_unary_expression( (yyvsp[-1].terminal), (yyvsp[0].expression) ); }
#line 1879 "grammar.tab.c"
    break;

  case 21:
#line 202 "src/grammar.y"
                                                { (yyval.expression) = create_unary_expression( (yyvsp[-3].terminal), (yyvsp[-1].type_name)); }
#line 1885 "grammar.tab.c"
    break;

  case 22:
#line 206 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1891 "grammar.tab.c"
    break;

  case 23:
#line 207 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1897 "grammar.tab.c"
    break;

  case 24:
#line 208 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1903 "grammar.tab.c"
    break;

  case 25:
#line 209 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1909 "grammar.tab.c"
    break;

  case 26:
#line 210 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1915 "grammar.tab.c"
    break;

  case 27:
#line 211 "src/grammar.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 1921 "grammar.tab.c"
    break;

  case 28:
#line 215 "src/grammar.y"
                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 1927 "grammar.tab.c"
    break;

  case 29:
#line 216 "src/grammar.y"
                                                { (yyval.expression) = create_cast_expression_typename((yyvsp[-2].type_name), (yyvsp[0].expression)); }
#line 1933 "grammar.tab.c"
    break;

  case 30:
#line 220 "src/grammar.y"
                                                        { (yyval.expression) = (yyvsp[0].expression); }
#line 1939 "grammar.tab.c"
    break;

  case 31:
#line 221 "src/grammar.y"
                                                        { (yyval.expression) = create_multiplicative_expression("*", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1945 "grammar.tab.c"
    break;

  case 32:
#line 222 "src/grammar.y"
                                                        { (yyval.expression) = create_multiplicative_expression("/", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1951 "grammar.tab.c"
    break;

  case 33:
#line 223 "src/grammar.y"
                                                        { (yyval.expression) = create_multiplicative_expression("%", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1957 "grammar.tab.c"
    break;

  case 34:
#line 227 "src/grammar.y"
                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 1963 "grammar.tab.c"
    break;

  case 35:
#line 228 "src/grammar.y"
                                                                { (yyval.expression) = create_additive_expression("+", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1969 "grammar.tab.c"
    break;

  case 36:
#line 229 "src/grammar.y"
                                                                { (yyval.expression) = create_additive_expression("-", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1975 "grammar.tab.c"
    break;

  case 37:
#line 233 "src/grammar.y"
                                                        { (yyval.expression) = (yyvsp[0].expression); }
#line 1981 "grammar.tab.c"
    break;

  case 38:
#line 234 "src/grammar.y"
                                                        { (yyval.expression) = create_shift_expression(">>", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1987 "grammar.tab.c"
    break;

  case 39:
#line 235 "src/grammar.y"
                                                        { (yyval.expression) = create_shift_expression("<<", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 1993 "grammar.tab.c"
    break;

  case 40:
#line 239 "src/grammar.y"
                                                        { (yyval.expression) = (yyvsp[0].expression); }
#line 1999 "grammar.tab.c"
    break;

  case 41:
#line 240 "src/grammar.y"
                                                        { (yyval.expression) = create_relational_expression("<", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2005 "grammar.tab.c"
    break;

  case 42:
#line 241 "src/grammar.y"
                                                        { (yyval.expression) = create_relational_expression(">", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2011 "grammar.tab.c"
    break;

  case 43:
#line 242 "src/grammar.y"
                                                        { (yyval.expression) = create_relational_expression("<=", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2017 "grammar.tab.c"
    break;

  case 44:
#line 243 "src/grammar.y"
                                                        { (yyval.expression) = create_relational_expression(">=", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2023 "grammar.tab.c"
    break;

  case 45:
#line 247 "src/grammar.y"
                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2029 "grammar.tab.c"
    break;

  case 46:
#line 248 "src/grammar.y"
                                                                { (yyval.expression) = create_equality_expression("==", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2035 "grammar.tab.c"
    break;

  case 47:
#line 249 "src/grammar.y"
                                                                { (yyval.expression) = create_equality_expression("!=", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2041 "grammar.tab.c"
    break;

  case 48:
#line 253 "src/grammar.y"
                                                        { (yyval.expression) = (yyvsp[0].expression); }
#line 2047 "grammar.tab.c"
    break;

  case 49:
#line 254 "src/grammar.y"
                                                        { (yyval.expression) = create_and_expression("&", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2053 "grammar.tab.c"
    break;

  case 50:
#line 258 "src/grammar.y"
                                                        { (yyval.expression) = (yyvsp[0].expression); }
#line 2059 "grammar.tab.c"
    break;

  case 51:
#line 259 "src/grammar.y"
                                                        { (yyval.expression) = create_exclusive_or_expression("^", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2065 "grammar.tab.c"
    break;

  case 52:
#line 263 "src/grammar.y"
                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2071 "grammar.tab.c"
    break;

  case 53:
#line 264 "src/grammar.y"
                                                                { (yyval.expression) = create_inclusive_or_expression("|", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2077 "grammar.tab.c"
    break;

  case 54:
#line 268 "src/grammar.y"
                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2083 "grammar.tab.c"
    break;

  case 55:
#line 269 "src/grammar.y"
                                                                { (yyval.expression) = create_logical_and_expression("&&", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2089 "grammar.tab.c"
    break;

  case 56:
#line 273 "src/grammar.y"
                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2095 "grammar.tab.c"
    break;

  case 57:
#line 274 "src/grammar.y"
                                                                { (yyval.expression) = create_logical_or_expression("||", (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2101 "grammar.tab.c"
    break;

  case 58:
#line 278 "src/grammar.y"
                                                                                { (yyval.expression) = (yyvsp[0].expression); }
#line 2107 "grammar.tab.c"
    break;

  case 59:
#line 279 "src/grammar.y"
                                                                                { (yyval.expression) = create_conditional_expression("?:", (yyvsp[-4].expression), (yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2113 "grammar.tab.c"
    break;

  case 60:
#line 283 "src/grammar.y"
                                                                         { (yyval.expression) = (yyvsp[0].expression); }
#line 2119 "grammar.tab.c"
    break;

  case 61:
#line 284 "src/grammar.y"
                                                                         { (yyval.expression)=create_assignment_expression((yyvsp[-2].expression),(yyvsp[-1].node),(yyvsp[0].expression)); }
#line 2125 "grammar.tab.c"
    break;

  case 62:
#line 288 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].terminal); }
#line 2131 "grammar.tab.c"
    break;

  case 63:
#line 289 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2137 "grammar.tab.c"
    break;

  case 64:
#line 290 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2143 "grammar.tab.c"
    break;

  case 65:
#line 291 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2149 "grammar.tab.c"
    break;

  case 66:
#line 292 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2155 "grammar.tab.c"
    break;

  case 67:
#line 293 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2161 "grammar.tab.c"
    break;

  case 68:
#line 294 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2167 "grammar.tab.c"
    break;

  case 69:
#line 295 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2173 "grammar.tab.c"
    break;

  case 70:
#line 296 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2179 "grammar.tab.c"
    break;

  case 71:
#line 297 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2185 "grammar.tab.c"
    break;

  case 72:
#line 298 "src/grammar.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2191 "grammar.tab.c"
    break;

  case 73:
#line 302 "src/grammar.y"
                                                 { (yyval.expression) = (yyvsp[0].expression); }
#line 2197 "grammar.tab.c"
    break;

  case 74:
#line 303 "src/grammar.y"
                                                 { (yyval.expression) = create_toplevel_expression((yyvsp[-2].expression), (yyvsp[0].expression)); }
#line 2203 "grammar.tab.c"
    break;

  case 75:
#line 308 "src/grammar.y"
                                         { (yyval.node) = (yyvsp[0].expression); }
#line 2209 "grammar.tab.c"
    break;

  case 76:
#line 312 "src/grammar.y"
                                                          { (yyval.declaration) = new_declaration( (yyvsp[-1].declaration_specifiers), NULL );}
#line 2215 "grammar.tab.c"
    break;

  case 77:
#line 313 "src/grammar.y"
                                                          {(yyval.declaration) = new_declaration( (yyvsp[-2].declaration_specifiers), (yyvsp[-1].init_declarator_list) );}
#line 2221 "grammar.tab.c"
    break;

  case 78:
#line 318 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = new_storage_class( (yyvsp[0].value) ); }
#line 2227 "grammar.tab.c"
    break;

  case 79:
#line 319 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = add_storage_class( (yyvsp[0].declaration_specifiers), (yyvsp[-1].value) ); }
#line 2233 "grammar.tab.c"
    break;

  case 80:
#line 320 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = new_type_specifier( (yyvsp[0].type_specifier) ); }
#line 2239 "grammar.tab.c"
    break;

  case 81:
#line 321 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = add_type_specifier( (yyvsp[0].declaration_specifiers), (yyvsp[-1].type_specifier) ); }
#line 2245 "grammar.tab.c"
    break;

  case 82:
#line 322 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = new_type_qualifier ( (yyvsp[0].value) );  }
#line 2251 "grammar.tab.c"
    break;

  case 83:
#line 323 "src/grammar.y"
                                                                 { (yyval.declaration_specifiers) = add_type_qualifier( (yyvsp[0].declaration_specifiers), (yyvsp[-1].value) ); }
#line 2257 "grammar.tab.c"
    break;

  case 84:
#line 327 "src/grammar.y"
                                                         { (yyval.init_declarator_list) = create_init_declarator_list( (yyvsp[0].declarator) ); }
#line 2263 "grammar.tab.c"
    break;

  case 85:
#line 328 "src/grammar.y"
                                                         { (yyval.init_declarator_list) = add_to_init_declarator_list ( (yyvsp[-2].init_declarator_list), (yyvsp[0].declarator) ); }
#line 2269 "grammar.tab.c"
    break;

  case 86:
#line 329 "src/grammar.y"
                                                                { (yyval.init_declarator_list) = NULL; }
#line 2275 "grammar.tab.c"
    break;

  case 87:
#line 330 "src/grammar.y"
                                                                { (yyval.init_declarator_list) = (yyvsp[-2].init_declarator_list); }
#line 2281 "grammar.tab.c"
    break;

  case 88:
#line 334 "src/grammar.y"
                                         { (yyval.declarator) = (yyvsp[0].declarator); }
#line 2287 "grammar.tab.c"
    break;

  case 89:
#line 336 "src/grammar.y"
                                                 { (yyval.declarator) = add_initializer_to_declarator( (yyvsp[-2].declarator), (yyvsp[-1].terminal), (yyvsp[0].expression) ); }
#line 2293 "grammar.tab.c"
    break;

  case 90:
#line 340 "src/grammar.y"
                        { (yyval.value) = TYPEDEF;  }
#line 2299 "grammar.tab.c"
    break;

  case 91:
#line 348 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2305 "grammar.tab.c"
    break;

  case 92:
#line 349 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2311 "grammar.tab.c"
    break;

  case 93:
#line 350 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2317 "grammar.tab.c"
    break;

  case 94:
#line 351 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2323 "grammar.tab.c"
    break;

  case 95:
#line 352 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2329 "grammar.tab.c"
    break;

  case 96:
#line 353 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2335 "grammar.tab.c"
    break;

  case 97:
#line 354 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2341 "grammar.tab.c"
    break;

  case 98:
#line 355 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2347 "grammar.tab.c"
    break;

  case 99:
#line 356 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2353 "grammar.tab.c"
    break;

  case 100:
#line 357 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2359 "grammar.tab.c"
    break;

  case 101:
#line 358 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2365 "grammar.tab.c"
    break;

  case 102:
#line 359 "src/grammar.y"
                                        { (yyval.type_specifier) = (yyvsp[0].type_specifier); }
#line 2371 "grammar.tab.c"
    break;

  case 103:
#line 370 "src/grammar.y"
                                                                { (yyval.type_specifier) = add_struct_declaration( (yyvsp[-3].type_specifier), (yyvsp[-1].struct_declaration_list)); }
#line 2377 "grammar.tab.c"
    break;

  case 104:
#line 371 "src/grammar.y"
                                                                { (yyval.type_specifier) =  (yyvsp[0].type_specifier); }
#line 2383 "grammar.tab.c"
    break;

  case 105:
#line 375 "src/grammar.y"
                                        { (yyval.type_specifier) = create_struct_type( (yyvsp[-1].value), (yyvsp[0].identifier) ); }
#line 2389 "grammar.tab.c"
    break;

  case 106:
#line 380 "src/grammar.y"
                         { (yyval.value) = STRUCT; }
#line 2395 "grammar.tab.c"
    break;

  case 107:
#line 381 "src/grammar.y"
                         { (yyval.value) = UNION; }
#line 2401 "grammar.tab.c"
    break;

  case 108:
#line 385 "src/grammar.y"
                                                        { (yyval.struct_declaration_list) = create_struct_declaration_list ( (yyvsp[0].struct_declaration) ); }
#line 2407 "grammar.tab.c"
    break;

  case 109:
#line 386 "src/grammar.y"
                                                        { (yyval.struct_declaration_list) = add_to_struct_declaration_list ( (yyvsp[-1].struct_declaration_list), (yyvsp[0].struct_declaration) ); }
#line 2413 "grammar.tab.c"
    break;

  case 110:
#line 390 "src/grammar.y"
                                                                 { (yyval.struct_declaration) = create_struct_declaration( (yyvsp[-2].specifier_qualifier_list), (yyvsp[-1].init_declarator_list) ); }
#line 2419 "grammar.tab.c"
    break;

  case 111:
#line 394 "src/grammar.y"
                                                        { (yyval.specifier_qualifier_list) = add_type_specifier_sq( (yyvsp[0].specifier_qualifier_list), (yyvsp[-1].type_specifier) ); }
#line 2425 "grammar.tab.c"
    break;

  case 112:
#line 395 "src/grammar.y"
                                                        { (yyval.specifier_qualifier_list) = create_type_specifier_sq( (yyvsp[0].type_specifier) ); }
#line 2431 "grammar.tab.c"
    break;

  case 113:
#line 396 "src/grammar.y"
                                                        { (yyval.specifier_qualifier_list) = add_type_qualifier_sq( (yyvsp[0].specifier_qualifier_list), (yyvsp[-1].value) ); }
#line 2437 "grammar.tab.c"
    break;

  case 114:
#line 397 "src/grammar.y"
                                                        { (yyval.specifier_qualifier_list) = create_type_qualifier_sq( (yyvsp[0].value) ); }
#line 2443 "grammar.tab.c"
    break;

  case 115:
#line 401 "src/grammar.y"
                                                         { (yyval.init_declarator_list) = create_init_declarator_list ( (yyvsp[0].declarator) ); }
#line 2449 "grammar.tab.c"
    break;

  case 116:
#line 402 "src/grammar.y"
                                                         { (yyval.init_declarator_list) = add_to_init_declarator_list( (yyvsp[-2].init_declarator_list), (yyvsp[0].declarator) ); }
#line 2455 "grammar.tab.c"
    break;

  case 117:
#line 406 "src/grammar.y"
                         { (yyval.declarator) = (yyvsp[0].declarator); }
#line 2461 "grammar.tab.c"
    break;

  case 118:
#line 412 "src/grammar.y"
                                                        { (yyval.type_specifier) = create_type_specifier( ENUM, NULL, (yyvsp[-1].enumerator_list) ); }
#line 2467 "grammar.tab.c"
    break;

  case 119:
#line 413 "src/grammar.y"
                                                        { (yyval.type_specifier) = create_type_specifier( ENUM, (yyvsp[-3].identifier),   (yyvsp[-1].enumerator_list) ); }
#line 2473 "grammar.tab.c"
    break;

  case 120:
#line 414 "src/grammar.y"
                                                        { (yyval.type_specifier) = create_type_specifier( ENUM, (yyvsp[0].identifier), (EnumeratorList *) NULL ); }
#line 2479 "grammar.tab.c"
    break;

  case 121:
#line 418 "src/grammar.y"
                                                 { (yyval.enumerator_list) = create_enumerator_list( (yyvsp[0].enumerator) ); }
#line 2485 "grammar.tab.c"
    break;

  case 122:
#line 419 "src/grammar.y"
                                                 { (yyval.enumerator_list) = add_to_enumerator_list( (yyvsp[-2].enumerator_list), (yyvsp[0].enumerator) ); }
#line 2491 "grammar.tab.c"
    break;

  case 123:
#line 423 "src/grammar.y"
                                                 { (yyval.enumerator) = create_enumerator( (yyvsp[0].identifier), NULL ); }
#line 2497 "grammar.tab.c"
    break;

  case 124:
#line 424 "src/grammar.y"
                                                 { (yyval.enumerator) = create_enumerator( (yyvsp[-2].identifier), (yyvsp[0].node) ); }
#line 2503 "grammar.tab.c"
    break;

  case 125:
#line 428 "src/grammar.y"
                         { (yyval.value) = CONST; }
#line 2509 "grammar.tab.c"
    break;

  case 126:
#line 433 "src/grammar.y"
                                         { (yyval.declarator) = create_declarator( (yyvsp[-1].pointer), (yyvsp[0].direct_declarator) ); }
#line 2515 "grammar.tab.c"
    break;

  case 127:
#line 434 "src/grammar.y"
                                         { (yyval.declarator) = create_declarator( NULL, (yyvsp[0].direct_declarator) ); }
#line 2521 "grammar.tab.c"
    break;

  case 128:
#line 438 "src/grammar.y"
                                                                 { (yyval.direct_declarator) = create_dir_declarator_id( ID , (yyvsp[0].identifier) ); }
#line 2527 "grammar.tab.c"
    break;

  case 129:
#line 441 "src/grammar.y"
                                                                 { (yyval.direct_declarator) = append_dir_declarator_arr( ARRAY, (yyvsp[-3].direct_declarator), (yyvsp[-1].constant) ); }
#line 2533 "grammar.tab.c"
    break;

  case 130:
#line 442 "src/grammar.y"
                                                                 { error_msg("Array dimension must be a positive integer", (yyvsp[-1].constant)->line_num, (yyvsp[-1].constant)->column ); (yyval.direct_declarator)=NULL; }
#line 2539 "grammar.tab.c"
    break;

  case 131:
#line 444 "src/grammar.y"
                                                                 { (yyval.direct_declarator) = append_dir_declarator_fun( FUNCTION, (yyvsp[-3].direct_declarator), (yyvsp[-1].parameter_type_list) ); }
#line 2545 "grammar.tab.c"
    break;

  case 132:
#line 445 "src/grammar.y"
                                                                 { (yyval.direct_declarator) = append_dir_declarator_fun( FUNCTION, (yyvsp[-2].direct_declarator), NULL ); }
#line 2551 "grammar.tab.c"
    break;

  case 133:
#line 449 "src/grammar.y"
                                                { (yyval.pointer) = create_pointer(); }
#line 2557 "grammar.tab.c"
    break;

  case 134:
#line 450 "src/grammar.y"
                                                { (yyval.pointer) = create_pointer( (yyvsp[0].type_qualifier_list), NULL ); }
#line 2563 "grammar.tab.c"
    break;

  case 135:
#line 451 "src/grammar.y"
                                                { (yyval.pointer) = create_pointer( NULL, (yyvsp[0].pointer) ); }
#line 2569 "grammar.tab.c"
    break;

  case 136:
#line 452 "src/grammar.y"
                                                { (yyval.pointer) = create_pointer( (yyvsp[-1].type_qualifier_list), (yyvsp[0].pointer) ); }
#line 2575 "grammar.tab.c"
    break;

  case 137:
#line 456 "src/grammar.y"
                                                { (yyval.type_qualifier_list) = create_type_qualifier_list( (yyvsp[0].value) ); }
#line 2581 "grammar.tab.c"
    break;

  case 138:
#line 457 "src/grammar.y"
                                                { (yyval.type_qualifier_list) = add_to_type_qualifier_list( (yyvsp[-1].type_qualifier_list), (yyvsp[0].value) ); }
#line 2587 "grammar.tab.c"
    break;

  case 139:
#line 461 "src/grammar.y"
                                         { (yyval.parameter_type_list) = (yyvsp[0].parameter_type_list) ; }
#line 2593 "grammar.tab.c"
    break;

  case 140:
#line 462 "src/grammar.y"
                                         { (yyval.parameter_type_list) = add_ellipsis_to_list( (yyvsp[-2].parameter_type_list) ); }
#line 2599 "grammar.tab.c"
    break;

  case 141:
#line 466 "src/grammar.y"
                                                         { (yyval.parameter_type_list) = create_parameter_list( (yyvsp[0].parameter_declaration) ); }
#line 2605 "grammar.tab.c"
    break;

  case 142:
#line 467 "src/grammar.y"
                                                         { (yyval.parameter_type_list) = add_to_parameter_list( (yyvsp[-2].parameter_type_list), (yyvsp[0].parameter_declaration) ); }
#line 2611 "grammar.tab.c"
    break;

  case 143:
#line 471 "src/grammar.y"
                                                        { (yyval.parameter_declaration) = create_parameter_declaration( (yyvsp[-1].declaration_specifiers), (yyvsp[0].declarator), NULL ); }
#line 2617 "grammar.tab.c"
    break;

  case 144:
#line 472 "src/grammar.y"
                                                        { (yyval.parameter_declaration) = create_parameter_declaration( (yyvsp[-1].declaration_specifiers), NULL, (yyvsp[0].abstract_declarator) ); }
#line 2623 "grammar.tab.c"
    break;

  case 145:
#line 473 "src/grammar.y"
                                                        { (yyval.parameter_declaration) = create_parameter_declaration( (yyvsp[0].declaration_specifiers), NULL, NULL ); }
#line 2629 "grammar.tab.c"
    break;

  case 146:
#line 482 "src/grammar.y"
                                                        { (yyval.type_name) = create_type_name( (yyvsp[0].specifier_qualifier_list), NULL); }
#line 2635 "grammar.tab.c"
    break;

  case 147:
#line 483 "src/grammar.y"
                                                        { (yyval.type_name) = create_type_name( (yyvsp[-1].specifier_qualifier_list), (yyvsp[0].abstract_declarator)); }
#line 2641 "grammar.tab.c"
    break;

  case 148:
#line 487 "src/grammar.y"
                                                { (yyval.abstract_declarator) = create_abstract_declarator( (yyvsp[0].pointer), NULL ); }
#line 2647 "grammar.tab.c"
    break;

  case 149:
#line 488 "src/grammar.y"
                                                { (yyval.abstract_declarator) = create_abstract_declarator( NULL, (yyvsp[0].direct_abstract_declarator) ); }
#line 2653 "grammar.tab.c"
    break;

  case 150:
#line 489 "src/grammar.y"
                                                { (yyval.abstract_declarator) = create_abstract_declarator( (yyvsp[-1].pointer), (yyvsp[0].direct_abstract_declarator) ); }
#line 2659 "grammar.tab.c"
    break;

  case 151:
#line 493 "src/grammar.y"
                                                                        { (yyval.direct_abstract_declarator) = create_direct_abstract_declarator( NULL ); }
#line 2665 "grammar.tab.c"
    break;

  case 152:
#line 494 "src/grammar.y"
                                                                        { (yyval.direct_abstract_declarator) = create_direct_abstract_declarator( (yyvsp[-1].constant) ); }
#line 2671 "grammar.tab.c"
    break;

  case 153:
#line 495 "src/grammar.y"
                                                                        { (yyval.direct_abstract_declarator) = append_direct_abstract_declarator( (yyvsp[-2].direct_abstract_declarator), NULL ); }
#line 2677 "grammar.tab.c"
    break;

  case 154:
#line 496 "src/grammar.y"
                                                                        { (yyval.direct_abstract_declarator) = append_direct_abstract_declarator( (yyvsp[-3].direct_abstract_declarator), (yyvsp[-1].constant) ); }
#line 2683 "grammar.tab.c"
    break;

  case 155:
#line 518 "src/grammar.y"
                                 { (yyval.statement) = (yyvsp[0].node); }
#line 2689 "grammar.tab.c"
    break;

  case 156:
#line 519 "src/grammar.y"
                                 { (yyval.statement) = (yyvsp[0].node); }
#line 2695 "grammar.tab.c"
    break;

  case 157:
#line 520 "src/grammar.y"
                                 { (yyval.statement) = create_expression_statement((yyvsp[0].expression)); }
#line 2701 "grammar.tab.c"
    break;

  case 158:
#line 521 "src/grammar.y"
                                 { (yyval.statement) = (yyvsp[0].node); }
#line 2707 "grammar.tab.c"
    break;

  case 159:
#line 522 "src/grammar.y"
                                 { (yyval.statement) = (yyvsp[0].node); }
#line 2713 "grammar.tab.c"
    break;

  case 160:
#line 523 "src/grammar.y"
                                         { (yyval.statement) = (yyvsp[0].node); }
#line 2719 "grammar.tab.c"
    break;

  case 161:
#line 527 "src/grammar.y"
                                         { (yyval.node) = create_non_term("labeled_statement", (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2725 "grammar.tab.c"
    break;

  case 162:
#line 528 "src/grammar.y"
                                                         { (yyval.node) = create_non_term("CASE", (yyvsp[-2].node), (yyvsp[0].statement)); }
#line 2731 "grammar.tab.c"
    break;

  case 163:
#line 529 "src/grammar.y"
                                         { (yyval.node) = create_non_term("DEFAULT", (yyvsp[-1].node)); }
#line 2737 "grammar.tab.c"
    break;

  case 164:
#line 533 "src/grammar.y"
                                                        { (yyval.node) = NULL; }
#line 2743 "grammar.tab.c"
    break;

  case 165:
#line 534 "src/grammar.y"
                                                        { (yyval.node) = create_non_term("compound_statement", (yyvsp[-1].node)); }
#line 2749 "grammar.tab.c"
    break;

  case 166:
#line 535 "src/grammar.y"
                                                        { local_symbol_table.clear_current_level(); (yyval.node) = create_non_term("compound_statement", (yyvsp[-1].declaration_list)); }
#line 2755 "grammar.tab.c"
    break;

  case 167:
#line 536 "src/grammar.y"
                                                        { local_symbol_table.clear_current_level(); (yyval.node) = create_non_term("compound_statement", (yyvsp[-2].declaration_list), (yyvsp[-1].node)); }
#line 2761 "grammar.tab.c"
    break;

  case 168:
#line 540 "src/grammar.y"
                                         { (yyval.declaration_list) = create_declaration_list( (yyvsp[0].declaration) ); }
#line 2767 "grammar.tab.c"
    break;

  case 169:
#line 541 "src/grammar.y"
                                         { (yyval.declaration_list) = add_to_declaration_list( (yyvsp[-1].declaration_list), (yyvsp[0].declaration) );}
#line 2773 "grammar.tab.c"
    break;

  case 170:
#line 545 "src/grammar.y"
                                         { (yyval.node) = (yyvsp[0].statement); }
#line 2779 "grammar.tab.c"
    break;

  case 171:
#line 546 "src/grammar.y"
                                         { (yyval.node) = create_non_term("statement_list", (yyvsp[-1].node), (yyvsp[0].statement)); }
#line 2785 "grammar.tab.c"
    break;

  case 172:
#line 551 "src/grammar.y"
                                         { (yyval.expression) = NULL; }
#line 2791 "grammar.tab.c"
    break;

  case 173:
#line 552 "src/grammar.y"
                                 { (yyval.expression) = (yyvsp[-1].expression); }
#line 2797 "grammar.tab.c"
    break;

  case 174:
#line 556 "src/grammar.y"
                                                                                { (yyval.node) = create_selection_statement_if( (yyvsp[-3].expression), (yyvsp[-1].label), (yyvsp[0].statement), NULL, NULL, NULL);}
#line 2803 "grammar.tab.c"
    break;

  case 175:
#line 557 "src/grammar.y"
                                                                                        { (yyval.node) = create_selection_statement_if( (yyvsp[-7].expression), (yyvsp[-5].label), (yyvsp[-4].statement), (yyvsp[-2]._goto), (yyvsp[-1].label), (yyvsp[0].statement) );}
#line 2809 "grammar.tab.c"
    break;

  case 176:
#line 558 "src/grammar.y"
                                                                                { (yyval.node) = create_non_term("SWITCH", (yyvsp[-2].expression), (yyvsp[0].statement)); }
#line 2815 "grammar.tab.c"
    break;

  case 177:
#line 562 "src/grammar.y"
                                                                 { (yyval.node) = create_iteration_statement_while( (yyvsp[-4].label), (yyvsp[-3].expression), (yyvsp[-1].label), (yyvsp[0].statement) ); }
#line 2821 "grammar.tab.c"
    break;

  case 178:
#line 563 "src/grammar.y"
                                                                         { (yyval.node) = create_iteration_statement_do_while("DO WHILE", (yyvsp[-7].label),(yyvsp[-6].statement),(yyvsp[-4].node),(yyvsp[-3].label)); }
#line 2827 "grammar.tab.c"
    break;

  case 179:
#line 564 "src/grammar.y"
                                                                                                 { (yyval.node) = create_iteration_statement_for( (yyvsp[-5].expression), (yyvsp[-4].label), (yyvsp[-3].expression), (yyvsp[-1].label), (yyvsp[0].statement) ); }
#line 2833 "grammar.tab.c"
    break;

  case 180:
#line 565 "src/grammar.y"
                                                                                                                                 { (yyval.node) = create_iteration_statement_for( (yyvsp[-9].expression), (yyvsp[-8].label), (yyvsp[-7].expression), (yyvsp[-6]._goto), (yyvsp[-5].label), (yyvsp[-4].expression), (yyvsp[-3]._goto), (yyvsp[-1].label), (yyvsp[0].statement) ); }
#line 2839 "grammar.tab.c"
    break;

  case 181:
#line 569 "src/grammar.y"
                                         { (yyval.node) = create_non_term("GOTO", (yyvsp[-1].identifier)); }
#line 2845 "grammar.tab.c"
    break;

  case 182:
#line 570 "src/grammar.y"
                                         { (yyval.node) = create_non_term("CONTINUE"); }
#line 2851 "grammar.tab.c"
    break;

  case 183:
#line 571 "src/grammar.y"
                                         { (yyval.node) = create_non_term("BREAK"); }
#line 2857 "grammar.tab.c"
    break;

  case 184:
#line 572 "src/grammar.y"
                                         { (yyval.node) = create_non_term("RETURN"); }
#line 2863 "grammar.tab.c"
    break;

  case 185:
#line 573 "src/grammar.y"
                                         { (yyval.node) = create_non_term("RETURN", (yyvsp[-1].expression)); }
#line 2869 "grammar.tab.c"
    break;

  case 186:
#line 577 "src/grammar.y"
                                 { root->add_child(create_non_term("external_declaration", (yyvsp[0].node))); }
#line 2875 "grammar.tab.c"
    break;

  case 187:
#line 578 "src/grammar.y"
                                                         { root->add_child(create_non_term("external_declaration", (yyvsp[0].node))); }
#line 2881 "grammar.tab.c"
    break;

  case 188:
#line 582 "src/grammar.y"
                                 { (yyval.node) = (yyvsp[0].function_definition); }
#line 2887 "grammar.tab.c"
    break;

  case 189:
#line 583 "src/grammar.y"
                         { (yyval.node) = add_to_global_symbol_table( (yyvsp[0].declaration) ); }
#line 2893 "grammar.tab.c"
    break;

  case 190:
#line 587 "src/grammar.y"
                                            { (yyval.function_definition) = create_function_defintion((yyvsp[-1].declaration_specifiers), (yyvsp[0].declarator), NULL); }
#line 2899 "grammar.tab.c"
    break;

  case 191:
#line 591 "src/grammar.y"
                                                         { (yyval.function_definition) = add_stmt_to_function_definition( (yyvsp[-1].function_definition), (yyvsp[0].node) ); }
#line 2905 "grammar.tab.c"
    break;

  case 192:
#line 596 "src/grammar.y"
                 { (yyval.label) = create_new_label(); }
#line 2911 "grammar.tab.c"
    break;

  case 193:
#line 597 "src/grammar.y"
                 { (yyval._goto) = create_new_goto(); }
#line 2917 "grammar.tab.c"
    break;


#line 2921 "grammar.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 600 "src/grammar.y"

#include <stdio.h>

extern char yytext[];
extern unsigned int column;
extern std::stringstream text;

void yyerror(const char *s)
//char *s;
{
	fflush(stdout);
	printf("%d:%d ERROR : %s\n",line_num,column,s);
	std::cout << text.str(); 
	printf("\n%*s\n", column, "^");
}
