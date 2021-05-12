#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include<3ac.h>
#include<vector>
#include<set>

#define NUM_ARCH_REGS 20
#define NUM_TEMP_REGS 10
#define NUM_REG_ARGS 4



class Address;
typedef struct _addresses ADDRESS;


typedef enum ARCH_REG_ {
	tINV = -1,
	zz = 0,
	at = 1,
	v0 = 2,
	v1 = 3,
	a0 = 4,
	a1 = 5,
	a2 = 6,
	a3 = 7,
	t0 = 8,
	t1 = 9,
	t2 = 10,
	t3 = 11,
	t4 = 12,
	t5 = 13,
	t6 = 14,
	t7 = 15,
	t8 = 16, //??
	t9 = 17,
} ARCH_REG;

std::ostream& operator<<( std::ostream &os, ARCH_REG & a);

typedef enum _registers {
        rINV = 0,       //Invalid      
        GP = 1,         //GlobalPointer
        FP = 2,         //FramePointer 
        SP = 3          //StackPointer 

} OFFSET_REGISTER;


class MemManUnit;
class Arg;

class MemoryLocation {
	unsigned int id;
	ARCH_REG reg;
	bool in_mem;
	OFFSET_REGISTER base_reg;
	long offset;
	bool is_ea;     // Should this be treated as an effective address?

			// if the value is set to true, a load will not be issued
			// for (base_reg + offset) otherwise a load will be issued

			// set to true for arrays, structs etc.
			// set to false otherwise

	friend MemoryLocation create_memory_location( unsigned int id, long offset );
	friend MemoryLocation create_temp_mem_location(unsigned int id,  ARCH_REG reg );
	friend void set_is_ea( unsigned int id );
	friend void process_arg( Arg * a);
	friend void issue_load_ea( ARCH_REG r, MemoryLocation & ml );
	friend void issue_load_ea( ARCH_REG r, ADDRESS & src );
	friend class MemManUnit;
public:
	MemoryLocation();
};
	
MemoryLocation create_memory_location( unsigned int id, long offset );
MemoryLocation create_temp_mem_location(unsigned int id,  ARCH_REG reg );
void set_is_ea( unsigned int id );

class MemManUnit {
public:
	int start_issue;
	std::vector<ARCH_REG> temp_stack;
	std::vector<unsigned int> reg_alloc_info;
	std::set<unsigned int> live_vals;
	std::map<unsigned int, MemoryLocation > memory_locations;

	MemManUnit();
	ARCH_REG get_reg(ARCH_REG dest, Address* a, int mem_valid /* 0 -> false, 1->true, 2->dont't change */ , bool load );
	ARCH_REG get_empty_reg( );
	void free_reg ( ARCH_REG reg );
	void store_and_free_reg ( ARCH_REG reg );
	void set_reg ( unsigned int table_id, ARCH_REG reg );
	void reset();
};

extern MemManUnit mmu;

typedef enum OP_CODE_ {
	INV = -1,
	LD,
	ST,
	ADD

} OP_CODE;

class AsmInstr {

	//Fill this class in
	std::string name;
	ARCH_REG dest;
	ARCH_REG src1;
	ARCH_REG src2;
	uint16_t immm;
	OP_CODE  opc;
};


extern std::vector< AsmInstr > asm_code;


void gen_asm_code( );
void issue_load( ARCH_REG r, OFFSET_REGISTER base, long offset );
void issue_load_ea( ARCH_REG r, MemoryLocation & ml );
void issue_store( ARCH_REG r, OFFSET_REGISTER base, long offset );



class Quad;
class Return;
class Label;
class Call;
class Arg;
class SaveLive;
class GoTo;

void process_quad( Quad * q );
void process_label( Label * l );
void process_goto( GoTo * g );
void process_call( Call * c );
void process_arg( Arg * a );
void process_return( Return * r );
void process_save_live( SaveLive * s );

void gen_asm_instr(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2);
void gen_asm_instr_imm(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2);
void gen_asm_instr_limm(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2);
void gen_asm_instr(std::string operation, ADDRESS & result, ADDRESS & arg1);
void gen_asm_instr_imm(std::string operation, ADDRESS & result, ADDRESS & arg1);

void gen_epilogue();
void gen_prologue( );

#endif
