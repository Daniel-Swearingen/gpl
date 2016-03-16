#include "variable.h"

Variable::Variable(Symbol *symbol) {
	sym = symbol;
}

Variable::Variable(Symbol *symbol,Expression *expression) {
	sym = symbol;
	expr = expression;
}