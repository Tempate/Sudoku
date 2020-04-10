/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>

#include "main.h"
#include "board.h"
#include "solver.h"

#define UNMODIFIED 0
#define MODIFIED 1
#define DEAD_END 2


void solve(Board &board) {
    calculatePossible(board);
    dfs(board, genBlankSquares(board), 0);
}

std::vector<Square> genBlankSquares(const Board &board) {
    std::vector<Square> blanks;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board.values[y][x] == 0)
                blanks.emplace_back(x, y);
        }
    }

    return blanks;
}

void calculatePossible(Board &board) {
    // Saves values that aren't possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board.values[y][x] != 0) {
                const Square sqr{x, y};

                board.colsPossible[sqr.x] |= pow2(board.values[y][x]);
                board.rowsPossible[sqr.y] |= pow2(board.values[y][x]);
                board.quadPossible[sqr.z] |= pow2(board.values[y][x]);
            }
        }
    }
    
    // Inverts them to get the possible
    for (int i = 0; i < WIDTH; i++)
        board.colsPossible[i] ^= MAX;

    for (int i = 0; i < HEIGHT; i++)
        board.rowsPossible[i] ^= MAX;

    for (int i = 0; i < QUADRANTS; i++)
        board.quadPossible[i] ^= MAX;
}

bool dfs(Board &board, const std::vector<Square> &blanks, const int index) {
    if (index >= blanks.size()) 
        return true;
    
    Square sqr = blanks[index];
    
    // Jumps to the next tile if the current one is already set
    if (board.values[sqr.y][sqr.x] != 0)
        return dfs(board, blanks, index + 1);
    
    int v = nextPossibleValue(board, sqr, 0);
    
    while (v != 0) {
        Board newBoard = board;
        newBoard.values[sqr.y][sqr.x] = v;
        
        updatePossible(newBoard, sqr);
        
        int state;

        do {
            state = setForced(newBoard, blanks, index + 1);
        } while (state == MODIFIED);
        
        if (state != DEAD_END && dfs(newBoard, blanks, index + 1)) {
            board = newBoard;
            return true;
        }

        v = nextPossibleValue(board, sqr, v);
    }

    // There was no possible value to be chosen
    return false;
}

// PRE: board.values[sqr.y][sqr.x] != 0
void updatePossible(Board &board, const Square &sqr) {
    assert(board.values[sqr.y][sqr.x] != 0);
    const int possible = MAX ^ pow2(board.values[sqr.y][sqr.x]);

    board.colsPossible[sqr.x] &= possible;
    board.rowsPossible[sqr.y] &= possible;
    board.quadPossible[sqr.z] &= possible;
}

// Returns 0 when there are no possible values left
int nextPossibleValue(const Board &board, Square &sqr, const int value) {
    const int possible = sqr.updatePossible(board);

    for (int k = value + 1; k <= RANGE; k++) {
        if (possible & pow2(k))
            return k;
    }

    return 0;
}

int setForced(Board &board, const std::vector<Square> &blanks, const int index) {
    int state = UNMODIFIED;
    
    for (int i = index; i < blanks.size() && state != DEAD_END; i++) {
        Square sqr = blanks[i];
        sqr.updatePossible(board);

        if (board.values[sqr.y][sqr.x])
            continue;

        const int phaseState = 2 - __builtin_popcount(sqr.possible);

        switch (phaseState) {
        case MODIFIED:
            board.values[sqr.y][sqr.x] = log2plus1(sqr.possible);
            updatePossible(board, sqr);
            state = MODIFIED;
            break;
        case DEAD_END:
            state = DEAD_END;
            break;
        }
    }

    return state;
}