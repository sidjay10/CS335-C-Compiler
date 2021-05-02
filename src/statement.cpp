#include <algorithm>
#include <assert.h>
#include <ast.h>
#include <iostream>
#include <list>
#include <sstream>
#include <expression.h>
#include<statement.h>
#include <symtab.h>
#include <vector>
#include <y.tab.h>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <iterator>
#include <3ac.h>

extern unsigned int line_num;
extern unsigned int column;

Type INVALID_TYPE;


//##############################################################################
//############################# STATEMENT #####################################
//##############################################################################







// ----------Statement List
Statement* create_statement_list(StatementList* slist,Statement* s1){

}

/// ------------Selection Statement-----------------------

Statement *create_selection_statement_if(std::string st,Expression *ex1, Statement *st1,Statement *st2){
    if(st2==nullptr){
        SelectionStatement* S = new SelectionStatement();
        
        S->name=st;
        S->add_children(ex1,st1);
    }
    else{
        SelectionStatement* S = new SelectionStatement();
        
        S->name=st;
        S->add_children(ex1,st1,st2);
    }
}

Statement *create_selection_statement_switch(std::string st,Expression *ex1, Statement *st1);
