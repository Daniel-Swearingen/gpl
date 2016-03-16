#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "gpl_type.h"
#include "variable.h"
#include <string>

class Expression {
public:
	//constructors
	Expression();
	Expression(int);
	Expression(double);
	Expression(string*);
	Expression(Variable*);
	Expression(Expression*,Operator_type,Expression*);

	//evaluators
	int eval_int();
	double eval_double();
	string eval_string();
	//setters
	void setType(int);
	void setInt(int);
	void setDouble(double);
	void setString(string*);
	void setOp(Operator_type);
	void setVar(Variable);
	void setLeft(Expression*);
	void setRight(Expression*);
	void setParent(Expression*);

	//getters
	int getType();
	int getInt();
	double getDouble();
	string* getString();

private:
	int _type;			//0 = int, 1 = double, 2 = string, 3 = bin op, 4 = unary op, 5 = variable
	int _intE;
	double _doubleE;
	string *_stringE;
	Operator_type _op;
	Variable *_var;
	Expression *l = NULL;
	Expression *r = NULL;
	Expression *p;
};

#endif