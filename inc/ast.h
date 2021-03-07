#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <vector>

class Node {
	public:
		unsigned long long int id;
	protected:
		Node ();
};


class Terminal : public Node {
	public:
		std::string name;
		std::string value;
		Terminal(const char * name_, const char * value_);
		void dotify();
		
};

class Non_Terminal : public Node {
	public:
		std::string name;
		std::vector <Node *> children;

		Non_Terminal(const char * name_);
		void add_child(Node * node);
		void dotify();

};

unsigned long long int get_next_node_id();
Node * create_terminal(const char * name, const char * value);
#endif
