#include<codegen.h>
#include<assert.h>

std::vector<AsmInstr> asm_code;

MemManUnit mmu;

MemoryLocation::MemoryLocation() : id(0), reg(tINV), in_mem(true), base_reg(rINV), offset(0), is_ea(false) {}  

MemoryLocation create_memory_location(unsigned int id,  long offset){
	MemoryLocation ml;
	ml.id = id;

//XXX : Check this logic
	if ( id & TEMP_ID_MASK  ) {
		// Should never reach here;
		assert(0);
	} else if ( id & GLOBAL_SYM_MASK ) {
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
			ml.offset = -offset;
		}
	} else if ( id & LOCAL_SYM_MASK ){
		ml.base_reg = FP;
		ml.offset = local_symbol_table.reqd_size - offset;
	} else {
		assert(0);
	}
	return ml;
}

MemoryLocation create_temp_mem_location(unsigned int id,  ARCH_REG reg ){
	MemoryLocation ml;
	ml.id = id;

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


MemManUnit::MemManUnit() : start_issue(t0), reg_alloc_info(std::vector<unsigned int>(NUM_ARCH_REGS, t0)) {} 

ARCH_REG MemManUnit::get_reg( ARCH_REG dest, Address * a , int mem_valid , bool load ) {
	auto it = memory_locations.find( a->table_id );
	if ( it != memory_locations.end() ) {
		if ( it->second.reg != tINV ) {
			return it->second.reg;
		}

		ARCH_REG r;
		if ( dest == tINV || dest > t9 ) {
			r = get_empty_reg();
		} else {
			r = dest;
		}
		assert( r != tINV && r <= t9 );
		reg_alloc_info[r] = a->table_id;
		it->second.reg = r;
		if ( load && it->second.in_mem && !it->second.is_ea ) {
			issue_load(r, it->second.base_reg, it->second.offset );
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
		if ( dest == tINV || dest > t9 ) {
			r = get_empty_reg();
		} else {
			r = dest;
		}
		assert( r != tINV && r <= t9 );
		reg_alloc_info[r] = a->table_id;
		memory_locations.insert({a->table_id,create_temp_mem_location(a->table_id, r)});
		return r;
	} else {
		std::cerr << "PANIC: Memory Location " << a->name << " not found\n";
		assert(0);
	}
	return tINV;
}

ARCH_REG MemManUnit::get_empty_reg() {
	for ( int r = t0; r < NUM_TEMP_REGS; r++ ) {
		if ( reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ] == 0 ) {
			ARCH_REG empty =  static_cast<ARCH_REG> ( (start_issue + r) % NUM_TEMP_REGS );
			start_issue = static_cast<ARCH_REG>( (empty + 1) % NUM_TEMP_REGS );
			return empty;
		}
	}
	for ( int r = t0; r < NUM_TEMP_REGS; r++ ) {
		// There shouldn't be a case where all locations are occupied by temporaries
		// so looking for a program variable is enough
		if ( ( reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ] & TEMP_ID_MASK )  == 0 ) {
			unsigned int table_id = reg_alloc_info[ (start_issue + r) % NUM_TEMP_REGS ]; 
			auto it = memory_locations.find( table_id );
			assert( it != memory_locations.end() );
			//Register spill
			issue_store( it->second.reg, it->second.base_reg, it->second.offset  );
			it->second.reg = tINV;
			it->second.in_mem = true;
			ARCH_REG empty =  static_cast<ARCH_REG> ( (start_issue + r) % NUM_TEMP_REGS );
			start_issue = static_cast<ARCH_REG>( (empty + 1) % NUM_TEMP_REGS );
			return empty;
		}
	}

	std::cerr << "PANIC: Unable to find empty register\n";
	assert(0);

	return tINV;

}

void MemManUnit::free_reg( ARCH_REG r ) {
	assert ( r != tINV );
	if ( r > t9 ){
		return;
	}
	unsigned int id = reg_alloc_info[r];
	auto it = memory_locations.find( id ) ;
	assert ( it != memory_locations.end() );
	it->second.reg = tINV;
	reg_alloc_info[r] = 0;
}

void MemManUnit::store_and_free_reg( ARCH_REG r ) {
	assert ( r != tINV );
	if ( r > t9 ){
		return;
	}
	unsigned int id = reg_alloc_info[r];
	auto it = memory_locations.find( id ) ;
	assert ( it != memory_locations.end() );
	it->second.reg = tINV;
	if ( it->second.in_mem != true ) {
		issue_store( r, it->second.base_reg, it->second.offset );
	}
	it->second.in_mem = true;
	reg_alloc_info[r] = 0;
}

void MemManUnit::set_reg ( unsigned int table_id, ARCH_REG r ) {
	if ( r >= t0 && r  <= NUM_TEMP_REGS ) {
		reg_alloc_info[r] = table_id;
	}
}


void issue_load( ARCH_REG r, OFFSET_REGISTER base, long offset ) {
	std::cout << "ASM: LD: " << r << " " << offset << "(" << base << ")\n"; 
}

void issue_store( ARCH_REG r, OFFSET_REGISTER base, long offset ){
	std::cout << "ASM: ST: " << r << " " << offset << "(" << base << ")\n"; 
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
			std::cout << "ASM: " << l->name << "\n";
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
//		asm_code.push_back();
	}
	gen_epilogue();

}


void process_quad ( Quad * q ) {
	// add, add
	// add, con
	// con, add
	// con, con
	// add
	// con
	if ( !q->result.alive ) {
		std::cout << "ASM: xxxxxxx\n";
		return;
	}
	if ( q->arg1.addr != nullptr && q->arg2.addr != nullptr ) {
		if ( q->arg1.addr->type != CON && q->arg2.addr->type != CON ) {
			ARCH_REG src1 = mmu.get_reg(tINV, q->arg1.addr , 2, true);
			ARCH_REG src2 = mmu.get_reg(tINV, q->arg2.addr , 2, true);
			
			ARCH_REG dest = tINV;
			if ( !q->arg2.alive ) {
				mmu.free_reg( src2 );
				dest = src2;
			} else if ( q->arg2.alive && q->arg2.next_use == nullptr ) {
				mmu.store_and_free_reg( src2 );
				dest = src2;
			}

			if ( !q->arg1.alive ) {
				mmu.free_reg( src1 );
				dest = src1;
			} 
			else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
				mmu.store_and_free_reg( src1 );
				dest = src1;
			}
			dest = mmu.get_reg( dest, q->result.addr, 0, false);
			
			std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 << ", t" << src2 <<"\n";
			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}


		}
		else if ( q->arg1.addr->type != CON && q->arg2.addr->type == CON ) {
			ARCH_REG src1 = mmu.get_reg(tINV, q->arg1.addr , 2, true);
			ARCH_REG dest = tINV;
			if ( !q->arg1.alive ) {
				mmu.free_reg( src1 );
				dest = src1;
			} else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
				mmu.store_and_free_reg( src1 );
				dest = src1;
			} 
			dest = mmu.get_reg( dest, q->result.addr, 0, false);
			
			std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 << ", " << q->arg2.addr->name <<"\n";
			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}

		}
		else if ( q->arg1.addr->type == CON && q->arg2.addr->type != CON ) {
			ARCH_REG src1 = mmu.get_reg(tINV, q->arg2.addr , 2, true);
			ARCH_REG dest = tINV;
			if ( !q->arg2.alive ) {
				mmu.free_reg( src1 );
				dest = src1;
			} else if ( q->arg2.alive && q->arg2.next_use == nullptr ) {
				mmu.store_and_free_reg( src1 );
				dest = src1;
			}
			dest = mmu.get_reg( dest, q->result.addr, 0, false);
			
			std::cout << "ASM: " << q->operation << " t" << dest << ", " << q->arg2.addr->name << ", t" << src1  <<"\n";
			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}

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
			ARCH_REG src1 = mmu.get_reg( tINV, q->arg1.addr , 2, true);
			ARCH_REG dest = tINV;
			if ( !q->arg1.alive ) {
				mmu.free_reg( src1 );
				dest = src1;
			} else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
				mmu.store_and_free_reg( src1 );
				dest = src1;
			}
			dest = mmu.get_reg(dest, q->result.addr, 0, false);
			std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 <<"\n";

			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}

		} else {
			ARCH_REG dest = mmu.get_reg(tINV, q->result.addr, 0, false);

			std::cout << "ASM: " << q->operation << " t" << dest << " " <<  q->operation << " " << *q->arg1.addr << "\n"; 
			if ( q->result.alive && q->result.next_use == nullptr ) {
				mmu.store_and_free_reg( dest );
			}
			//Ideally this case should be optimised out
			;
		}

	}  

}

void process_return( Return * r ) {
	if (r->retval.addr != nullptr) {
		if ( r->retval.addr->type == CON ) {
			std::cout << "ASM: " << "li" << " v" << 0 << ", " << r->retval.addr->name <<"\n";
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, r->retval.addr, 2, true );
			std::cout << "ASM: " << "mv" << " v" << 0 << " ,t" << reg <<"\n";
		}
	}

	
}


void process_arg( Arg * a) {
	if ( a->num < NUM_REG_ARGS ) {
		auto it = mmu.memory_locations.find( a->num | FUN_ARG_MASK );
		ARCH_REG arg_reg = static_cast<ARCH_REG>( a->num + a0 ); 
		if ( it != mmu.memory_locations.end() ) {
			mmu.temp_stack.push_back( arg_reg );
			std::cout << "ASM: " << "push a" << a->num << "\n";
		}
		if ( a->arg.addr->type == CON ) {
			std::cout << "ASM: " << "li" << " a" << a->num << ", " << a->arg.addr->name <<"\n";
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, a->arg.addr, 2, true );
			if ( reg != arg_reg ) {
				std::cout << "ASM: " << "mv" << " a" << a->num << ", t" << reg <<"\n";
			}
		}
	} else {
		if ( a->arg.addr->type == CON ) {
			ARCH_REG reg = mmu.get_empty_reg();
			std::cout << "ASM: " << "li" << " t" << reg << ", " << a->arg.addr->name <<"\n";
			std::cout << "ASM: " << "push t" << reg << "\n";
		} else {
			ARCH_REG reg = mmu.get_reg( tINV, a->arg.addr, 2, true );
			std::cout << "ASM: " << "push t" << reg << "\n";
		}
	} 
}

void process_call( Call * c) {
	std::cout << "ASM: " << "jal " << c->function_name << "\n";
	if ( c->retval.addr != nullptr && c->retval.alive == true ) {
			ARCH_REG reg = mmu.get_reg( tINV, c->retval.addr, 2, true );
			std::cout << "ASM: " << "mv" << " t" << reg << ", v0" << "\n";
	} 
	
	while ( !mmu.temp_stack.empty() ) {
		std::cout << "ASM: " << "pop a" << mmu.temp_stack.back() - a0  << "\n";
		mmu.temp_stack.pop_back();
	}
}

void process_goto( GoTo * g ) {
	if ( g->res.addr == nullptr ) {
		std::cout << "ASM: " << "j " << g->label->name << "\n";
		return;
	} 
	// Constant type should be optimised out in 3AC
	assert(g->res.addr->type != CON );
	ARCH_REG reg = mmu.get_reg( tINV, g->res.addr, 2, true );
	std::string str;
	if ( g->condition == true ) {
		str = "br.true";
	} else {
		str = "br.false";
	}
	std::cout << "ASM: " << str << " t" << reg << " " << g->label->name << "\n";
}

void process_save_live( SaveLive * s ){
	


}

void gen_epilogue() {
	// TODO: Implement return stack cleanup
	size_t reqd_size = local_symbol_table.reqd_size < 32 ? 32 : local_symbol_table.reqd_size;
	reqd_size = reqd_size + mmu.temp_stack.size() * 4;
	if ( reqd_size < 0x8000 ) {
		reqd_size = reqd_size & 0xffff;
		std::cout << "ASM: " << "addiu $sp, $sp, " << (short) reqd_size << "\n";
	} else {
		std::cout << "ASM: " << "lui $at, " << (short) (reqd_size >> 16) << "\n";
		std::cout << "ASM: " << "li $at, " << (short)  (reqd_size & 0xffff ) << "\n";
		std::cout << "ASM: " << "addu $sp, $sp, $at\n";
	}
	std::cout << "ASM: " << "pop $fp\n";
	std::cout << "ASM: " << "pop $ra\n";
	std::cout << "ASM: " << "jr $ra\n";
}

void gen_prologue( ) {
	std::cout << "ASM: \t.text\n"; 
	std::cout << "ASM: " <<  local_symbol_table.function_name << ":\n";
	std::cout << "ASM: " << "addiu $sp, $sp," << -8 <<"\n";
	std::cout << "ASM: " << "sw $ra, 4($sp)\n";
	std::cout << "ASM: " << "sw $fp, 0($sp)\n";
	std::cout << "ASM: " << "mv $fp, $sp\n";
	size_t reqd_size = local_symbol_table.reqd_size < 32 ? 32 : local_symbol_table.reqd_size;
	if ( reqd_size <= 0x8000 ) {
		reqd_size = ( ~reqd_size + 1 ) & 0xffff;
		std::cout << "ASM: " << "addiu $sp, $sp, " << (short) reqd_size << "\n";
	} else {
		std::cout << "ASM: " << "lui $at, " << (short) (reqd_size >> 16) << "\n";
		std::cout << "ASM: " << "li $at, " << (short)  (reqd_size & 0xffff ) << "\n";
		std::cout << "ASM: " << "subu $sp, $sp, $at\n";
	}
}
