#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <string>

#include "main.h"
#include "board.h"

#define MAX (1 << RANGE) - 1

Board::Board() {
    for (int y = 0; y < HEIGHT; y++)
        values[y] = {0};
}

bool Board::complete() const {
    for (const auto &row : values) {
        for (const int value : row) {
            if (!(value > 0 && value <= RANGE))
                return false;
        }
    }

    return true;
}

bool Board::checkRow() const {
    for (int y = 0; y < HEIGHT; y++) {
        int row = 0;
        
        for (int x = 0; x < WIDTH; x++) {
            const int bin = toBinary(values[y][x]);

            if (bin & row)
                return false;

            row ^= bin;
        }
    }

    return true;
}

bool Board::checkCol() const {
    for (int x = 0; x < WIDTH; x++) {
        int col = 0;
        
        for (int y = 0; y < HEIGHT; y++) {
            const int bin = toBinary(values[y][x]);

            if (bin & col)
                return false;

            col ^= bin;
        }
    }

    return true;
}

bool Board::checkReg() const {
    for (int y = 0; y < HEIGHT / REGION; y += REGION) {
        for (int x = 0; x < WIDTH / REGION; x += REGION) {
            int reg = 0;

            for (int j = y; j < y + REGION; j++) {
                for (int i = x; i < x + REGION; i++) {
                    const int bin = toBinary(values[j][i]);

                    if (bin & reg)
                        return false;

                    reg ^= bin;
                }
            }
        }
    }

    return true;
}

int Board::getValue(const Token &token) const {
    return values[token.y][token.x];
}

void Board::setValue(const Token &token, const int value) {
    values[token.y][token.x] = value;
    updatePossible(token);
}

std::vector<Token> Board::getTokens(const int type) const {
    std::vector<Token> squares;
    squares.reserve(HEIGHT * WIDTH);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (type == BLANK && values[y][x] == 0)
                squares.emplace_back(x, y);
            else if (type == NOT_BLANK && values[y][x] != 0)
                squares.emplace_back(x, y);
        }
    }

    return squares;
}

void Board::setRandomValue(const Token &token) {
    std::vector<int> possibleValues = binaryToList(getPossible(token));    
    assert(possibleValues.size() > 0);

    setValue(token, possibleValues[rand() % values.size()]);
}

int Board::nextPossibleValue(const Token &token, const int value) const {
    const int possible = getPossible(token) >> value;

    if (!possible)
        return UNMODIFIED;

    return value + __builtin_ctz(possible) + 1; 
}

int Board::setForcedToken(const Token &token, const int possible) {
    if (__builtin_popcount(possible) != 1)    
        return UNMODIFIED;

    setValue(token, fromBinary(possible));

    return MODIFIED;
}

int Board::setForcedInRow(const Token &token) {
    int possible = getPossible(token);

    for (int x = 0; x < WIDTH; x++) {
        if (values[token.y][x] == 0 && x != token.x)
            possible &= MAX ^ getPossible(Token{x, token.y});
    }

    return setForcedToken(token, possible);
}

int Board::setForcedInCol(const Token &token) {
    int possible = getPossible(token);

    for (int y = 0; y < HEIGHT; y++) {
        if (values[y][token.x] == 0 && y != token.y)
            possible &= MAX ^ getPossible(Token{token.x, y});
    }

    return setForcedToken(token, possible);
}

int Board::setForcedInReg(const Token &token) {
    int possible = getPossible(token);

    const int y0 = REGION * (token.y / REGION);
    const int x0 = REGION * (token.x / REGION);

    for (int y = y0; y < y0 + REGION; y++) {
        for (int x = x0; x < x0 + REGION; x++) {
            if (values[y][x] == 0 && !(x == token.x && y == token.y))
                possible &= MAX ^ getPossible(Token{x, y});
        }
    }

    return setForcedToken(token, possible);
}