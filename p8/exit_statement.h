#ifndef EXIT_STATEMENT_H
#define EXIT_STATEMENT_H

#include "statement.h"

class Exit_statement : public Statement {
public:
	Exit_statement(){};
	Exit_statement(int,Expression*);
	void execute();
private:
	Expression* _value;
	int _lineNumber;
};

#endif