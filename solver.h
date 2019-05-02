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

#include <stdbool.h>
#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

    
    static inline int GET_SQR(int i, int j) { return SQR * (int)(i/SQR) + (int)(j/SQR); }
    static inline char GET_VAL(unsigned short c) { return (c-1) >> 8; }
    static inline char GET_X(unsigned short c) { return (c-1) % 16; }
    static inline char GET_Y(unsigned short c) { return ((c-1) & 240) >> 4; }
    static inline unsigned short SET_VAL(int i, int j, int k) { return (k << 8) + (i << 4) + j + 1; }
    static inline bool VAL_IN_BYTE(unsigned short byte, int k) { return (byte & (1<<k)) != 0; } 
    
    void calculatePossible(Board *board);
    void updateTileAdded(Board *board, const int y, const int x);
    
    void depthFS(Board *board);
    void addNextPossibleValue(Board *board, char *index, unsigned short *c, const char start, const char i, const char j);
    
#ifdef __cplusplus
}
#endif

#endif /* SOLVER_H */

