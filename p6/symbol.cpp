#include "symbol.h"

using namespace std;

Symbol::Symbol(Symbol *symbol) {
	_type = symbol->get_type();
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

string Symbol::get_string_value() {
	return *(string*)_value;
}

string* Symbol::getStringValue() {
	return (string*)_value;
}

Game_object* Symbol::get_game_object_value() {
	return (Game_object*)_value;
}

Animation_block* Symbol::get_animation_block_value() {
	return (Animation_block*)_value;
}

const int Symbol::get_int_value(int index) {
	return *(((int*)_value)+index);
}

const double Symbol::get_double_value(int index) {
	return *((double*)_value+index);
}

string Symbol::get_string_value(int index) {
	return *((string*)_value+index);
}

string* Symbol::getStringValue(int index) {
	return ((string*)_value+index);
}

Game_object* Symbol::get_game_object_value(int index) {
	switch(get_type()) {
		case TRIANGLE_ARRAY:
			return ((Triangle*)_value+index);
			break;
		case PIXMAP_ARRAY:
			return ((Pixmap*)_value+index);
			break;
		case CIRCLE_ARRAY:
			return ((Circle*)_value+index);
			break;
		case RECTANGLE_ARRAY:
			return ((Rectangle*)_value+index);
			break;
		case TEXTBOX_ARRAY:
			return ((Textbox*)_value+index);
			break;
		default:
			break;
	}
	return ((Game_object*)_value+index);
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

Gpl_type Symbol::get_type() {
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

void Symbol::set(void* value) {
	_value = value;
}

void Symbol::set(int value) {
	*(int*)_value = value;
}

void Symbol::set(double value) {
	*(double*)_value = value;
}

void Symbol::set(string value) {
	*(string*)_value = value;
}

void Symbol::set(int value, int index) {
	if (index < _size) {
		*((int*)_value+index) = value;
	}
}

void Symbol::set(double value, int index) {
	if (index < _size) {
		*((double*)_value+index) = value;
	}
}

void Symbol::set(string value, int index) {
	if (index < _size) {
		*((string*)_value+index) = value;
	}
}

void Symbol::setSize(int size) {
	_size = size;
}

bool Symbol::is_game_object() {
	if (get_type() & GAME_OBJECT) {
		return true;
	} else {
		return false;
	}
}

void Symbol::printSymbol(ostream &os){
	if (getArrType() == "int"){
		os << getArrType() << " " << getName() << " = " << get_int_value() << "\n";
	} else if (getArrType() == "double"){
		os << getArrType() << " " << getName() << " = " << get_double_value() << "\n";
	} else if (getArrType() == "string"){
		os << getType() << " " << getName() << " = " << "\"" << get_string_value() << "\"\n";
	} else if ((get_type() & GAME_OBJECT) && !(get_type() & ARRAY)) {
		get_game_object_value()->print(getName(),os);
	} else if (getArrType() == "int array") {
		for (int j = 0; j < getArrSize(); ++j) {
			os << getType() << " " << getName() << "[" << j << "] = " << get_int_value(j) << "\n";
		}
	} else if (getArrType() == "double array") {
		for (int j = 0; j < getArrSize(); ++j) {
			os << getType() << " " << getName() << "[" << j << "] = " << get_double_value(j) << "\n";
		}
	} else if (getArrType() == "string array") {
		for (int j = 0; j < getArrSize(); ++j) {
			os << getType() << " " << getName() << "[" << j << "] = \"" << get_string_value(j) << "\"\n";
		}
	} else if (get_type() & GAME_OBJECT_ARRAY) {
		stringstream ss;
		for (int j = 0; j < getArrSize(); ++j) {
			ss << getName() << "[" << j << "]";
			get_game_object_value(j)->print(ss.str(),os);
			ss.str("");
		}
	} else if (getArrType() == "animation_block") {
		os << getType() << " " << getName() << "\n";
	}
	/*
	if (is_game_object() && getArrSize() < 0) {
		get_game_object_value()->print(getName(),os);
	}
	*/
}
