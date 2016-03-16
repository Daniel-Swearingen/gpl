#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include "error.h"
#include "symbol.h"
using namespace std;


class Symbol_table {
public:
	static Symbol_table *instance();
    Symbol* lookup(string);
    void print(ostream&);
    void add(Symbol*);
    unordered_map<string,Symbol*> table;
    Symbol_table() {};
private:
    static Symbol_table *m_instance;



    Symbol_table(const Symbol_table &);
    const Symbol_table &operator=(const Symbol_table &);
};

