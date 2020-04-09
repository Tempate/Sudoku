#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

struct Square {
    int x;
    int y;
    int z;

    Square(const int x, const int y): x{x}, y{y} {
        // The z coordinate is the quadrant
        z = 3 * (y / 3) + x / 3;
    }

    int getPossible(const Board &board) const {
        return board.colsPossible[x] & board.rowsPossible[y] & board.quadPossible[z];
    }
};

void solve(Board &board);
bool dfs(Board &board, const std::vector<Square> &blanks, int index);

void calculatePossible(Board &board);
void updatePossible(Board &board, const Square &sqr);

std::vector<Square> genBlankSquares(const Board &board);
int nextPossibleValue(const Board &board, const Square &sqr, const int current);
int setForced(Board &board, const std::vector<Square> &blanks, const int index);

#endif /* SOLVER_H */

