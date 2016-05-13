#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "statement.h"

class If_statement : public Statement {
public:
	If_statement(Expression*,Statement_block*);
	If_statement(Expression*,Statement_block*,Statement_block*);
	void execute();
private:
	Expression* _ifExpr;
	Statement_block *_thenBlock;
	Statement_block *_elseBlock;
};

#endif