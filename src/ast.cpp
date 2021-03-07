#include "ast.h"

#include<string>

static unsigned long long int id_count = 0;

unsigned long long int get_next_node_id() {
	return id_count++;
}

Node::Node() : id(get_next_node_id()) {};

Terminal::Terminal(const char * name_, const char * value_) {
	name = std::string(name_);
	if (value_) value = std::string(value_);
}


Non_Terminal::Non_Terminal(const char * name_) {
	name = std::string(name_);
}

void Terminal:: dotify () {}
void Non_Terminal:: dotify () {}

void Non_Terminal::add_child (Node * node) {}

Node * create_terminal(const char* name, const char * value) {
	Terminal * terminal_node = new Terminal(name,value);
	return terminal_node;
}


