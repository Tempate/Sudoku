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
    int pos, m;
    
    if (checkBoard(*board) == false) {
        printf("The given board contains errors.");
        exit(0);
    }
    
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
    
    int k;
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

// Finds the values for a certain row that are forced and fills them
void fillRows(Board *board) {
    int **row = (int **) malloc(HEIGHT * sizeof(int *));
    unsigned char **pos = (unsigned char **) malloc(HEIGHT * sizeof(char *));
    
    inline int f(int i, int j) { return i; }
    
    alloc2Dints(HEIGHT, row);
    alloc2Duchars(HEIGHT, pos);
    findPossibilities(*board, row, pos, f);
    fillUnique(board, HEIGHT, row, pos);   
}

// Finds the values for a certain column that are forced and fills them
void fillCols(Board *board) {
    int **col = (int **) malloc(WIDTH * sizeof(int *));
    unsigned char **pos = (unsigned char **) malloc(WIDTH * sizeof(char *));
    
    inline int f(int i, int j) { return j; }
    
    alloc2Dints(WIDTH, col);
    alloc2Duchars(WIDTH, pos);
    findPossibilities(*board, col, pos, f);
    fillUnique(board, WIDTH, col, pos); 
}

// Finds the values for a certain column that are forced and fills them
void fillSqrs(Board *board) {
    int **sqr = (int **) malloc(SQRS * sizeof(int *));
    unsigned char **pos = (unsigned char **) malloc(SQRS * sizeof(char *));
    
    alloc2Dints(SQRS, sqr);
    alloc2Duchars(SQRS, pos);
    findPossibilities(*board, sqr, pos, GETSQR);
    fillUnique(board, SQRS, sqr, pos); 
}

// Finds all the values that have one possibility
unsigned char *findForced(Board board) {
    unsigned char *forced = malloc(HEIGHT * sizeof(char));
    int index = 0;
    
    calculatePossible(&board);
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (__builtin_popcount(board.possible[i][j]) == 1) {
                forced[index++] = CRD(i,j);
            }
        }
    }
    
    for (int i = index; i < HEIGHT; i++) {
        forced[i] = 0;
    }
    
    return forced;
}

// Sets the only possible values for the wanted squares
void setForced(Board *board, unsigned char *forced) {
    unsigned char val;
    int index = 0;
    
    while (forced[index] != 0) {
        val = forced[index];
        board->values[X(val)][Y(val)] = log2(board->possible[X(val)][Y(val)]) + 1;
        index++;
    }
}


void solve(Board *board) {
    int max = 50;
    
    while (!finishedBoard(*board) && max-- > 0) {
        calculatePossible(board);
        setForced(board, findForced(*board));
        fillRows(board);
        fillCols(board);
        fillSqrs(board);
        printBoard(*board);
    }
}


// AUXILIARY FUNCTIONS

// Allocates memory for a 2D int array
void alloc2Dints(const int a, int **arr) {
    for (int i = 0; i < a; i++) {
        arr[i] = (int *) malloc(RANGE * sizeof(int));
        memset(arr[i], 0, RANGE * sizeof(char));
    }
}

// Allocates memory for a 2D unsigned char array
void alloc2Duchars(const int a, unsigned char **arr) {
    for (int i = 0; i < a; i++) {
        arr[i] = (unsigned char *) malloc(RANGE * sizeof(char));
        memset(arr[i], 0, RANGE * sizeof(char));
    }
}

/* It deals with two arrays:
 *  c: keeps count of the number of times a certain value appears 
 *     in a row, col or square.
 *  pos: keeps the position of the last element counted for each value 
 */
void findPossibilities(Board board, int **c, unsigned char **pos, int (*f)(int,int)) {
    int m;
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            m = (*f)(i,j);
            if (board.possible[i][j] != 0) {
                for (int k = 0; k < RANGE; k++) {
                    if ((board.possible[i][j] & (1<<k)) != 0) {
                        c[m][k]++;
                        pos[m][k] = CRD(i,j);
                    }
                }
            }
        }
    }
}

// Fills the values that only have one possibility for that row, column or square
void fillUnique(Board *board, const int a, int **c, unsigned char **pos) {
    for (int i = 0; i < a; i++) {
        for (int k = 0; k < RANGE; k++) {
            if (c[i][k] == 1) {
                board->values[X(pos[i][k])][Y(pos[i][k])] = k+1;
            }
        }
    }
}
