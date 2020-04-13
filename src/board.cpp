#include <iostream>
#include <cassert>
#include <array>
#include <vector>

#include "main.h"
#include "board.h"

#define MAX (1 << RANGE) - 1

Board::Board() {
    for (int y = 0; y < HEIGHT; y++)
        values[y] = {0};
}

void Board::print() const {
    std::cout << std::endl;

    for (const auto row : values) {
        for (const int token : row) {
            if (token == 0)
                std::cout << " .";
            else
                std::cout << " " << token;
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

bool Board::complete() const {
    for (const auto row : values) {
        for (const int token : row) {
            if (token <= 0 || token > RANGE)
                return false;
        }
    }

    return true;
}

bool Board::check() const {
    assert(complete());

    // Ensure rows have an iteration of numbers from 1 to 9
    for (int j = 0; j < HEIGHT; j++) {
        int numbers = 0;
        
        for (int i = 0; i < WIDTH; i++)
            numbers ^= toBinary(values[j][i]);

        if (numbers != MAX)
            return false;
    }

    // Ensure columns have an iteration of numbers from 1 to 9
    for (int i = 0; i < WIDTH; i++) {
        int numbers = 0;
        
        for (int j = 0; j < HEIGHT; j++)
            numbers ^= toBinary(values[j][i]);

        if (numbers != MAX)
            return false;
    }

    // Ensure quadrants have an iteration of numbers from 1 to 9
    for (int i = 0; i < HEIGHT / REGION; i += REGION) {
        for (int j = 0; j < WIDTH / REGION; j += REGION) {
            int numbers = 0;

            for (int k = i; k < i + REGION; k++) {
                for (int l = j; l < j + REGION; l++)
                    numbers ^= toBinary(values[k][l]);
            }

            if (numbers != MAX)
                return false;
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

void Board::calculatePossible() {
    for (int i = 0; i < WIDTH; i++)
        colsPossible[i] = MAX;

    for (int i = 0; i < HEIGHT; i++)
        rowsPossible[i] = MAX;

    for (int i = 0; i < REGIONS; i++)
        regsPossible[i] = MAX;

    // Saves values that aren't possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (values[y][x] != 0) {
                const int bin = toBinary(values[y][x]);
                const Token token{x, y};

                colsPossible[token.x] ^= bin;
                rowsPossible[token.y] ^= bin;
                regsPossible[token.z] ^= bin;
            }
        }
    }
}

int Board::getPossible(const Token &token) const {
    return colsPossible[token.x] & rowsPossible[token.y] & regsPossible[token.z];
}

void Board::updatePossible(const Token &token) {
    const int possible = MAX ^ toBinary(getValue(token));

    colsPossible[token.x] &= possible;
    rowsPossible[token.y] &= possible;
    regsPossible[token.z] &= possible;
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

int Board::setForced(std::vector<Token> &blanks) {
    int state = UNMODIFIED;
    
    for (int i = blanks.size() - 1; i >= 0; i--) {
        Token token = blanks[i];
        assert(getValue(token) == 0);

        const int possible = getPossible(token);

        if (!possible)
            return DEAD_END;
            
        if ((setForcedToken(token, possible) == MODIFIED) || 
            (setForcedInRow(token) == MODIFIED) ||
            (setForcedInCol(token) == MODIFIED) ||
            (setForcedInReg(token) == MODIFIED)) {
            blanks.erase(blanks.begin() + i);
            state = MODIFIED;
        }
    }

    return state;
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

    const int y0 = 3 * (token.y / 3);
    const int x0 = 3 * (token.x / 3);

    for (int y = y0; y < y0 + 3; y++) {
        for (int x = x0; x < x0 + 3; x++) {
            if (values[y][x] == 0 && !(x == token.x && y == token.y))
                possible &= MAX ^ getPossible(Token{x, y});
        }
    }

    return setForcedToken(token, possible);
}