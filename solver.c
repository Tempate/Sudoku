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
    unsigned int rows[HEIGHT], cols[WIDTH], sqrs[SQRS];
    int pos, m, k;
    
    // In the case HEIGHT == WIDTH == SQRS loops would be optimized
    for (int i = 0; i < HEIGHT; i++) rows[i] = 0;
    for (int i = 0; i < WIDTH; i++) cols[i] = 0;
    for (int i = 0; i < SQRS; i++) sqrs[i] = 0;
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] != 0) {
                m = GETSQR(i,j);
                pos = POS(board->values[i][j]);
                rows[i] |= pos;
                cols[j] |= pos;
                sqrs[m] |= pos;
                board->possible[i][j] = 0;
            }
        }
    }
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                k = GETSQR(i,j);
                board->possible[i][j] = (unsigned int)(pow(2, RANGE) - 1) ^ (rows[i] | cols[j] | sqrs[k]);
            } else {
                board->possible[i][j] = 0;
            }
        }
    }
}

void depthFS(Board *board) {
    unsigned int *c = (unsigned int *) malloc(HEIGHT * WIDTH * sizeof(int *));
    int index = 0;
    
    FEEDFORWARD:
    calculatePossible(board);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                if (board->possible[i][j] == 0) {
                    index--;
                    goto BACKPROPAGATION;
                } else {
                    for (int k = 0; k < RANGE; k++) {
                        if ((board->possible[i][j] & (1<<k)) != 0) {
                            c[index] = SET_VAL(i,j,k+1);
                            board->values[i][j] = k+1;
                            calculatePossible(board);
                            index++;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    index--;
    if (!completeBoard(*board)) {
        BACKPROPAGATION:
        board->values[GET_Y(c[index])][GET_X(c[index])] = 0;
        calculatePossible(board);
        
        while ((1 << GET_VAL(c[index])) > board->possible[GET_Y(c[index])][GET_X(c[index])]) {
            if (index == 0) {
                printf("[-] A solution wasn't found.\n");
                exit(1);
            }
            
            index--;
            board->values[GET_Y(c[index])][GET_X(c[index])] = 0;
            calculatePossible(board);
        }
        
        for (int k = GET_VAL(c[index]); k < RANGE; k++) {
            if ((board->possible[GET_Y(c[index])][GET_X(c[index])] & (1<<k)) != 0) {
                board->values[GET_Y(c[index])][GET_X(c[index])] = k+1;
                c[index] = ((k+1)<<8) + (c[index] & 255);
                index++;
                goto FEEDFORWARD;
            }
        }
    } else if (!checkBoard(*board)) {
        printf("[-] The solution found has errors.");
        exit(1);
    }
}
