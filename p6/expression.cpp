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
	setString(*stringE);
	setType(2);
	setIniType(2);
}

Expression::Expression(Expression *e1 ,Operator_type op,Expression *e2) {
	if (e2 != NULL) {
		setLeft(e1);
		setRight(e2);
		setOp(op);
		if (op == LESS_THAN || op == LESS_THAN_EQUAL || op == GREATER_THAN || op == GREATER_THAN_EQUAL ||
			op == EQUAL || op == NOT_EQUAL || op == AND || op == OR) {
			setType(0);
		} else if (e1->getType() == 2 || e2->getType() == 2) {
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
		if (op == SIN || op == COS || op == TAN || op == ASIN || op == ACOS || op == ATAN || op == SQRT) {
			setType(1);
		} else if (op == FLOOR || op == NOT) {
			setType(0);
		} else if (e1->getType() == 0) {
			setType(0);
		} else if (e1->getType() == 1) {
			setType(1);
		}
		//cout << "Expression: " << eval_double() << ", OP: " << operator_to_string(getOp()) << endl;
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
	string lValS,rValS;
	double lValD,rValD;
	stringstream ss;
	Operator_type op;
	Expression *l = getl();
	Expression *r = getr();

	if (l != NULL) {
		if (l->getType() == 2) {
			lValS = l->eval_string();
		} if (l->getType() == 1) {
			lValD = l->eval_double();
		} else {
			lVal = l->eval_int();
		}
	}

	if (r != NULL) {
		if (r->getType() == 2) {
			rValS = r->eval_string();
		} else if (r->getType() == 1) {
			rValD = r->eval_double();
		} else {
			rVal = r->eval_int();
		}
	}

	if ((op = getOp()) == PLUS) {
		value = lVal + rVal;
	} else if (op == MOD) {
		value = lVal % rVal;
	} else if (op == MULTIPLY) {
		value = lVal * rVal;
	} else if (op == MINUS) {
		value = lVal - rVal;
	} else if (op == ABS) {
		value = abs(lVal);
	} else if (op == FLOOR) {
		if (l->getType() == 0) {
			value = floor(lVal);
		} else if (l->getType() == 1) {
			value = floor(lValD);
		}
	} else if (op == UNARY_MINUS) {
		value = -lVal;
	} else if (op == RANDOM) {
		value = rand() % lVal;
	} else if (op == NOT) {
		if (l->getType() == 0) {
			if (lVal == 0) {
				value = 1;
			} else {
				value = 0;
			}
		} else {
			if (lValD == 0) {
				value = 1;
			} else {
				value = 0;
			}
		}
	} else if (op == LESS_THAN) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS < rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() < rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS < ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD < rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal < rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD < rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() < rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS < ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal < rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == LESS_THAN_EQUAL) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS <= rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() <= rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS <= ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD <= rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal <= rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD <= rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() <= rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS <= ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal <= rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == GREATER_THAN) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS > rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() > rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS > ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD > rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal > rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD > rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() > rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS > ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal > rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == GREATER_THAN_EQUAL) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS >= rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() >= rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS >= ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD >= rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal >= rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD >= rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() >= rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS >= ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal >= rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == EQUAL) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS == rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() == rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS == ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD == rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal == rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD == rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() == rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS == ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal == rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == NOT_EQUAL) {
		if (l->getType() == 2 && r->getType() == 2) {
			if (lValS != rValS) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 2) {
			ss << lValD;
			if (ss.str() != rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 1) {
			ss << rValD;
			if (lValS != ss.str()) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD != rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal != rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD != rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			ss << lVal;
			if (ss.str() != rValS) {
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (l->getType() == 2 && r->getType() == 0) {
			ss << rVal;
			if (lValS != ss.str()){
				value = 1;
			} else {
				value = 0;
			}
			ss.str("");
		} else if (lVal != rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == AND) {
		if (l->getType() == 2 && r->getType() == 2) {
			//error
		} else if (l->getType() == 1 && r->getType() == 2) {
			//error
		} else if (l->getType() == 2 && r->getType() == 1) {
			//error
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD && rVal) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal && rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD && rValD) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			//error
		} else if (l->getType() == 2 && r->getType() == 0) {
			//error
		} else if (lVal && rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == OR) {
		if (l->getType() == 2 && r->getType() == 2) {
			//error
		} else if (l->getType() == 1 && r->getType() == 2) {
			//error
		} else if (l->getType() == 2 && r->getType() == 1) {
			//error
		} else if (l->getType() == 1 && r->getType() == 0) {
			if (lValD != 0 || rVal != 0) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 1) {
			if (lVal != 0 || rValD != 0) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 1 && r->getType() == 1) {
			if (lValD != 0 || rValD != 0) {
				value = 1;
			} else {
				value = 0;
			}
		} else if (l->getType() == 0 && r->getType() == 2) {
			//error
		} else if (l->getType() == 2 && r->getType() == 0) {
			//error
		} else if (lVal || rVal) {
			value = 1;
		} else {
			value = 0;
		}
	} else if (op == DIVIDE) {
		value = lVal/rVal;
	}
	
	if (l == NULL) {
		if (getType() == 0) {
			return getInt();
		} else {
			return getDouble();
		}
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
		if (l->getType() == 0) {
			lVal = (double)l->eval_int();
		} else {
			lVal = l->eval_double();
		}
	}
	if (r != NULL) {
		if (r->getType() == 0) {
			rVal = (double)r->eval_int();
		} else {
			rVal = r->eval_double();
		}
	}
	if ((op = getOp()) == PLUS) {
		value = lVal + rVal;
	} else if (op == MULTIPLY) {
		value = lVal * rVal;
	} else if (op == SIN) {
		value = sin(lVal/180*M_PI);
	} else if (op == COS) {
		value = cos(lVal/180*M_PI);
	} else if (op == TAN) {
		value = tan(lVal/180*M_PI);
	} else if (op == ASIN) {
		value = (asin(lVal)*180/M_PI);
	} else if (op == ACOS) {
		value = acos(lVal)*180/M_PI;
	} else if (op == ATAN) {
		value = atan(lVal)*180/M_PI;
	} else if (op == FLOOR) {
		value = eval_int();
	} else if (op == SQRT) {
		value = sqrt(lVal);
	} else if (op == ABS) {
		value = abs(lVal);
	} else if (op == UNARY_MINUS) {
		value = -lVal;
	} else if (op == MINUS) {
		value = lVal - rVal;
	} else if (op == NOT) {
		lVal = eval_int();
	} else if (op == DIVIDE) {
		if (getType() == 0) {
			value = eval_int();
		} else {
			value = lVal/rVal;
		}
		//value = lVal / rVal;
	} else if (op == LESS_THAN || op == LESS_THAN_EQUAL || op == GREATER_THAN || op == GREATER_THAN_EQUAL ||
			op == EQUAL || op == NOT_EQUAL || op == AND || op == OR) {
		value = eval_int();
	}
	
	//cout << "\tValue: " << value << endl;
	if (l == NULL) {
		if (getType() == 0) {
			return (double)getInt();
		} 
		return getDouble();
	}

	//cout << "\tValue: " << value << endl;
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
		if (l->getType() == 2) {
			lVal = l->eval_string();
		} else if (l->getType() == 1) {
			ss << l->eval_double();
			lVal = ss.str();
			ss.str("");
		} else if (l->getType() == 0) {
			ss << l->eval_int();
			lVal = ss.str();
			ss.str("");
		}
	}
	if (r != NULL) {
		if (r->getType() == 2) {
			rVal = r->eval_string();
		} else if (r->getType() == 1) {
			ss << r->eval_double();
			rVal = ss.str();
			ss.str("");
		} else if (r->getType() == 0) {
			ss << r->eval_int();
			rVal = ss.str();
			ss.str("");
		}
	}
	//cout << "lVal : rVal , Type: " << lVal << ":" << rVal << ", " << getType() << " | " << operator_to_string(getOp()) << endl;
	if ((op = getOp()) == PLUS) {
		if (getType() == 0) {
			ss << eval_int();
			value = ss.str();
			ss.str("");
		} else if (getType() == 1) {
			ss << eval_double();
			value = ss.str();
			ss.str("");
		} else {
			value = lVal + rVal;	
		}
	} else if (op == LESS_THAN || op == LESS_THAN_EQUAL || op == GREATER_THAN || op == GREATER_THAN_EQUAL ||
			op == EQUAL || op == NOT_EQUAL || op == AND || op == OR) {
		value = to_string(eval_int());
	} else if (getType() == 1) {
		ss << eval_double();
		value = ss.str();
		//cout << "\t\tValue: " << value << endl;
	} else if (getType() == 0) {
		ss << eval_int();
		value = ss.str();
	}
	
	if (l == NULL) {
		if (getType() == 2) {
			return getString();
		} else if (getType() == 1) {
			ss << getDouble();
			return ss.str();
		} else if (getType() == 0) {
			ss << getInt();
			return ss.str();
		}
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

void Expression::setString(string stringE) {
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

string Expression::getString() {
	return _stringE;
}
