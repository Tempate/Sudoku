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

#include "board.h"


Board blankBoard(void) {
    /* Creates an empty board with all values set to zero */
    Board *board = malloc(sizeof(Board));
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board->values[i][j] = 0;
            board->possible[i][j] = (int) pow(2, RANGE) - 1;
        }
    }
    
    return *board;
}

Board filledBoard(int values[HEIGHT][WIDTH]) {
    /* Creates a board and files the values from an array */
    Board *board = malloc(sizeof(Board));
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board->values[i][j] = values[i][j];
            board->possible[i][j] = 0;
        }
    }
    
    return *board;
}

Board randomBoard(void) {
    /* Creates a random board
       Every square has a 33% of having a random value
     */
    Board *board = malloc(sizeof(Board)); 
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (rand() % 3 == 0) {
                board->values[i][j] = 1 + (rand() % RANGE);
            } else {
                board->values[i][j] = 0;
            }
            board->possible[i][j] = 0;
        }
    }
    
    return *board;
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

void printPossible(Board board) {
    printf("Possible board values: \n");
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] == 0) {
                printf("%d%d : ", i+1, j+1);

                bool first = true;
                for (int k = 0; k < RANGE; k++) {
                    if (1 << k & board.possible[i][j]) {
                        if (!first) printf(", ");
                        printf("%d", k + 1);
                        first = false;
                    }
                }

                printf("\n");
            }
            
        }
    }
}

