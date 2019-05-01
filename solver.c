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
int called = 0;
// Calculates the possible values inside of certain squares
void calculatePossible(Board *board) {
    called++;
    unsigned int rows[HEIGHT], cols[WIDTH], sqrs[SQRS];
    int pos, m, k;
    
    // In the case HEIGHT == WIDTH == SQRS loops would be optimized
    for (int i = 0; i < HEIGHT; i++) rows[i] = 0;
    for (int i = 0; i < WIDTH; i++) cols[i] = 0;
    for (int i = 0; i < SQRS; i++) sqrs[i] = 0;
    
    //Goes tile by tile and ORs against the values in the respective arrays
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
    
    //Inverts the values
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board->values[i][j] == 0) {
                k = GETSQR(i,j);
                board->possible[i][j] = ALL ^ (rows[i] | cols[j] | sqrs[k]);
            } else {
                board->possible[i][j] = 0;
            }
        }
    }
}

//J
//Call this instead of calculatePossible when the value of a tile is changed
//This function removes 'val' from possible in the row, col and sqr
//PRE: board->values[x][y] != 0
void updateTileAdded(Board *board, int y, int x){
    unsigned int val = ALL ^ POS(board->values[y][x]);
    board->possible[y][x] = 0;
    //Update rows
    for (int j = 0; j < WIDTH; ++j){
        if (board->values[y][j] == 0)
            board->possible[y][j] &= val;
    }

    //Update cols
    for (int i = 0; i < HEIGHT; ++i){
        if (board->values[i][x] == 0) 
            board->possible[i][x] &= val;
    }

    int offsetX = (int)SQR*(x / SQR), offsetY = (int)SQR*(y / SQR);
    //Update sqrs
    for (int i = offsetY; i < offsetY + SQR; ++i){
        for (int j = offsetX; j < offsetX + SQR; ++j){
            if (board->values[i][j] == 0) 
                board->possible[i][j] &= val;
        }
    }
}

//J
//temp is there because if the value is set to 0, it would be impossible to retrieve
void updateTileRemoved(Board *board, unsigned char temp, int y, int x){
    unsigned int val = POS(temp);
    unsigned int row = 0, col = 0, sqr = 0;

    //Update rows
    for (int j = 0; j < WIDTH; ++j){
        if (board->values[y][j] == 0)
            board->possible[y][j] |= val;
        else
            row |= board->values[y][j];
    }

    //Update cols
    for (int i = 0; i < HEIGHT; ++i){
        if (board->values[i][x] == 0) 
            board->possible[i][x] |= val;
        else
            col |= board->values[i][x];
    }

    //Update sqrs
    int offsetX = (int)SQR*(x / SQR), offsetY = (int)SQR*(y / SQR);
    for (int i = offsetY; i < offsetY + SQR; ++i){
        for (int j = offsetX; j < offsetX + SQR; ++j){
            if (board->values[i][j] == 0) 
                board->possible[i][j] |= val;
            else
                sqr |= board->values[i][j];
        }
    }
    
    board->possible[y][x] = (ALL ^ (row | col | sqr)) | val;
}

void depthFS(Board *board) {
    unsigned int *c = (unsigned int *) malloc(HEIGHT * WIDTH * sizeof(int *));
    int index = 0;
    int x, y;   //J

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
                            
                            updateTileAdded(board, i, j);   //J

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

        //some memoization
        x = GET_X(c[index]);
        y = GET_Y(c[index]);

        board->values[y][x] = 0;
        calculatePossible(board);   //HERE
        
        while ((1 << GET_VAL(c[index])) > board->possible[y][x]) {
            if (index == 0) {
                printf("[-] A solution wasn't found.\n");
                exit(1);
            }
            
            index--;
            y = GET_Y(c[index]);
            x = GET_X(c[index]);

            board->values[y][x] = 0;
            calculatePossible(board);   //HERE
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
    printf("Calls to calculatePossible: %d\n", called); //J
}
