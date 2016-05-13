#include "for_statement.h"

For_statement::For_statement(Statement_block* initializer,Expression* condition,Statement_block* incrementor,Statement_block* body) {
	_initializer = initializer;
	if (condition->getType() == 0) {
		_condition = condition;	
	} else {
        Error::error(Error::INVALID_TYPE_FOR_FOR_STMT_EXPRESSION);
	}
	_incrementor = incrementor;
	_body = body;
}

void For_statement::execute() {
	_initializer->execute();
	//int i = _initializer->getVector()->at(1)->getValue()->eval_int();
	//for (_condition->execute();incrementor->execute())

	while (_condition->eval_int()) {
		_body->execute();
		_incrementor->execute();
	}
}