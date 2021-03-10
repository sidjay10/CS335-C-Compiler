#ifndef __AST_H__
#define __AST_H__

#include <string>
#include <vector>
#include <fstream>

template<typename Base, typename T>
inline bool instanceof(const T*) {
   return std::is_base_of<Base, T>::value;
}

class Node {
	public:
		unsigned long long int id;
		int is_printed = 1;

		virtual void dotify() = 0;
		virtual void add_child(Node * node);
		virtual void add_children (Node * node1, Node * node2);
		virtual void add_children (Node * node1, Node * node2, Node * node3);
		virtual void add_children (Node * node1, Node * node2, Node * node3, Node * node4);
	protected:
		Node ();
		virtual ~Node() {}
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
		void add_children (Node * node1, Node * node2);
		void add_children (Node * node1, Node * node2, Node * node3);
		void add_children (Node * node1, Node * node2, Node * node3, Node * node4);
		void dotify();

};

unsigned long long int get_next_node_id();
Node * create_terminal(const char * name, const char * value);
Node * create_non_term(const char * name);
Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5, Node* node6, Node* node7);
Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5, Node* node6);
Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5);
Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4);
Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3);
Node * create_non_term(const char* name, Node* node1, Node* node2);
Node * create_non_term(const char* name, Node* node1);

void file_writer(std::string s);

#endif
