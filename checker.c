/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdbool.h>
#include <math.h>
#include "board.h"

bool checkBoard(Board board) {
    int pos, row, col, sqr;
    
    // Checks to see if all rows match
    for (int i = 0; i < HEIGHT; i++) {
        row = 0;
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] != 0) {
                pos = POS(board.values[i][j]);
                if ((pos & row) != 0) return false;
                row |= pos;
            }
        }
    }
    
    // Checks to see if all columns match
    for (int i = 0; i < WIDTH; i++) {
        col = 0;
        for (int j = 0; j < HEIGHT; j++) {
            if (board.values[j][i] != 0) {
                pos = POS(board.values[j][i]);
                if ((pos & col) != 0) return false;
                col |= pos;
            }
        }
    }
    
    // Checks to see if all squares match
    for (int i = 0; i < HEIGHT / SQR; i += SQR) {
        for (int j = 0; j < WIDTH / SQR; j += SQR) {
            sqr = 0;
            for (int k = i; k < i + SQR; k++) {
                for (int l = j; l < j + SQR; l++) {
                    if (board.values[k][l] != 0) {
                        pos = POS(board.values[k][l]);
                        if ((pos & sqr) != 0) return false;
                        sqr |= pos;
                    }
                }
            }
        }
    }
    
    return true;
}

bool completeBoard(Board board) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] <= 0 | board.values[i][j] > RANGE) return false;
        }
    }
    
    return true;
}

bool finishedBoard(Board board) {
    return completeBoard(board) && checkBoard(board);
}
