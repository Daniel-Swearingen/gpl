#include "variable.h"

Variable::Variable(Symbol *symbol) {
	sym = symbol;
}

Variable::Variable(Symbol *symbol,Expression *expression) {
	sym = symbol;
	expr = expression;
}

Variable::Variable(Symbol *symbol, string memb) {
	setIsMemberVariable(true);
	sym = symbol;
	member = memb;
}

Variable::Variable(Symbol *symbol, Expression *expression, string memb) {
	setIsMemberVariable(true);
	sym = symbol;
	expr = expression;
	member = memb;
}

void Variable::setIsMemberVariable(bool b) {
	isMemberVariable = b;
}

Gpl_type Variable::getType() {
	Gpl_type type;
	if (getIsMemberVariable()){
		if (getExpression() != NULL) {
			getSymbol()->get_game_object_value()->get_member_variable_type(member,type);
			return type;
		} else {
			getSymbol()->get_game_object_value()->get_member_variable_type(member,type);
			return type;
		}
	} else {
		if (getSymbol()->get_type() == INT || getSymbol()->get_type() == INT_ARRAY) {
			return INT;
		} else if (getSymbol()->get_type() == DOUBLE || getSymbol()->get_type() == DOUBLE_ARRAY) {
			return DOUBLE;
		} else if (getSymbol()->get_type() == STRING || getSymbol()->get_type() == STRING_ARRAY) {
			return STRING;
		} else if (getSymbol()->get_type() & GAME_OBJECT) {
			return GAME_OBJECT;
		} else {
			return ANIMATION_BLOCK;
		}
	}
}

Symbol* Variable::getSymbol() {
	return sym;
}
Expression* Variable::getExpression() {
	return expr;
}
int Variable::getInt() {
	int value;
	if (getExpression() != NULL) {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value(getExpression()->eval_int())->get_member_variable(member,value);
		} else {
			return getSymbol()->get_int_value(getExpression()->eval_int());
		}
	} else {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value()->get_member_variable(member,value);
		} else {
			return getSymbol()->get_int_value();
		}
	}
	return value;
}

double Variable::getDouble() {
	double value;
	if (getExpression() != NULL) {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value(getExpression()->eval_int())->get_member_variable(member,value);
		} else {
			return getSymbol()->get_double_value(getExpression()->eval_int());
		}
	} else {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value()->get_member_variable(member,value);
		} else {
			return getSymbol()->get_double_value();
		}
	}
	return value;
}

string* Variable::getString() {
	string* value = new string();
	if (getExpression() != NULL) {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value(getExpression()->eval_int())->get_member_variable(member,*value);
		} else {
			return getSymbol()->getStringValue(getExpression()->eval_int());
		}
	} else {
		if (getIsMemberVariable()) {
			getSymbol()->get_game_object_value()->get_member_variable(member,*value);
			return value;
		} else {
			return getSymbol()->getStringValue();
		}
	}
	return value;
}

bool Variable::getIsMemberVariable() {
	return isMemberVariable;
}

Animation_block* Variable::getAnimationBlock() {
	return getSymbol()->get_animation_block_value();
}

string Variable::getMember() {
	return member;
}