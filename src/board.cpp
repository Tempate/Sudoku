#include <iostream>
#include <cassert>
#include <array>
#include <vector>

#include "main.h"
#include "board.h"

#define MAX (1 << RANGE) - 1

void Board::print() const {
    std::cout << std::endl;

    for (const auto row : values) {
        for (const int sqr : row) {
            if (sqr == 0)
                std::cout << " .";
            else
                std::cout << " " << sqr;
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

bool Board::complete() const {
    for (const auto row : values) {
        for (const int sqr : row) {
            if (sqr <= 0 || sqr > RANGE)
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
    for (int i = 0; i < HEIGHT / QUADRANT; i += QUADRANT) {
        for (int j = 0; j < WIDTH / QUADRANT; j += QUADRANT) {
            int numbers = 0;

            for (int k = i; k < i + QUADRANT; k++) {
                for (int l = j; l < j + QUADRANT; l++)
                    numbers ^= toBinary(values[k][l]);
            }

            if (numbers != MAX)
                return false;
        }
    }

    return true;
}

std::vector<Square> Board::genBlankSquares() const {
    std::vector<Square> blanks;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (values[y][x] == 0)
                blanks.emplace_back(x, y);
        }
    }

    return blanks;
}

void Board::calculatePossible() {
    // Saves values that aren't possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (values[y][x] != 0) {
                const int bin = toBinary(values[y][x]);
                const Square sqr{x, y};

                colsPossible[sqr.x] |= bin;
                rowsPossible[sqr.y] |= bin;
                quadPossible[sqr.z] |= bin;
            }
        }
    }
    
    // Invert them to get the possible
    for (int i = 0; i < WIDTH; i++)
        colsPossible[i] ^= MAX;

    for (int i = 0; i < HEIGHT; i++)
        rowsPossible[i] ^= MAX;

    for (int i = 0; i < QUADRANTS; i++)
        quadPossible[i] ^= MAX;
}