/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdbool.h>
#include <math.h>
#include "board.h"


bool checkBoard(Board board) {
    //In a solved sudoku, all the numbers add to 45 in each row / col / sqr
    int pos, row, col, sqr;
    
    // Checks to see if all rows match
    for (int i = 0; i < HEIGHT; i++) {
        row = 0;
        for (int j = 0; j < WIDTH; j++) {
            row += board.values[i][j];
        }
        if (row != SUM)
            return false;
    }
    
    // Checks to see if all columns match
    for (int i = 0; i < WIDTH; i++) {
        col = 0;
        for (int j = 0; j < HEIGHT; j++) {
            col += board.values[j][i];
        }
        if (col != SUM)
            return false;
    }
    
    // Checks to see if all squares match
    for (int i = 0; i < HEIGHT / SQR; i += SQR) {
        for (int j = 0; j < WIDTH / SQR; j += SQR) {
            sqr = 0;
            for (int k = i; k < i + SQR; k++) {
                for (int l = j; l < j + SQR; l++) {
                    sqr += board.values[k][l];
                }
            }
            if (sqr != SUM)
                return false;
        }
    }
    
    return true;
}

bool completeBoard(Board board) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] <= 0 || board.values[i][j] > RANGE) return false;
        }
    }
    
    return true;
}

bool finishedBoard(Board board) {
    return completeBoard(board) && checkBoard(board);
}