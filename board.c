/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include "board.h"


Board getBoardWithValues(int values[HEIGHT][WIDTH]) {
    /* Creates a board and files the values from an array */
    Board board = (Board) {
        .rowsPos = {0},
        .colsPos = {0},
        .sqrsPos = {0}
    };
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board.values[i][j] = values[i][j];
        }
    }
    
    return board;
}


void printBoard(Board board) {
    printf("Board values: \n");
    printf("---------------------------\n");
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] == 0) {
                printf("   ");
            } else {
                printf(" %hhu ", board.values[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("---------------------------\n");
}

