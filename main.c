/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: alpha
 *
 * Created on April 27, 2019, 7:42 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "tests.h"
#include "solver.h"

int main(int argc, char** argv) {
    
    srand(time(NULL));
    
    testBFS5();
    
    return (EXIT_SUCCESS);
}

