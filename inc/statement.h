#ifndef __STATEMENT_H__
#define __STATEMENT_H__

#include <ast.h>
#include <deque>
#include <map>
#include <string>
#include <symtab.h>
#include <3ac.h>

class Statement : public Non_Terminal {
    public:
        Address *res;
        std::vector<GoTo*> truelist;
        std::vector<GoTo*> nextlist;
        std::vector<GoTo*> falselist;
        std::vector<GoTo*> breaklist;
        std::vector<GoTo*> continuelist;
        std::vector<GoTo*> caselist;
    Statement() : Non_Terminal("") {};   
};

class TopLevelStatement : public Statement {
    public:
        Statement *s1;
    
};

class StatementList : public Statement{
    public :
    std::vector <Statement * > args;
    StatementList(){
    }
    
};

Statement* create_statement_list(StatementList* slist,Statement* s1);

class ExpressionStatement : public Statement{
    public:
        Expression *e1;
        ExpressionStatement(){
            e1=nullptr;
        }

};

Statement* create_expression_statement(Expression* e1);

class SelectionStatement : public Statement{
    public:
        Expression *ex1;
        Statement *st1;
        Statement *st2;
        std::string st;
        SelectionStatement(){
            ex1=nullptr;
            st2=nullptr;
            st1=nullptr;
            st="";
        }
};
Statement *create_selection_statement_if( Expression *ex, Label * l1, Statement *st1, GoTo * _goto, Label * l2, Statement *st2 );
Statement *create_selection_statement_switch(std::string st,Expression *ex1, Statement *st1);

class IterationStatement : public Statement {
    Expression* e1;
    Statement* s1;
    Statement *s2;
    Statement *s3;
    std::string st;
    IterationStatement(){
        e1=nullptr;
        s1=nullptr;
        s2=nullptr;
        s3=nullptr;
        e1=nullptr;
        st="";
    }
};

Statement* create_iteration_statement_while(std::string, Expression *e1, Statement *s1 );
Statement* create_iteration_statement_do_while(std::string st, Expression *e1, Statement *s1);
Statement* create_iteration_statement_for_a(std::string st,Statement*s1, Statement* s2, Statement* s3);
Statement* create_iteration_statement_for_b(std::string st,Statement*s1, Statement* s2, Expression *e1,Statement* s3);

class JumpStatement : public Statement {
    public:
        Expression *st1;
        Identifier *id;
        std::string st; 
        JumpStatement() {
            st1=nullptr;
            id=nullptr;
            st="";
        };
};

Statement* create_jump_statement_go_to(std::string st,Identifier* id);
Statement* create_jump_statement(std::string st);
Statement* create_jump_statement_ret_exp(std::string st,Expression* ex);

class LabeledStatement: public Statement{
    public:
        Statement *st1;
        Identifier *id;
        std::string st;
        Expression *ex;
        LabeledStatement() {
            st1=nullptr;
            ex=nullptr;
            st="";
            ex=nullptr;  
        };

};

Statement* create_labeled_statement_def(std::string st,Identifier *id,Statement* s1);
Statement* create_labeled_statement_case(std::string st,Expression *ex,Statement* s1);
Statement* create_labeled_statement_def(std::string st,Statement* s1);

class CompoundStatement : public Statement{
    public:
        StatementList *sl1;
        DeclarationList *dl1;
        std::string st;
        CompoundStatement() {
            st="";
            sl1=nullptr;
            dl1=nullptr;
        };
};

Statement* create_compound_statement_1(std::string st,StatementList* sl1);
Statement* create_compound_statement_2(std::string st,DeclarationList* dl1);
Statement* create_compound_statement_3(std::string st,DeclarationList* dl1,StatementList* sl1);

#endif
