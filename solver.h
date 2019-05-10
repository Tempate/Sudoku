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
    static inline unsigned char GET_X(unsigned short c) { return (c) & 0xf; }
    static inline unsigned char GET_Y(unsigned short c) { return ((c) & 0xf0) >> 4; }
    static inline unsigned char GET_CRD(int i, int j) { return (i << 4) + j; }
    static inline bool VAL_IN_BYTE(unsigned short byte, int k) { return (byte & (1<<k)) != 0; } 
    
    
    void solve(Board *board);
    
    bool dfs(Board *board, unsigned char index);
    char setAllForced(Board *board, unsigned char min);
    
    void calculatePossible(Board *board);
    void updateTileAdded(Board *board, char y, char x);
    
    unsigned char getBlankSquares(Board board);
    unsigned char getNextPossibleValue(Board board, const char val, const char x, const char y);
    
    unsigned char log_2(unsigned short index);
    
#ifdef __cplusplus
}
#endif

#endif /* SOLVER_H */

