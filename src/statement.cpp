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

std::map<std::string,Label *> label_iden;
std::map<std::string,std::vector<GoTo *> & > goto_iden;
std::map<std::string, Label *> switch_label;
Type* switch_type=nullptr;
Label* switch_temp=nullptr;
//##############################################################################
//############################# STATEMENT #####################################
//##############################################################################

Statement *create_selection_statement_if( Expression *ex, GoTo * _false, Label * l1, Statement *st1, GoTo * _goto, Label * l2, Statement *st2 ){
    SelectionStatement* S = new SelectionStatement();
    if(st2==nullptr){
		S->name = "IF";
        S->add_children(ex,st1);
		backpatch( ex->truelist, l1 );
		_false->set_res( ex->res ); 
		ex->falselist.push_back(_false);
		if( l2 != nullptr ) {
			backpatch( ex->falselist, l2 );
		}
		append(S->nextlist,ex->falselist);
		if ( st1 != nullptr ) {
			append(S->nextlist,st1->nextlist);
			append(S->breaklist,st1->breaklist);
			append(S->continuelist,st1->continuelist);
			append(S->caselist,st1->caselist);
		}
		if ( _goto != nullptr ) {
			S->nextlist.push_back( _goto );
		}
    }
	else {
		assert(l2 != nullptr );
		assert(_goto != nullptr );

		S->name = "IF ELSE";
    	S->add_children(ex,st1,st2);
		backpatch( ex->truelist, l1 );
		_false->set_res( ex->res ); 
		ex->falselist.push_back(_false);
		backpatch( ex->falselist, l2 );

		if ( st1 != nullptr ) {
			append(S->nextlist,st1->nextlist);
			append(S->breaklist,st1->breaklist);
			append(S->continuelist,st1->continuelist);
			append(S->caselist,st1->caselist);
		}
		S->nextlist.push_back( _goto );
		append(S->nextlist,st2->nextlist);
		append(S->breaklist,st2->breaklist);
		append(S->continuelist,st2->continuelist);
		append(S->caselist,st2->caselist);
    }
	switch_temp=nullptr;
    return S;
}


Statement *create_selection_statement_switch(GoTo * _test, Expression *ex1, Statement* st1, GoTo * _next ){
	SelectionStatement * S= new SelectionStatement();
	S->name="SWITCH";
	S->add_children(ex1,st1);
	Type t = ex1->type;
	if (! t.isInt() )
	{
		error_msg("Invalid Type " + t.get_name() + " in Switch Case",line_num);
	}

	S->nextlist.push_back(_next);

	Address *t1, *t2;
	t1=ex1->res;
	t2= new_temp();
	_test->label = create_new_label();
	Label * default_label = nullptr;
    for(auto i:switch_label){
		if (i.first == "" ) {
			default_label = i.second;
			continue;
		}
		Address * con = new_3const(0);
		con->name=i.first;
		emit( t2 , "==",t1,con );
		GoTo *_goto=create_new_goto_cond(t2,true);
		_goto->label=i.second;
	}
	if ( default_label != nullptr ) {
		create_new_goto(default_label);
	}
	switch_label.clear();
	switch_type=nullptr; 
	append(S->nextlist,st1->breaklist);
    append(S->nextlist,st1->nextlist);
	append(S->continuelist,st1->continuelist);
	append(S->caselist,st1->caselist);
	switch_temp=nullptr;
	return S;
}




/// ------------Expression Statement-----------------------//
Statement* create_expression_statement(Expression* ex1)
{
	if ( ex1 == nullptr ) {
		return nullptr;
	}
	ExpressionStatement *S= new ExpressionStatement();
	S->add_child(ex1);
	append(S->nextlist,ex1->truelist); 
	append(S->nextlist,ex1->falselist); 
	return S;
}


/// ------------ Iteration Statement --------------------//
Statement* create_iteration_statement_while(Label * l1, Expression *ex1, GoTo * _false, Label * l2, Statement *st1 ){
	IterationStatement * S = new IterationStatement();
	S->add_children(ex1,st1);
	S->name = "WHILE";

	backpatch(ex1->truelist,l2);
	_false->set_res( ex1->res ); 
	ex1->falselist.push_back(_false);
	append(S->nextlist,ex1->falselist);
	if ( st1 != nullptr ) {
		backpatch(st1->nextlist,l1);
		backpatch(st1->continuelist,l1);
		append(S->nextlist,st1->breaklist);
	}
	GoTo * _goto = create_new_goto(l1);

	return S;
	
}

Statement* create_iteration_statement_do_while(Label *l1,Statement *st1,Label *l2,Expression *ex1 , GoTo * _false)
{
	IterationStatement *S=new IterationStatement();
	S->add_children(ex1,st1);
	S->name="DO WHILE";
	
	backpatch(ex1->truelist,l1);
	_false->set_res( ex1->res ); 
	ex1->falselist.push_back(_false);
	append(S->nextlist,ex1->falselist);
	if ( st1 != nullptr ) {
		backpatch(st1->nextlist,l2);
		backpatch(st1->continuelist,l2);
		append(S->nextlist,st1->breaklist);
	}
	GoTo * _goto = create_new_goto(l1);
	return S;
}

Statement* create_iteration_statement_for(Expression * ex1, Label * l1, Expression * ex2, Label * l2, Statement* st1) {

	IterationStatement *S=new IterationStatement();
	S->add_children( ex1, ex2, st1);
	S->name = "FOR";
	if ( ex2 != nullptr ) {
		backpatch( ex2->truelist, l2 );
		append(S->nextlist, ex2->falselist);
	}
	if ( st1 != nullptr ) {
		backpatch(st1->nextlist,l1);
		backpatch(st1->continuelist,l1);
		append(S->nextlist, st1->breaklist);
	}
	GoTo * _goto = create_new_goto(l1);
	return S;
}


Statement* create_iteration_statement_for( Expression * ex1, Label * l1, Expression * ex2, GoTo * _goto1, Label * l2, Expression * ex3, GoTo * _goto2 , Label * l3, Statement* st1 ){

	IterationStatement *S=new IterationStatement();
	S->add_children( ex1, ex2, ex3, st1);
	S->name = "FOR";

	if ( ex2 != nullptr ) {
		backpatch( ex2->truelist, l3 );
		append(S->nextlist, ex2->falselist);
	}
	_goto1->label = l3;
	_goto2->label = l1;

	if ( st1 != nullptr ) {
		backpatch(st1->nextlist,l2);
		backpatch(st1->continuelist,l2);
		append(S->nextlist, st1->breaklist);
		append(S->caselist, st1->caselist);
	}
	GoTo * _goto = create_new_goto(l2);
	return S;
}

/// ------------ Jump Statement --------------------//
Statement* create_jump_statement_go_to(Identifier *id)
{
	JumpStatement *S=new JumpStatement();
	S->add_child(id);
	S->name = "GOTO";
	auto it = label_iden.find(id->value);
	if ( it == label_iden.end() ) {
		auto it = goto_iden.find(id->value);
		if(it == goto_iden.end()){
			GoTo * _goto = create_new_goto();
			std::vector<GoTo * > & vec = * new std::vector<GoTo * >;
			vec.push_back(_goto);
			goto_iden.insert({id->value, vec });
		}
		else{
			std::vector<GoTo *> & vec = it->second;
			GoTo * _goto = create_new_goto();
			vec.push_back(_goto);
		}
	}
	else {
		GoTo * _goto = create_new_goto( it->second );
	}
	return S;

}

Statement* create_jump_statement(int _type){
	JumpStatement *S=new JumpStatement();
	if (_type == CONTINUE){
		GoTo * _goto = create_new_goto();
		S->name="continue";
		S->continuelist.push_back(_goto);
	}
	else if(_type == BREAK){
		GoTo * _goto = create_new_goto();
		S->name="break";
		S->breaklist.push_back(_goto);
	}
	else if (_type == RETURN ) {
		S->name="return";
		create_new_return(nullptr);
	}
	return S;
}

Statement* create_jump_statement_exp(Expression *ex)
{	JumpStatement *S=new JumpStatement();
	S->name="return expression";
	S->add_child(ex);
	if ( ex->type != INVALID_TYPE && local_symbol_table.return_type != ex->type ) {
		error_msg("Function expects return type of " + local_symbol_table.return_type.get_name() + ". Got " + ex->type.get_name(),line_num);
	}
	create_new_return(ex->res);
	return S;
}
///--------------Labelled Statement-------------------///
Statement* create_labeled_statement_iden( Identifier *id, Label* l, Statement* st1 ){
	LabeledStatement *S = new LabeledStatement();
	S->add_children( id, st1 );
	S->name = "GOTO";
	auto it = label_iden.find( id->value );
	if (it == label_iden.end()){
		label_iden.insert({id->value, l });
	}
	else{
		error_msg("duplicate label " + id->value, id->line_num, id->column);
		return S;
	}
	auto it2 = goto_iden.find(id->value);
	if ( it2 != goto_iden.end() ) {
		backpatch(it2->second,l);
		it2->second.clear();
		goto_iden.erase(id->value);
	}
	return S;
}


Statement* create_labeled_statement_case(Constant *con,Label* l,Statement* s1){
	IterationStatement *S = new IterationStatement();
	S->add_children( con ,s1);
	S->name = "CASE";
	if (! con->ConstantType.isInt()) {
		error_msg("Case label not an interger constant.",con->line_num,con->column);
		}
	if (switch_type==NULL)
		{switch_type=&(con->ConstantType);}
	std::string s="";
	if (con->ConstantType.isUnsigned() && con->ConstantType.isInt()){
		s=std::to_string(con->val.ui);
		}
	else if (con->ConstantType.isInt()){
		s=std::to_string(con->val.i);
		}
	auto it=switch_label.find(s);
	if (it!=switch_label.end()){
		error_msg("Duplicate case value",con->line_num,con->column);
	}
	else{
		switch_label.insert({s,l});
		}
	//switch_temp=l;
    append(S->breaklist, s1->breaklist);
	append(S->nextlist,s1->nextlist);
	append(S->caselist, s1->caselist);
	return S;
}

Statement* create_labeled_statement_def(Label* l, Statement* s1){
	if(switch_temp!=NULL){
		backpatch(s1->nextlist,switch_temp);
	}
	IterationStatement *S = new IterationStatement();
	S->add_child(s1);
	S->name = "DEFAULT";
	switch_label.insert({"",l});
	append(S->nextlist, s1->breaklist);
	append(S->caselist, s1->caselist);
	//error_msg("case",s1->line_num);
	switch_temp=l;
    append(S->breaklist, s1->breaklist);
	append(S->nextlist,s1->nextlist);
	append(S->caselist, s1->caselist);
	return S;
} 



//--------------Statement List--------------------------///


Statement* create_statement_list( Statement * st1 ){
	if ( st1 == nullptr ) {
		return nullptr;
	}
	StatementList * S = new StatementList();
	S->name = "statement_list";
	S->add_child( st1 );
	append(S->nextlist,st1->nextlist);
	append(S->continuelist,st1->continuelist);
	append(S->breaklist,st1->breaklist);
	append(S->caselist,st1->caselist);

	return S;

}
Statement* add_to_statement_list( Statement * stl, Statement * st1 ) {
	if ( st1 == nullptr ) {
		return stl;
	}
	else if ( stl == nullptr ) {
		return create_statement_list( st1 );
	}
	
	stl->add_child( st1 );
	stl->nextlist = st1->nextlist;
	append(stl->continuelist,st1->continuelist);
	append(stl->breaklist,st1->breaklist);
	append(stl->caselist,st1->caselist);
	return stl;

}


//--------------Compound Statement----------------------///
