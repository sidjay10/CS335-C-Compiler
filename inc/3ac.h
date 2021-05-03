#ifndef __3AC_H__
#define __3AC_H__

#include <ast.h>
#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <symtab.h>

extern unsigned long long instructions;
unsigned long long get_next_instr();

extern unsigned long long temporaries;

typedef enum ADD_TYPE_ {
	ID3 = 1,
	CON = 2,
	TEMP = 3,
	MEM = 4
} ADD_TYPE;

class Address {
public:
	std::string name;
	ADD_TYPE type;
	Address( std::string name, ADD_TYPE type);
	
};

std::ostream& operator<<(std::ostream& os, const Address& a);

extern unsigned long long instructions;
Address * new_temp();
Address * new_mem();

template <typename T>
Address * new_3const(T val) {
	return new Address(std::to_string(val), CON);
}


Address * new_3id(std::string id);

class ThreeAC {
public:
	unsigned int instr;

	ThreeAC();
	ThreeAC( bool no_add );

};


class Quad : public ThreeAC {
public:
	Address * result;
	std::string operation;
	Address * arg1;
	Address * arg2;
	Quad * next;

Quad ( Address * result, std::string operation, Address * arg1, Address * arg2 );

		
};

#define MEM_EMIT(a,b) \
	if ( (a)->res->type == MEM ) { \
		(b) = new_temp(); \
		emit((b),"()",(a)->res,nullptr); \
	} else if ( (a)->res->type == ID3 ) { \
		(b) = new_temp(); \
		emit((b),"()",(a)->res,nullptr); \
	} else { \
		(b) = (a)->res; \
	}


std::ostream& operator<<(std::ostream& os, const Quad& q);

int emit(  Address * result, std::string operation, Address * arg1, Address * arg2 );

class Label : public ThreeAC {
	public:
	std::string name;
	unsigned long long instructions_id;
	Label();
};

extern unsigned long long label_count;
Label * create_new_label();
 
class GoTo : public ThreeAC {
	public : 
		Label * label;
		GoTo();
		
};

GoTo * create_new_goto();
GoTo * create_new_goto(Label * label);


std::ostream& operator<<(std::ostream& os, const Label& l);

std::ostream& operator<<(std::ostream& os, const GoTo& g);

void backpatch( std::vector <GoTo * > & go_v, Label * label );

// Append v2 at the end of vector 1
void append ( std::vector <GoTo *> & v1, std::vector <GoTo *> & v2);
#endif
