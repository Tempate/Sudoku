#ifndef TOKEN_H
#define TOKEN_H

struct Token;

#include "board.h"

static inline int calcRegion(const int x, const int y) {
    return 3 * (y / 3) + x / 3;
}

struct Token {
    int x;
    int y;
    int z;

    int possible;

    Token(const int x, const int y): x{x}, y{y} {
        z = calcRegion(x, y);
    }

    Token(const Board &board, const int x, const int y): x{x}, y{y} {
        z = calcRegion(x, y);
        updatePossible(board);
    }

    int updatePossible(const Board &board);
    int randomValue(const Board &board);

    std::vector<int> getPossibleValues(const Board &board);
};


#endif /* TOKEN_H */