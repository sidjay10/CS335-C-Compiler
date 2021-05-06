#include <ast.h>
#include <iostream>
#include <expression.h>
#include <symtab.h>
#include <sstream>
#include <3ac.h>

unsigned long long instructions = 1;
unsigned long long labels = 1;

std::vector< ThreeAC * > ta_code;


Label::Label() : ThreeAC(false) {
	dead = false;
	name = "L"+std::to_string(labels++);
	instructions_id = instructions;
}

Label::~Label() {}

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

Address::Address(std::string _name, ADD_TYPE _type ) : name (_name) , type(_type), ta_instr(nullptr) {};

Address * new_temp() {
	return new Address("t" + std::to_string(temporaries++), TEMP);
}

Address * new_mem() {
	return new Address("t" + std::to_string(temporaries++), MEM);
}


Address * new_3id(std::string id){
	return new Address(id, ID3);
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
	}
	return ;
}

void append( std::vector <GoTo *> & v1, std::vector <GoTo *> & v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
}


ThreeAC::ThreeAC() : instr ( get_next_instr() ), dead(true) { };

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


void dead_code_eliminate() {
	GoTo * _goto = nullptr;
	Label * label = nullptr;
	Label * label1 = nullptr;
	Label * label2 = nullptr;
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		label = dynamic_cast<Label* >(*it);
		if ( _goto != nullptr && _goto->res == nullptr && label == nullptr ) {
			(*it)->dead = true;
			continue;
		} else {
			_goto = nullptr;
		}
		
		if( _goto == nullptr ) {
			_goto = dynamic_cast<GoTo *>(*it); 
		}

		if ( label1 != nullptr && label != nullptr ) {
			(*it)->dead = true;
			*label = *label1;
			label->dead = true;
		}
		label1 = label;

	}

}

void dump_and_reset_3ac() {


	dead_code_eliminate();
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		std::cout << "3AC: " << (*it)->print();
		if( (*it)->dead == true ) {
			std::cout << " xxxx";
		}
		std::cout << "\n";
	}
	instructions = 1;
	temporaries = 1;
	ta_code.clear();
}
