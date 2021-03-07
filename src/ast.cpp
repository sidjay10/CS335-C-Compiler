#include "ast.h"


#include<assert.h>
#include<string>
#include<iostream>
#include<sstream>

static unsigned long long int id_count = 0;

unsigned long long int get_next_node_id() {
	return id_count++;
}

Node::Node() : id(get_next_node_id()) {};

Terminal::Terminal(const char * name_, const char * value_) {
	name = std::string(name_);
//	std::cout << id << " " << name  << "\n";
	if (value_) value = std::string(value_);
}


Non_Terminal::Non_Terminal(const char * name_) {
	name = std::string(name_);
//	std::cout << id << " " << name  << "\n";
}


void Node::add_child (Node * node) {
	assert(0);
}

void Node::add_children (Node * node1, Node * node2) {
	assert(0);
}

void Node::add_children (Node * node1, Node * node2, Node * node3) {
	assert(0);
}


void Node::add_children (Node * node1, Node * node2, Node * node3, Node * node4) {
	assert(0);
}

void Terminal:: dotify () {
	std::stringstream ss;
	ss << "\t" << id << " [label=\"" << name << "\"];\n";
	std::cout << ss.str();
}
void Non_Terminal:: dotify () {
	std::stringstream ss;
	ss << "\t" << id << " [label=\"" << name << "\"];\n";
	for (auto it = children.begin(); it != children.end(); it++) {
		ss << "\t" << id << " -> " << (*it)->id << ";\n";
	}

	std::cout << ss.str();
}

void Non_Terminal::add_child (Node * node) {
	children.push_back(node);
}


void Non_Terminal::add_children (Node * node1, Node * node2) {
	children.push_back(node1);
	children.push_back(node2);
}

void Non_Terminal::add_children (Node * node1, Node * node2, Node * node3) {
	children.push_back(node1);
	children.push_back(node2);
	children.push_back(node3);
}

void Non_Terminal::add_children (Node * node1, Node * node2, Node * node3, Node * node4) {
	children.push_back(node1);
	children.push_back(node2);
	children.push_back(node3);
	children.push_back(node4);
}

Node * create_terminal(const char* name, const char * value) {
	Terminal * terminal_node = new Terminal(name,value);
	terminal_node->dotify();
	return terminal_node;
}


Node * create_non_term(const char* name) {
	Non_Terminal * node = new Non_Terminal(name);
	return node;
}

