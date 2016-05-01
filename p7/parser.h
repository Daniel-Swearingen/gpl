// updated 2/14/2016

/*
  ALWAYS include parser.h instead of y.tab.h

  Here is why:

    Since the union created by bison contains several gpl objects,
    a forward class declaration, or including the .h file must be 
    done before the union is created.

    Bison generates y.tab.h where the union is created. 
    I can't find a way to get bison to generate a y.tab.h that includes 
    forward class declarations and #include's, so I created this file.


*/

#ifndef PARSER_H
#define PARSER_H

// for each type used in the union, you will need to 
// put the appropriate include here
// For example, the union must hold a C++ string, so <string> is included
//   since it is bad programming practice to include "using namespace std"
//   in a .h file, you will have to put "std::string" in the union instead
//   of "string"
#include "expression.h"

#include "event_manager.h"
#include "gpl_type.h"
#include "window.h"
#include "print_statement.h"
#include "exit_statement.h"
#include "assign_statement.h"
#include "if_statement.h"
#include "for_statement.h"
#include <string>
#include <stack>
#include "y.tab.h"

#endif // #ifndef PARSER_H
