#include <ast.h>
#include <iostream>
#include <expression.h>
#include <symtab.h>
#include <sstream>
#include <3ac.h>
#include <vector>
#include <assert.h>
#include <algorithm>

unsigned long long instructions = 1;
unsigned long long labels = 1;

std::vector< ThreeAC * > ta_code;
std::map< unsigned int, TacInfo > tac_info_table;


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
	

Quad::Quad ( Address * _result, std::string _operation, Address * _arg1, Address * _arg2 ) : result(_result), operation(_operation), arg1(_arg1), arg2(_arg2) { 
	if( arg1 != nullptr && arg1->ta_instr!=nullptr) {
		arg1->ta_instr->dead = false;
	}
	if( arg2 != nullptr && arg2->ta_instr != nullptr) {
		arg2->ta_instr->dead = false;
	}
	if ( operation == "()s" ) {
		result->ta_instr->dead = false;
		dead = false;
	}
};

Quad::~Quad () {
	delete result;
	delete arg1;
	delete arg2;

}

std::ostream& operator<<(std::ostream& os, const Quad& q){
	if ( q.operation == "call" && q.result == nullptr ) {
		os << q.instr << ": " << "call " << *q.arg1 << " " << *q.arg2;
	} else if ( q.operation == "call") {
		os << q.instr << ": " << *q.result << " = call " << *q.arg1 << " " << *q.arg2;
	} else if ( q.arg2 != nullptr ) {
		os << q.instr << ": " << *q.result << " = " << *q.arg1 << " " << q.operation << " " << *q.arg2 ;
	} else if ( q.operation == "()" ) {
		os << q.instr << ": " << *q.result << " = " <<  "(" << *q.arg1 << ")" ;
	} else if ( q.operation == "()s" ) {
		os << q.instr << ": " << "(" << *q.result << ")" << " = " << *q.arg1;
	} else if ( q.operation == "=" ) {
		os << q.instr << ": " <<  *q.result  << " = " << *q.arg1;
	} else if ( q.operation == "=s" ) {
		os << q.instr << ": " <<  *q.result  << " = " << *q.arg1;
	} else if ( q.operation == "push" ) {
		os << q.instr << ": " <<  "push " << *q.arg1;
	} else if ( q.operation.substr(0,3) == "arg" ) {
		os << q.instr << ": " << q.operation  << " " << *q.arg1;
	} else {
		os << q.instr << ": " << *q.result << " = " << q.operation << " " << *q.arg1 ;
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

Address::Address(std::string _name, ADD_TYPE _type ) : name (_name) , type(_type), ta_instr(nullptr), alive(true), next_use(nullptr)  {};

Address * new_temp() {
	Address * t = new Address("t" + std::to_string(temporaries), TEMP );
	t->table_id = TEMP_ID_MASK | temporaries;
	temporaries++;
	tac_info_table.insert({t->table_id,TacInfo()});
	return t;
}

Address * new_mem() {
	Address * t = new Address("t" + std::to_string(temporaries), MEM );
	t->table_id = TEMP_ID_MASK | temporaries;
	temporaries++;
	tac_info_table.insert({t->table_id,TacInfo()});
	return t;
}


Address * new_3id(SymTabEntry * symbol) {
	Address * a = new Address(symbol->name, ID3);
	a->table_id = symbol->id;
	tac_info_table.insert({a->table_id,TacInfo(symbol)});
	return a;
}

std::ostream& operator<<(std::ostream& os, const Address& a){
	if ( a.type == ID3 ) {
		os << a.name;
	} else {
		os << a.name;
	}

	if ( a.alive == true ) {
		os << "(L)";
	} else {
		os << "(D)";
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


ThreeAC::ThreeAC() : instr ( get_next_instr() ), dead(true) , bb_no(0) { };

ThreeAC::ThreeAC(bool no_add ) : instr ( instructions ), dead(true) { };

ThreeAC::~ThreeAC () {};

unsigned long long get_next_instr() {
	return instructions++;
}

GoTo::GoTo () : label(nullptr) , res(nullptr) { dead = false; };

GoTo::~GoTo () {
	delete label;
	delete res;
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
	//std::cout << "3AC: " << *_goto << "\n";
	ta_code.push_back(_goto);
	return _goto;
}

GoTo * create_new_goto_cond( Address * res, bool condition ) {
	GoTo * _goto = new GoTo();
	_goto->res = res;
	_goto->condition = condition;
	//std::cout << "3AC: " << *_goto << "\n";
	if ( res != nullptr ) {
		res->ta_instr->dead = false;
	}
	ta_code.push_back(_goto);
	return _goto;
}
		
void GoTo::set_res( Address * _res ){
	res = _res;
	if ( res != nullptr && res->ta_instr != nullptr ) {
		res->ta_instr->dead = false;
	}
	
}

std::string GoTo::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const GoTo& g){

	os << g.instr <<": ";
	if ( g.res == nullptr ) {
		os << "goto";
	} else {
		if (g.condition == true ) {
			os << "br.true " << *(g.res);
		} else {
			os << "br.false " << *(g.res);
		}
	} 
	if ( g.label == nullptr ) {
		os << " -----";
		return os;
	}

	os << " " << *(g.label);
	return os;
}


Return::Return() : ThreeAC(), retval(nullptr) {dead = false;};

Return::~Return() {};

std::string Return::print() {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Return& r){
	if ( r.retval == nullptr ) {
		os << "return";
		return os;
	}

	os << "return " << *r.retval;
	return os;
}

Return * create_new_return( Address * retval ){
	Return * _return = new Return();
	_return->retval = retval;
	ta_code.push_back(_return);
	return _return;
}


TacInfo::TacInfo() : alive(true), next_use(nullptr), symbol(nullptr) {};
TacInfo::TacInfo(SymTabEntry * _symbol) : alive(true), next_use(nullptr), symbol(_symbol) {};



TacInfo * create_tac_info(SymTabEntry * symbol){
	TacInfo * t = new TacInfo();
	t->symbol = symbol;
	return t;
}

// int eval(){
	
// }

int arithmetic_optimisation(Quad* q){
	std:: string val1="";
	std:: string val2="";
	if(q->arg1!=nullptr){
	val1= q->arg1->name;
	}
	if(q->arg2!=nullptr){
	val2 = q->arg2->name;
	}

	// constant folding
	if(q->arg1->type == CON && q->arg2->type == CON){
		
		int x = std::stoi(q->arg1->name);
		int y = std::stoi(q->arg2->name);
		std::string oper=q->operation;
		if(oper=="+"){
			q->arg2=nullptr;
			q->arg1->name = std::to_string(x+y);
		}
		else if(oper=="-"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x-y);	
		}
		else if(oper=="*"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x*y);
		}
		else if(oper=="/"){
			q->arg2=nullptr;
			if(y==0){
				error_msg("Division by zero not possible",line_num);
			}
			q->arg2->name=std::to_string(x/y);
		}
		else if(oper==">>"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x>>y);
		}
		else if(q->operation =="<<"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x<<y);
		}
		else if(oper=="|"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x|y);
		}
		else if(oper=="&"){
			q->arg2=nullptr;
			q->arg1->name=std::to_string(x&y);
		}
		q->operation="";
		if(q->result->type==TEMP){
			q->dead=true;
		}
	}
	else{
		q->dead=true;
	//+-0
		if((q->arg2->name=="0" || q->arg2->name=="0.0") && (q->operation=="+" || q->operation=="-")){
			q->operation="";
			q->arg2=nullptr;
		}
		else if((q->arg1->name=="0" || q->arg1->name=="0.0") && (q->operation=="+" || q->operation=="-")){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2=nullptr;
		}
		//*0
		
		else if((q->arg2->name=="0" || q->arg2->name=="0.0") && q->operation=="*"){
			q->operation="";
			q->arg2=nullptr;
		}
		else if((q->arg1->name=="0" || q->arg1->name=="0.0") && q->operation=="*"){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2=nullptr;
		}
		//"0/0"
		else if((q->arg2->name=="0" || q->arg2->name=="0.0") && q->operation=="/"){
			error_msg( "Division by zero not possible:", line_num );
		}
		else if((q->arg1->name=="0" || q->arg1->name=="0") && q->operation=="/"){
			q->operation="";
			q->arg2=nullptr;
		}
		//*/1
		else if((q->arg2->name=="1" || q->arg2->name=="1.0") && (q->operation=="*"|| q->operation=="/")){
			q->operation="";
			q->arg2=nullptr;
		}
		else if((q->arg1->name=="1" || q->arg1->name=="1.0") && q->operation=="*"){
			q->operation="";
			q->arg1=q->arg2;
			q->arg2=nullptr;
		}
		//2*x
		else if((q->arg2->name=="2" || q->arg2->name=="2.0") && q->operation=="*"){
			q->operation="+";
			q->arg2=q->arg1;
		}
		else if((q->arg1->name=="2" || q->arg1->name=="2.0") && q->operation=="*"){
			q->operation="+";
			q->arg1=q->arg2;
		}	
		//0.5x
		else if((q->arg2->name=="2" || q->arg2->name=="2.0") && q->operation=="/"){
			q->operation=">>";
			q->arg2->name="1";
		}
		else{
			q->dead=false;
		}

	}
}

void optimise_pass1() {
	GoTo * _goto1 = nullptr;
	GoTo * _goto2 = nullptr;
	Label * label1 = nullptr;
	Label * label2 = nullptr;
	Quad * q = nullptr;
	int fl;
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		if ( (*it)->dead == true ) {
			continue;
		}
		_goto1 = dynamic_cast<GoTo *>(*it); 
		if ( _goto1 != nullptr && _goto1->res != nullptr && _goto1->res->type == CON) {
			int value = std::stoi(_goto1->res->name);
			if (( value == 0 && _goto1->condition == false) || (value != 0 && _goto1->condition == true )){
				// Always taken;
				;
			} else {
				// Never taken;
				_goto1->label->reference_count--;
				_goto1->dead = true;
				continue;
			}
			
			if( _goto1->res->ta_instr != nullptr ) {
				_goto1->res = nullptr;
				_goto1->res->ta_instr->dead = true;
			} else {
				delete _goto1->res;
				_goto1->res = nullptr;
			}

		}


		label1 = dynamic_cast<Label* >(*it);
		if( label1 != nullptr && label1->reference_count == 0 ) {
			(*it)->dead = true;
			continue;
		}
		if ( _goto2 != nullptr && _goto2->res == nullptr && ( label1 == nullptr || label1->reference_count == 0)  ) {
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
		if(label1==nullptr && _goto1==nullptr){
			q = dynamic_cast<Quad *>(*it);
			if(q!=nullptr && q->arg1!=nullptr && q->arg2!=nullptr){
				fl=arithmetic_optimisation(q);
			if (fl==1) (*it)->dead=true;
			}
		}

		_goto2 = _goto1; 
		label2 = label1;
	}
}

void dump_and_reset_3ac() {

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
	instructions = 1;
	temporaries = 1;
	ta_code.clear();
	tac_info_table.clear();
}

void create_basic_blocks() {	
	int basic_blks = 1;
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
		if( _goto != nullptr || _return != nullptr ) {
			basic_blks++;
			incremented = true;
		}
	}
	
}

void create_next_use_info(){
	auto it = ta_code.rbegin();
	int basic_blk = (*it)->bb_no;
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
		
			if ( q->arg1 != nullptr && q->arg1->type != CON ) {
				auto it = get_entry_from_table(q->arg1);
				q->arg1->alive = it->second.alive;
				q->arg1->next_use = it->second.next_use;
			}
			
			if ( q->arg2 != nullptr && q->arg2->type != CON ) {
				auto it = get_entry_from_table(q->arg2);
				q->arg2->alive = it->second.alive;
				q->arg2->next_use = it->second.next_use;
			}
			
			if ( q->result != nullptr ) {
				assert(q->result->type != CON);
				auto it = get_entry_from_table(q->result);
				q->result->alive = it->second.alive;
				q->result->next_use = it->second.next_use;
				it->second.alive = false;
				it->second.next_use = nullptr;

			}
			if ( q->arg1 != nullptr && q->arg1->type != CON ) {
				auto it = get_entry_from_table(q->arg1);
				it->second.alive = true;
				it->second.next_use = q;
			}

			if ( q->arg2 != nullptr && q->arg2->type != CON ) {
				auto it = get_entry_from_table(q->arg2);
				it->second.alive = true;
				it->second.next_use = q;
			}

			continue;	

		}

		GoTo * g = dynamic_cast<GoTo *>(*it);

		if ( g != nullptr && g->res != nullptr && g->res->type != CON ) {
			auto it = get_entry_from_table(g->res);
			it->second.alive = true;
			it->second.next_use = g;
			continue;

		}
		
		Return * r = dynamic_cast<Return *>(*it);

		if ( r != nullptr && r->retval != nullptr && r->retval->type != CON ) {
			auto it = get_entry_from_table(r->retval);
			it->second.alive = true;
			it->second.next_use = r;
			continue;
		}
	}
}

void reset_tac_info_table() {

	for ( auto it = tac_info_table.begin(); it != tac_info_table.end(); it++ ) {
		it->second.alive = true;
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
