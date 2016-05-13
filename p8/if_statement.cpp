#include "if_statement.h"


If_statement::If_statement(Expression* ifExpr, Statement_block* thenBlock) {
	_ifExpr = ifExpr;
	_thenBlock = thenBlock;
}

If_statement::If_statement(Expression* ifExpr, Statement_block* thenBlock, Statement_block* elseBlock) {
	_ifExpr = ifExpr;
	_thenBlock = thenBlock;
	_elseBlock = elseBlock;
}

void If_statement::execute() {
	if (_elseBlock == NULL) {
		if (_ifExpr->eval_int()) {
			_thenBlock->execute();
		}
	} else {
		if (_ifExpr->eval_int()) {
			_thenBlock->execute();
		} else {
			_elseBlock->execute();
		}
	}
}