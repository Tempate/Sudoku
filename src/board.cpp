#include <iostream>
#include <array>

#include "board.h"

#define MAX (1 << RANGE) - 1

void Board::print() const {
    std::cout << std::endl;

    for (const auto row : values) {
        for (const int sqr : row) {
            if (sqr) {
                std::cout << "   ";
            } else {
                std::cout << " " << sqr << " ";
            }
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

// The board must be complete to check if all numbers are correct
bool Board::check() const {
    // Ensure rows have an iteration of numbers from 1 to 9
    for (int j = 0; j < HEIGHT; j++) {
        int numbers = 0;
        
        for (int i = 0; i < WIDTH; i++)
            numbers ^= 1 << (values[j][i] - 1);

        if (numbers != MAX)
            return false;
    }

    // Ensure columns have an iteration of numbers from 1 to 9
    for (int i = 0; i < WIDTH; i++) {
        int numbers = 0;
        
        for (int j = 0; j < HEIGHT; j++)
            numbers ^= 1 << (values[j][i] - 1);

        if (numbers != MAX)
            return false;
    }

    // Ensure quadrants have an iteration of numbers from 1 to 9
    for (int i = 0; i < HEIGHT / SQR; i += SQR) {
        for (int j = 0; j < WIDTH / SQR; j += SQR) {
            int numbers = 0;

            for (int k = i; k < i + SQR; k++) {
                for (int l = j; l < j + SQR; l++)
                    numbers ^= 1 << (values[k][l] - 1);
            }

            if (numbers != MAX)
                return false;
        }
    }

    return true;
}
