#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <sstream>
#include <iostream>
#include "gpl_type.h"
#include "game_object.h"
#include "rectangle.h"
#include "circle.h"
#include "textbox.h"
#include "pixmap.h"
#include "triangle.h"
#include "animation_block.h"

using namespace std;

class Symbol {
public:
	string getName();
	string getType();
	Gpl_type get_type();
	string getArrType();
	int getArrSize();
	void* getValue();
	void setType(Gpl_type);
	void setName(string);
	void set(void*);
	void set(int);
	void set(double);
	void set(string);
	void set(int,int);
	void set(double,int);
	void set(string,int);
	void setSize(int);
	const int get_int_value();
	const int get_int_value(int);
	const double get_double_value();
	const double get_double_value(int);
	string get_string_value();
	string* getStringValue();
	string get_string_value(int);
	string* getStringValue(int);
	Game_object* get_game_object_value();
	Game_object* get_game_object_value(int);
	Animation_block* get_animation_block_value();

	bool is_game_object();
	void printSymbol(ostream&);
	Symbol(){};
	Symbol(Symbol*);
private:
	Gpl_type _type;
	string _name;
	void* _value;
	int _size = -1;
};

#endif