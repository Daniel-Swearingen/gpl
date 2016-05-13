#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include "statement.h"

class Print_statement : public Statement {
public:
	Print_statement(Expression*,int);
	Expression* getValue();
	void execute();
private:
	Expression* _value;
	int _lineNumber;

};

#endif