/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   board.h
 * Author: alpha
 *
 * Created on April 27, 2019, 7:50 PM
 */

#ifndef BOARD_H
#define BOARD_H

#define HEIGHT 9
#define WIDTH HEIGHT
#define RANGE HEIGHT
#define SQR 3
#define SQRS WIDTH * HEIGHT / (SQR * SQR)
#define TOTAL HEIGHT*WIDTH
#define SIZE (1<<RANGE)-1

#ifdef __cplusplus
extern "C" {
#endif
   
    
    static inline int POS(int x) { return 1 << (x-1); }
    static inline int POW2(char pow) { return 1 << pow; }
    
    typedef struct {
        unsigned char values[HEIGHT][WIDTH];
        
        unsigned short rowsPos[HEIGHT];
        unsigned short colsPos[WIDTH];
        unsigned short sqrsPos[SQRS];
    } Board;
    
    
    Board getBoardWithValues(int values[HEIGHT][WIDTH]);
    void printBoard(Board board);


#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */

