#ifndef VARIABLE_H
#define VARIABLE_H
class Expression;
#include "symbol.h"
#include "expression.h"

class Variable {
public:
	Variable(Symbol*);
	Variable(Symbol*,Expression*);
	Gpl_type getType();
	Symbol* getSymbol();
	int getInt();
	double getDouble();
	string* getString();
private:
	Symbol *sym;
	Expression *expr;
};

#endif