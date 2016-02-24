#include <string>
#include <sstream>
#include <iostream>
#include "gpl_type.h"
using namespace std;

class Symbol {
public:
	string getName();
	string getType();
	int size();
	void setType(Gpl_type);
	void setName(string);
	void setValue(void*);
	void setSize(int);
	const int get_int_value();
	const int get_int_value(int);
	const double get_double_value();
	const double get_double_value(int);
	const string get_string_value();
	const string get_string_value(int);
	void printSymbol();
	Symbol(){};
private:
	Gpl_type _type;
	string _name;
	void *_value;
	int _size;
};