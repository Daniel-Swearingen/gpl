// updated 2/14/2016

#include <vector>
#include "animation_block.h"
#include "game_object.h"
#include "gpl_assert.h"
#include "symbol.h"
#include "symbol_table.h"
using namespace std;

void Animation_block::initialize(Symbol *parameter_symbol, string name) {
 	m_name = name;
 	// during error recovery a NULL parameter_symbol can be passed to Animation_block()
 	if (parameter_symbol)
    	assert(parameter_symbol->is_game_object());
  	m_parameter_symbol = parameter_symbol;
}

void Animation_block::execute(Game_object *argument) {
	Symbol_table symbolTable;
    Symbol_table* t = symbolTable.instance();
    Game_object* temp = m_parameter_symbol->get_game_object_value();

	m_parameter_symbol->set(argument);
	_body->execute();
	m_parameter_symbol->set(temp);
}

void Animation_block::setFlag(bool flag) {
	_flag = flag;
}

bool Animation_block::getFlag() {
	return _flag;
}

void Animation_block::setBody(Statement_block* body) {
	_body = body;
}

Statement_block* Animation_block::getBody() {
	return _body;
}