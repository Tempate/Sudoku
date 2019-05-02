/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

// The grading system only works for 9x9

#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "grader.h"
#include "solver.h"


double grade(Board board) {
    double givens = (double) numberOfGivens(board);
    double bound = (double) lowestBoundRowsCols(board);
    double nodes = (double) treeLength(board);
    
    return givens/2 + bound/4 + nodes/4;
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
            if (board.values[j][i] != 0) d++;
        }
        
        if (c > 1 && c < min) min = c;
        if (d > 1 && d < min) min = d;
    }
    
    if (min > 5) {
        return 0;
    } else {
        return 5 - min;
    }
}

int treeLength(Board board) {
    long count = depthFS(&board);
    
    if (count <= 100) {
        return 1;
    } else if (count <= 1000) {
        return 2;
    } else if (count <= 10000) {
        return 3;
    } else if (count <= 100000) {
        return 4;
    } else {
        return 5;
    }
}