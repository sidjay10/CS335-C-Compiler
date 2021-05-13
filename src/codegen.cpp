#include<codegen.h>
#include<assert.h>

std::vector<AsmInstr> asm_code;

MemManUnit mmu;

MemoryLocation::MemoryLocation() : id(0), name(""), reg(tINV), in_mem(true), size(0), base_reg(rINV), offset(0), is_ea(false) {}  

MemoryLocation create_memory_location(std::string name, unsigned int id,  long offset, int size ){
	MemoryLocation ml;
	ml.id = id;
	ml.size = size;
	ml.name = name;

//XXX : Check this logic
	if ( id & TEMP_ID_MASK  ) {
		// Should never reach here;
		assert(0);
	} else if ( id & GLOBAL_SYM_MASK || id & STRING_MASK ) {
		ml.base_reg = GP;
		ml.offset = offset;
	} else if ( id & FUN_ARG_MASK ) {
		int arg_no = id & 0x0fffffff;
		if ( arg_no == 0 ) {
			ml.reg = a0;
			ml.in_mem = false;
		} else if ( arg_no == 1 ) {
			ml.reg = a1;
			ml.in_mem = false;
		} else if ( arg_no == 2 ) {
			ml.reg = a2;
			ml.in_mem = false;
		} else if ( arg_no == 3 ) {
			ml.reg = a3;
			ml.in_mem = false;
		} else {
			ml.base_reg = FP;
			//FIXME : Verify offset value
			//ml.offset = -offset;
			ml.offset = 8 + 4*(arg_no - 4);
		}
	} else if ( id & LOCAL_SYM_MASK ){
		ml.base_reg = FP;
		ml.offset = -local_symbol_table.reqd_size + offset;
	} else {
		assert(0);
	}
	return ml;
}

MemoryLocation create_temp_mem_location(std::string name, unsigned int id,  ARCH_REG reg , int size ){
	MemoryLocation ml;
	ml.id = id;
	ml.size = size;
	ml.name = name;

//XXX : Check this logic
	if ( id & TEMP_ID_MASK  ) {
		ml.reg = reg;

	} else {
		assert(0);
	}
	return ml;
}

void set_is_ea( unsigned int id ){
	auto it = mmu.memory_locations.find( id );
	assert( it != mmu.memory_locations.end() );
	it->second.is_ea = true;
}


MemManUnit::MemManUnit() : start_issue(0), reg_alloc_info(std::vector<unsigned int>(NUM_ARCH_REGS, 0)) {} 

ARCH_REG MemManUnit::get_reg( ARCH_REG dest, Address * a , int mem_valid , bool load ) {
	auto it = memory_locations.find( a->table_id );
	if ( it != memory_locations.end() ) {
		if ( it->second.reg != tINV ) {
			if ( mem_valid == 1 ) {
				it->second.in_mem = true;
			} else if ( mem_valid == 0 ) {
				it->second.in_mem = false;
			}
			return it->second.reg;
		}

		ARCH_REG r;
		if ( dest == tINV || dest > t9 || dest < t0 ) {
			r = get_empty_reg();
		} else {
			r = dest;
		}
		assert( r != tINV && r <= t9  && r >= t0 );
		reg_alloc_info[r - t0 ] = a->table_id;
		it->second.reg = r;
		if ( load && it->second.in_mem && !it->second.is_ea ) {
			issue_load(r, it->second );
		} else if ( load && it->second.is_ea ) {
			issue_load_ea(r, it->second );
		} else if ( load ) {
			std::cerr << "PANIC : Loading value not in memory\n";
			assert(0);
		} 
		if ( mem_valid == 1 ) {
			it->second.in_mem = true;
		} else if ( mem_valid == 0 ) {
			it->second.in_mem = false;
		}
		return r;
		
	} else if ( a->table_id & TEMP_ID_MASK ) {
		ARCH_REG r;
		if ( dest == tINV || dest > t9  || dest < t0) {
			r = get_empty_reg();
		} else {
			r = dest;
		}
		assert( r != tINV && r <= t9 && r >= t0 );
		reg_alloc_info[r - t0] = a->table_id;
		memory_locations.insert({a->table_id,create_temp_mem_location(a->name, a->table_id, r, a->size )});
		return r;
	} else {
		std::cerr << "PANIC: Memory Location " << a->name << " not found\n";
		assert(0);
	}
	return tINV;
}

ARCH_REG MemManUnit::get_empty_reg() {
	for ( int r = 0; r < NUM_TEMP_REGS; r++ ) {
		if ( reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ] == 0 ) {
			ARCH_REG empty =  static_cast<ARCH_REG> ( t0 + ( (start_issue + r ) % NUM_TEMP_REGS ) );
			start_issue =  (start_issue + r + 1 ) % NUM_TEMP_REGS;
			return empty;
		}
	}
	//Try to store variables first;
	for ( int r = 0; r < NUM_TEMP_REGS; r++ ) {
		if ( ( reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ] & TEMP_ID_MASK )  == 0 ) {
			unsigned int table_id = reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ]; 
			auto it = memory_locations.find( table_id );
			assert( it != memory_locations.end() );
			//Register spill
			issue_store( it->second.reg, it->second );
			it->second.reg = tINV;
			it->second.in_mem = true;
			ARCH_REG empty =  static_cast<ARCH_REG> ( t0 +( (start_issue + r ) % NUM_TEMP_REGS) );
			start_issue =  (start_issue + r + 1 ) % NUM_TEMP_REGS;
			return empty;
		}
	}
	//If no varaible can be freed, spill a temporary to the stack
	for ( int r = 0; r < NUM_TEMP_REGS; r++ ) {
		if ( ( reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ] & TEMP_ID_MASK )  != 0 ) {
			unsigned int table_id = reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ]; 
			auto it = memory_locations.find( table_id );
			assert( it != memory_locations.end() );
			//Register spill
			ARCH_REG empty =  static_cast<ARCH_REG> ( t0 +( (start_issue + r ) % NUM_TEMP_REGS) );
			stack_size += 4;
			std::stringstream ss;
			ss << "ASM: \t" << "addiu $sp, $sp, -4\n"; 
			std::cout << ss.str();
			it->second.base_reg = FP;
			it->second.offset = stack_size;
			it->second.size = WORD_SIZE;
			issue_store( empty, it->second );
			it->second.reg = tINV;
			it->second.in_mem = true;
			start_issue =  (start_issue + r + 1 ) % NUM_TEMP_REGS;
			return empty;
		}
	}

	std::cerr << "PANIC: Unable to find empty register\n";
	assert(0);

	return tINV;

}

void MemManUnit::free_reg( ARCH_REG r ) {
	assert ( r != tINV );
	if ( r < t0 || r > t9 ){
		return;
	}
	unsigned int id = reg_alloc_info[r - t0];
	auto it = memory_locations.find( id ) ;
	assert ( it != memory_locations.end() );
	it->second.reg = tINV;
	reg_alloc_info[ r - t0 ] = 0;
}

void MemManUnit::store_and_free_reg( ARCH_REG r ) {
	assert ( r != tINV );
	if ( r < t0 || r > t9 ){
		return;
	}
	unsigned int id = reg_alloc_info[ r - t0 ];
	auto it = memory_locations.find( id ) ;
	assert ( it != memory_locations.end() );
	it->second.reg = tINV;
	if ( it->second.in_mem != true ) {
		issue_store( r, it->second );
	}
	it->second.in_mem = true;
	reg_alloc_info[r - t0] = 0;
}

void MemManUnit::reset() {
	start_issue = 0;
	temp_stack.clear();
	std::fill(reg_alloc_info.begin(), reg_alloc_info.end(), 0);
	live_vals.clear();
	memory_locations.clear();
	globals.clear();
	strings.clear();
	stack_size = 0;
}


#if 0
void MemManUnit::set_reg ( unsigned int table_id, ARCH_REG r ) {
	if ( r >= t0 && r  <= NUM_TEMP_REGS ) {
		reg_alloc_info[r] = table_id;
	}
}
#endif


void issue_load( ARCH_REG r, MemoryLocation & ml ) {
	//TODO: Implement different sizes
	//TODO: Implement different offset sizes
	std::stringstream ss;
	std::string load;
	if ( ml.size == 1 ) {
		load = "lb";
	} else if (ml.size == WORD_SIZE ) {
		load = "lw";
	} else {
		assert(0);
	}
	if ( ml.base_reg == FP ) {
		ss << "ASM: \t" << load << " " << r << ", " <<  ml.offset << "($fp)\n"; 
	} else if ( ml.base_reg == GP ) {
		ss << "ASM: \t" << load << " " << r << ", " << ml.name << "\n";
	} else {
		 assert(0);
	}
	std::cout << ss.str();
}
void issue_load_ea( ARCH_REG r, MemoryLocation & ml ) {
	//TODO: Implement different sizes
	//TODO: Implement different offset sizes
	std::stringstream ss;
	if ( ml.base_reg == FP ) {
		ss << "ASM: \t" << "addiu " << r << ", $fp, " << ml.offset << "\n"; 
	} else if ( ml.base_reg == GP ) {
		ss << "ASM: \t" << "la " << r << ", " << ml.name << "\n";
	}
	std::cout << ss.str();
}

void issue_load_ea( ARCH_REG r, ADDRESS & src ) {
	//TODO: Implement different sizes
	//TODO: Implement different offset sizes
	auto it = mmu.memory_locations.find(src.addr->table_id);
	assert( it != mmu.memory_locations.end() );
	MemoryLocation & ml = it->second;
	std::stringstream ss;
	if ( ml.base_reg == FP ) {
		ss << "ASM: \t" << "addiu " << r << ", $fp " << ml.offset << "\n"; 
	} else if ( ml.base_reg == GP ) {
		ss << "ASM: \t" << "la " << r << ", " << ml.name << "\n";
	}
	std::cout << ss.str();
}

void issue_store( ARCH_REG r, MemoryLocation & ml ) {
	//TODO: Implement different sizes
	//TODO: Implement different offset sizes
	std::stringstream ss;
	std::string store;
	if ( ml.size == 1 ) {
		store = "sb";
	} else if (ml.size == WORD_SIZE ) {
		store = "sw";
	} else {
		assert(0);
	}
	if ( ml.base_reg == FP ) {
		ss << "ASM: \t" << store << " " << r << ", " <<  ml.offset << "($fp)\n"; 
	} else if ( ml.base_reg == GP ) {
		ss << "ASM: \t" << store << " " << r << ", " << ml.name << "\n";
	} else {
		 assert(0);
	}
	std::cout << ss.str();
}

void gen_asm_code( ){
	gen_prologue();
	for ( auto i:ta_code ) {
		if ( i->dead ) { 
			continue; 
		}
	
		Quad * q = dynamic_cast<Quad *>(i);
		if ( q != nullptr ) {
			process_quad(q);
			continue;
		}


		Label * l = dynamic_cast<Label *>(i);
		if ( l != nullptr ) {
			std::stringstream ss;
			ss << "ASM: " << "__" << local_symbol_table.function_name << "_" << l->name << "__:\n";
			std::cout << ss.str();
			continue;
		}

		Return * r = dynamic_cast<Return *>(i);

		if ( r != nullptr ) {
			process_return(r);
			continue;

		}
		
		Arg * a = dynamic_cast<Arg *>(i);

		if ( a != nullptr ) {
			process_arg(a);
			continue;

		}

		Call * c = dynamic_cast<Call *>(i);

		if ( c != nullptr ) {
			process_call(c);
			continue;

		}

		GoTo * g = dynamic_cast<GoTo *>(i);

		if ( g != nullptr ) {
			process_goto(g);
			continue;

		}
		SaveLive* s = dynamic_cast<SaveLive *>(i);

		if ( s != nullptr ) {
			process_save_live(s);
			continue;

		}
//		asm_code.push_back();
	}
	gen_epilogue();
	mmu.reset();

}


void process_quad ( Quad * q ) {
	if ( q->arg1.addr != nullptr && q->arg2.addr != nullptr ) {
		if ( q->arg1.addr->type != CON && q->arg2.addr->type != CON ) {
			gen_asm_instr( q->operation, q->result, q->arg1, q->arg2 );

		}
		else if ( q->arg1.addr->type != CON && q->arg2.addr->type == CON ) {
			gen_asm_instr_imm( q->operation, q->result, q->arg1, q->arg2);
		}
		else if ( q->arg1.addr->type == CON && q->arg2.addr->type != CON ) {
			gen_asm_instr_limm( q->operation, q->result, q->arg1, q->arg2);

		} else {
			ARCH_REG dest = mmu.get_reg(tINV, q->result.addr, 0, false);
			std::cout << "ASM: " << q->operation << " t" << dest << " " << *q->arg1.addr<< ", " << *q->arg2.addr << "\n"; 
			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}
			//Ideally this case should be optimised out
			;
		}


	} else if ( q->arg1.addr != nullptr ) {
		if ( q->arg1.addr->type != CON ) {
			gen_asm_instr( q->operation, q->result, q->arg1);

		} else {
			gen_asm_instr_imm( q->operation, q->result, q->arg1);
			;
		}
	}  
}


void gen_asm_instr(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2){

	std::stringstream ss;

	if ( !result.alive ) {
		ss << "ASM: xxxx\t";
	}


	ARCH_REG src1 = mmu.get_reg(tINV, arg1.addr , 2, true);
	ARCH_REG src2 = mmu.get_reg(tINV, arg2.addr , 2, true);
	
	ARCH_REG dest = tINV;
	if ( !arg2.alive ) {
		mmu.free_reg( src2 );
		dest = src2;
	} else if ( arg2.alive && arg2.next_use == nullptr ) {
		mmu.store_and_free_reg( src2 );
		dest = src2;
	}

	if ( !arg1.alive ) {
		mmu.free_reg( src1 );
		dest = src1;
	} 
	else if ( arg1.alive && arg1.next_use == nullptr ) {
		mmu.store_and_free_reg( src1 );
		dest = src1;
	}

	dest = mmu.get_reg( dest, result.addr, 0, false);



	if ( operation == "+" ) {
		ss << "ASM: \t" << "addu" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	} else if ( operation == "-" ) {
		ss << "ASM: \t" << "subu" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	} else if ( operation == "==" ) {
		ss << "ASM: \t" << "seq" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == "!=" ) {
		ss << "ASM: \t" << "sne" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == ">=" ) {
		ss << "ASM: \t" << "sge" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == "<=" ) {
		ss << "ASM: \t" << "sle" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == ">" ) {
		ss << "ASM: \t" << "sgt" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == "<" ) {
		ss << "ASM: \t" << "slt" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == "*" ) {
		ss << "ASM: \t" << "mul" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}  else if ( operation == "/" ) {
		ss << "ASM: \t" << "div" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}	else if ( operation == "|" ) {
		ss << "ASM: \t" << "or" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}	else if ( operation == "&" ) {
		ss << "ASM: \t" << "and" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}	else if ( operation == "^" ) {
		ss << "ASM: \t" << "xor" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	} 	else if ( operation == ">>" ) {
		ss << "ASM: \t" << "srav" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "<<" ) {
		ss << "ASM: \t" << "sllv" << " " << dest <<", " << src1 << ", " << src2<<"\n";
	}else {
		std::cerr << "PANIC: unknown operation " << operation << "\n";
		assert(0);
	}

	std::cout << ss.str();

	if ( result.alive && result.next_use == nullptr ) {
		mmu.store_and_free_reg( dest );
	}

}
void gen_asm_instr_imm(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2){
	std::stringstream ss;

	ARCH_REG src1 = mmu.get_reg(tINV, arg1.addr , 2, true);
	if ( !result.alive ) {
		ss << "ASM: xxxx\t";
	}
	
	
	ARCH_REG dest = tINV;

	if ( !arg1.alive ) {
		mmu.free_reg( src1 );
		dest = src1;
	} 
	else if ( arg1.alive && arg1.next_use == nullptr ) {
		mmu.store_and_free_reg( src1 );
		dest = src1;
	}

	dest = mmu.get_reg( dest, result.addr, 0, false);

	int value = ( int ) std::stoi(arg2.addr->name);
	if ( value >= (int) -0x8000 && value < (int) 0x8000 ) {
		if ( operation == "+" ) {
			ss << "ASM: \t" << "addiu" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		} else if ( operation == "-" ) {
			ss << "ASM: \t" << "addiu" << " " << dest <<", " << src1 << ", " <<  (short) -value <<"\n";
		} else if ( operation == "==" ) {
			ss << "ASM: \t" << "seq" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "!=" ) {
			ss << "ASM: \t" << "sne" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == ">=" ) {
			ss << "ASM: \t" << "sge" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "<=" ) {
			ss << "ASM: \t" << "sle" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == ">" ) {
			ss << "ASM: \t" << "sgt" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "<" ) {
			ss << "ASM: \t" << "slt" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "*" ) {
			ss << "ASM: \t" << "mul" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "/" ) {
			ss << "ASM: \t" << "div" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == ">>" ) {
			ss << "ASM: \t" << "sra" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "<<" ) {
			ss << "ASM: \t" << "sll" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "^" ) {
			ss << "ASM: \t" << "xori" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "|" ) {
			ss << "ASM: \t" << "ori" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		}  else if ( operation == "&" ) {
			ss << "ASM: \t" << "andi" << " " << dest <<", " << src1 << ", " << (short) value <<"\n";
		} else {
			std::cerr << "PANIC: unknown operation " << operation << "\n";
			assert(0);
		}
	} else {
		ss << "ASM: \t" << "li $v1, "  << value << "\n";
		if ( operation == "+" ) {
			ss << "ASM: \t" << "addu" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		} else if ( operation == "-" ) {
			ss << "ASM: \t" << "subu" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		} else if ( operation == "==" ) {
			ss << "ASM: \t" << "seq" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "!=" ) {
			ss << "ASM: \t" << "sne" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == ">=" ) {
			ss << "ASM: \t" << "sge" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "<=" ) {
			ss << "ASM: \t" << "sle" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == ">" ) {
			ss << "ASM: \t" << "sgt" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "<" ) {
			ss << "ASM: \t" << "slt" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "*" ) {
			ss << "ASM: \t" << "mul" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "/" ) {
			ss << "ASM: \t" << "div" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}	else if ( operation == "^" ) {
			ss << "ASM: \t" << "xor" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "|" ) {
			ss << "ASM: \t" << "or" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		}  else if ( operation == "&" ) {
			ss << "ASM: \t" << "and" << " " << dest <<", " << src1 << ", " << static_cast<ARCH_REG>(v1) <<"\n";
		} else {
			std::cerr << "PANIC: unknown operation " << operation << "\n";
			assert(0);
		}

	}

	std::cout << ss.str();

	if ( result.alive && result.next_use == nullptr ) {
		mmu.store_and_free_reg( dest );
	}




}

void gen_asm_instr_limm(std::string operation, ADDRESS & result, ADDRESS & arg1, ADDRESS & arg2){
	std::stringstream ss;

	ARCH_REG src2 = mmu.get_reg(tINV, arg2.addr , 2, true);
	if ( !result.alive ) {
		ss << "ASM: xxxx\t";
	}
	
	
	ARCH_REG dest = tINV;

	if ( !arg2.alive ) {
		mmu.free_reg( src2 );
		dest = src2;
	} 
	else if ( arg2.alive && arg2.next_use == nullptr ) {
		mmu.store_and_free_reg( src2 );
		dest = src2;
	}

	dest = mmu.get_reg( dest, result.addr, 0, false);

	int value = ( int ) std::stoi(arg1.addr->name);
	ARCH_REG src1 = tINV;
	if ( value == 0 ) {
		src1 = zz;
	} else {
		src1 = v1;
		ss << "ASM: \t" << "li $v1, "  << value << "\n";
	}

	
	if ( operation == "+" ) {
		ss << "ASM: \t" << "addu" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	} else if ( operation == "-" ) {
		ss << "ASM: \t" << "subu" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	} else if ( operation == "==" ) {
		ss << "ASM: \t" << "seq" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "!=" ) {
		ss << "ASM: \t" << "sne" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == ">=" ) {
		ss << "ASM: \t" << "sge" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "<=" ) {
		ss << "ASM: \t" << "sle" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == ">" ) {
		ss << "ASM: \t" << "sgt" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "<" ) {
		ss << "ASM: \t" << "slt" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "*" ) {
		ss << "ASM: \t" << "mul" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "/" ) {
		ss << "ASM: \t" << "div" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}	else if ( operation == "^" ) {
		ss << "ASM: \t" << "xor" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "|" ) {
		ss << "ASM: \t" << "or" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	}  else if ( operation == "&" ) {
		ss << "ASM: \t" << "and" << " " << dest <<", " << src1 << ", " << src2 <<"\n";
	} else {
		std::cerr << "PANIC: unknown operation " << operation << "\n";
		assert(0);
	}

	std::cout << ss.str();
	if ( result.alive && result.next_use == nullptr ) {
		mmu.store_and_free_reg( dest );
	}


}

void gen_asm_instr(std::string operation, ADDRESS & result, ADDRESS & arg1){

	std::stringstream ss;
	if ( operation != "()s" && !result.alive ) {
		ss << "ASM: xxxx\t";
	}
	ARCH_REG src1 = mmu.get_reg(tINV, arg1.addr , 2, true);
	ARCH_REG dest = tINV;

	if ( operation != "()s" && !arg1.alive ) {
		mmu.free_reg( src1 );
		dest = src1;
	} 
	else if ( operation != "()s" &&  arg1.alive && arg1.next_use == nullptr  ) {
		mmu.store_and_free_reg( src1 );
		dest = src1;
	}

	 if ( operation == "()s" ) {
		dest = mmu.get_reg( tINV, result.addr, 2, true);
		assert( dest != tINV );
	 } else {
		dest = mmu.get_reg( dest, result.addr, 0, false);
	}


	if ( operation == "=" ) {
		if ( dest != src1 ) {
			ss << "ASM: \t" << "move" << " " << dest <<", " << src1 <<"\n";
		}
	} else if ( operation == "()" ) {
		// TODO: Implement load sizes;
		if ( arg1.addr->size == 1 ) {
			ss << "ASM: \t" << "lb" << " " << dest <<", 0(" << src1 << ")\n";
		} else if ( arg1.addr->size == WORD_SIZE ) {
			ss << "ASM: \t" << "lw" << " " << dest <<", 0(" << src1 << ")\n";
		} else {
			assert(0);
		}
	} else if ( operation == "la" ) {
		issue_load_ea(dest,arg1);
//		ss << "ASM: \t" << "lw" << " " << dest <<", 0(" << src1 << ")\n";
	} else if ( operation == "()s" ) {
		// TODO: Implement  store  sizes;
		if ( result.addr->size == 1 ) {
			ss << "ASM: \t" << "sb" << " " << src1 <<", 0(" << dest << ")\n";
		} else if ( result.addr->size == WORD_SIZE ) {
			ss << "ASM: \t" << "sw" << " " << src1 <<", 0(" << dest << ")\n";
		} else {
			assert(0);
		}
	} else if ( operation == "!" ) {
		ss << "ASM: \t" << "not" << " " << dest <<", " << src1 <<"\n";
	} else {
		std::cerr << "PANIC: unknown operation " << operation << "\n";
		assert(0);
	}


	std::cout << ss.str();

	if ( result.alive && result.next_use == nullptr ) {
		mmu.store_and_free_reg( dest );
	}
}

void gen_asm_instr_imm(std::string operation, ADDRESS & result , ADDRESS & arg1){
	std::stringstream ss;
	if ( operation != "()s" && !result.alive ) {
		ss << "ASM: xxxx\t";
	}
	ARCH_REG dest = tINV;
	 if ( operation == "()s" ) {
		dest = mmu.get_reg( tINV, result.addr, 2, true);
		assert( dest != tINV );
	 } else {
		dest = mmu.get_reg( tINV, result.addr, 0, false);
	}
	int value = ( int ) std::stoi(arg1.addr->name);

	if ( operation == "=" ) {
			ss << "ASM: \t" << "li "<< dest << ", " << value << "\n";
	} else if ( operation == "()s" ) {
		ss << "ASM: \t" << "li $v1, " << value << "\n";
		// TODO: Implement  store  sizes;
		if ( result.addr->size == 1 ) {
			ss << "ASM: \t" << "sb" << " $v1, 0(" << dest << ")\n";
		} else if ( result.addr->size == WORD_SIZE ) {
			ss << "ASM: \t" << "sw" << " $v1, 0(" << dest << ")\n";
		} else {
			assert(0);
		}

	} else {
		std::cerr << "PANIC: unknown operation " << operation << "\n";
		assert(0);
	}

	std::cout << ss.str();

	if ( !result.alive  ) {
		mmu.free_reg( dest );
	}
	else if ( result.alive && result.next_use == nullptr ) {
		mmu.store_and_free_reg( dest );
	}
}

void process_return( Return * r ) {
	std::stringstream ss;
	if (r->retval.addr != nullptr) {
		if ( r->retval.addr->type == CON ) {
			int value = ( int ) std::stoi(r->retval.addr->name);
				ss << "ASM: \t" << "li $v0, " << value << "\n";
				std::cout << ss.str();
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, r->retval.addr, 2, true );
			ss << "ASM: \t" << "move $v0, " << reg <<"\n";
			std::cout << ss.str();
			if ( !r->retval.alive  ) {
				mmu.free_reg( reg );
			}
			else if ( r->retval.alive && r->retval.next_use == nullptr ) {
				mmu.store_and_free_reg( reg );
			}
		}
	}



	
}


void process_arg( Arg * a) {
	std::stringstream ss;
	if ( a->num < NUM_REG_ARGS ) {
		auto it = mmu.memory_locations.find( a->num | FUN_ARG_MASK );
		ARCH_REG arg_reg = static_cast<ARCH_REG>( a->num + a0 ); 
		if ( it != mmu.memory_locations.end() ) {
			mmu.temp_stack.push_back( arg_reg );
			ss << "ASM: \t" << "addiu $sp, $sp, -4\n"; 
			ss << "ASM: \t" << "sw " << arg_reg << ", " << 0 << "($sp)\n";
		}
		if ( a->arg.addr->type == CON ) {
			int value = std::stoi(a->arg.addr->name);
				ss << "ASM: \t" << "li " << arg_reg << ", " << value << "\n";
			std::cout << ss.str(); 
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, a->arg.addr, 2, true );
			if ( reg != arg_reg ) {
				ss << "ASM: \t" << "move " << arg_reg << ", " << reg <<"\n";
			}
			std::cout << ss.str(); 
			if ( !a->arg.alive ) {
				mmu.free_reg( reg );
			} else if ( a->arg.alive && a->arg.next_use == nullptr ) {
				mmu.store_and_free_reg( reg );
			}
			return;
		}
	} else {
		if ( a->arg.addr->type == CON ) {
			int value = std::stoi(a->arg.addr->name);
			mmu.temp_stack.push_back( at );
			ss << "ASM: \t" << "li $v1, " << value << "\n";
			ss << "ASM: \t" << "addiu $sp, $sp, -4\n"; 
			if ( a->arg.addr->size == 1 ) {
				ss << "ASM: \t" << "sb $v1, " << 0 << "($sp)\n";
			} else if ( a->arg.addr->size == WORD_SIZE ) {
				ss << "ASM: \t" << "sw $v1, " << 0 << "($sp)\n";
			} else {
				assert(0);
			}
			std::cout << ss.str(); 
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, a->arg.addr, 2, true );
			mmu.temp_stack.push_back( at );
			ss << "ASM: \t" << "addiu $sp, $sp, -4\n"; 
			if ( a->arg.addr->size == 1 ) {
				ss << "ASM: \t" << "sb " << reg << ", " << 0 << "($sp)\n";
			} else if ( a->arg.addr->size == WORD_SIZE ) {
				ss << "ASM: \t" << "sw " << reg << ", " << 0 << "($sp)\n";
			} else {
				assert(0);
			}
			std::cout << ss.str();
			if ( !a->arg.alive ) {
				mmu.free_reg( reg );
			} else if ( a->arg.alive && a->arg.next_use == nullptr ) {
				mmu.store_and_free_reg( reg );
			}
			return;
		}
	} 
}

void process_call( Call * c) {
	std::stringstream ss;
	ss << "ASM: \t" << "jal " << c->function_name << "\n";
	if ( c->retval.addr != nullptr && c->retval.alive == true ) {
			ARCH_REG reg = mmu.get_reg( tINV, c->retval.addr, 2, true );
			ss << "ASM: \t" << "move" << " " << reg << ", $v0" << "\n";
	} 
	
	while ( !mmu.temp_stack.empty() ) {
		ARCH_REG r = mmu.temp_stack.back();
		if ( r != at ) {
			ss << "ASM: \t" << "lw " << r << ", " << 0 << "($sp)\n";
		}
		ss << "ASM: \t" << "addiu $sp, $sp, 4\n"; 
		mmu.temp_stack.pop_back();
	}
	std::cout << ss.str();
}

void process_goto( GoTo * g ) {
	std::stringstream ss;
	if ( g->res.addr == nullptr ) {
		ss << "ASM: \t" << "j __" << local_symbol_table.function_name << "_" << g->label->name << "__\n";
		std::cout << ss.str();
		return;
	} 
	// Constant type should be optimised out in 3AC
	assert(g->res.addr->type != CON );
	ARCH_REG reg = mmu.get_reg( tINV, g->res.addr, 2, true );
	ss << "ASM: \t";
	if ( g->condition == true ) {
		ss << "bnez ";
	} else {
		ss << "beqz ";
	}
	ss << reg << ", __"  << local_symbol_table.function_name << "_" << g->label->name << "__\n";
	std::cout << ss.str();
	if ( !g->res.alive ) {
		mmu.free_reg( reg );
	} else if ( g->res.alive && g->res.next_use == nullptr ) {
		mmu.store_and_free_reg( reg );
	}
}

void process_save_live( SaveLive * s ){
	std::stringstream ss;
	for ( int r = 0; r < NUM_TEMP_REGS; r++ ) {
		ARCH_REG reg = static_cast<ARCH_REG>( t0 + (r % NUM_TEMP_REGS) );
		if ( mmu.reg_alloc_info[ r % NUM_TEMP_REGS ] & TEMP_ID_MASK ) {
				if ( ! s->save_temps ) {
					continue;
				}
				// Put this on the stack;
				assert ( reg != tINV && reg <= t9 && reg >= t0 );
				mmu.temp_stack.push_back( reg );
				ss << "ASM: \t" << "addiu $sp, $sp, -4\n"; 
				ss << "ASM: \t" << "sw " << reg << ", " << 0 << "($sp)\n";
		} else if ( mmu.reg_alloc_info[ r % NUM_TEMP_REGS ] != 0 ) {
			mmu.store_and_free_reg( reg );
		}
		std::cout << ss.str();	
		ss.clear();
		ss.str("");
	}
	


}

void gen_epilogue() {
	//TODO: Save callee saved registers??
	std::stringstream ss;
	//size_t reqd_size = local_symbol_table.reqd_size < 32 ? 32 : local_symbol_table.reqd_size;
	//reqd_size = reqd_size + mmu.temp_stack.size() * 4;
	//if ( reqd_size < 0x8000 ) {
	//	reqd_size = reqd_size & 0xffff;
	//	ss << "ASM: \t" << "addiu $sp, $sp, " << (short) reqd_size << "\n";
	//} else {
	//	ss << "ASM: \t" << "li $v1, " << reqd_size  << "\n";
	//	ss << "ASM: \t" << "addu $sp, $sp, $v1\n";
	//}
	ss << "ASM: \t" << "move $sp, $fp\n";
	ss << "ASM: \t" << "lw $fp, 0($sp)\n";
	ss << "ASM: \t" << "lw $ra, 4($sp)\n";
	ss << "ASM: \t" << "addiu $sp, $sp, " << 8 <<"\n";
	ss << "ASM: \t" << "jr $ra\n";
	
	ss << "ASM: \t" << "\n\n#####################\n\n";
		if ( !mmu.strings.empty() ) {
		ss << "ASM: \t" << ".data\n";
		for ( auto i : mmu.strings ) {
			ss << "ASM: " << i.first << ":\t.asciiz\t" << i.second << "\n"  ;
		}
	}
	mmu.strings.clear();
	ss << "ASM: \t" << "\n\n#####################\n\n";
	std::cout << ss.str();

}

void gen_prologue( ) {

	std::stringstream ss;
	ss << "ASM: \t" << "\n\n#####################\n\n";
		if ( !mmu.globals.empty() ) {
		ss << "ASM: \t" << ".data\n";
		for ( auto i : mmu.globals ) {
			ss << "ASM: "<< "\t.globl " << i.first << "\n";
			ss << "ASM: "<< "\t.align 2\n";
			ss << "ASM: " << i.first << ": " << "\t.space " << i.second << "\n"; 
		}
	}
	mmu.globals.clear();
	ss << "ASM: \t" << "\n\n#####################\n\n";
	//TODO: Save callee saved registers??
	ss << "ASM: \t.text\n"; 
	ss << "ASM: \t.globl " << local_symbol_table.function_name << "\n"; 
	ss << "ASM: \t" << "\n\n###########################\n\n";
	ss << "ASM: " <<  local_symbol_table.function_name << ":\n";
	ss << "ASM: \t" << "addiu $sp, $sp, " << -8 <<"\n";
	ss << "ASM: \t" << "sw $ra, 4($sp)\n";
	ss << "ASM: \t" << "sw $fp, 0($sp)\n";
	ss << "ASM: \t" << "move $fp, $sp\n";
	mmu.stack_size = local_symbol_table.reqd_size < 32 ? 32 : local_symbol_table.reqd_size;
	size_t reqd_size = mmu.stack_size;
	if ( reqd_size <= 0x8000 ) {
		reqd_size = ( ~reqd_size + 1 ) & 0xffff;
		ss << "ASM: \t" << "addiu $sp, $sp, " << (short) reqd_size << "\n";
	} else {
		ss << "ASM: \t" << "li $v1, " << reqd_size << "\n";
		ss << "ASM: \t" << "subu $sp, $sp, $v1\n";
	}
	ss << "ASM: \t" << "\n";
	std::cout << ss.str();
}

std::ostream& operator<<( std::ostream &os, ARCH_REG & a){
	switch ( a ) {
		case zz:
			os << "$0";
			break;
		case t0:
			os << "$t0";
			break;
		case t1:
			os << "$t1";
			break;
		case t2:
			os << "$t2";
			break;
		case t3:
			os << "$t3";
			break;
		case t4:
			os << "$t4";
			break;
		case t5:
			os << "$t5";
			break;
		case t6:
			os << "$t6";
			break;
		case t7:
			os << "$t7";
			break;
		case t8:
			os << "$t8";
			break;
		case t9:
			os << "$t9";
			break;
		case a0:
			os << "$a0";
			break;
		case a1:
			os << "$a1";
			break;
		case a2:
			os << "$a2";
			break;
		case a3:
			os << "$a3";
			break;
		case v0:
			os << "$v0";
			break;
		case v1:
			os << "$v1";
			break;
		case tINV:
			os << "$tINV";
		default:
			assert(0);
	}
	return os;
}
