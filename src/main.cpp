#include <iostream>
#include <chrono>
#include <ctime>

#include "main.h"
#include "board.h"
#include "tests.h"
#include "benchmark.h"
#include "generator.h"

int main() {
    srand(time(NULL));

    Board board = generateBoard();
    board.print();

    benchmark_solver();
    return EXIT_SUCCESS;
}