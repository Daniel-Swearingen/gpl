/*
	Base class for statements
*/
#ifndef STATEMENT_H
#define STATEMENT_H

#include "expression.h"
#include "error.h"

class Statement {
public:
	Statement(){};
	virtual void execute(){};
private:
	Expression* _expr;
};

#endif