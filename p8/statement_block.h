#ifndef STATEMENT_BLOCK_H
#define STATEMENT_BLOCK_H

#include <vector>
#include <iostream>
class Statement;

class Game_object;


class Statement_block {
public:
    Statement_block();
    Statement_block(Statement_block*);
    std::vector<Statement*> getVector();
    void addStatement(Statement*);
    void execute();
private:
    std::vector<Statement*> _statements;
};

#endif // #ifndef STATEMENT_BLOCK_H
