#include <ast.h>
#include <iostream>
#include <expression.h>
#include <symtab.h>
#include <sstream>
#include <3ac.h>
#include <y.tab.h>

unsigned long long instructions = 1;
unsigned long long labels = 1;

Label* getNextLabel(){
	Label* l = new Label();
	l->instructions_id = labels++;
	l->name = "L"+std::to_string(l->instructions_id)+":";
	return l;
}

std::vector< ThreeAC * > three_ac_code;

Quad::Quad ( Address * _result, std::string _operation, Address * _arg1, Address * _arg2 ) : result(_result), operation(_operation), arg1(_arg1), arg2(_arg2) {};

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

int emit(  Address * result, std::string operation, Address * arg1, Address * arg2 ) {
	Quad * q = new Quad(result,operation,arg1,arg2);
	three_ac_code.push_back(q);
	std::cout << "3AC: " << *q << "\n";
	return three_ac_code.size();

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
void backpatch(std::vector<GoTo*> go_v, Label* label){
	for(int i=0;i<go_v.size();i++){
		go_v[i]->instruction_id=label->instructions_id;
	}
	return ;
}

ThreeAC::ThreeAC() : instr ( get_next_instr() ){ };

ThreeAC::ThreeAC(bool no_add ) : instr ( instructions ){ };

unsigned long long get_next_instr() {
	return instructions++;
}