/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../board.h"
#include "../checker.h"
#include "../solver.h"
#include "../grader.h"


void testNewBoard(void) {
    Board board = blankBoard();
    printBoard(board);
    printPossible(board);
}

void testChecker(void) {
    int values0[4][4] = {{4,3,2,1}, {1,2,3,4}, {2,4,1,3}, {3,1,4,2}};
    int values1[4][4] = {{1,2,3,4}, {3,4,1,2}, {2,3,4,1}, {4,1,2,3}};
    int values2[4][4] = {{1,2,3,4}, {3,4,1,2}, {1,3,4,2}, {4,2,1,3}};
    int values3[4][4] = {{1,3,1,4}, {2,4,3,2}, {3,1,4,1}, {4,2,2,3}};
    int values4[4][4] = {{1,2,3,4}, {2,3,4,1}, {3,4,1,2}, {4,1,2,3}};
    
    Board board0 = filledBoard(values0);
    Board board1 = filledBoard(values1);
    Board board2 = filledBoard(values2);
    Board board3 = filledBoard(values3);
    Board board4 = filledBoard(values4);
    
    // Working
    printBoard(board0);
    printf("Working: %s\n", finishedBoard(board0) ? "true" : "false");
    
    // Working
    printBoard(board1);
    printf("Working: %s\n", finishedBoard(board1) ? "true" : "false");
    
    // Wrong columns
    printBoard(board2);
    printf("Working: %s\n", checkBoard(board2) ? "true" : "false");
    
    // Wrong rows
    printBoard(board3);
    printf("Working: %s\n", checkBoard(board3) ? "true" : "false");
     
    // Wrong squares
    printBoard(board4);
    printf("Working: %s\n", checkBoard(board4) ? "true" : "false");
}

void testPossible(void) {
    int values0[4][4] = {{0,3,2,1}, {1,0,3,4}, {2,4,0,3}, {3,1,4,0}};
    int values1[4][4] = {{1,0,0,0}, {0,2,0,0}, {0,0,3,0}, {0,0,0,4}};
    
    Board board0 = filledBoard(values0);
    Board board1 = filledBoard(values1);
    
    calculatePossible(&board0);
    printBoard(board0);
    printPossible(board0);
    
    calculatePossible(&board1);
    printBoard(board1);
    printPossible(board1);
}

void testPossibleTile(void) {
    int values[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    
    Board board = filledBoard(values);
    
    calculatePossible(&board);
    printBoard(board);
    printPossible(board);
    
    board.values[0][0] = 1;
    
    updateTileAdded(&board, 0, 0);
    printBoard(board);
    printPossible(board);
}

void testDFS1(void) {
    int values[4][4] = {{1,2,0,0},{3,4,0,2},{2,0,0,4},{0,3,2,1}};
    Board board = filledBoard(values);
    
    printBoard(board);
    depthFS(&board);
    printBoard(board);
}

void testDFS2(void) {
    int values[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    Board board = filledBoard(values);
    
    printBoard(board);
    depthFS(&board);
    printBoard(board);
}

void testDFS3(void) {
    int values[9][9] = {
        {0,0,3,0,0,1,7,2,0},
        {0,0,0,0,0,4,0,0,0},
        {0,0,0,7,0,0,1,4,9},
        {0,1,4,8,0,0,0,0,5},
        {2,8,0,0,0,0,0,7,4},
        {7,0,0,0,0,2,6,8,0},
        {9,5,2,0,0,8,0,0,0},
        {0,0,0,3,0,0,0,0,0},
        {0,6,7,9,0,0,4,0,0}
    };
    Board board = filledBoard(values);
    
    printBoard(board);
    depthFS(&board);
    printBoard(board);
}

void testDFS4(void) {
    int values[9][9] = {
        {0,0,5,8,0,0,0,0,7},
        {4,0,0,0,0,0,0,0,0},
        {0,0,0,0,3,0,0,2,8},
        {2,0,0,0,0,4,5,0,0},
        {0,0,4,0,0,0,0,0,0},
        {0,0,0,0,7,6,0,3,0},
        {8,0,1,7,0,0,4,0,0},
        {0,0,2,1,8,0,0,0,0},
        {7,0,0,0,0,0,3,8,0}
    };
    
    Board board = filledBoard(values);
    
    printBoard(board);
    depthFS(&board);
    printBoard(board);
}

void testDFS5(void) {
    int values[9][9] = {
        {0,0,0,2,0,0,0,0,0},
        {0,0,0,0,0,8,4,0,0},
        {0,7,0,4,0,0,8,9,0},
        {0,0,0,0,5,0,0,1,0},
        {0,1,0,0,0,0,0,0,0},
        {0,0,3,0,0,6,0,0,2},
        {0,3,0,0,0,5,7,0,8},
        {8,0,0,0,3,0,0,0,0},
        {0,6,1,0,0,0,0,4,0}
    };
    
    Board board = filledBoard(values);
    
    printBoard(board);
    unsigned int steps = depthFS(&board);
    printBoard(board);
    printf("Steps: %lu\n", steps);
}

void testDFS6(void) {
    int values[9][9] = {
        {0, 2, 4,  0, 0, 0,  0, 0, 0},
        {0, 0, 0,  0, 0, 7,  1, 0, 0},
        {0, 9, 0,  0, 0, 0,  0, 0, 0},
        {0, 0, 0,  0, 0, 0,  0, 8, 4},
        {0, 0, 0,  0, 7, 5,  0, 0, 0},
        {6, 0, 0,  0, 3, 0,  0, 0, 0},
        {0, 0, 0,  4, 0, 0,  0, 2, 9},
        {0, 0, 0,  2, 0, 0,  3, 0, 0},
        {1, 0, 0,  0, 0, 0,  0, 0, 0} //17
    };
    
    Board board = filledBoard(values);
    
    printBoard(board);
    depthFS(&board);
    printBoard(board);
}
