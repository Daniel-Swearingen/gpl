#include "exit_statement.h"

Exit_statement::Exit_statement(int lineNumber, Expression* value) {
	if (value->getType() == 0) {
		_value = value;
		_lineNumber = lineNumber;
	} else {
		//error
	}
	
}

void Exit_statement::execute() {
	cout << "gpl[" << _lineNumber << "]: " << "exit(" << _value->eval_int() << ")" << endl; 
	exit(_value->eval_int());
}