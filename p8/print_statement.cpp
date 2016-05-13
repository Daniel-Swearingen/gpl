#include "print_statement.h"

Print_statement::Print_statement(Expression* value,int lineNumber) {
	_value = value;
	_lineNumber = lineNumber;
}

void Print_statement::execute() {
	cout << "print[" << _lineNumber << "]: " << _value->eval_string() << endl;
}

Expression* Print_statement::getValue() {
	return _value;
}