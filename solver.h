/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   solver.h
 * Author: alpha
 *
 * Created on April 28, 2019, 9:52 AM
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

    
    void calculatePossible(Board *board);
    
    unsigned char* findForced(Board board);
    void setForced(Board *board, unsigned char *forced);
    
    void fillRows(Board *board);
    void fillCols(Board *board);
    void fillSqrs(Board *board);
    
    void solve(Board *board);
    
    // Auxiliary functions
    void alloc2Dints(const int a, int **arr);
    void alloc2Duchars(const int a, unsigned char **arr);
    void findPossibilities(Board board, int **c, unsigned char **pos, int (*f)(int,int));
    void fillUnique(Board *board, const int a, int **c, unsigned char **pos);


#ifdef __cplusplus
}
#endif

#endif /* SOLVER_H */

