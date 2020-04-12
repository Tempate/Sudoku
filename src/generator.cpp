#include <vector>

#include "main.h"
#include "board.h"
#include "solver.h"
#include "generator.h"

// Generates a board with a unique solution
Board generateBoard() {
    Board board;

    std::vector<Token> blanks = board.getTokens(BLANK);
    board.calculatePossible();

    // Set a few random values
    while (blanks.size() > 64) {
        Token token = randomPop<Token>(blanks);
        board.fill(token, token.randomValue(board));

        int state;

        do {
            state = setForced(board, blanks);
        } while (state == MODIFIED);
    }

    // Solve the board
    if (solve(board, 1) == 0) {
        std::cout << "[-] The generated Sudoku isn't solvable" << std::endl;
        assert(false);
    }

    std::vector<Token> squares = board.getTokens(NOT_BLANK);

    // Removes values until you get multiple solutions
    while (squares.size() > 0) {
        assert(squares.size() > 0);

        Token token = randomPop<Token>(squares);
        Board newBoard = board;
        newBoard.fill(token, 0);
        newBoard.calculatePossible();

        if (solve(newBoard, 2) == 1)
            board = newBoard;
    }

    return board;
}
