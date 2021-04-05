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
	if (value_) value = std::string(value_);
}


Non_Terminal::Non_Terminal(const char * name_) {
	name = std::string(name_);
}




/* These should never be called in objects of type Node or Terminal */
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

/* Print in the appropriate format for the dot script */
void Terminal:: dotify () {
	if(is_printed){
		is_printed = 0;
		std::stringstream ss;
		ss << "\t" << id << " [label=\"" << name << " : " << value << "\"];\n";
		file_writer(ss.str());
	}
}

void Non_Terminal:: dotify () {
	if(is_printed){
		is_printed = 0;
		std::stringstream ss;
		ss << "\t" << id << " [label=\"" << name << "\"];\n";
		for (auto it = children.begin(); it != children.end(); it++) {
			ss << "\t" << id << " -> " << (*it)->id << ";\n";
		}
		file_writer(ss.str());

		for(auto it = children.begin(); it != children.end(); it++){
			(*it)->dotify();
		}
	}
}

void Non_Terminal::add_child (Node * node) {
	if( node != NULL) {
		children.push_back(node);
	}
}


void Non_Terminal::add_children (Node * node1, Node * node2) {
	if( node1 != NULL) {
		children.push_back(node1);
	}
	if( node2 != NULL) {
		children.push_back(node2);
	}
}

void Non_Terminal::add_children (Node * node1, Node * node2, Node * node3) {
	if( node1 != NULL) {
		children.push_back(node1);
	}
	if( node2 != NULL) {
		children.push_back(node2);
	}
	if( node3 != NULL) {
		children.push_back(node3);
	}
}

void Non_Terminal::add_children (Node * node1, Node * node2, Node * node3, Node * node4) {
	if( node1 != NULL) {
		children.push_back(node1);
	}
	if( node2 != NULL) {
		children.push_back(node2);
	}
	if( node3 != NULL) {
		children.push_back(node3);
	}
	if( node4 != NULL) {
		children.push_back(node4);
	}
}

Node * create_terminal(const char* name, const char * value) {
	Terminal * terminal_node = new Terminal(name,value);
	return terminal_node;
}


Node * create_non_term(const char* name) {
	Non_Terminal * node = new Non_Terminal(name);
	return node;
}

Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5, Node* node6, Node* node7) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2, node3, node4);
	node->add_children(node5, node6, node7);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}

Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5, Node* node6) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2, node3, node4);
	node->add_children(node5, node6);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}

Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4, Node* node5) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2, node3);
	node->add_children(node4, node5);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}




Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3, Node* node4) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2, node3, node4);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}

Node * create_non_term(const char* name, Node* node1, Node* node2, Node* node3) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2, node3);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}

Node * create_non_term(const char* name, Node* node1, Node* node2) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_children(node1, node2);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}

Node * create_non_term(const char* name, Node* node1) {
	Non_Terminal * node = new Non_Terminal(name);
	node->add_child(node1);
	if( node->children.empty() ) {
		delete node;
		return NULL;
	}
	return node;
}
