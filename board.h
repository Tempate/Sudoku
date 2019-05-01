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
#define WIDTH 9
#define RANGE 9
#define ALL (unsigned int)((1<<(RANGE))-1)  //J
#define SUM (RANGE*(RANGE+1))/2             //J
#define SQR 3

#ifdef __cplusplus
extern "C" {
#endif
    
    static inline int POS(int x) { return 1 << (x-1); }
    static inline int GETSQR(int i, int j) { return SQR * (int)(i/SQR) + (int)(j/SQR); }
    
    typedef struct {
        unsigned char values[HEIGHT][WIDTH];
        unsigned int possible[HEIGHT][WIDTH];
        //unsigned int remeaning[]; //J, nothing implemented thus far, use this to improve the speed of the "for(i) for(j) if([i][j] == 0)" loops
    } Board;
    
    Board blankBoard(void);
    Board filledBoard(int values[HEIGHT][WIDTH]);
    Board randomBoard(void);
    Board duplicateBoard(Board *b);
    
    void printBoard(Board board);
    void printPossible(Board board);


#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */

