/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   checker.h
 * Author: alpha
 *
 * Created on April 27, 2019, 8:26 PM
 */

#ifndef CHECKER_H
#define CHECKER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool checkBoard(Board board);
    bool completeBoard(Board board);
    bool finishedBoard(Board board);

#ifdef __cplusplus
}
#endif

#endif /* CHECKER_H */

