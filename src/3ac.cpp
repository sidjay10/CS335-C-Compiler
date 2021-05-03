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
	

Quad::Quad ( Address * _result, std::string _operation, Address * _arg1, Address * _arg2 ) : result(_result), operation(_operation), arg1(_arg1), arg2(_arg2) {};

Quad::~Quad () {
	delete result;
	delete arg1;
	delete arg2;

}

std::ostream& operator<<(std::ostream& os, const Quad& q){
	if ( q.arg2 != nullptr ) {
	os << q.instr << ": " << *q.result << " = " << *q.arg1 << " " << q.operation << " " << *q.arg2 ;
	} else if ( q.operation == "()" ) {
	os << q.instr << ": " << *q.result << " = " <<  "(" << *q.arg1 << ")" ;
	} else if ( q.operation == "()s" ) {
	os << q.instr << ": " << "(" << *q.result << ")" << " = " << *q.arg1;
	} else if ( q.operation == "=" ) {
	os << q.instr << ": " <<  *q.result  << " = " << *q.arg1;

	} else {
	os << q.instr << ": " << *q.result << " = " << q.operation << " " << *q.arg1 ;

	}
	return os;
}

unsigned long long emit(  Address * result, std::string operation, Address * arg1, Address * arg2 ) {
	Quad * q = new Quad(result,operation,arg1,arg2);
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

Address::Address(std::string _name, ADD_TYPE _type ) : name (_name) , type(_type) {};

Address * new_temp() {
	return new Address("t" + std::to_string(temporaries++), TEMP);
}

Address * new_mem() {
	return new Address("m" + std::to_string(temporaries++), MEM);
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


ThreeAC::ThreeAC() : instr ( get_next_instr() ){ };

ThreeAC::ThreeAC(bool no_add ) : instr ( instructions ){ };

ThreeAC::~ThreeAC () {};

unsigned long long get_next_instr() {
	return instructions++;
}

GoTo::GoTo () : label(nullptr) , res(nullptr) {};

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
	ta_code.push_back(_goto);
	return _goto;
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

void dump_and_reset_3ac() {
	for ( auto it = ta_code.begin(); it != ta_code.end(); it++ ){
		std::cout << "3AC: " << (*it)->print() << "\n";
	}
	instructions = 1;
	temporaries = 1;
	ta_code.clear();
}