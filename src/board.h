#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

class Board;

#include "token.h"

#define HEIGHT 9
#define WIDTH 9
#define RANGE 9

#define REGION 3
#define REGIONS WIDTH * HEIGHT / (REGION * REGION)

#define MAX (1 << RANGE) - 1

enum {BLANK, NOT_BLANK};

class Board {
    public:
    std::array<std::array<int, WIDTH>, HEIGHT> values;

    std::array<int, HEIGHT> colsPossible = {0};
    std::array<int, WIDTH> rowsPossible = {0};
    std::array<int, REGIONS> quadPossible = {0};

    Board();

    Board(const std::array<std::array<int, WIDTH>, HEIGHT> values):
    values{values} {};

    void print() const;
    bool complete() const;
    bool check() const;

    int getValue(const Token &token) const;
    void fill(const Token &token, const int value);
    std::vector<Token> getTokens(const int type) const;
    
    void calculatePossible();
    void updatePossible(const Token &token);
};

#endif /* BOARD_H */
