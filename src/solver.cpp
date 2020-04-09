/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <vector>

#include "main.h"
#include "board.h"
#include "solver.h"

#define MODIFIED 1
#define DEAD_END 2


void solve(Board &board) {
    const std::vector<Square> blanks = genBlankSquares(board);
    
    calculatePossible(board);
    dfs(board, blanks, 0);
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
    // Saves what values are not possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board.values[y][x] != 0) {
                const Square sqr{x, y};

                board.colsPossible[sqr.x] |= board.values[y][x];
                board.rowsPossible[sqr.y] |= board.values[y][x];
                board.quadPossible[sqr.z] |= board.values[y][x];
            }
        }
    }
    
    // Inverts them to get the possible
    for (int i = 0; i < HEIGHT; i++)
        board.colsPossible[i] ^= MAX;

    for (int i = 0; i < WIDTH; i++)
        board.rowsPossible[i] ^= MAX;

    for (int i = 0; i < QUADRANTS; i++)
        board.quadPossible[i] ^= MAX;
}

bool dfs(Board &board, const std::vector<Square> &blanks, const int index) {
    // If all values are set, the recursion is finished
    if (index >= blanks.size()) 
        return true;
    
    const Square sqr = blanks[index];
    
    // Goes to the next tile if the current one is already set
    if (board.values[sqr.y][sqr.x] != 0)
        return dfs(board, blanks, index + 1);
    
    int v = nextPossibleValue(board, sqr, 0);
    
    while (v != 0) {
        Board new_board = board;
        new_board.values[sqr.y][sqr.x] = v;
        
        updatePossible(new_board, sqr);
        
        int state;

        do {
            state = setForced(new_board, blanks, index + 1);
        } while (state == MODIFIED);
        
        
        if (state != DEAD_END && dfs(new_board, blanks, index + 1)) {
            board = new_board;
            return true;
        }

        v = nextPossibleValue(board, sqr, v + 1);
    }

    // There was no possible value to be chosen
    return false;
}

// PRE: board.values[sqr.y][sqr.x] != 0
void updatePossible(Board &board, const Square &sqr) {
    const int possible = MAX ^ pow2(board.values[sqr.y][sqr.x]);

    board.colsPossible[sqr.x] &= possible;
    board.rowsPossible[sqr.y] &= possible;
    board.quadPossible[sqr.z] &= possible;
}

// Returns 0 when there are no possible values left
int nextPossibleValue(const Board &board, const Square &sqr, const int value) {
    const int possible = sqr.getPossible(board);

    for (int k = value + 1; k < RANGE; k++) {
        if (possible & pow2(k))
            return k;
    }

    return 0;
}

int setForced(Board &board, const std::vector<Square> &blanks, const int index) {
    int state = 0;
    
    for (int i = index; i < blanks.size() && state != DEAD_END; i++) {
        const Square sqr = blanks[i];
        const int possible = sqr.getPossible(board);

        if (board.values[sqr.y][sqr.x] == 0) {
            state = 2 - __builtin_popcount(possible);

            if (state == MODIFIED) {
                board.values[sqr.y][sqr.x] = log2plus1(possible);
                updatePossible(board, sqr);
            }
        }
    }

    return state;
}