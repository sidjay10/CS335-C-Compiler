#include <ast.h>
#include <bits/stdc++.h>
#include <iostream>
#include <expression.h>
#include <symtab.h>
#include <sstream>
#include <3ac.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <unordered_set>
#include <bits/stdc++.h>
unsigned long long instructions = 1;
unsigned long long labels = 1;

std::vector< ThreeAC * > ta_code;
std::map< unsigned int, TacInfo > tac_info_table;
std::unordered_set<std::string> var_rep;

Label::Label() : ThreeAC(false) {
	reference_count = 0;
	dead = false;
	name = "L"+std::to_string(labels++);
	instruction_id = instructions;
}

Label::~Label() {}

//void Label::update_targets( Label * label ) {
//	for ( auto it = references.begin(); it != references.end(); it ++ ) {
//		(*it)->label = label;
//		label->references.push_back(*it);
//		label->reference_count++;
//	}
//}

Label* create_new_label(){
	Label* l = new Label();
	//std::cout << "3AC: " << *l << "\n";
	ta_code.push_back(l);
	return l;
}

std::string Label::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Label& l){
	os << l.name << ":" ;
	return os;
}
	

Quad::Quad ( Address * _result, std::string _operation, Address * _arg1, Address * _arg2 ) { 

	operation = _operation;

	result.addr = _result;
	result.alive = true;
	result.next_use = nullptr;

	arg1.addr = _arg1;
	arg1.alive = true;
	arg1.next_use = nullptr;

	arg2.addr = _arg2;
	arg2.alive = true;
	arg2.next_use = nullptr;

	if( arg1.addr != nullptr && arg1.addr->ta_instr!=nullptr) {
		arg1.addr->ta_instr->dead = false;
	}
	if( arg2.addr != nullptr && arg2.addr->ta_instr != nullptr) {
		arg2.addr->ta_instr->dead = false;
	}
	if ( operation == "()s" ) {
		result.addr->ta_instr->dead = false;
		dead = false;
	}
};

Quad::~Quad () {
	delete result.addr;
	delete arg1.addr;
	delete arg2.addr;

}

std::ostream& operator<<(std::ostream& os, const ADDRESS & a){
	os << *a.addr;

	if ( a.addr->type == CON ) {
		return os;
	 }

	if ( a.alive ) {
		os << "(L)";
	} else {
		os << "(D)";
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, const Quad& q){
	if ( q.arg2.addr != nullptr ) {
		os << q.instr << ": " << q.result << " = " << q.arg1 << " " << q.operation << " " << q.arg2 ;
	} else if ( q.operation == "()" ) {
		os << q.instr << ": " << q.result << " = " <<  "(" << q.arg1 << ")" ;
	} else if ( q.operation == "()s" ) {
		os << q.instr << ": " << "(" << q.result << ")" << " = " << q.arg1;
	} else if ( q.operation == "=" ) {
		os << q.instr << ": " <<  q.result  << " = " << q.arg1;
	} else if ( q.operation == "=s" ) {
		os << q.instr << ": " <<  q.result  << " = " << q.arg1;
	} else if ( q.operation == "push" ) {
		os << q.instr << ": " <<  "push " << q.arg1;
	} else if ( q.operation.substr(0,3) == "arg" ) {
		os << q.instr << ": " << q.operation  << " " << q.arg1;
	} else {
		os << q.instr << ": " << q.result << " = " << q.operation << " " << q.arg1 ;
	}
	return os;
}

unsigned long long emit(  Address * result, std::string operation, Address * arg1, Address * arg2 ) {
	Quad * q = new Quad(result,operation,arg1,arg2);
	if ( result != nullptr ) {
		result->ta_instr = q;
	}
	if ( result == nullptr || result->type == ID3 ) {
		q->dead = false;
	}
	ta_code.push_back(q);
	//std::cout << "3AC: " << *q << "\n";
	return ta_code.size();

}

std::string Quad::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

unsigned long long temporaries = 1;

Address::Address(std::string _name, ADD_TYPE _type ) : name (_name) , type(_type), ta_instr(nullptr)  {};

Address * new_temp() {
	Address * t = new Address("t" + std::to_string(temporaries), TEMP );
	t->table_id = TEMP_ID_MASK | temporaries;
	temporaries++;
	tac_info_table.insert({t->table_id,TacInfo(false)});
	return t;
}

Address * new_mem() {
	Address * t = new Address("t" + std::to_string(temporaries), MEM );
	t->table_id = TEMP_ID_MASK | temporaries;
	temporaries++;
	tac_info_table.insert({t->table_id,TacInfo(false)});
	return t;
}

Address * new_3id(SymTabEntry * symbol) {
	Address * a = new Address(symbol->name, ID3);
	a->table_id = symbol->id;
	tac_info_table.insert({a->table_id,TacInfo(symbol)});
	mmu.memory_locations.insert({a->table_id,create_memory_location( symbol->id, symbol->offset )});
	if ( symbol->type.is_ea() ) {
		set_is_ea( symbol->id );
	}
	return a;
}

std::ostream& operator<<(std::ostream& os, const Address& a){
	if ( a.type == ID3 ) {
		os << a.name;
	} else {
		os << a.name;
	}
	return os;
}
void backpatch(std::vector<GoTo*> & go_v, Label* label){
	if ( label == nullptr ) {
		return;
	}
	for ( auto it = go_v.begin(); it != go_v.end(); it++ ) {
		(*it)->label=label;
		label->reference_count++;
		//label->references.push_back(*it);
	}
	return ;
}

void backpatch(GoTo * _goto, Label* label){
	if ( label == nullptr ) {
		return;
	}
	_goto->label=label;
	label->reference_count++;
	//label->references.push_back(_goto);
	return ;
}

void append( std::vector <GoTo *> & v1, std::vector <GoTo *> & v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
}


ThreeAC::ThreeAC() : instr ( get_next_instr() ), bb_no(0) , dead(true) { };

ThreeAC::ThreeAC(bool no_add ) : instr ( instructions ), dead(true) { };

ThreeAC::~ThreeAC () {};

unsigned long long get_next_instr() {
	return instructions++;
}

GoTo::GoTo () : label(nullptr) , res({nullptr, true, nullptr}) { dead = false; };

GoTo::~GoTo () {
	delete label;
	delete res.addr;
}

GoTo * create_new_goto() {
	GoTo * _goto = new GoTo();
	//std::cout << "3AC: " << *_goto <<"\n";
	ta_code.push_back(_goto);
	return _goto;
}

GoTo * create_new_goto( Label * label) {
	GoTo * _goto = new GoTo();
	_goto->label = label;
	label->reference_count++;
	//std::cout << "3AC: " << *_goto << "\n";
	ta_code.push_back(_goto);
	return _goto;
}

GoTo * create_new_goto_cond( Address * res, bool condition ) {
	GoTo * _goto = new GoTo();
	_goto->res.addr = res;
	_goto->condition = condition;
	//std::cout << "3AC: " << *_goto << "\n";
	if ( res != nullptr ) {
		res->ta_instr->dead = false;
	}
	ta_code.push_back(_goto);
	return _goto;
}
		
void GoTo::set_res( Address * _res ){
	res.addr = _res;
	if ( res.addr != nullptr && res.addr->ta_instr != nullptr ) {
		res.addr->ta_instr->dead = false;
	}
	
}

std::string GoTo::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const GoTo& g){

	os << g.instr <<": ";
	if ( g.res.addr == nullptr ) {
		os << "goto";
	} else {
		if (g.condition == true ) {
			os << "br.true " << *(g.res.addr);
		} else {
			os << "br.false " << *(g.res.addr);
		}

		if ( g.res.addr->type != CON ) {
			if ( g.res.alive ) {
				os << "(L)";
			} else {
				os << "(D)";
			}
		}
	} 
	if ( g.label == nullptr ) {
		os << " -----";
		return os;
	}

	os << " " << *(g.label);
	return os;
}

Arg::Arg( Address * _addr, int _num ) : ThreeAC(), arg({_addr, true, nullptr}), num(_num)  { 
	dead = false;
	if( arg.addr != nullptr && arg.addr->ta_instr!=nullptr) {
		arg.addr->ta_instr->dead = false;
	}

};

Arg * create_new_arg( Address * addr, int num ) {
	Arg * a = new Arg(addr, num);
	ta_code.push_back(a);
	return a;
}

std::string Arg::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Arg& a){
	os << a.instr <<": ";
	if ( a.arg.addr != nullptr ) {
		os << "arg " << a.num << " = ";	
	
		os << *a.arg.addr;

		if ( a.arg.alive ) { 
			os << "(L)"; 
		} else { 
			os << "(D)";
		}

	}
	return os;

}


Call::Call( Address * _addr, std::string f_name ) : ThreeAC(), retval({_addr, true, nullptr}), function_name( f_name)  {dead = false;};

Call * create_new_call( Address * addr, std::string f_name ) {
	Call * c = new Call(addr, f_name);
	ta_code.push_back(c);
	return c;
}

std::string Call::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}


std::ostream& operator<<(std::ostream& os, const Call& c){
	os << c.instr <<": ";
	if ( c.retval.addr != nullptr ) {
		os << *c.retval.addr;

		assert ( c.retval.addr->type != CON );
		
		if ( c.retval.alive ) { 
			os << "(L)"; 
		} else { 
			os << "(D)";
		}

		os << " = ";
	}
	os << "call " << c.function_name; 
	return os;

}


Return::Return( Address * _retval ) : ThreeAC(), retval({_retval, true, nullptr}) {
	dead = false;
	if( retval.addr != nullptr && retval.addr->ta_instr!=nullptr) {
		retval.addr->ta_instr->dead = false;
	}
}

Return::~Return() {};

std::string Return::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Return& r){
	os << r.instr << ": ";

	if ( r.retval.addr == nullptr ) {
		os << "return";
		return os;
	}

	os << "return " << *r.retval.addr;

	if ( r.retval.addr->type != CON ) {

		if ( r.retval.alive ) {
			os << "(L)";
		} else {
			os << "(D)";
		}
	}

	return os;
}

Return * create_new_return( Address * retval ){
	Return * _return = new Return(retval);
	ta_code.push_back(_return);
	return _return;
}


TacInfo::TacInfo() : alive(true), next_use(nullptr), symbol(nullptr) {};
TacInfo::TacInfo(SymTabEntry * _symbol) : alive(true), next_use(nullptr), symbol(_symbol) {};
TacInfo::TacInfo( bool _alive ) : alive(_alive), next_use(nullptr), symbol(nullptr) {};



TacInfo * create_tac_info(SymTabEntry * symbol){
	TacInfo * t = new TacInfo();
	t->symbol = symbol;
	return t;
}

// int eval(){
	
// }



void arithmetic_optimise(Quad* q){
	std:: string val1="";
	std:: string val2="";
	if(q->arg1.addr==nullptr || q->arg2.addr==nullptr){
		if(q->arg1.addr!=nullptr && q->result.addr->name == q->arg1.addr->name && q->operation =="="){
			q->dead=true;
		}
		return;
	}


	if(q->arg1.addr!=nullptr){
	val1= q->arg1.addr->name;
	}
	if(q->arg2.addr!=nullptr){
	val2 = q->arg2.addr->name;
	}

	// constant folding
	if(q->arg1.addr->type == CON && q->arg2.addr->type == CON){
		
		int x = std::stoi(q->arg1.addr->name);
		int y = std::stoi(q->arg2.addr->name);
		std::string oper=q->operation;
		if(oper=="+"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name = std::to_string(x+y);
		}
		else if(oper=="-"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x-y);	
		}
		else if(oper=="*"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x*y);
		}
		else if(oper=="/"){
			
			if(y==0){
				error_msg("Division by zero not possible",line_num);
				return;
			}
			else{
				q->arg2.addr=nullptr;
				q->arg1.addr->name=std::to_string(x/y);
			}
		}
		else if(oper==">>"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x>>y);
		}
		else if(q->operation =="<<"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x<<y);
		}
		else if(oper=="|"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x|y);
		}
		else if(oper=="&"){
			q->arg2.addr=nullptr;
			q->arg1.addr->name=std::to_string(x&y);
		}
		q->operation="";
		if(q->result.addr->type==TEMP){
			q->dead=true;
			*q->result.addr = *q->arg1.addr;
		}
	}
	else{
		
	//+-0
		if((q->arg2.addr->name=="0" || q->arg2.addr->name=="0.0") && (q->operation=="+" || q->operation=="-")){
			q->operation="";			
			q->arg2.addr=nullptr;
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		else if((q->arg1.addr->name=="0" || q->arg1.addr->name=="0.0") && (q->operation=="+" || q->operation=="-")){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2.addr=nullptr;

			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		//*0
		
		else if((q->arg2.addr->name=="0" || q->arg2.addr->name=="0.0") && q->operation=="*"){
			q->operation="";
			q->arg2.addr=nullptr;
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		else if((q->arg1.addr->name=="0" || q->arg1.addr->name=="0.0") && q->operation=="*"){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2.addr=nullptr;
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		//"0/0"
		else if((q->arg2.addr->name=="0" || q->arg2.addr->name=="0.0") && q->operation=="/"){
			error_msg( "Division by zero not possible:", line_num );
		}
		else if((q->arg1.addr->name=="0" || q->arg1.addr->name=="0.0") && q->operation=="/"){
			q->operation="";
			q->arg2.addr=nullptr;
			q->arg1.addr->name="0";
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		// /*1 
		
		else if((q->arg2.addr->name=="1" || q->arg2.addr->name=="1.0") && (q->operation=="*"|| q->operation=="/")){
			q->operation="";
			q->arg2.addr=nullptr;
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		else if((q->arg1.addr->name=="1" || q->arg1.addr->name=="1.0") && q->operation=="*"){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2.addr=nullptr;
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		//2*x
		else if(q->arg2.addr->type==CON && ceil(log2(std::stoi(q->arg2.addr->name))) == floor(log2(std::stoi(q->arg2.addr->name))) && q->operation=="*"){
			q->operation="<<";
			q->arg2.addr->name=std::to_string((int)ceil(log2(stoi(q->arg2.addr->name))));
		}
		else if(q->arg1.addr->type==CON && ceil(log2(std::stoi(q->arg1.addr->name))) == floor(log2(std::stoi(q->arg1.addr->name))) && q->operation=="*"){
			q->operation="<<";
			q->arg1=q->arg2;
			q->arg2.addr->name=std::to_string((int)ceil(log2(stoi(q->arg1.addr->name))));
		}
		//0.5x
		else if(q->arg2.addr->type==CON && ceil(log2(std::stoi(q->arg2.addr->name))) == floor(log2(std::stoi(q->arg2.addr->name))) && q->operation=="/"){
			q->operation=">>";
			q->arg2.addr->name=std::to_string((int)ceil(log2(stoi(q->arg2.addr->name))));
		}
		//x/x
		else if(q->arg2.addr->name==q->arg1.addr->name && q->operation=="/"){
			q->operation="";
			q->arg2.addr=nullptr;
			q->arg1.addr->name="1";
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
		//x-x
		else if(q->arg2.addr->name==q->arg1.addr->name && q->operation=="-"){
			q->operation="";
			q->arg2.addr=nullptr;
			q->arg1.addr->name="0";
			*q->result.addr = *q->arg1.addr;
			q->dead=true;
		}
	}
}
/*
void repeat_var(Quad* q){
	if(q->result.addr!=nullptr){
		if(var_rep.find(q->result.addr->name)==var_rep.end()){
		var_rep.insert(q->result.addr->name);
		}
	}
	if(q->arg1.addr!=nullptr){
		if(var_rep.find(q->arg1.addr->name)!=var_rep.end()){
		var_rep.erase(q->arg1.addr->name);
		}
	}
	if(q->arg2.addr!=nullptr){
		if(var_rep.find(q->arg2.addr->name)!=var_rep.end()){
		var_rep.erase(q->arg2.addr->name);
		}
	}
}
*/

SaveLive::SaveLive() : ThreeAC(false) , save_temps(true) { dead = false;};


SaveLive * create_new_save_live() {
	SaveLive * s = new SaveLive();
	ta_code.push_back(s);
	return s;
}

std::string SaveLive::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const SaveLive& s){
	os << "Save Live";
	return os;
}




/*
void print_rep(){
for(auto i:var_rep){
	std::cout<<i<<" is redundant.\n";
	}
}
*/

void optimise_pass1() {
	GoTo * _goto1 = nullptr;
	GoTo * _goto2 = nullptr;
	Label * label1 = nullptr;
	Label * label2 = nullptr;
	Quad * quad = nullptr;
//	int fl;
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		if ( (*it)->dead == true ) {
			continue;
		}
		_goto1 = dynamic_cast<GoTo *>(*it); 
		if ( _goto1 != nullptr && _goto1->res.addr != nullptr && _goto1->res.addr->type == CON) {
			int value = std::stoi(_goto1->res.addr->name);
			if (( value == 0 && _goto1->condition == false) || (value != 0 && _goto1->condition == true )){
				// Always taken;
				;
			} else {
				// Never taken;
				_goto1->label->reference_count--;
				_goto1->dead = true;
				continue;
			}
			
			if( _goto1->res.addr->ta_instr != nullptr ) {
				_goto1->res.addr = nullptr;
				_goto1->res.addr->ta_instr->dead = true;
			} else {
				delete _goto1->res.addr;
				_goto1->res.addr = nullptr;
			}

		}


		label1 = dynamic_cast<Label* >(*it);
		if( label1 != nullptr && label1->reference_count == 0 ) {
			(*it)->dead = true;
			continue;
		}
		if ( _goto2 != nullptr && _goto2->res.addr == nullptr && ( label1 == nullptr || label1->reference_count == 0)  ) {
			(*it)->dead = true;
			if ( _goto1 != nullptr ) {
				_goto1->label->reference_count--;
			}
			continue;
		}

		if ( label2 != nullptr && label1 != nullptr ) {
			(*it)->dead = true;
			*label1 = *label2;
			label1->dead = true;
			continue;
		}

		if ( label2 != nullptr && _goto1 != nullptr && _goto1->res.addr == nullptr ) {
			(*it)->dead = true;
			label2->name = _goto1->label->name;
			label2->instruction_id = _goto1->label->instruction_id;
			_goto1->label->reference_count += label2->reference_count;
			label2->dead = true;
			_goto1->dead = true;
			continue;
		}

		quad = dynamic_cast<Quad *>(*it);
		if (quad!=nullptr){
			arithmetic_optimise(quad);
			;
			
		}
		_goto2 = _goto1; 
		label2 = label1;
	}
}

void dump_and_reset_3ac( ) {
	optimise_pass1();
	create_basic_blocks();
	create_next_use_info();
	
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		std::cout << "3AC: " << (*it)->bb_no << ":  " << (*it)->print();
		if( (*it)->dead == true ) {
			std::cout << " xxxx";
		}
		std::cout << "\n";
	}
	gen_asm_code( );
	//print_rep();
	instructions = 1;
	temporaries = 1;
	ta_code.clear();
	tac_info_table.clear();
	//var_rep.clear();
}

void create_basic_blocks() {	
	unsigned int basic_blks = 1;
	bool incremented = true;
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		if ( (*it)->dead == true ) {
			continue;
		}
		Label * label = dynamic_cast<Label *> (*it);
		if ( label != nullptr && !incremented ) {
			basic_blks++;
			(*it)->bb_no = basic_blks;
			continue;
		}
		incremented = false;
		(*it)->bb_no = basic_blks;
		GoTo * _goto = dynamic_cast<GoTo *> (*it);
		Return * _return = dynamic_cast<Return *>(*it);
		Call * call = dynamic_cast<Call *>(*it);
		if(   _goto != nullptr || _return != nullptr ) {
			basic_blks++;
			incremented = true;
		}
	}
}

void create_next_use_info(){
	auto it = ta_code.rbegin();
	unsigned int basic_blk = (*it)->bb_no;
	for ( ; it != ta_code.rend(); it++ ){
		if ( (*it)->dead == true ) {
			continue;
		}

		if ((*it)->bb_no != basic_blk ) {
			reset_tac_info_table();
			basic_blk = (*it)->bb_no;
		}

		Quad * q = dynamic_cast<Quad *>(*it);

		if ( q != nullptr ) {
		
	//		if ( q->arg1.addr != nullptr && q->arg1.addr->type != CON ) {
	//			auto it = get_entry_from_table(q->arg1.addr);
	//			q->arg1.alive = it->second.alive;
	//			q->arg1.next_use = it->second.next_use;
	//		}
	//		
	//		if ( q->arg2.addr != nullptr && q->arg2.addr->type != CON ) {
	//			auto it = get_entry_from_table(q->arg2.addr);
	//			q->arg2.alive = it->second.alive;
	//			q->arg2.next_use = it->second.next_use;
	//		}
			
			if ( q->result.addr != nullptr ) {
				assert(q->result.addr->type != CON);
				auto it = get_entry_from_table(q->result.addr);
				q->result.alive = it->second.alive;
				q->result.next_use = it->second.next_use;
				it->second.alive = false;
				it->second.next_use = nullptr;

			}
			if ( q->arg1.addr != nullptr && q->arg1.addr->type != CON ) {
				auto it = get_entry_from_table(q->arg1.addr);
				q->arg1.alive = it->second.alive;
				q->arg1.next_use = it->second.next_use;
				it->second.alive = true;
				it->second.next_use = &q->arg1;
			}

			if ( q->arg2.addr != nullptr && q->arg2.addr->type != CON ) {
				auto it = get_entry_from_table(q->arg2.addr);
				q->arg2.alive = it->second.alive;
				q->arg2.next_use = it->second.next_use;
				it->second.alive = true;
				it->second.next_use = &q->arg2;
			}

			continue;	

		}

		GoTo * g = dynamic_cast<GoTo *>(*it);

		if ( g != nullptr && g->res.addr != nullptr && g->res.addr->type != CON ) {
			auto it = get_entry_from_table(g->res.addr);
			g->res.alive = it->second.alive;
			g->res.next_use = it->second.next_use;
			it->second.alive = true;
			it->second.next_use = &g->res;
			continue;

		}
		
		Return * r = dynamic_cast<Return *>(*it);

		if ( r != nullptr && r->retval.addr != nullptr && r->retval.addr->type != CON ) {
			auto it = get_entry_from_table(r->retval.addr);
			r->retval.next_use = it->second.next_use;
			if ( it->second.next_use == nullptr ) {
				r->retval.alive = false;
			} else {
				r->retval.alive = true;
			}
			it->second.alive = false;
			it->second.next_use = &r->retval;
			continue;
		}


		Call * c = dynamic_cast<Call *>(*it);
		if ( c != nullptr && c->retval.addr != nullptr && c->retval.addr->type != CON ) {
			auto it = get_entry_from_table(c->retval.addr);
			c->retval.alive = it->second.alive;
			c->retval.next_use = it->second.next_use;
			it->second.alive = false;
			it->second.next_use = nullptr;
			continue;
		}

		Arg * a = dynamic_cast<Arg *>(*it);
		if ( a != nullptr && a->arg.addr != nullptr && a->arg.addr->type != CON ) {
			auto it = get_entry_from_table(a->arg.addr);
			a->arg.alive = it->second.alive;
			a->arg.next_use = it->second.next_use;
			it->second.alive = true;
			it->second.next_use = &a->arg;
			continue;
		}
	}
}

void reset_tac_info_table() {

	for ( auto it = tac_info_table.begin(); it != tac_info_table.end(); it++ ) {
		if ( it->first & TEMP_ID_MASK ) {
			it->second.alive = false;
		} else {
			it->second.alive = true;
		}
		it->second.next_use = nullptr;
		
	}
}

std::map <unsigned int, TacInfo >::iterator  get_entry_from_table( Address * a ) {
	auto it = tac_info_table.find( a->table_id );
	if ( it == tac_info_table.end() ) {
		assert(0);
	}
	return it;
}
