#include <vector>

#include "main.h"
#include "board.h"
#include "token.h"

int Token::updatePossible(const Board &board) {
    possible = board.colsPossible[x] & board.rowsPossible[y] & board.regsPossible[z];
    return possible;
}

int Token::randomValue(const Board &board) {
    std::vector<int> values = getPossibleValues(board);    
    assert(values.size() > 0);

    return values[rand() % values.size()];
}

std::vector<int> Token::getPossibleValues(const Board &board) {
    std::vector<int> values;

    updatePossible(board);
    
    for (int v = 1; v <= RANGE; v++) {
        if (toBinary(v) & possible)
            values.push_back(v);
    }

    return values;
}