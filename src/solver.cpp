#include <cassert>
#include <vector>

#include "main.h"
#include "board.h"
#include "solver.h"

#define UNMODIFIED 0
#define MODIFIED 1
#define DEAD_END 2


void solve(Board &board) {
    board.calculatePossible();
    dfs(board, board.genBlankSquares(), 0);
}

bool dfs(Board &board, const std::vector<Square> &blanks, const int index) {
    assert(index >= 0);

    if (index >= blanks.size()) {
        assert(board.check());
        return true;
    }
    
    Square sqr = blanks[index];

    // Jumps to the next tile if the current one is already set
    if (board.values[sqr.y][sqr.x] != 0)
        return dfs(board, blanks, index + 1);
    
    int v = nextPossibleValue(board, sqr, 0);
    
    while (v != 0) {
        Board newBoard = board;
        newBoard.values[sqr.y][sqr.x] = v;

        assert(board.values[sqr.y][sqr.x] == 0);
        assert(newBoard.values[sqr.y][sqr.x] != 0);

        updatePossible(newBoard, sqr);
        
        int state;

        do {
            state = setForced(newBoard, blanks, index + 1);
        } while (state == MODIFIED);
        
        if (state != DEAD_END && dfs(newBoard, blanks, index + 1)) {
            assert(newBoard.check());
            board = newBoard;
            return true;
        }

        v = nextPossibleValue(board, sqr, v);
    }

    return false;
}

void updatePossible(Board &board, const Square &sqr) {
    assert(board.values[sqr.y][sqr.x] != 0);
    const int possible = MAX ^ toBinary(board.values[sqr.y][sqr.x]);

    board.colsPossible[sqr.x] &= possible;
    board.rowsPossible[sqr.y] &= possible;
    board.quadPossible[sqr.z] &= possible;
}

// Returns 0 when there are no possible values left
int nextPossibleValue(const Board &board, Square &sqr, const int value) {
    const int possible = sqr.updatePossible(board);

    for (int k = value + 1; k <= RANGE; k++) {
        if (possible & toBinary(k))
            return k;
    }

    return 0;
}

int setForced(Board &board, const std::vector<Square> &blanks, const int index) {
    int state = UNMODIFIED;
    
    for (int i = index; i < blanks.size(); i++) {
        Square sqr = blanks[i];

        if (board.values[sqr.y][sqr.x])
            continue;

        sqr.updatePossible(board);

        if (!sqr.possible)
            return DEAD_END;
            
        if ((setForcedTile(board, sqr, sqr.possible) == MODIFIED) || 
            (setForcedInRow(board, sqr)              == MODIFIED) ||
            (setForcedInColumn(board, sqr)           == MODIFIED) ||
            (setForcedInQuadrant(board, sqr)         == MODIFIED))
           state = MODIFIED;
    }

    return state;
}

int setForcedTile(Board &board, const Square &sqr, const int possible) {
    if (__builtin_popcount(possible) != 1)    
        return UNMODIFIED;

    board.values[sqr.y][sqr.x] = fromBinary(possible);
    updatePossible(board, sqr);
    return MODIFIED;
}

int setForcedInRow(Board &board, const Square &sqr) {
    int possible = sqr.possible;

    for (int x = 0; x < WIDTH; x++) {
        if (board.values[sqr.y][x] != 0 || x == sqr.x)
            continue;

        Square aux{board, x, sqr.y};
        possible &= MAX ^ aux.possible;
    }

    return setForcedTile(board, sqr, possible);
}

int setForcedInColumn(Board &board, const Square &sqr) {
    int possible = sqr.possible;

    for (int y = 0; y < HEIGHT; y++) {
        if (board.values[y][sqr.x] != 0 || y == sqr.y)
            continue;

        Square aux{board, sqr.x, y};
        possible &= MAX ^ aux.possible;
    }

    return setForcedTile(board, sqr, possible);
}

int setForcedInQuadrant(Board &board, const Square &sqr) {
    int possible = sqr.possible;

    const int y0 = 3 * (sqr.y / 3);
    const int x0 = 3 * (sqr.x / 3);

    for (int y = y0; y < y0 + 3; y++) {
        for (int x = x0; x < x0 + 3; x++) {
            if (board.values[y][x] != 0 || (x == sqr.x && y == sqr.y))
                continue;

            Square aux{board, x, y};
            possible &= MAX ^ aux.possible;
        }
    }

    return setForcedTile(board, sqr, possible);
}