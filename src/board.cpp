#include <iostream>
#include <cassert>
#include <array>
#include <vector>

#include "main.h"
#include "token.h"
#include "board.h"

#define MAX (1 << RANGE) - 1

Board::Board() {
    values = {{ {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} }};
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

void Board::fill(const Token &token, const int value) {
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
    colsPossible = {0};
    rowsPossible = {0};
    quadPossible = {0};

    // Saves values that aren't possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (values[y][x] != 0) {
                const int bin = toBinary(values[y][x]);
                const Token token{x, y};

                colsPossible[token.x] |= bin;
                rowsPossible[token.y] |= bin;
                quadPossible[token.z] |= bin;
            }
        }
    }
    
    // Invert them to get the possible
    for (int i = 0; i < WIDTH; i++)
        colsPossible[i] ^= MAX;

    for (int i = 0; i < HEIGHT; i++)
        rowsPossible[i] ^= MAX;

    for (int i = 0; i < REGIONS; i++)
        quadPossible[i] ^= MAX;
}

void Board::updatePossible(const Token &token) {
    const int possible = MAX ^ toBinary(getValue(token));

    colsPossible[token.x] &= possible;
    rowsPossible[token.y] &= possible;
    quadPossible[token.z] &= possible;
}