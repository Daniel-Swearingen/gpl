// updated 2/14/16

#include "statement_block.h"
#include "statement.h"
using namespace std;

Statement_block::Statement_block(){

}
Statement_block::Statement_block(Statement_block* block) {
	_statements = block->getVector();
}

// this function is called for all non-animation_block statement_blocks
// Implement it for p7
void Statement_block::execute(){
	for (vector<Statement*>::iterator it = _statements.begin(); it != _statements.end(); ++it) {
		(*it)->execute();
	}
}

void Statement_block::addStatement(Statement* statement) {
	_statements.push_back(statement);
}

vector<Statement*> Statement_block::getVector() {
	return _statements;
}