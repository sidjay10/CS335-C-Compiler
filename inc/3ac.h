#ifndef __3AC_H__
#define __3AC_H__

#include <ast.h>
#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <symtab.h>
#include <codegen.h>
#include <unordered_set>

extern unsigned long long instructions;
unsigned long long get_next_instr();



extern unsigned long long temporaries;

typedef enum ADD_TYPE_ {
	ID3 = 1,
	CON = 2,
	TEMP = 3,
	MEM = 4
}ADD_TYPE;


class ThreeAC;

#define TEMP_ID_MASK 0x80000000

class Address {
public:
	std::string name;
//	long value;
	ADD_TYPE type;
	ThreeAC * ta_instr;
	unsigned int table_id;
	Address( std::string name, ADD_TYPE type);
	Address( long value, ADD_TYPE type);
	
};

typedef struct _addresses {
	Address * addr;
	bool alive;
	struct _addresses * next_use;
} ADDRESS;

class TacInfo {
	private:
		bool alive;
		ADDRESS * next_use;
		SymTabEntry * symbol;
	public:
		TacInfo();
		TacInfo(SymTabEntry * );
		TacInfo( bool );
	friend TacInfo * create_tac_info(SymTabEntry * symbol);
	friend void reset_tac_info_table();
	friend void create_next_use_info();
};

TacInfo * create_tac_info(SymTabEntry * symbol);
TacInfo * create_tac_info(SymTabEntry * symbol, bool live, ThreeAC * next_use);

extern std::map< unsigned int, TacInfo > tac_info_table;
std::map< unsigned int, TacInfo >::iterator get_entry_from_table( Address * );

std::ostream& operator<<(std::ostream& os, const Address& a);

extern unsigned long long instructions;
Address * new_temp();
Address * new_mem();


typedef enum _const_type {
	INT3 = 1,
	FLOAT3
} CONST_TYPE;

template <typename T>
Address * new_3const(T val, CONST_TYPE con ) {
	return new Address(std::to_string(val), CON);
}


Address * new_3id(SymTabEntry * symbol);

class ThreeAC {
public:
	unsigned int instr;
	unsigned int bb_no;
	bool dead;
	ThreeAC();
	ThreeAC( bool no_add );
	virtual std::string print() = 0;
	virtual ~ThreeAC();

};

extern std::vector< ThreeAC * > ta_code;


class Quad : public ThreeAC {
public:

	ADDRESS result, arg1, arg2;

//	Address * result;
	std::string operation;
//	Address * arg1;
//	Address * arg2;
//	Quad * next;


	Quad ( Address * result, std::string operation, Address * arg1, Address * arg2 );
	std::string print();
	~Quad();

};


std::ostream& operator<<(std::ostream& os, const ADDRESS & a);

#define MEM_EMIT(a,b) \
	if ( (a)->res->type == MEM ) { \
		(b) = new_temp(); \
		emit((b),"()",(a)->res,nullptr); \
	} else if ( (a)->res->type == ID3 ) { \
		(b) = (a)->res; \
	} else { \
		(b) = (a)->res; \
	}

#define BACKPATCH(a) { \
	Label * _lab; 			\
	if ( (a)->falselist.size() != 0 || (a)->truelist.size() != 0 ) { \
		_lab = create_new_label(); \
	} \
	backpatch((a)->falselist,_lab); \
	backpatch((a)->truelist,_lab); \
}

#define SAVE_REGS( E, t1, t2 ) { \
    if ( (t1)->type == TEMP || (t1)->type == MEM ) { \
        (E)->res = (t1); \
	(E)->res->type = TEMP; \
    } else if ( (t2)->type == TEMP || (t2)->type == MEM ){ \
        (E)->res = (t2); \
	(E)->res->type = TEMP; \
    } else { \
        (E)->res = new_temp(); \
    } \
}

std::ostream& operator<<(std::ostream& os, const Quad& q);

unsigned long long emit(  Address * result, std::string operation, Address * arg1, Address * arg2 );


class GoTo;

class Label : public ThreeAC {
	public:
		std::string name;
		unsigned long long instruction_id;
		unsigned int reference_count;
		Label();
		std::string print();
		~Label();
	//	void update_targets( Label * label );
};

extern unsigned long long labels;
Label * create_new_label();
 
class GoTo : public ThreeAC {
		Label * label;
	public : 
		ADDRESS res;
		bool condition;
		GoTo();
		std::string print();
		~GoTo();
		void set_res( Address * res );
	friend std::ostream& operator<<(std::ostream& os, const GoTo& g);
	friend GoTo * create_new_goto_cond( Address * res, bool condition );
	friend void optimise_pass1();
	friend void backpatch(std::vector<GoTo*> & go_v, Label* label);
	friend void backpatch(GoTo* _goto, Label* label);
	friend GoTo * create_new_goto( Label * label);
	friend void process_goto( GoTo * g );
//	friend void Label::update_targets( Label * label );

};



GoTo * create_new_goto();
GoTo * create_new_goto(Label * label);
GoTo * create_new_goto_cond(Address * res, bool condition);


std::ostream& operator<<(std::ostream& os, const Label& l);

std::ostream& operator<<(std::ostream& os, const GoTo& g);

class Arg : public ThreeAC {
	public:
		ADDRESS arg;
		int num;
		
		Arg( Address * _addr, int count );
		std::string print();
		
};

std::ostream& operator<<(std::ostream& os, const Arg& a );
Arg * create_new_arg( Address * addr , int count );

class Call : public ThreeAC {
	public:
		ADDRESS retval;
		std::string function_name;
		
		Call( Address * _addr, std::string f_name );
		std::string print();
		
};

std::ostream& operator<<(std::ostream& os, const Call& c);
Call * create_new_call( Address * addr , std::string f_name );

class Return : public ThreeAC {
	public : 
		ADDRESS retval;
		Return( Address * _retval );
		std::string print();
		~Return();
	friend Return * create_new_return( Address * retval );
};

Return * create_new_return(Address * retval);

std::ostream& operator<<(std::ostream& os, const Return& r);


class SaveLive : public ThreeAC {
	public:
		bool save_temps;
		SaveLive();
		std::string print();
};

SaveLive * create_new_save_live();
std::ostream& operator<<(std::ostream& os, const SaveLive& s);

extern std::unordered_set<std::string> var_rep;
void backpatch( std::vector <GoTo * > & go_v, Label * label );
void backpatch(GoTo* _goto, Label* label);

// Append v2 at the end of vector 1
void append ( std::vector <GoTo *> & v1, std::vector <GoTo *> & v2);

void dump_and_reset_3ac(); 
void optimise_pass1();
void create_basic_blocks();
void create_next_use_info();
void reset_tac_info_table();
#endif
