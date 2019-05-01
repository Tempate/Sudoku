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

    static inline int GET_VAL(unsigned int c) { return (c-1) >> 8; }
    static inline int GET_X(unsigned int c) { return (c-1) & 15; } //J
    static inline int GET_Y(unsigned int c) { return ((c-1) & 240) >> 4; }
    static inline int SET_VAL(int i, int j, int k) { return (k << 8) + (i << 4) + j + 1; }
    
    void calculatePossible(Board *board);
    void depthFS(Board *board);
    
    void feedforward(Board *board, unsigned int *c, int *index);
    void backpropagation(Board *board, unsigned int *c, int *index);

#ifdef __cplusplus
}
#endif

#endif /* SOLVER_H */

