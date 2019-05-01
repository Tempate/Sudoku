/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   grader.h
 * Author: alpha
 *
 * Created on May 1, 2019, 12:11 AM
 */

#ifndef GRADER_H
#define GRADER_H

#ifdef __cplusplus
extern "C" {
#endif

    double grade(Board board);
    int numberOfGivens(Board board);
    int lowestBoundRowsCols(Board board);

#ifdef __cplusplus
}
#endif

#endif /* GRADER_H */

