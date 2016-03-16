#include "expression.h"

//constructors
Expression::Expression(){

}

Expression::Expression(int intE) {
	setInt(intE);
	setType(0);
}

Expression::Expression(double doubleE) {
	setDouble(doubleE);
	setType(1);
}

Expression::Expression(string *stringE) {
	setString(stringE);
	setType(2);
}

Expression::Expression(expression *e1 ,Operator_type op,expression *e2) {
	if (e2 != NULL) {
		setLeft(e1);
		setRight(e2);
		setOp(op);
		setType(3);	
	} else {
		setLeft(e1);
		setRight(NULL);
		setOp(op);
		setType(4);
	}	
}

Expression::Expression(Variable *var) {
	setVar(var);
	setType(5);
}

//evaluators

int Expression::eval_int() {
	int value;
	if (getType() == 0) {
		value = getInt();
	} else if (getType() == 1) {
		//error
	} else if (getType() == 2) {
		//error
	} else if (getType() == 3) {
		Expression* l = getl();
		Expression* r = getr();
		op = getOp();
		if ((l->getType() == 0 || l->getType() == 1) && r->getType() == 0 || r->getType() == 1) {
			if (op == PLUS) {
				value = l->int_eval() + r->int_eval();
			} else if (op == MINUS) {
				value == l->int_eval() - r->int_eval();
			} else if (op == DIVIDE) {
				value == l->int_eval() / r->int_eval();
			} else if (op == MULTIPLY) {
				value = l->int_eval() * r->int_eval();
			} else if (op == MOD) {
				value = l->int_eval() % r->int_eval();
			} else if (op == )

		}
	}
}

double Expression::eval_double() {

}

string Expression::eval_string() {

}

//setters

void Expression::setType(int type) {
	if (type < 0 || type > 5) {
		//error
	} else {
		_type = type;
	}
}

void Expression::setInt(int intE) {
	_intE = intE;
}

void Expression::setDouble(double doubleE) {
	_doubleE = doubleE;
}

void Expression::setString(string* stringE) {
	_stringE = stringE;
}

void Expression::setOp(Operator_type op) {
	_op = op;
}

void Expression::setVar(variable *var) {
	_var = var;
}

void Expression::setLeft(expression *left) {
	l = left;
}

void Expression::setRight(expression *right) {
	r = right;
}

void Expression::setParent(expression *parent) {
	p = parent;
}

//getters
Operator_type Expression::getOp() {
	return _op;
}

Expression* Expression::getL() {
	return _l;
}

Expression* Expression::getR() {
	return _r;
}

int Expression::getType() {
	return _type;
}

int Expression::getInt() {
	return _intE;
}

double Expression::getDouble() {
	return _doubleE;
}

string* Expression::getString() {
	return _stringE;
}
