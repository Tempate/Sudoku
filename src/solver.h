#ifndef SOLVER_H
#define SOLVER_H

#include <vector>

#include "main.h"
#include "board.h"

template <typename Board>
int dfs(Board &board, std::vector<Token> &blanks, const int maxSolutions) {
    if (blanks.size() == 0) {
        assert(board.solved());
        return 1;
    }
    
    Token token = popLast<Token>(blanks);

    assert(board.getValue(token) == 0);
    
    int value = board.nextPossibleValue(token, 0);
    int count = 0;
    
    while (value != UNMODIFIED) {
        Board newBoard = board;
        newBoard.setValue(token, value);

        std::vector<Token> newBlanks = blanks;

        assert(board.getValue(token) == 0);
        assert(newBoard.getValue(token) != 0);
        
        int state;

        do {
            state = newBoard.setForced(newBlanks);
        } while (state == MODIFIED);

        if (state != DEAD_END) {
            count += dfs<Board>(newBoard, newBlanks, maxSolutions);

            if (count >= maxSolutions) {
                if (maxSolutions == 1) {
                    assert(newBoard.solved());
                    board = newBoard;
                }
                
                return count;
            }
        }

        value = board.nextPossibleValue(token, value);
    }

    return count;
}

template <typename Board>
int solve(Board &board, const int flag) {
    const int maxSolutions = (flag == UNLIMITED) ? std::numeric_limits<int>::max() : flag;

    std::vector<Token> blanks = board.getTokens(BLANK);
    board.calculatePossible();

    return dfs<Board>(board, blanks, maxSolutions);
}

#endif /* SOLVER_H */

