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

struct Square;

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

    std::vector<Square> genBlankSquares() const;
    void calculatePossible();
};

static inline int calcQuadrant(const int x, const int y) {
    return 3 * (y / 3) + x / 3;
}

struct Square {
    int x;
    int y;
    int z;

    int possible;

    Square(const int x, const int y): x{x}, y{y} {
        z = calcQuadrant(x, y); // The z coordinate is the quadrant
    }

    Square(const Board &board, const int x, const int y): x{x}, y{y} {
        z = calcQuadrant(x, y);
        updatePossible(board);
    }

    int updatePossible(const Board &board) {
        possible = board.colsPossible[x] & board.rowsPossible[y] & board.quadPossible[z];
        return possible;
    }
};

#endif /* BOARD_H */
