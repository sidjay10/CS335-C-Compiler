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

Statement *create_selection_statement_if( Expression *ex, Label * l1, Statement *st1, Goto * _goto, Label * l2, Statement *st2 ){
    if(st2==nullptr){
        SelectionStatement* S = new SelectionStatement();
        
	S->name = "IF";
        S->add_children(ex,st1);
	backpatch( ex.nextlist, l1 );
	S.nextlist.insert( S.nextlist.end(), ex.falselist.begin(), ex.falselist.end() );
	S.nextlist.insert( S.nextlist.end(), st1.nextlist.begin(), st1.nextlist.end() );
	
    }
    else{
	asser(l2 != nullptr );
	asser(_goto != nullptr );

        SelectionStatement* S = new SelectionStatement();
        
	S->name = "IF ELSE";
        S->add_children(ex,st1,st2);
	backpatch( ex.nextlist, l1 );
	backpatch( ex.falselist, l2 );
	S.nextlist.insert( S.nextlist.end(), ex1.falselist.begin(), ex1.falselist.end() );
	S.nextlist.insert( S.nextlist.end(), st1.nextlist.begin(), st1.nextlist.end() );
	S.nextlist.insert( _goto );
	S.nextlist.insert( S.nextlist.end(), st2.nextlist.begin(), st2.nextlist.end() );
    }
}

Statement *create_selection_statement_switch(std::string st,Expression *ex1, Statement *st1);
