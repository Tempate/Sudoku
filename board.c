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
            board->possible[i][j] = ALL;
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

Board duplicateBoard(Board *b){
    Board *newB = malloc(sizeof(Board));

    //TODO: Improve this using std::copy
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            newB->values[i][j] = b->values[i][j];
            newB->possible[i][j] = b->possible[i][j];
        }
    }

    return *newB;
}

void updateRemeaning(Board *board){
    //TODO: if we want to use unsigned int, instead of -1 use MAXINDEX + 1,
    //so that there are no collisions (i dont understand how the indices work that well)
    int nums = 0;
    int newA[WIDTH*HEIGHT];
    //Detect the number of elements and copy them to the temp array
    for (int i = 0; i < board->remLen; ++i){
        if (board->remeaning[i] != -1)
            newA[nums++] = board->remeaning[i];
    }
    
    if (nums != board->remLen){
        //Copy the elements back to the array
        for (int i = 0; i < nums; ++i)
            board->remeaning[i] = newA[i];

        //Set the rest of elems to -1 (not really necessary)
        for (int i = nums; i < WIDTH*HEIGHT; ++i)
            board->remeaning[i] = -1;

        board->remLen = nums;
    }
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