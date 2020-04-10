#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

struct Square {
    int x;
    int y;
    int z;

    int possible;

    Square(const int x, const int y): x{x}, y{y} {
        z = 3 * (y / 3) + x / 3; // The z coordinate is the quadrant
    }

    Square(const Board &board, const int x, const int y): x{x}, y{y} {
        z = 3 * (y / 3) + x / 3;
        updatePossible(board);
    }

    int updatePossible(const Board &board) {
        possible = board.colsPossible[x] & board.rowsPossible[y] & board.quadPossible[z];
        return possible;
    }
};

void solve(Board &board);
std::vector<Square> init(Board &board);
bool dfs(Board &board, const std::vector<Square> &blanks, int index);

void updatePossible(Board &board, const Square &sqr);

int nextPossibleValue(const Board &board, Square &sqr, const int current);
int setForced(Board &board, const std::vector<Square> &blanks, const int index);

#endif /* SOLVER_H */

