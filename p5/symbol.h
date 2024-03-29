#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <sstream>
#include <iostream>
#include "gpl_type.h"
using namespace std;

class Symbol {
public:
	string getName();
	string getType();
	Gpl_type getGpl_type();
	string getArrType();
	int getArrSize();
	void* getValue();
	void setType(Gpl_type);
	void setName(string);
	void setValue(void*);
	void setSize(int);
	const int get_int_value();
	const int get_int_value(int);
	const double get_double_value();
	const double get_double_value(int);
	string* get_string_value();
	string* get_string_value(int);

	void printSymbol();
	Symbol(){};
	Symbol(Symbol*);
private:
	Gpl_type _type;
	string _name;
	void* _value;
	int _size = -1;
};

#endif