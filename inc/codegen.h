#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include<3ac.h>
#include<vector>
#include<set>

#define NUM_ARCH_REGS 10



class Address;


typedef enum ARCH_REG_ {
	tINV = -1,
	t0 = 0,
	t1 = 1,
	t2 = 2,
	t3 = 3,
	t4 = 4,
	t5 = 5,
	t6 = 6,
	t7 = 7,
	t8 = 8,
	t9 = 9,
	a0 = 10,
	a1 = 11,
	a2 = 12,
	a3 = 13
} ARCH_REG;

typedef enum _registers {
        rINV = 0,       //Invalid      
        GP = 1,         //GlobalPointer
        FP = 2,         //FramePointer 
        SP = 3          //StackPointer 

} OFFSET_REGISTER;


class MemManUnit;

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
	friend class MemManUnit;
public:
	MemoryLocation();
};
	
MemoryLocation create_memory_location( unsigned int id, long offset );
MemoryLocation create_temp_mem_location(unsigned int id,  ARCH_REG reg );
void set_is_ea( unsigned int id );

class MemManUnit {
public:
	ARCH_REG start_issue;
	std::vector<ARCH_REG> temp_stack;
	std::vector<unsigned int> reg_alloc_info;
	std::set<unsigned int> live_vals;
	std::map<unsigned int, MemoryLocation > memory_locations;

	MemManUnit();
	ARCH_REG get_reg( Address* a, int mem_valid /* 0 -> false, 1->true, 2->dont't change */ , bool load );
	ARCH_REG get_empty_reg( );
	void free_reg ( ARCH_REG reg );
	void store_and_free_reg ( ARCH_REG reg );
	void set_reg ( unsigned int table_id, ARCH_REG reg );
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


void gen_asm_code();
void issue_load( ARCH_REG r, OFFSET_REGISTER base, long offset );
void issue_store( ARCH_REG r, OFFSET_REGISTER base, long offset );


#endif
