#include "symbol_table.h"

using namespace std;

Symbol_table *Symbol_table::m_instance = 0;

Symbol_table * Symbol_table::instance() {
 	if (m_instance == 0)
		m_instance = new Symbol_table();
	return m_instance;
}

Symbol* Symbol_table::lookup(string name) {
	unordered_map<string,Symbol*>::const_iterator it = table.find(name);
	if (it == table.end()) {
		return NULL;
	} else {
		return it->second;
	}
}

void Symbol_table::print(ostream &os) {
	Symbol* symbol;
	string used[table.size()];
	int j = 0;
	for(unordered_map<string,Symbol*>::iterator it = table.begin(); it != table.end(); ++it) {
		used[j] = it->first;
		++j;
	}
	alphabetize (used);
	for (int i = 0; i < table.size(); ++i) {
		symbol = lookup(used[i]);
		os << symbol->getType() << " " << symbol->getName() << " = ";
		if (symbol->getType() == "int"){
			os << symbol->get_int_value() << "\n";
		} else if (symbol->getType() == "double"){
			os << symbol->get_double_value() << "\n";
		} else if (symbol->getType() == "string"){
			os << "\"" << symbol->get_string_value() << "\"" << "\n";
		}
	}

}

void Symbol_table::add(Symbol symbol) {
	Symbol* doesItExist = lookup(symbol.getName());
	if (doesItExist == NULL) {
		table[symbol.getName()] = &symbol;
	}
}

void Symbol_table::alphabetize(string arr[]) {
	string stringPlaceholder;
	int i = 0;
	int j = 1;
	string key;
	int size = table.size();
	for(int ij = 1; j < size; ++j) {
		key = arr[j];
		i = j-1;
		while (i >= 0 && strcmp(arr[i].c_str(),key.c_str()) > 0) {
			stringPlaceholder = arr[i+1];
			arr[i+1] = arr[i];
			arr[i] = stringPlaceholder;
			i = i-1;
		}
		arr[i+1] = key;
	}
}