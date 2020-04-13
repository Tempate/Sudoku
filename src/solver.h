#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

#define UNMODIFIED 0
#define MODIFIED 1
#define DEAD_END 2

#define NO_VALUES_LEFT 0

int solve(Board &board, const int flag);
int dfs(Board &board, std::vector<Token> &blanks, const int n);

void updatePossible(Board &board, const Token &token);

int setForced(Board &board, std::vector<Token> &blanks);

int setForcedTile(Board &board, const Token &token, const int possible);

int setForcedInRow   (Board &board, const Token &token);
int setForcedInColumn(Board &board, const Token &token);
int setForcedInRegion(Board &board, const Token &token);

#endif /* SOLVER_H */

