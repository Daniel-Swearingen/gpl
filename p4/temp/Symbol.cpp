#include "Symbol.h"

using namespace std;

const int Symbol::get_int_value() {
	return *(int*)_value;
}

const double Symbol::get_double_value() {
	return *(double*)_value;
}

const string Symbol::get_string_value() {
	return *(string*)_value;
}

const int Symbol::get_int_value(int index) {
	return *((int*)_value+index);
}

const double Symbol::get_double_value(int index) {
	return *((double*)_value+index);
}

const string Symbol::get_string_value(int index) {
	return *((string*)_value+index);
}

int Symbol::size() {
	return _size;
}

string Symbol::getType() {
	return gpl_type_to_string(_type);
}

string Symbol::getName() {
	return _name;
}
void Symbol::setType(Gpl_type type) {
	_type = type;
}

void Symbol::setName(string name) {
	_name = name;
}

void Symbol::setValue(void* value) {
	_value = value;
}

void Symbol::setSize(int size) {
	_size = size;
}

void Symbol::printSymbol(){
	cout << _type << " " << _name << ": " << Symbol::get_string_value(1) << "\n";
	//cout << "\n" << (string*)_value << "\n" << (string*)_value+1 << "\n";
}
