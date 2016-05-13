#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H

#include "statement.h"
#include "gpl_type.h"
#include <sstream>

class Assign_statement : public Statement {
public:
	enum ASSIGN_TYPE {
		ASSIGN = 0,
		PLUSASSIGN = 1,
		MINUSASSIGN = 2
	};

	Assign_statement(Variable* var, Expression* value, ASSIGN_TYPE t);
	void execute();
	Expression* getValue();
private:
	Variable* _var;
	Expression* _value;
	ASSIGN_TYPE _t;
};

#endif