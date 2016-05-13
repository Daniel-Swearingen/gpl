#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "statement.h"

class For_statement : public Statement {
public:
	For_statement(Statement_block*,Expression*,Statement_block*,Statement_block*);
	void execute();
private:
	Statement_block* _initializer;
	Statement_block* _incrementor;
	Statement_block* _body;
	Expression* _condition;
};

#endif