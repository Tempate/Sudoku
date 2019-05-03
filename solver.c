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

#define SQRS WIDTH * HEIGHT / (SQR * SQR)


// Calculates the possible values inside of certain squares
void calculatePossible(Board *board) {
    unsigned int rows[HEIGHT] = {0}, cols[WIDTH] = {0}, sqrs[SQRS] = {0};
    int pos, m, k;
    
    // Goes tile by tile and ORs against the values in the respective arrays
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] != 0) {
                m = GET_SQR(i,j);
                pos = POS(board->values[i][j]);
                rows[i] |= pos;
                cols[j] |= pos;
                sqrs[m] |= pos;
                board->possible[i][j] = 0;
            }
        }
    }
    
    // Inverts the values
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                k = GET_SQR(i,j);
                board->possible[i][j] = SIZE ^ (rows[i] | cols[j] | sqrs[k]);
            } else {
                board->possible[i][j] = 0;
            }
        }
    }
}


// Call this instead of calculatePossible when the value of a tile is changed
// This function removes 'val' from possible in the row, col and sqr
// It also checks to make sure it's leaving no tile with no possible values
// which is what it returns
// PRE: board->values[x][y] != 0
bool updateTileAdded(Board *board, const int y, const int x) {
    unsigned int val = SIZE ^ POS(board->values[y][x]);
    int x0 = SQR*(int)(x/SQR), y0 = SQR*(int)(y/SQR);
    
    board->possible[y][x] = 0;
    
    for (int j = 0; j < WIDTH; j++){
        if (board->values[y][j] == 0) {
            board->possible[y][j] &= val;
            if (board->possible[y][j] == 0) return false;
        }
            
    }

    for (int i = 0; i < HEIGHT; i++){
        if (board->values[i][x] == 0) {
            board->possible[i][x] &= val;
            if (board->possible[i][x] == 0) return false;
        } 
    }
    
    for (int i = y0; i < y0 + SQR; i++){
        for (int j = x0; j < x0 + SQR; j++){
            if (board->values[i][j] == 0) {
                board->possible[i][j] &= val;
                if (board->possible[i][j] == 0) return false;
            }
        }
    }
    
    return true;
}

unsigned int depthFS(Board *board) {
    unsigned short stack[HEIGHT*WIDTH];
    char index = 0, val, x, y;
    unsigned int count = 0;
    bool possible;
    
    calculatePossible(board);
    
    FEEDFORWARD:
    count++;
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                possible = addNextPossibleValue(board, &index, stack, 0, i, j);
                if (!possible) goto BACKPROPAGATION;
            }
        }
    }
    
    if (finishedBoard(*board)) return count;
    
    BACKPROPAGATION:
        
    do {
        index--;
        if (index < 0) {
            printf("[-] A solution wasn't found.\n");
            exit(1);
        }

        val = GET_VAL(stack[index]);
        x = GET_X(stack[index]);
        y = GET_Y(stack[index]);

        board->values[y][x] = 0;
        calculatePossible(board);
    } while ((1 << val) > board->possible[y][x]);

    possible = addNextPossibleValue(board, &index, stack, val, y, x);

    if (possible) {
        goto FEEDFORWARD;
    } else {
        goto BACKPROPAGATION;
    }
    
    return count;
}

// Returns whether the changes leave any tile with no possible values
bool addNextPossibleValue(Board *board, char *index, unsigned short *c, const char start, const char i, const char j) {
    for (int k = start; k < RANGE; k++) {
        if (VAL_IN_BYTE(board->possible[i][j], k)) {
            board->values[i][j] = k+1;
            
            c[*index] = (unsigned short) SET_VAL(i,j,k+1);
            (*index)++;
            
            return updateTileAdded(board,i,j);
        }
    }
}
