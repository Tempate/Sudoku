/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// The grading system only works for 9x9

#include <stdlib.h>

#include "board.h"
#include "grader.h"


double grade(Board board) {
    int givens = numberOfGivens(board);
    int bound = lowestBoundRowsCols(board);
    
    return givens * 2/3 + bound / 3;
}

int numberOfGivens(Board board) {
    int count = 0;
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] != 0) count++;
        }
    }
    
    if (count >= 50) {
        return 1;
    } else if (count >= 36) {
        return 2;
    } else if (count >= 32) {
        return 3;
    } else if (count >= 28) {
        return 4;
    } else {
        return 5;
    }
}

int lowestBoundRowsCols(Board board) {
    int min = RANGE, c, d;
    
    for (int i = 0; i < HEIGHT; i++) {
        c = 0, d = 0;
        
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[i][j] != 0) c++;
        }
        
        for (int j = 0; j < WIDTH; j++) {
            if (board.values[j][i] != 0) d++;
        }
        
        if (c < min) min = c;
        if (d < min) min = d;
    }
    
    if (min > 5) {
        return 0;
    } else {
        return 5 - min;
    }
}