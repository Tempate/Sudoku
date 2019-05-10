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

void testDFS1(void) {
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
    Board board = getBoardWithValues(values);
    
    printBoard(board);
    solve(&board);
    printBoard(board);
}

void testDFS2(void) {
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
    
    Board board = getBoardWithValues(values);
    
    printBoard(board);
    solve(&board);
    printBoard(board);
}

void testDFS3(void) {
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
    
    Board board = getBoardWithValues(values);
    
    printBoard(board);
    solve(&board);
    printBoard(board);
}

void testDFS4(void) {
    int values[9][9] = {
        {0,2,4,0,0,0,0,0,0},
        {0,0,0,0,0,7,1,0,0},
        {0,9,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,8,4},
        {0,0,0,0,7,5,0,0,0},
        {6,0,0,0,3,0,0,0,0},
        {0,0,0,4,0,0,0,2,9},
        {0,0,0,2,0,0,3,0,0},
        {1,0,0,0,0,0,0,0,0}
    };
    
    Board board = getBoardWithValues(values);
    
    printBoard(board);
    solve(&board);
    printBoard(board);
}