#include "expression.h"

//constructors
Expression::Expression(){

}

Expression::Expression(int intE) {
	setInt(intE);
	setType(0);
	setIniType(0);
}

Expression::Expression(double doubleE) {
	setDouble(doubleE);
	setType(1);
	setIniType(1);
}

Expression::Expression(string *stringE) {
	setString(stringE);
	setType(2);
	setIniType(2);
}

Expression::Expression(Expression *e1 ,Operator_type op,Expression *e2) {
	if (e2 != NULL) {
		setLeft(e1);
		setRight(e2);
		setOp(op);
		if (e1->getType() == 2 || e2->getType() == 2) {
			setType(2);
		} else if (e1->getType() == 1 || e2->getType() == 1) {
			setType(1);
		} else if (e1->getType() == 0 && e2->getType() == 0) {
			setType(0);
		}
	} else {
		setLeft(e1);
		setRight(NULL);
		setOp(op);
		if (e1->getType() == 0) {
			setType(0);
		} else if (e1->getType() == 1) {
			setType(1);
		}
	}	
}

Expression::Expression(Variable *var) {
	setVar(var);
	if (var->getType() & INT) {
		setType(0);
		setInt(var->getInt());
	} else if (var->getType() & DOUBLE) {
		setType(1);
		setDouble(var->getDouble());
	} else if (var->getType() & STRING) {
		setType(2);
		setString(var->getString());
	}
}

//evaluators

int Expression::eval_int() {
	int value;
	int lVal,rVal;
	Operator_type op;
	Expression *l = getl();
	Expression *r = getr();

	if (l != NULL) {
		lVal = l->eval_int();
		cout << "2: " << getInt() << endl;
	}
	if (r != NULL) {
		rVal = r->eval_int();
	}
	if ((op = getOp()) == PLUS) {
		value = lVal + rVal;
	} else if (op == MULTIPLY) {
		value = lVal * rVal;
	}
	
	if (l == NULL) {
		return getInt();
	}
	return value;
}

double Expression::eval_double() {
	double value;
	double lVal,rVal;
	Operator_type op;
	Expression *l = getl();
	Expression *r = getr();

	if (l != NULL) {
		if (getType() == 0) {
			lVal = (double)eval_int();
		} else {
			lVal = l->eval_double();
		}
	}
	if (r != NULL) {
		if (getType() == 0) {
			rVal = (double)eval_int();
		} else {
			rVal = r->eval_double();
		}
	}
	if ((op = getOp()) == PLUS) {
		value = lVal + rVal;
	} else if (op == MULTIPLY) {
		value = lVal * rVal;
	} else if (op == SIN) {
		cout << lVal << endl;
		value = sin(lVal/180*M_PI);
	}
	
	if (l == NULL) {
		if (getType() == 0) {
			return (double)getInt();
		} 
		return getDouble();
	}
	return value;
}

string Expression::eval_string() {
	string value;
	string lVal,rVal;
	Operator_type op;
	stringstream ss;
	Expression *l = getl();
	Expression *r = getr();

	if (l != NULL) {
		if (getType() == 1) {
			ss << eval_double();
			lVal = ss.str();
			ss.flush();
		} else {
			lVal = l->eval_string();
		}
	}
	if (r != NULL) {
		if (getType() == 1) {
			//ss << eval_double();
			//rVal = ss.str();
			//ss.flush();
		} else {
			rVal = r->eval_string();	
		}
	}	
	if (l == NULL || r == NULL) {
		if (getType() == 0) {
			ss << eval_int();
		} else if (getType() == 1) {
			ss << eval_double();
		} else {
			ss << *getString();
		}
		value = ss.str();
		ss.flush();
		return value;
	} else if ((op = getOp()) == PLUS) {
		value = lVal + rVal;
	}
	return value;
}

//setters
void Expression::setIniType(int iniType) {
	_iniType = iniType;
}

void Expression::setType(int type) {
	if (type < 0 || type > 2) {
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

void Expression::setVar(Variable *var) {
	_var = var;
}

void Expression::setLeft(Expression *left) {
	l = left;
}

void Expression::setRight(Expression *right) {
	r = right;
}


//getters
int Expression::getIniType() {
	return _iniType;
}
Operator_type Expression::getOp() {
	return _op;
}

Expression* Expression::getl() {
	return l;
}

Expression* Expression::getr() {
	return r;
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
