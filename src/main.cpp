#include <iostream>

#include "main.h"
#include "boards/standard.h"
#include "generator.h"

int main() {
    srand(time(NULL));

    StandardBoard board = generateSudoku<StandardBoard>();
    std::cout << board;

    return EXIT_SUCCESS;
}