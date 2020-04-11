#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

void solve(Board &board);
std::vector<Square> init(Board &board);
bool dfs(Board &board, const std::vector<Square> &blanks, int index);

void updatePossible(Board &board, const Square &sqr);

int nextPossibleValue(const Board &board, Square &sqr, const int current);
int setForced(Board &board, const std::vector<Square> &blanks, const int index);

int setForcedTile(Board &board, const Square &sqr, const int possible);

int setForcedInRow     (Board &board, const Square &sqr);
int setForcedInColumn  (Board &board, const Square &sqr);
int setForcedInQuadrant(Board &board, const Square &sqr);

#endif /* SOLVER_H */

