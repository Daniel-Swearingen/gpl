#include "assign_statement.h"

Assign_statement::Assign_statement(Variable* var,Expression* value, ASSIGN_TYPE t) {
	_var = var;
	_value = value;
	_t = t;
}

void Assign_statement::execute() {
	//Is member variable and not an array
	if (_var->getIsMemberVariable() && _var->getExpression() == NULL) {
		string member = _var->getMember();
		if (_var->getType() == INT) {
			int curVal;
			_var->getSymbol()->get_game_object_value()->get_member_variable(member,curVal);
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_int());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,curVal+_value->eval_int());
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,curVal-_value->eval_int());
			} else {
				//error
			}
		} else if (_var->getType() == DOUBLE) {
			double curVal;
			_var->getSymbol()->get_game_object_value()->get_member_variable(member,curVal);
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_double());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_double()+1);
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_double()-1);
			} else {
				//error
			}
		} else if (_var->getType() == STRING) {
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_string());
			} else if (_t == PLUSASSIGN) {
				string curVal;
				_var->getSymbol()->get_game_object_value()->get_member_variable(member,curVal);
				_var->getSymbol()->get_game_object_value()->set_member_variable(member,curVal+_value->eval_string());

			}
		} else if (_var->getType() == ANIMATION_BLOCK) {
			_var->getSymbol()->get_game_object_value()->set_member_variable(member,_value->eval_animation_block());
		}
		//Is member variable and an array
	} else if (_var->getIsMemberVariable() && _var->getExpression() != NULL) {
		string member = _var->getMember();
		if (_var->getType() == INT) {
			int curVal;
			_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->get_member_variable(member,curVal);
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,_value->eval_int());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,curVal+_value->eval_int());
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,curVal-_value->eval_int());
			} else {
				//error
			}
		} else if (_var->getType() == DOUBLE) {
			double curVal;
			_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->get_member_variable(member,curVal);
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,_value->eval_double());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,curVal+_value->eval_double());
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,curVal-_value->eval_double());
			} else {
				//error
			}		
		} else if (_var->getType() == STRING) {
			if (_t == ASSIGN) {
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,_value->eval_string());
			} else if (_t == PLUSASSIGN) {
				string curVal;
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->get_member_variable(member,curVal);
				_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,curVal+_value->eval_string());
			} 
		} else if (_var->getType() == ANIMATION_BLOCK) {
			_var->getSymbol()->get_game_object_value(_var->getExpression()->eval_int())->set_member_variable(member,_value->eval_animation_block());
		}
		//not member variable and not an array
	} else if (_var->getExpression() == NULL) {
		if (_var->getType() == INT) {
			int curVal = _var->getSymbol()->get_int_value();
			if (_t == ASSIGN) {
				_var->getSymbol()->set(_value->eval_int());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->set(curVal+_value->eval_int());
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->set(curVal-_value->eval_int());
			}
		} else if (_var->getType() == DOUBLE) {
			double curVal = _var->getSymbol()->get_double_value();
			if (_t == ASSIGN) {
				_var->getSymbol()->set(_value->eval_double());
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->set(curVal+_value->eval_double());
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->set(curVal-_value->eval_double());
			}
		} else if (_var->getType() == STRING) {
			if (_t == ASSIGN) {
				_var->getSymbol()->set(_value->eval_string());
			} else if (_t == PLUSASSIGN) {
				string curVal  = _var->getSymbol()->get_string_value();
				_var->getSymbol()->set(curVal+_value->eval_string());
			}
		} 
		//not member variable and an array
	} else if (_var->getType() & INT) {
		int curVal = _var->getSymbol()->get_int_value(_var->getExpression()->eval_int());
		stringstream ss;

		if (_var->getSymbol()->getArrSize() < _var->getExpression()->eval_int() || _var->getExpression()->eval_int() < 0) {
			ss << _var->getExpression()->eval_int();
			string element = ss.str();
            Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,_var->getSymbol()->getName(),element);
            if(_t == ASSIGN) {
				_var->getSymbol()->set(_value->eval_int(),0);
			} else if (_t == PLUSASSIGN) {
				_var->getSymbol()->set(curVal+_value->eval_int(),0);
			} else if (_t == MINUSASSIGN) {
				_var->getSymbol()->set(curVal-_value->eval_int(),0);
			}
		} else if(_t == ASSIGN) {
			_var->getSymbol()->set(_value->eval_int(),_var->getExpression()->eval_int());
		} else if (_t == PLUSASSIGN) {
			_var->getSymbol()->set(curVal+_value->eval_int(),_var->getExpression()->eval_int());
		} else if (_t == MINUSASSIGN) {
			_var->getSymbol()->set(curVal-_value->eval_int(),_var->getExpression()->eval_int());
		}
	} else if (_var->getType() & DOUBLE) {
		double curVal = _var->getSymbol()->get_double_value(_var->getExpression()->eval_int());
		if (_t == ASSIGN) {
			_var->getSymbol()->set(_value->eval_double(),_var->getExpression()->eval_int());
		} else if (_t == PLUSASSIGN) {
			_var->getSymbol()->set(curVal+_value->eval_double(),_var->getExpression()->eval_int());
		} else if (_t == MINUSASSIGN) {
			_var->getSymbol()->set(curVal-_value->eval_double(),_var->getExpression()->eval_int());
		}
	} else if (_var->getType() & STRING) {
		if (_t == ASSIGN) {
			_var->getSymbol()->set(_value->eval_string(),_var->getExpression()->eval_int());
		} else if (_t == PLUSASSIGN) {
			string curVal = _var->getSymbol()->get_string_value(_var->getExpression()->eval_int());
			_var->getSymbol()->set(curVal+_value->eval_string(),_var->getExpression()->eval_int());

		}
	}

}

Expression* Assign_statement::getValue() {
	return _value;
}