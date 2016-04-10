#include "variable.h"

Variable::Variable(Symbol *symbol) {
	sym = symbol;
}

Variable::Variable(Symbol *symbol,Expression *expression) {
	sym = symbol;
	expr = expression;
}

Gpl_type Variable::getType() {
	string typeStr;
	typeStr = getSymbol()->Symbol::getType();
	if (typeStr == "int" || typeStr == "int array") {
		return INT;
	} else if (typeStr == "double" || typeStr == "double array") {
		return DOUBLE;
	} else {
		return STRING;
	}
}

Symbol* Variable::getSymbol() {
	return sym;
}
Expression* Variable::getExpression() {
	return expr;
}
int Variable::getInt() {
	if (getExpression() != NULL) {
		return getSymbol()->get_int_value(getExpression()->eval_int());
	} else {
		return getSymbol()->get_int_value();
	}
}

double Variable::getDouble() {
	if (getExpression() != NULL) {
		return getSymbol()->get_double_value(getExpression()->eval_int());
	} else {
		return getSymbol()->get_double_value();
	}
}

string Variable::getString() {
	if (getExpression() != NULL) {
		return getSymbol()->get_string_value(getExpression()->eval_int());
	} else {
		return getSymbol()->get_string_value();
	}
}