#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

#include "main.h"
#include "board.h"
#include "solver.h"

template <typename Board>
Board generateSudoku() {
    Board board;

    std::vector<Token> blanks = board.getTokens(BLANK);
    board.calculatePossible();

    // Set HEIGHT + WIDTH + 1 random values
    while (blanks.size() > (WIDTH - 1) * (HEIGHT - 1)) {
        Token token = randomPop<Token>(blanks);
        board.setRandomValue(token);

        int state;

        do {
            state = board.setForced(blanks);
        } while (state == MODIFIED);
    }

    // Solve the board
    const int solvable = solve<Board>(board, 1);

    if (!solvable) {
        std::cout << "[-] The generated Sudoku isn't solvable" << std::endl;
        assert(false);
    }

    // Remove all the forced values
    std::vector<Token> squares = board.getTokens(NOT_BLANK);

    while (squares.size() > 0) {
        Board newBoard = board;
        newBoard.setValue(randomPop<Token>(squares), 0);
        newBoard.calculatePossible();

        if (solve<Board>(newBoard, 2) == 1)
            board = newBoard;
    }

    return board;
}

#endif /* GENERATOR_H */
