#include "symbol_table.h"

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
	/*
	if (it == table.end()) {
		return NULL;
	} else {
		return it->second;
	}
	*/
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
		if (symbol->getArrType() == "int"){
			os << symbol->getArrType() << " " << symbol->getName() << " = " << symbol->get_int_value() << "\n";
		} else if (symbol->getArrType() == "double"){
			os << symbol->getArrType() << " " << symbol->getName() << " = " << symbol->get_double_value() << "\n";
		} else if (symbol->getArrType() == "string"){
			os << symbol->getType() << " " << symbol->getName() << " = " << "\"" << *symbol->get_string_value() << "\"\n";
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
				os << symbol->getType() << " " << symbol->getName() << "[" << j << "] = \"" << *symbol->get_string_value(j) << "\"\n";
			}
		}
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
