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


ARCH_REG MemManUnit::get_reg( Address * a , int mem_valid , bool load ) {
	auto it = memory_locations.find( a->table_id );
	if ( it != memory_locations.end() ) {
		if ( it->second.reg != tINV ) {
			return it->second.reg;
		} else {
			ARCH_REG r = get_empty_reg( );
			assert( r != tINV && r <= t9 );
			reg_alloc_info[r] = a->table_id;
			it->second.reg = r;
			if ( load && it->second.in_mem ) {
				issue_load(r, it->second.base_reg, it->second.offset );
			} else if ( load && !it->second.in_mem ) {
				std::cerr << "PANIC : Loading value not in memory\n";
				assert(0);
			} 
			if ( mem_valid == 1 ) {
				it->second.in_mem = true;
			} else if ( mem_valid == 0 ) {
				it->second.in_mem = false;
			}
			return r;
		}
		
	} else if ( a->table_id & TEMP_ID_MASK ) {
		ARCH_REG r = get_empty_reg( );
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
	for ( int r = t0; r < NUM_ARCH_REGS; r++ ) {
		if ( reg_alloc_info[ (start_issue + r) % NUM_ARCH_REGS ] == 0 ) {
			ARCH_REG empty =  static_cast<ARCH_REG> ( (start_issue + r) % NUM_ARCH_REGS );
			start_issue = static_cast<ARCH_REG>( (empty + 1) % NUM_ARCH_REGS );
			return empty;
		}
	}
	for ( int r = t0; r < NUM_ARCH_REGS; r++ ) {
		// There shouldn't be a case where all locations are occupied by temporaries
		// so looking for a program variable is enough
		if ( ( reg_alloc_info[ (start_issue + r) % NUM_ARCH_REGS ] & TEMP_ID_MASK )  == 0 ) {
			unsigned int table_id = reg_alloc_info[ (start_issue + r) % NUM_ARCH_REGS ]; 
			auto it = memory_locations.find( table_id );
			assert( it != memory_locations.end() );
			//Register spill
			issue_store( it->second.reg, it->second.base_reg, it->second.offset  );
			it->second.reg = tINV;
			it->second.in_mem = true;
			ARCH_REG empty =  static_cast<ARCH_REG> ( (start_issue + r) % NUM_ARCH_REGS );
			start_issue = static_cast<ARCH_REG>( (empty + 1) % NUM_ARCH_REGS );
			return empty;
		}
	}

	std::cerr << "PANIC: Unable to find empty register\n";
	assert(0);

	return tINV;

}

void MemManUnit::free_reg( ARCH_REG r ) {
	unsigned int id = reg_alloc_info[r];
	auto it = memory_locations.find( id ) ;
	assert ( it != memory_locations.end() );
	it->second.reg = tINV;
	reg_alloc_info[r] = 0;
}

void MemManUnit::store_and_free_reg( ARCH_REG r ) {
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
	if ( r >= t0 && r  <= NUM_ARCH_REGS ) {
		reg_alloc_info[r] = table_id;
	}
}


void issue_load( ARCH_REG r, OFFSET_REGISTER base, long offset ) {
	std::cout << "ASM: LD: " << r << " " << offset << "(" << base << ")\n"; 
}

void issue_store( ARCH_REG r, OFFSET_REGISTER base, long offset ){
	std::cout << "ASM: ST: " << r << " " << offset << "(" << base << ")\n"; 
}

void gen_asm_code(){
	for ( auto i:ta_code ) {
		if ( i->dead ) { 
			continue; 
		}
	
		Quad * q = dynamic_cast<Quad *>(i);
		if ( q != nullptr ) {
			// add, add
			// add, con
			// con, add
			// con, con
			// add
			// con
			if ( !q->result.alive ) {
				std::cout << "ASM: xxxxxxx\n";
				continue;
			}
			if ( q->arg1.addr != nullptr && q->arg2.addr != nullptr ) {
				if ( q->arg1.addr->type != CON && q->arg2.addr->type != CON ) {
					ARCH_REG src1 = mmu.get_reg( q->arg1.addr , 2, true);
					ARCH_REG src2 = mmu.get_reg( q->arg2.addr , 2, true);
					if ( !q->arg1.alive ) {
						mmu.free_reg( src1 );
					} 
					else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
						mmu.store_and_free_reg( src1 );
					}

					if ( !q->arg2.alive ) {
						mmu.free_reg( src2 );
					} else if ( q->arg2.alive && q->arg2.next_use == nullptr ) {
						mmu.store_and_free_reg( src2 );
					}
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);
					
					std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 << ", t" << src2 <<"\n";
					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}


				}
				else if ( q->arg1.addr->type != CON && q->arg2.addr->type == CON ) {
					ARCH_REG src1 = mmu.get_reg( q->arg1.addr , 2, true);
					//ARCH_REG src2 = get_reg( q->arg2.addr , 2);
					if ( !q->arg1.alive ) {
						mmu.free_reg( src1 );
					} else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
						mmu.store_and_free_reg( src1 );
					}
					//if ( !q->arg2.alive ) {
					//	mmu.free_reg( src2 );
					//}
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);
					
					std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 << ", " << q->arg2.addr->name <<"\n";
					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}

				}
				else if ( q->arg1.addr->type == CON && q->arg2.addr->type != CON ) {
					ARCH_REG src1 = mmu.get_reg( q->arg2.addr , 2, true);
					//ARCH_REG src2 = get_reg( q->arg2.addr , 2);
					if ( !q->arg2.alive ) {
						mmu.free_reg( src1 );
					} else if ( q->arg2.alive && q->arg2.next_use == nullptr ) {
						mmu.store_and_free_reg( src1 );
					}
					//if ( !q->arg1.alive ) {
					//	mmu.free_reg( src2 );
					//}
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);
					
					std::cout << "ASM: " << q->operation << " t" << dest << ", " << q->arg2.addr->name << ", t" << src1  <<"\n";
					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}

				} else {
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);

					std::cout << "ASM: " << q->operation << " t" << dest << " " << *q->arg1.addr<< ", " << *q->arg2.addr << "\n"; 
					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}
					//Ideally this case should be optimised out
					;
				}


			} else if ( q->arg1.addr != nullptr ) {
				if ( q->arg1.addr->type != CON ) {
					ARCH_REG src1 = mmu.get_reg( q->arg1.addr , 2, true);
					//ARCH_REG src2 = get_reg( q->arg2.addr , 2);
					if ( !q->arg1.alive ) {
						mmu.free_reg( src1 );
					} else if ( q->arg1.alive && q->arg1.next_use == nullptr ) {
						mmu.store_and_free_reg( src1 );
					}
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);
					
					std::cout << "ASM: " << q->operation << " t" << dest << ", t" << src1 <<"\n";

					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}

				} else {
					ARCH_REG dest = mmu.get_reg( q->result.addr, 0, false);

					std::cout << "ASM: " << q->operation << " t" << dest << " " <<  q->operation << " " << *q->arg1.addr << "\n"; 
					if ( q->result.alive && q->result.next_use == nullptr ) {
						mmu.store_and_free_reg( dest );
					}
					//Ideally this case should be optimised out
					;
				}

			}  

		}

		Label * l = dynamic_cast<Label *>(i);

		if ( l != nullptr ) {
			std::cout << "ASM: " << l->name << "\n";
		}
//		asm_code.push_back();
	}

}
