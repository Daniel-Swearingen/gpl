#include "symbol.h"

using namespace std;

Symbol::Symbol(Symbol *symbol) {
	_type = symbol->getGpl_type();
	_name = symbol->getName();
	_size = symbol->getArrSize();
	_value = symbol->getValue();
}

const int Symbol::get_int_value() {
	return *(int*)_value;
}

const double Symbol::get_double_value() {
	return *(double*)_value;
}

string* Symbol::get_string_value() {
	return (string*)_value;
}

const int Symbol::get_int_value(int index) {
	return *(((int *)_value)+index*8);
}

const double Symbol::get_double_value(int index) {
	return *((double*)_value+index);
}

string* Symbol::get_string_value(int index) {
	return ((string*)_value+index);
}

void* Symbol::getValue() {
	return _value;
}

int Symbol::getArrSize() {
	return _size;
}

string Symbol::getType() {
	return gpl_type_to_base_string(_type);
}

Gpl_type Symbol::getGpl_type() {
	return _type;
}

string Symbol::getArrType() {
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
	cout << getType() << " " << _name << ": " << Symbol::get_int_value() << "\n";
}
