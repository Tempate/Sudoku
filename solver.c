/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "checker.h"
#include "solver.h"

unsigned char blanks[TOTAL];
unsigned char blanks_size = 0;


/* Takes a board and fills it with a solution
 */
void solve(Board *board) {
    getBlankSquares(*board);
    calculatePossible(board);
    
    dfs(board, 0);
}


/* This is a recursive implementation of Depth First Search
 * It works by setting all possible forced values and, when there aren't,
 * generating a new branch for every possible value.
 * Whenever a solution is found, it escalates back up saving the working copy.
 */
bool dfs(Board *board, unsigned char index) {
    Board new_board;
    unsigned char x, y, v;
    unsigned char state, new_index;
    
    // If all values are set, the recursion is finished
    if (index >= blanks_size) return true;
    
    x = GET_X(blanks[index]);
    y = GET_Y(blanks[index]);
    
    // Goes to the next tile if the current one is already set
    if (board->values[y][x] != 0)
        return dfs(board, index+1);
    
    v = getNextPossibleValue(*board, board->values[y][x], x, y);
    
    while (v != 0) {
        // Generates a new copy of the board to improve performance
        new_board = *board;
        new_board.values[y][x] = v;
        new_index = index + 1;
        
        // Removes the picked value from the possible ones
        updateTileAdded(&new_board, y, x);
        
        // Sets all the possible forced tiles
        do {
            state = setAllForced(&new_board, new_index);
        } while (state == 1);
        
        
        if (state != 2 && dfs(&new_board, new_index)) {
            *board = new_board;
            return true;
        }

        v = getNextPossibleValue(*board, v, x, y);
    }

    // There was no possible value to be chosen
    return false;
}

/* Calculates all the possible values for each row, column, and square
 */
void calculatePossible(Board *board) {
    unsigned short pos;
    unsigned char m;
    
    // Goes tile by tile and saves all the found values
    for (char i = 0; i < HEIGHT; i++) {
        for (char j = 0; j < WIDTH; j++) {
            if (board->values[i][j] != 0) {
                m = GET_SQR(i,j);
                pos = POS(board->values[i][j]);
                
                board->rowsPos[i] |= pos;
                board->colsPos[j] |= pos;
                board->sqrsPos[m] |= pos; 
            }
        }
    }
    
    // Inverts the values so only the possible ones remain
    // This can be done because HEIGHT == WIDTH == SQRS
    for (char k = 0; k < HEIGHT; k++) {
        board->rowsPos[k] ^= SIZE;
        board->colsPos[k] ^= SIZE;
        board->sqrsPos[k] ^= SIZE;
    }
}


/* Updates the possible values for the row, column, and square of the 
 * tile that was changed.
 * PRE: board->values[y][x] != 0
 */
void updateTileAdded(Board *board, char y, char x) {
    unsigned short val = SIZE ^ POS(board->values[y][x]);
    unsigned char m = GET_SQR(y,x);
    
    board->rowsPos[y] &= val;
    board->colsPos[x] &= val;
    board->sqrsPos[m] &= val;
}

/* Saves the tile that are blank to the blanks array.
 * Using binary to save memory, it maps i to the most significant nibble
 * and j to the least significant nibble.
 * It sets the globals blanks and blanks_size.
 */
unsigned char getBlankSquares(Board board) {
    for (char i = 0; i < HEIGHT; i++) {
        for (char j = 0; j < WIDTH; j++) {
            if (board.values[i][j] == 0) {
                blanks[blanks_size++] = GET_CRD(i, j);
            }
        }
    }
}

/* Takes a tile with a value and returns its next possible value.
 * If none, it returns 0.
 */
unsigned char getNextPossibleValue(Board board, const char val, const char x, const char y) {
    unsigned char m = GET_SQR(y, x);
    unsigned short possible = board.rowsPos[y] & board.colsPos[x] & board.sqrsPos[m];
    
    for (char k = val; k < RANGE; k++) {
        if (VAL_IN_BYTE(possible, k))
            return k + 1;
    }
    
    return 0;
}

/* Sets all the tiles that only have one possible value.
 * 
 * Returns:
 *  1 if a modification was made
 *  2 if there is a tile with no possible value
 *  any other value if no modifications were made
 */
char setAllForced(Board *board, unsigned char min) {
    unsigned char x, y, m;
    unsigned short possible;
    char state = 0;

    for (char i = min; i < blanks_size && state != 2; i++) {
        x = GET_X(blanks[i]);
        y = GET_Y(blanks[i]);
        m = GET_SQR(y, x);
        possible = board->rowsPos[y] & board->colsPos[x] & board->sqrsPos[m];
        
        if (board->values[y][x] == 0) {
            state = 2 - __builtin_popcount(possible);
            
            if (state == 1) {
                board->values[y][x] = log_2(possible)+1;
                updateTileAdded(board, y, x);
            }
        }
    }
    
    return state;
}

unsigned char log_2(unsigned short index) {
    unsigned char val = 0;
    while (index >>= 1) ++val;
    return val;
} 