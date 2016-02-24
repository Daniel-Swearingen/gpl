all: Symbol.o symbol_table.o
	g++ main.cpp -std=c++11 Symbol.o symbol_table.o 

Symbol.o: gpl_type.o
	g++ Symbol.cpp gpl_type.o -std=c++11 -c -o Symbol.o

symbol_table.o:
	g++ symbol_table.cpp -std=c++11 -c -o symbol_table.o

gpl_type.o: gpl_assert.o
	g++ gpl_type.cpp gpl_assert.o -std=c++11 -c -o gpl_type.o

gpl_assert.o:
	g++ gpl_assert.cpp -std=c++11 -c -o gpl_assert.o

clean:
	rm *.o a.out