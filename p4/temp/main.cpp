#include "symbol_table.h"



int main() {
	Symbol mySymbol;
	Symbol anotherSymbol;
	Symbol_table myTable;

	double myDouble[2] = {3.14519,4.4};
	stringstream ss;
	string myString[10];

	for (int i = 0; i < 10; ++i) {
		myString[i] = "My Name is Daniel";
	}
	anotherSymbol.setType(DOUBLE);
	anotherSymbol.setName("myDouble");
	anotherSymbol.setValue(&myDouble);

	mySymbol.setType(STRING);
	mySymbol.setName("myString");
	mySymbol.setValue(&myString);

	myTable.add(anotherSymbol);
	myTable.add(mySymbol);
	myTable.print(cout);
}