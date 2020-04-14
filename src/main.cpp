#include <iostream>
#include <chrono>
#include <ctime>

#include "main.h"
#include "board.h"
#include "generator.h"

int main() {
    srand(time(NULL));

    Board board = generateSudoku<Board>();
    std::cout << board;

    return EXIT_SUCCESS;
}