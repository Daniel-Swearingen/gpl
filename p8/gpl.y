

%{  
extern int yylex();               // this lexer function returns next token
extern int yyerror(const char *); // used to print errors
extern int line_count;            // current line in the input; from record.l

#include "error.h"                // class for printing errors (used by gpl)
#include "parser.h"
#include <iostream>
#include "symbol_table.h"
#include "variable.h"
using namespace std;

Symbol_table symbolTable;
Game_object *object_under_construction;
string object_under_construction_name;
stack<Statement_block*> statement_block_stack;
Window::Keystroke ks;
%} 



%union {
 int                    union_int;
 std::string            *union_string;  // MUST be a pointer to a string (this sucks!)
 double                 union_double;
 char                   union_char;
 Gpl_type               union_Gpl_type;
 Expression             *union_Expression;
 Operator_type          union_Operator_type;
 Symbol*                union_Symbol;
 Window::Keystroke      union_Keystroke;
 Statement_block*       union_Statement_block;
 Statement*             union_Statement;
}

%error-verbose

%token T_INT                 "int"
%token T_DOUBLE              "double"
%token T_STRING              "string"
%token T_TRIANGLE            "triangle"
%token T_PIXMAP              "pixmap"
%token T_CIRCLE              "circle"
%token T_RECTANGLE           "rectangle"
%token T_TEXTBOX             "textbox"
%token <union_int> T_FORWARD "forward" // value is line number
%token T_INITIALIZATION      "initialization"

%token T_TRUE                "true"
%token T_FALSE               "false"

%token T_TOUCHES             "touches"
%token T_NEAR                "near"

%token T_ANIMATION           "animation"

%token T_IF                  "if"
%token T_FOR                 "for"
%token T_ELSE                "else"
%token <union_int> T_PRINT   "print" // value is line number
%token <union_int> T_EXIT    "exit" // value is line number

%token T_LPAREN              "("
%token T_RPAREN              ")"
%token T_LBRACE              "{"
%token T_RBRACE              "}"
%token T_LBRACKET            "["
%token T_RBRACKET            "]"
%token T_SEMIC               ";"
%token T_COMMA               ","
%token T_PERIOD              "."

%token T_ASSIGN              "="
%token T_PLUS_ASSIGN         "+="
%token T_MINUS_ASSIGN        "-="

%token T_ASTERISK            "*"
%token T_DIVIDE              "/"
%token T_MOD                 "%"
%token T_PLUS                "+"
%token T_MINUS               "-"
%token T_SIN                 "sin"
%token T_COS                 "cos"
%token T_TAN                 "tan"
%token T_ASIN                "asin"
%token T_ACOS                "acos"
%token T_ATAN                "atan"
%token T_SQRT                "sqrt"
%token T_FLOOR               "floor"
%token T_ABS                 "abs"
%token T_RANDOM              "random"

%token T_LESS                "<"
%token T_GREATER             ">"
%token T_LESS_EQUAL          "<="
%token T_GREATER_EQUAL       ">="
%token T_EQUAL               "=="
%token T_NOT_EQUAL           "!="

%token T_AND                 "&&"
%token T_OR                  "||"
%token T_NOT                 "!"

%token T_ON                  "on"
%token T_SPACE               "space"
%token T_LEFTARROW           "leftarrow"
%token T_RIGHTARROW          "rightarrow"
%token T_UPARROW             "uparrow"
%token T_DOWNARROW           "downarrow"
%token T_LEFTMOUSE_DOWN      "leftmouse_down"
%token T_MIDDLEMOUSE_DOWN    "middlemouse_down"
%token T_RIGHTMOUSE_DOWN     "rightmouse_down"
%token T_LEFTMOUSE_UP        "leftmouse_up"
%token T_MIDDLEMOUSE_UP      "middlemouse_up"
%token T_RIGHTMOUSE_UP       "rightmouse_up"
%token T_MOUSE_MOVE          "mouse_move"
%token T_MOUSE_DRAG          "mouse_drag"
%token T_F1                  "f1"
%token T_AKEY                "akey"
%token T_SKEY                "skey"
%token T_DKEY                "dkey"
%token T_FKEY                "fkey"
%token T_HKEY                "hkey"
%token T_JKEY                "jkey"
%token T_KKEY                "kkey"
%token T_LKEY                "lkey"
%token T_WKEY                "wkey"

%token <union_string> T_ID              "identifier"
%token <union_int> T_INT_CONSTANT       "int constant"
%token <union_double> T_DOUBLE_CONSTANT "double constant"
%token <union_string> T_STRING_CONSTANT "string constant"

// special token that does not match any production
// used for characters that are not part of the language
%token T_ERROR               "error"

%left T_OR
%left T_AND
%left T_EQUAL T_NOT_EQUAL
%left T_LESS T_GREATER T_LESS_EQUAL T_GREATER_EQUAL
%left T_PLUS T_MINUS
%left T_ASTERISK T_DIVIDE T_MOD
%right T_NOT 
%nonassoc IF_NO_ELSE
%nonassoc T_ELSE

%type <union_Gpl_type> T_INT
%type <union_Gpl_type> T_DOUBLE
%type <union_Gpl_type> T_STRING
%type <union_Gpl_type> simple_type
%type <union_Gpl_type> object_type
%type <union_Symbol>   animation_parameter

%type <union_Expression> optional_initializer
%type <union_Expression> expression
%type <union_Expression> primary_expression
%type <union_Expression> variable
%type <union_Operator_type> math_operator
%type <union_Operator_type> geometric_operator

%type <union_Keystroke> keystroke
%type <union_Statement_block> statement_block
%type <union_Statement_block> end_of_statement_block
%type <union_Statement_block> if_block
%type <union_Statement> statement
%type <union_Statement> if_statement
%type <union_Statement> for_statement
%type <union_Statement> print_statement
%type <union_Statement> assign_statement
%type <union_Statement> exit_statement
%type <union_string> check_animation_parameter
%% // indicates the start of the rules

//---------------------------------------------------------------------
program:
    declaration_list block_list{
        Symbol_table* t = symbolTable.instance();
        t->checkAnimationsForBodies();
    }
    ;

//---------------------------------------------------------------------
declaration_list:
    declaration_list declaration
    | empty 
    ;

//---------------------------------------------------------------------
declaration:
    variable_declaration T_SEMIC
    | object_declaration T_SEMIC
    | forward_declaration T_SEMIC
    ;

//---------------------------------------------------------------------
variable_declaration:
    simple_type  T_ID  optional_initializer {   
        Symbol_table* t = symbolTable.instance();
        Symbol* s = new Symbol();
        if($3 != NULL) {
            if ($1 & INT) {
                s->setType($1);
                s->setName(*$2);
                if ($3->getType() == 1) {
                    s->set(new int(0));
                    Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,"double",*$2,"int");
                } else if ($3->getType() == 2) {
                    s->set(new int(0));
                    Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,"string",*$2,"int");
                } else {
                    s->set(new int($3->eval_int()));
                }
            } else if ($1 & DOUBLE) {
                s->setType($1);
                s->setName(*$2);
                if ($3->getType() == 2) {
                    s->set(new double(0));
                    Error::error(Error::INVALID_TYPE_FOR_INITIAL_VALUE,"string",*$2,"double");
                } else {
                    s->set(new double($3->eval_double()));
                }
            } else if ($1 & STRING) {
                s->setType($1);
                s->set(new string($3->eval_string()));
                s->setName(*$2);
            }
        } else {
            if ($1 & INT) {
                s->setType($1);
                s->set(new int(0));
                s->setName(*$2);
            } else if ($1 & DOUBLE) {
                s->setType($1);
                s->set(new double(0));
                s->setName(*$2);
            } else if ($1 & STRING) {
                s->setType($1);
                s->set(new string(""));
                s->setName(*$2);
            }
        }
        t->add(s);
    }
    | simple_type  T_ID  T_LBRACKET expression T_RBRACKET {
        if ($4->getType() != 0) {
            if ($4->getType() == 1) {
                Error::error(Error::INVALID_ARRAY_SIZE,*$2,$4->eval_string());
            } else if ($4->getType() == 2) {
                Error::error(Error::INVALID_ARRAY_SIZE,*$2,$4->eval_string());
            }
        } else if ($4->eval_int() == 0) {
            Error::error(Error::INVALID_ARRAY_SIZE,*$2,"0");
        } else {
            Symbol_table* t = symbolTable.instance();
            Symbol* s = new Symbol();
            s->setSize($4->eval_int());
            s->setName(*$2);
            if ( $1 & INT) {
                s->setType(INT_ARRAY);
                int* myIntArr = new int[$4->eval_int()];
                for (int i = 0; i < $4->eval_int(); ++i) {
                    myIntArr[i] = 0;
                }
                s->set(myIntArr);
            } else if ( $1 & DOUBLE_ARRAY) {
                s->setType(DOUBLE_ARRAY);
                double* myDoubleArr = new double[$4->eval_int()];
                for (int i = 0; i < $4->eval_int(); ++i) {
                    myDoubleArr[i] = 0;
                }
                s->set(myDoubleArr);
            } else if ( $1 & STRING_ARRAY) {
                s->setType(STRING_ARRAY);
                string* myStringArr = new string[$4->eval_int()];
                for (int i = 0; i < $4->eval_int(); ++i) {
                    myStringArr[i] = "";
                }
                s->set(myStringArr);
            }
            t->add(s);
        }
    }
    ;

//---------------------------------------------------------------------
simple_type:
    T_INT {   
        $$ = INT;
    }   
    | T_DOUBLE {
       $$ = DOUBLE;
    }
    | T_STRING {
        $$ = STRING;
    }
    ;

//---------------------------------------------------------------------
optional_initializer:
    T_ASSIGN expression {
        $$ = $2;
    }
    | empty {
        $$ = NULL;
    }
    ;

//---------------------------------------------------------------------
object_declaration:
    object_type T_ID {
        object_under_construction_name = *$2;
        Symbol *sym = new Symbol();
        if ($1 == TRIANGLE) {
            object_under_construction = new Triangle();
            sym->setType(TRIANGLE);
        } else if ($1 == PIXMAP) {
            object_under_construction = new Pixmap();
            sym->setType(PIXMAP);
        } else if ($1 == CIRCLE) {
            object_under_construction = new Circle();
            sym->setType(CIRCLE);
        } else if ($1 == RECTANGLE) {
            object_under_construction = new Rectangle();
            sym->setType(RECTANGLE);
        } else {
            object_under_construction = new Textbox();
            sym->setType(TEXTBOX);
        }
        Symbol_table* t = symbolTable.instance();
        sym->setName(*$2);
        sym->set(object_under_construction);
        t->add(sym);
    } T_LPAREN parameter_list_or_empty T_RPAREN
    | object_type T_ID T_LBRACKET expression T_RBRACKET {
        Symbol_table* t = symbolTable.instance();
        Symbol *sym = new Symbol();
        sym->setName(*$2);
        sym->setSize($4->eval_int());
        if ($1 == TRIANGLE) {
            Triangle* myTriangleArr = new Triangle[$4->eval_int()];
            sym->set(myTriangleArr);
            sym->setType(TRIANGLE_ARRAY);
        } else if ($1 == PIXMAP) {
            sym->set(new Pixmap[$4->eval_int()]);
            sym->setType(PIXMAP_ARRAY);
        } else if ($1 == CIRCLE) {
            sym->set(new Circle[$4->eval_int()]);
            sym->setType(CIRCLE_ARRAY);
        } else if ($1 == RECTANGLE) {
            sym->set(new Rectangle[$4->eval_int()]);
            sym->setType(RECTANGLE_ARRAY);
        } else {
            sym->set(new Textbox[$4->eval_int()]);
            sym->setType(TEXTBOX_ARRAY);
        }
        t->add(sym);
    }
    ;

//---------------------------------------------------------------------
object_type:
    T_TRIANGLE{
        $$ = TRIANGLE;
    }
    | T_PIXMAP {
        $$ = PIXMAP;
    }
    | T_CIRCLE {
        $$ = CIRCLE;
    }
    | T_RECTANGLE {
        $$ = RECTANGLE;
    }
    | T_TEXTBOX {
        $$ = TEXTBOX;
    }
    ;

//---------------------------------------------------------------------
parameter_list_or_empty :
    parameter_list {

    }
    | empty
    ;

//---------------------------------------------------------------------
parameter_list :
    parameter_list T_COMMA parameter
    | parameter {

    }
    ;

//---------------------------------------------------------------------
parameter:
    T_ID T_ASSIGN expression {
        Status stat;
        Symbol_table* t = symbolTable.instance();
        if (*$1 == "size") {
            if (t->lookup(object_under_construction_name)->get_type() & 128 ) {
                stat = object_under_construction->set_member_variable(*$1,$3->eval_int());
            } else {
                stat = object_under_construction->set_member_variable(*$1,$3->eval_double());
            }
        } else if (*$1 == "x" || *$1 == "y" || *$1 == "w" || *$1 == "h" || *$1 == "visible" || *$1 == "proximity" || *$1 == "drawing_order" || *$1 == "radius" || *$1 == "space" ||*$1 == "user_int" || *$1 == "user_int2" || *$1 == "user_int3" || *$1 == "user_int4" || *$1 == "user_int5") {
            if ($3->getType() > 0) {
                stat = MEMBER_NOT_OF_GIVEN_TYPE;
            } else {
                stat = object_under_construction->set_member_variable(*$1,$3->eval_int());    
            }
        } else if (*$1 == "red" || *$1 == "green" || *$1 == "blue" || *$1 == "skew" || *$1 == "rotation" || *$1 == "rotation" || *$1 == "user_double" || *$1 == "user_double2" || *$1 == "user_double3" || *$1 == "user_double4" || *$1 == "user_double5") {
            stat = object_under_construction->set_member_variable(*$1,$3->eval_double());
        } else if (*$1 == "text" || *$1 == "filename" || *$1 == "user_string" || *$1 == "user_string2" || *$1 == "user_string3" || *$1 == "user_string4" || *$1 == "user_string5") {
            stat = object_under_construction->set_member_variable(*$1,$3->eval_string());
        } else if (*$1 == "animation_block") {
            if ($3->eval_animation_block()->get_parameter_symbol()->get_type() != t->lookup(object_under_construction_name)->get_type()) {
                Error::error(Error::TYPE_MISMATCH_BETWEEN_ANIMATION_BLOCK_AND_OBJECT,object_under_construction_name,$3->eval_animation_block()->name());
            } else {
                stat = object_under_construction->set_member_variable(*$1,$3->eval_animation_block());
            }
        } else {
            stat = MEMBER_NOT_DECLARED;
        }


        if (stat == MEMBER_NOT_OF_GIVEN_TYPE) {
            Error::error(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE,object_under_construction_name,*$1);
        } else if (stat == MEMBER_NOT_DECLARED) {
            Error::error(Error::UNKNOWN_CONSTRUCTOR_PARAMETER,t->lookup(object_under_construction_name)->getType(),*$1);
        }
    }
    ;

//---------------------------------------------------------------------
forward_declaration:
    T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN {
        Symbol_table* t = symbolTable.instance();
        Symbol* sym = new Symbol();
        sym->set(new Animation_block());
        sym->setName(*$3);
        sym->setType(ANIMATION_BLOCK);
        sym->get_animation_block_value()->initialize($5, *$3);
        t->add(sym);
    }
    ;

//---------------------------------------------------------------------
block_list:
    block_list block
    | empty
    ;

//---------------------------------------------------------------------
block:
    initialization_block
    | animation_block
    | on_block
    ;

//---------------------------------------------------------------------
initialization_block:
    T_INITIALIZATION statement_block {
        Event_manager* em = Event_manager::instance();
        em->addStatementBlock(Window::INITIALIZE,$2);
    }
    ;

//---------------------------------------------------------------------
animation_block:
    T_ANIMATION T_ID T_LPAREN check_animation_parameter {
        Symbol_table* t = symbolTable.instance();
        if (t->lookup(*$2) == NULL) {
            statement_block_stack.push(new Statement_block());
            Error::error(Error::NO_FORWARD_FOR_ANIMATION_BLOCK,*$2);
        } else {
            if (t->lookup(*$2)->get_animation_block_value()->getFlag() == true) {
                statement_block_stack.push(new Statement_block());
                Error::error(Error::PREVIOUSLY_DEFINED_ANIMATION_BLOCK,*$2);
            } else if (t->lookup(*$2)->get_animation_block_value()->get_parameter_symbol()->getName() != t->lookup(*$4)->getName()) {
                statement_block_stack.push(new Statement_block());
                Error::error(Error::ANIMATION_PARAM_DOES_NOT_MATCH_FORWARD);
            } else {
                statement_block_stack.push(t->lookup(*$2)->get_animation_block_value());
            }
        }

    } T_RPAREN T_LBRACE statement_list T_RBRACE end_of_statement_block {

        Symbol_table* t = symbolTable.instance();
        if (t->lookup(*$2) != NULL) {
            t->lookup(*$2)->get_animation_block_value()->setFlag(true);
            t->lookup(*$2)->get_animation_block_value()->setBody($10);
        }
    }
    ;

//---------------------------------------------------------------------
animation_parameter:
    object_type T_ID {
        Symbol_table* t = symbolTable.instance();
        if (t->lookup(*$2) != NULL) {
            Error::error(Error::ANIMATION_PARAMETER_NAME_NOT_UNIQUE,*$2);
            $$ = NULL;
        } else {
        Symbol *sym = new Symbol();
            if ($1 == TRIANGLE) {
                sym->set(new Triangle());
                sym->setType(TRIANGLE);
            } else if ($1 == PIXMAP) {
                sym->set(new Pixmap());
                sym->setType(PIXMAP);
            } else if ($1 == CIRCLE) {
                sym->set(new Circle());
                sym->setType(CIRCLE);
            } else if ($1 == RECTANGLE) {
                sym->set(new Rectangle());
                sym->setType(RECTANGLE);
            } else {
                sym->set(new Textbox());
                sym->setType(TEXTBOX);
            }
            sym->setName(*$2);
            sym->get_game_object_value()->never_animate();
            sym->get_game_object_value()->never_draw();
            t->add(sym);
            $$ = sym;
        }
    }
    ;

//---------------------------------------------------------------------
check_animation_parameter:
    T_TRIANGLE T_ID {
        $$ = $2;
    }
    | T_PIXMAP T_ID {
        $$ = $2;
    }
    | T_CIRCLE T_ID {
        $$ = $2;
    }
    | T_RECTANGLE T_ID {
        $$ = $2;
    }
    | T_TEXTBOX T_ID {
        $$ = $2;
    }
    ;

//---------------------------------------------------------------------
on_block:
    T_ON keystroke statement_block {
        Event_manager* em = Event_manager::instance();
        em->addStatementBlock($2,$3);
    }
    ;

//---------------------------------------------------------------------
keystroke:
    T_SPACE {
        $$ = Window::SPACE;
    }
    | T_UPARROW {
        $$ = Window::UPARROW;
    }
    | T_DOWNARROW {
        $$ = Window::DOWNARROW;
    }
    | T_LEFTARROW {
        $$ = Window::LEFTARROW;
    }
    | T_RIGHTARROW {
        $$ = Window::RIGHTARROW;
    }
    | T_LEFTMOUSE_DOWN {
        $$ = Window::LEFTMOUSE_DOWN;
    }
    | T_MIDDLEMOUSE_DOWN {
        $$ = Window::MIDDLEMOUSE_DOWN;
    }
    | T_RIGHTMOUSE_DOWN {
        $$ = Window::RIGHTMOUSE_DOWN;
    }
    | T_LEFTMOUSE_UP {
        $$ = Window::RIGHTMOUSE_UP;
    }
    | T_MIDDLEMOUSE_UP {
        $$ = Window::MIDDLEMOUSE_UP;
    }
    | T_RIGHTMOUSE_UP {
        $$ = Window::RIGHTMOUSE_UP;
    }
    | T_MOUSE_MOVE {
        $$ = Window::MOUSE_MOVE;
    }
    | T_MOUSE_DRAG {
        $$ = Window::MOUSE_DRAG;
    }
    | T_AKEY {
        $$ = Window::AKEY;
    }
    | T_SKEY {
        $$ = Window::SKEY;
    }
    | T_DKEY {
        $$ = Window::DKEY;
    }
    | T_FKEY {
        $$ = Window::FKEY;
    }
    | T_HKEY {
        $$ = Window::HKEY;
    }
    | T_JKEY {
        $$ = Window::JKEY;
    }
    | T_KKEY {
        $$ = Window::KKEY;
    }
    | T_LKEY {
        $$ = Window::LKEY;
    }
    | T_WKEY {
        $$ = Window::WKEY;
    }
    | T_F1 {
        $$ = Window::F1;
    }
    ;

//---------------------------------------------------------------------
if_block:
    statement_block_creator statement end_of_statement_block {
        $$ = $3;
    }
    | statement_block {
        $$ = $1;
    }
    ;

//---------------------------------------------------------------------
statement_block:
    T_LBRACE statement_block_creator statement_list T_RBRACE end_of_statement_block {
        $$ = $5;
    }
    ;

//---------------------------------------------------------------------
statement_block_creator:
    {
       statement_block_stack.push(new Statement_block());
    }
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
end_of_statement_block:
    {
        $$ = new Statement_block(statement_block_stack.top());
        statement_block_stack.pop();
    }
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
statement_list:
    statement_list statement 
    | empty
    ;

//---------------------------------------------------------------------
statement:
    if_statement {
        $$ = $1;
    }
    | for_statement {
        $$ = $1;
    }
    | assign_statement T_SEMIC {
        $$ = $1;
    }
    | print_statement T_SEMIC {
        $$ = $1;
    }
    | exit_statement T_SEMIC {
        $$ = $1;
    }
    ;

//---------------------------------------------------------------------
if_statement:
    T_IF T_LPAREN expression T_RPAREN if_block %prec IF_NO_ELSE {
        if ($3->getType() != 0) {
            Error::error(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION);
        } else {
            If_statement* st = new If_statement($3,$5);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        }
    }
    | T_IF T_LPAREN expression T_RPAREN if_block T_ELSE if_block {
        if ($3->getType() != 0) {
            Error::error(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION);
        } else {
            If_statement* st = new If_statement($3,$5,$7);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        }
    }
    ;

//---------------------------------------------------------------------
for_statement:
    T_FOR T_LPAREN statement_block_creator assign_statement end_of_statement_block T_SEMIC expression T_SEMIC statement_block_creator assign_statement end_of_statement_block T_RPAREN statement_block {
        For_statement* st = new For_statement($5,$7,$11,$13);
        statement_block_stack.top()->addStatement(st);
        $$ = st;
    }
    ;

//---------------------------------------------------------------------
print_statement:
    T_PRINT T_LPAREN expression T_RPAREN {
        if ($3->getType() == 2 || $3->getType() == 1 || $3->getType() == 0) {
            Print_statement* st = new Print_statement($3,$1);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        } else if ($3->getType() == 3) {
            Error::error(Error::INVALID_TYPE_FOR_PRINT_STMT_EXPRESSION);
        }
    }
    ;

//---------------------------------------------------------------------
exit_statement:
    T_EXIT T_LPAREN expression T_RPAREN {
        if ($3->getType() == 0) {
            Exit_statement* st = new Exit_statement($1,$3);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        } else {
            string type;
            if ($3->getType() == 1) {
                type = "double";
            } else if ($3->getType()) {
                type = "string";
            } else {
                type = "Game_object";
            }
            Error::error(Error::EXIT_STATUS_MUST_BE_AN_INTEGER,type);
        }
    }
    ;

//---------------------------------------------------------------------
assign_statement:
    variable T_ASSIGN expression {
        Symbol_table* t = symbolTable.instance();
        if ($3->getVar() != NULL) {
            if ($1->getVar()->getSymbol()->get_type() & GAME_OBJECT && $3->getVar()->getSymbol()->get_type() & ANIMATION_BLOCK) {
                
                Symbol* param = $3->getVar()->getSymbol()->get_animation_block_value()->get_parameter_symbol();
                if ($1->getVar()->getSymbol()->getType() != param->getType()) {
                    Error::error(Error::ANIMATION_BLOCK_ASSIGNMENT_PARAMETER_TYPE_ERROR,$1->getVar()->getSymbol()->getType(),param->getType());
                }
                
            }
        }
        if ($1->getType() < $3->getType()) {
            string t1,t2;
            if ($1->getType() == 0) {
                t1 = "int";
            } else if ($1->getType() == 1) {
                t1 = "double";
            } else {
                t1 = "string";
            }
            if ($3->getType() == 0) {
                t2 = "int";
            } else if ($3->getType() == 1) {
                t2 = "double";
            } else {
                t2 = "string";
            }
            Error::error(Error::ASSIGNMENT_TYPE_ERROR,t1,t2);
        } else if ($1->getVar() != NULL && $1->getVar()->getType() == GAME_OBJECT) {
            Error::error(Error::INVALID_LHS_OF_ASSIGNMENT,$1->getVar()->getSymbol()->getName(),$1->getVar()->getSymbol()->get_game_object_value()->type());
        }  else {
            Assign_statement* st = new Assign_statement($1->getVar(),$3,Assign_statement::ASSIGN);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        }
    }
    | variable T_PLUS_ASSIGN expression {

        if ($1->getVar() != NULL && $1->getVar()->getType() == GAME_OBJECT) {
            Error::error(Error::INVALID_LHS_OF_PLUS_ASSIGNMENT,$1->getVar()->getSymbol()->getName(),$1->getVar()->getSymbol()->get_game_object_value()->type());
        } else if ($1->getType() < $3->getType()) {
            string t1,t2;
            if ($1->getType() == 0) {
                t1 = "int";
            } else if ($1->getType() == 1) {
                t1 = "double";
            } else {
                t1 = "string";
            }
            if ($3->getType() == 0) {
                t2 = "int";
            } else if ($3->getType() == 1) {
                t2 = "double";
            } else {
                t2 = "string";
            }
            Error::error(Error::PLUS_ASSIGNMENT_TYPE_ERROR,t1,t2);
        } else {
            Assign_statement* st = new Assign_statement($1->getVar(),$3,Assign_statement::PLUSASSIGN);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        }
    }
    | variable T_MINUS_ASSIGN expression {
        if ($1->getType() == 2) {
            string t1,t2;
            if ($1->getType() == 0) {
                t1 = "int";
            } else if ($1->getType() == 1) {
                t1 = "double";
            } else {
                t1 = "string";
            }
            Error::error(Error::INVALID_LHS_OF_MINUS_ASSIGNMENT,$1->getVar()->getSymbol()->getName(),t1);
        } else if ($1->getType() < $3->getType()) {
            string t1,t2;
            if ($1->getType() == 0) {
                t1 = "int";
            } else if ($1->getType() == 1) {
                t1 = "double";
            } else {
                t1 = "string";
            }
            if ($3->getType() == 0) {
                t2 = "int";
            } else if ($3->getType() == 1) {
                t2 = "double";
            } else {
                t2 = "string";
            }
            Error::error(Error::MINUS_ASSIGNMENT_TYPE_ERROR,t1,t2);
        } else {
            Assign_statement* st = new Assign_statement($1->getVar(),$3,Assign_statement::MINUSASSIGN);
            statement_block_stack.top()->addStatement(st);
            $$ = st;
        }
    }
    ;

//---------------------------------------------------------------------
variable:
    T_ID {
        Symbol_table* t = symbolTable.instance();
        if (t->lookup(*$1) != NULL) {
            $$ = new Expression(new Variable(t->lookup(*$1)));
        } else {
            $$ = new Expression(0);
            Error::error(Error::UNDECLARED_VARIABLE,*$1);
        }
    }
    | T_ID T_LBRACKET expression T_RBRACKET {
        if ($3->getType() != 0) {
            if ($3->getType() == 1) {
                Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A double expression");    
            } else {
                Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A string expression");
            }
            $$ = new Expression(0);
        } else {
            Symbol_table* t = symbolTable.instance();
            if (t->lookup(*$1) != NULL) {
                if ($3->getType() == 0) {
                    if (t->lookup(*$1)->getArrSize() == -1) {
                        $$ = new Expression(0);
                        Error::error(Error::VARIABLE_NOT_AN_ARRAY,*$1);
                    } else if ($3->getVar() == NULL && ($3->eval_int() > t->lookup(*$1)->getArrSize() || $3->eval_int() < 0)) {
                        $$ = new Expression(0);
                        Error::error(Error::ARRAY_INDEX_OUT_OF_BOUNDS,*$1,$3->eval_string());                        
                    } else {
                        $$ = new Expression(new Variable(t->lookup(*$1),$3));
                    }
                }
            } else {
                $$ = new Expression(0);
            }   
        }
    }
    | T_ID T_PERIOD T_ID {
        Symbol_table* t = symbolTable.instance();
        if (t->lookup(*$1) == NULL) {
            Error::error(Error::UNDECLARED_VARIABLE,*$1);
            $$ = new Expression(0);
        } else {
            int value1;
            double value2;
            string value3;
            if (!(t->lookup(*$1)->get_type() & GAME_OBJECT)) {
                Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
                $$ = new Expression(0);
            } /* else if (t->lookup(*$1)->get_game_object_value()->get_member_variable(*$3,value1) != OK && t->lookup(*$1)->get_game_object_value()->get_member_variable(*$3,value2) != OK && t->lookup(*$1)->get_game_object_value()->get_member_variable(*$3,value3) != OK) {
                Error::error(Error::UNDECLARED_MEMBER,*$1,*$3);
                $$ = new Expression(0);
            } */else {
                $$ = new Expression(new Variable(t->lookup(*$1),*$3));
            }
        }
    }
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID {
        Symbol_table* t = symbolTable.instance();
        if ($3->getType() == 1) {
            Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A double expression");
            $$ = new Expression(0);
        } else if ($3->getType() == 2) {
            Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A string expression");
            $$ = new Expression(0);
        } else if ($3->getType() == 3) {
            Error::error(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER,*$1,"A animation_block expression");
            $$ = new Expression(0);
        }
        if (!(t->lookup(*$1)->get_type() & GAME_OBJECT)) {
                Error::error(Error::LHS_OF_PERIOD_MUST_BE_OBJECT,*$1);
                $$ = new Expression(0);
        } else {
            $$ = new Expression(new Variable(t->lookup(*$1),$3,*$6));
        }
    }
    ;

//---------------------------------------------------------------------
expression:
    primary_expression {
        $$ = $1;
    }
    | expression T_OR expression {
        if ($1->getType() == 2 && $3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"||");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"||");
        } else if ($1->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"||");
        } else if ($3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"||");
        } else if (($1->getType() == 0 || $1->getType() == 1) && ($3->getType() == 0 || $3->getType() == 1)) {
            $$ = new Expression($1,OR,$3);
        }
    }
    | expression T_AND expression {
        if ($1->getType() == 2 && $3->getType() == 2){
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"&&");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"&&");
        } else if ($1->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"&&");
        } else if ($3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"&&");
        } else if (($1->getType() == 0 || $1->getType() == 1) && ($3->getType() == 0 || $3->getType() == 1)) {
            $$ = new Expression($1,AND,$3);
        }
    }
    | expression T_LESS_EQUAL expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,LESS_THAN_EQUAL,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_GREATER_EQUAL  expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,GREATER_THAN_EQUAL,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_LESS expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,LESS_THAN,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_GREATER  expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,GREATER_THAN,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_EQUAL expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,EQUAL,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_NOT_EQUAL expression {
        if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,NOT_EQUAL,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_PLUS expression {
        if ($1->getType() > 2 && $3->getType() < 3) {
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"+");
            $$ = new Expression(0);
        } else if ($1->getType() < 3 && $3->getType() > 2){
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"+");
            $$ = new Expression(0);
        } else if ($1->getType() > 2 && $3->getType() > 2) {
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"+");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"+");
            $$ = new Expression(0);
        } else if ($1->getType() == 0 || $1->getType() == 1 || $1->getType() == 2) {
            if ($3->getType() == 0 || $3->getType() == 1 || $3->getType() == 2) {
                $$ = new Expression($1,PLUS,$3);
            }
        } 
    }
    | expression T_MINUS expression {
        if ($1->getType() == 2 && $3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"-");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"-");
        } else if ($1->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"-");
        } else if ($3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"-");
        } else if ($1->getType() == 0 || $1->getType() == 1) {
            if ($3->getType() == 0 || $3->getType() == 1) {
                $$ = new Expression($1,MINUS,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_ASTERISK expression {
        if ($1->getType() == 2 && $3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"*");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"*");
        } else if ($1->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"*");
        } else if ($3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"*");
        } else if ($1->getType() == 0 || $1->getType() == 1) {
            if ($3->getType() == 0 || $3->getType() == 1) {
                $$ = new Expression($1,MULTIPLY,$3);
            }
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_DIVIDE expression {
        if ($1->getType() == 2 && $3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"/");
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"/");
        } else if ($1->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"/");
        } else if ($3->getType() == 2) {
            $$ = new Expression(0);
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"/");
        } else if ($3->eval_double() == 0) {
            $$ = new Expression(0);
            Error::error(Error::DIVIDE_BY_ZERO_AT_PARSE_TIME);
        } else if ($3->getType() == 0 || $3->getType() == 1) {
            $$ = new Expression($1,DIVIDE,$3);
        } else {
            //error
            $$ = new Expression(0);
        }
    }
    | expression T_MOD expression {
        if ($1->getType() == 0 && $3->getType() == 0) {
            if ($3->eval_int() == 0) {
                Error::error(Error::MOD_BY_ZERO_AT_PARSE_TIME);
                $$ = new Expression(0);
            } else if ($3->getType() == 0) {
                $$ = new Expression($1,MOD,$3);
            }
        } else {
            if ($1->getType() != 0 && $3->getType() != 0) {
                $$ = new Expression(0);
                Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"%");
                Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"%");
            } else if ($1->getType() != 0) {
                $$ = new Expression(0);
                Error::error(Error::INVALID_LEFT_OPERAND_TYPE,"%");
            } else if ($3->getType() != 0) {
                $$ = new Expression(0);
                Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"%");
            }
        }
    }
    | T_MINUS  expression {
        if ($2->getType() == 2) {
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"-");
            $$ = new Expression(0);
        }
        $$ = new Expression($2,UNARY_MINUS,NULL);
    }
    | T_NOT  expression {
        if ($2->getType() == 2) {
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,"!");
            $$ = new Expression(0);
        } else {
            $$ = new Expression($2,NOT,NULL);
        }
    }
    | math_operator T_LPAREN expression T_RPAREN {
        if ($3->getType() == 2) {
            Error::error(Error::INVALID_RIGHT_OPERAND_TYPE,operator_to_string($1));
        }
        $$ = new Expression($3,$1,NULL);
    }
    | variable geometric_operator variable {
        if ($1->getVar()->getType() != GAME_OBJECT && $3->getVar()->getType() != GAME_OBJECT) {
            $$ = new Expression(0);
            Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,$1->getVar()->getSymbol()->getName());
            Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,$3->getVar()->getSymbol()->getName());
        } else if ($1->getVar()->getType() != GAME_OBJECT) {
            Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,$1->getVar()->getSymbol()->getName());
            $$ = new Expression(0);
        } else if ($3->getVar()->getType() != GAME_OBJECT) {
            Error::error(Error::OPERAND_MUST_BE_A_GAME_OBJECT,$3->getVar()->getSymbol()->getName());
            $$ = new Expression(0);
        } else {
            $$ = new Expression($1,$2,$3);
        }
    }
    ;

//---------------------------------------------------------------------
primary_expression:
    T_LPAREN  expression T_RPAREN {
        $$ = $2;
    }
    | variable {
        $$ = $1;
    }
    | T_INT_CONSTANT {
        $$ = new Expression($1);
    }
    | T_TRUE {
        $$ = new Expression(1);
    }
    | T_FALSE {
        $$ = new Expression(0);
    }
    | T_DOUBLE_CONSTANT {
        $$ = new Expression($1);
    }
    | T_STRING_CONSTANT {
        $$ = new Expression(new string(*$1));
    }
    ;

//---------------------------------------------------------------------
geometric_operator:
    T_TOUCHES {
        $$ = TOUCHES;
    }
    | T_NEAR {
        $$ = NEAR;
    }
    ;

//---------------------------------------------------------------------
math_operator:
    T_SIN {
        $$ = SIN;
    }
    | T_COS {
        $$ = COS;
    }
    | T_TAN {
        $$ = TAN;
    }
    | T_ASIN {
        $$ = ASIN;
    }
    | T_ACOS {
        $$ = ACOS;
    }
    | T_ATAN {
        $$ = ATAN;
    }
    | T_SQRT {
        $$ = SQRT;
    }
    | T_ABS {
        $$ = ABS;
    }
    | T_FLOOR {
        $$ = FLOOR;
    }
    | T_RANDOM {
        $$ = RANDOM;
    }
    ;

//---------------------------------------------------------------------
empty:
    // empty goes to nothing so that you can use empty in productions
    // when you want a production to go to nothing
    ;
