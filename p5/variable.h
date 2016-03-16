
#ifndef VARIABLE_H
#define VARIABLE_H
class Expression;
#include "symbol.h"
#include "expression.h"

class Variable {
public:
	Variable(Symbol*);
	Variable(Symbol*,Expression*);
private:
	Symbol *sym;
	Expression *expr;
};

#endif