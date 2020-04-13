#include <cassert>
#include <vector>
#include <limits>

#include "main.h"
#include "board.h"
#include "solver.h"

int solve(Board &board, const int flag) {
    const int n = (flag == UNLIMITED) ? std::numeric_limits<int>::max() : flag;

    std::vector<Token> blanks = board.getTokens(BLANK);
    board.calculatePossible();

    return dfs(board, blanks, n);
}

int dfs(Board &board, std::vector<Token> &blanks, const int n) {
    if (blanks.size() == 0) {
        assert(board.check());
        return 1;
    }
    
    Token token = popLast<Token>(blanks);

    assert(board.getValue(token) == 0);
    
    int v = board.nextPossibleValue(token, 0);
    int count = 0;
    
    while (v != NO_VALUES_LEFT) {
        Board newBoard = board;
        newBoard.setValue(token, v);

        std::vector<Token> newBlanks = blanks;

        assert(board.getValue(token) == 0);
        assert(newBoard.getValue(token) != 0);
        
        int state;

        do {
            state = setForced(newBoard, newBlanks);
        } while (state == MODIFIED);

        if (state != DEAD_END) {
            count += dfs(newBoard, newBlanks, n);

            if (count >= n) {
                if (n == 1)
                    board = newBoard;
                
                return count;
            }
        }

        v = board.nextPossibleValue(token, v);
    }

    return count;
}

int setForced(Board &board, std::vector<Token> &blanks) {
    int state = UNMODIFIED;
    
    for (int i = blanks.size() - 1; i >= 0; i--) {
        Token token = blanks[i];
        assert(board.getValue(token) == 0);

        const int possible = board.getPossible(token);

        if (!possible)
            return DEAD_END;
            
        if ((setForcedTile(board, token, possible) == MODIFIED) || 
            (setForcedInRow(board, token)    == MODIFIED) ||
            (setForcedInColumn(board, token) == MODIFIED) ||
            (setForcedInRegion(board, token) == MODIFIED)) {
            blanks.erase(blanks.begin() + i);
            state = MODIFIED;
        }
    }

    return state;
}

int setForcedTile(Board &board, const Token &token, const int possible) {
    if (__builtin_popcount(possible) != 1)    
        return UNMODIFIED;

    board.setValue(token, fromBinary(possible));

    return MODIFIED;
}

int setForcedInRow(Board &board, const Token &token) {
    int possible = board.getPossible(token);

    for (int x = 0; x < WIDTH; x++) {
        if (board.values[token.y][x] == 0 && x != token.x)
            possible &= MAX ^ board.getPossible(Token{x, token.y});
    }

    return setForcedTile(board, token, possible);
}

int setForcedInColumn(Board &board, const Token &token) {
    int possible = board.getPossible(token);

    for (int y = 0; y < HEIGHT; y++) {
        if (board.values[y][token.x] == 0 && y != token.y)
            possible &= MAX ^ board.getPossible(Token{token.x, y});
    }

    return setForcedTile(board, token, possible);
}

int setForcedInRegion(Board &board, const Token &token) {
    int possible = board.getPossible(token);

    const int y0 = 3 * (token.y / 3);
    const int x0 = 3 * (token.x / 3);

    for (int y = y0; y < y0 + 3; y++) {
        for (int x = x0; x < x0 + 3; x++) {
            if (board.values[y][x] == 0 && !(x == token.x && y == token.y))
                possible &= MAX ^ board.getPossible(Token{x, y});
        }
    }

    return setForcedTile(board, token, possible);
}