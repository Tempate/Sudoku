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
#define SIZE (1<<RANGE)-1

#ifdef __cplusplus
extern "C" {
#endif
    
    static inline int POS(int x) { return 1 << (x-1); }
    
    typedef struct {
        unsigned char values[HEIGHT][WIDTH];
        unsigned short possible[HEIGHT][WIDTH];
    } Board;
    
    Board blankBoard(void);
    Board filledBoard(int values[HEIGHT][WIDTH]);
    Board randomBoard(void);
    Board duplicateBoard(Board board);
    
    void printBoard(Board board);
    void printPossible(Board board);


#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */

