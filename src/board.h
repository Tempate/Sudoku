#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#define HEIGHT 9
#define WIDTH 9
#define RANGE 9

#define QUADRANT 3
#define QUADRANTS WIDTH * HEIGHT / (QUADRANT * QUADRANT)

#define MAX (1 << RANGE) - 1

class Board {
    public:
    std::array<std::array<int, WIDTH>, HEIGHT> values;

    std::array<int, HEIGHT> colsPossible = {0};
    std::array<int, WIDTH> rowsPossible = {0};
    std::array<int, QUADRANTS> quadPossible = {0};

    Board() {};

    Board(const std::array<std::array<int, WIDTH>, HEIGHT> values):
    values{values} {};

    void print() const;
    bool complete() const;
    bool check() const;
};

#endif /* BOARD_H */
