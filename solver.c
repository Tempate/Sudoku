/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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
// PRE: board->values[x][y] != 0
void updateTileAdded(Board *board, const int y, const int x) {
    unsigned int val = SIZE ^ POS(board->values[y][x]);
    int x0 = SQR*(int)(x/SQR), y0 = SQR*(int)(y/SQR);
    board->possible[y][x] = 0;
    
    // Update rows
    for (int j = 0; j < WIDTH; j++){
        if (board->values[y][j] == 0)
            board->possible[y][j] &= val;
    }

    // Update cols
    for (int i = 0; i < HEIGHT; i++){
        if (board->values[i][x] == 0) 
            board->possible[i][x] &= val;
    }
    
    // Update sqrs
    for (int i = y0; i < y0 + SQR; i++){
        for (int j = x0; j < x0 + SQR; j++){
            if (board->values[i][j] == 0) 
                board->possible[i][j] &= val;
        }
    }
}

void depthFS(Board *board) {
    unsigned short *c = malloc(HEIGHT * WIDTH * sizeof(short));
    char index = 0, val, x, y;
    
    
    FEEDFORWARD:
    
    calculatePossible(board);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                if (board->possible[i][j] == 0) {
                    goto BACKPROPAGATION;
                } else {
                    addNextPossibleValue(board, &index, c, 0, i, j);
                }
            }
        }
    }
    
    if (!completeBoard(*board)) {
        BACKPROPAGATION:
        
        do {
            index--;
            if (index < 0) {
                printf("[-] A solution wasn't found.\n");
                exit(1);
            }
            
            val = GET_VAL(c[index]);
            x = GET_X(c[index]);
            y = GET_Y(c[index]);

            board->values[y][x] = 0;
            calculatePossible(board);
        } while ((1 << val) > board->possible[y][x]);
        
        addNextPossibleValue(board, &index, c, val, y, x);
        goto FEEDFORWARD;
    } else if (!checkBoard(*board)) {
        printf("[-] The solution found has errors.");
        exit(1);
    }
}

void addNextPossibleValue(Board *board, char *index, unsigned short *c, const char start, const char i, const char j) {
    for (int k = start; k < RANGE; k++) {
        if (VAL_IN_BYTE(board->possible[i][j], k)) {
            c[*index] = (unsigned short) SET_VAL(i,j,k+1);
            board->values[i][j] = k+1;

            updateTileAdded(board,i,j);

            (*index)++;
            return;
        }
    }
}