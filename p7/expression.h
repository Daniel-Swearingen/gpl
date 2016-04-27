#ifndef EXPRESSION_H
#define EXPRESSION_H
class Variable;
#include "gpl_type.h"
#include "variable.h"
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include "error.h"
#include "animation_block.h"

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
	Animation_block* eval_animation_block();
	//setters
	void setType(int);
	void setIniType(int);
	void setInt(int);
	void setDouble(double);
	void setString(string*);
	void setAnimation_block(Animation_block*);
	void setOp(Operator_type);
	void setVar(Variable*);
	void setLeft(Expression*);
	void setRight(Expression*);

	//getters
	int getType();
	int getIniType();
	int getInt();
	Expression* getl();
	Expression* getr();
	double getDouble();
	string *getString();
	Operator_type getOp();

private:
	int _type;			//0 = int, 1 = double, 2 = string
	int _iniType;
	int _intE;
	double _doubleE;
	string _stringE;
	Animation_block *_animation_blockE;
	Operator_type _op;
	Variable *_var;
	Expression *l = NULL;
	Expression *r = NULL;
	Expression *p;
};

#endif