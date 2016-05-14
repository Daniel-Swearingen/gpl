#include "symbol_table.h"
#include "gpl_type.h"
#include "error.h"
using namespace std;

Symbol_table* Symbol_table::m_instance = 0;

Symbol_table* Symbol_table::instance() {
 	if (m_instance == 0)
		m_instance = new Symbol_table();
	return m_instance;
}

Symbol* Symbol_table::lookup(string name) {
	unordered_map<string,Symbol*>::const_iterator it = table.find(name);

	for(it = table.begin(); it != table.end(); it++) {
		if (it->first == name) {
			return it->second;
		}
	}
	return NULL;
}

void Symbol_table::print(ostream &os) {
	Symbol* symbol;
	vector<string> used;
	for(unordered_map<string,Symbol*>::iterator it = table.begin(); it != table.end(); ++it) {
		used.push_back(it->first);
	}
	sort(used.begin(),used.end());
	
	for (unsigned int i = 0; i < table.size(); ++i) {
		symbol = lookup(used[i]);
		symbol->printSymbol(os);
		/*
		if (symbol->getArrType() == "int"){
			os << symbol->getArrType() << " " << symbol->getName() << " = " << symbol->get_int_value() << "\n";
		} else if (symbol->getArrType() == "double"){
			os << symbol->getArrType() << " " << symbol->getName() << " = " << symbol->get_double_value() << "\n";
		} else if (symbol->getArrType() == "string"){
			os << symbol->getType() << " " << symbol->getName() << " = " << "\"" << symbol->get_string_value() << "\"\n";
		} else if (symbol->getArrType() == "game_object") {
			symbol->get_game_object_value()->print(symbol->getName(),os);
		} else if (symbol->getArrType() == "int array") {
			for (int j = 0; j < symbol->getArrSize(); ++j) {
				os << symbol->getType() << " " << symbol->getName() << "[" << j << "] = " << symbol->get_int_value(j) << "\n";
			}
		} else if (symbol->getArrType() == "double array") {
			for (int j = 0; j < symbol->getArrSize(); ++j) {
				os << symbol->getType() << " " << symbol->getName() << "[" << j << "] = " << symbol->get_double_value(j) << "\n";
			}
		} else if (symbol->getArrType() == "string array") {
			for (int j = 0; j < symbol->getArrSize(); ++j) {
				os << symbol->getType() << " " << symbol->getName() << "[" << j << "] = \"" << symbol->get_string_value(j) << "\"\n";
			}
		} else if (symbol->getArrType() == "game_object array") {
			for (int j = 0; j < symbol->getArrSize(); ++j) {
				symbol->get_game_object_value(j)->print(symbol->getName(),os);
			}
		}
		*/
	}
}

void Symbol_table::add(Symbol *symbol) {
	Symbol* doesItExist = lookup(symbol->getName());
	if (doesItExist == NULL) {
		table[symbol->getName()] = symbol;
	} else {
		Error::error(Error::PREVIOUSLY_DECLARED_VARIABLE,symbol->getName());
	}
}

void Symbol_table::checkAnimationsForBodies() {
	Symbol* tempSym;
	for(unordered_map<string,Symbol*>::iterator it = table.begin(); it != table.end(); ++it) {
		tempSym = it->second;
		if (tempSym->get_type() == ANIMATION_BLOCK) {
			if (!(tempSym->get_animation_block_value()->getFlag())) {
		        Error::error(Error::NO_BODY_PROVIDED_FOR_FORWARD,it->first);
			}
		}
	}
}