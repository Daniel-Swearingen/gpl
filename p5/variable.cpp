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

int Variable::getInt() {
	return getSymbol()->get_int_value();
}

double Variable::getDouble() {
	return getSymbol()->get_double_value();
}

string* Variable::getString() {
	return getSymbol()->get_string_value();
}