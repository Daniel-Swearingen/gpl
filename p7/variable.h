#ifndef VARIABLE_H
#define VARIABLE_H
class Expression;

#include "symbol.h"
#include "expression.h"
#include "animation_block.h"
class Symbol;
using namespace std;

class Variable {
public:
	Variable(Symbol*);
	Variable(Symbol*,Expression*);
	Variable(Symbol*,string);
	Variable(Symbol*,Expression*,string);
	void setIsMemberVariable(bool);
	Gpl_type getType();
	Symbol* getSymbol();
	Expression* getExpression();
	int getInt();
	double getDouble();
	string* getString();
	bool getIsMemberVariable();
	Animation_block* getAnimationBlock();
	string getMember();

private:
	bool isMemberVariable = false;
	string member = "";
	Symbol *sym;
	Expression *expr = NULL;
};

#endif