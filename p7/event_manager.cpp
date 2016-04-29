// updated 2/14/2016

#include "event_manager.h"
#include "statement_block.h"
using namespace std;

/* static */ Event_manager *Event_manager::m_instance = 0;

/* static */ Event_manager * Event_manager::instance()
{
  if (!m_instance)
    m_instance = new Event_manager();
  return m_instance;
}

Event_manager::Event_manager(){

}

Event_manager::~Event_manager(){

}

void Event_manager::addStatementBlock(Window::Keystroke keystroke, Statement_block* statement_block){
	_events[keystroke].push_back(statement_block);
}

void Event_manager::execute_handlers(Window::Keystroke keystroke){
	for (vector<Statement_block*>::iterator it = _events[keystroke].begin(); it != _events[keystroke].end(); ++it) {
		(*it)->execute();
	}
}
