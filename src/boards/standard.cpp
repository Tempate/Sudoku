#include "../main.h"
#include "../board.h"
#include "standard.h"


std::ostream& operator<<(std::ostream& os, const StandardBoard &board) {
    os << "\n";

    for (const auto row : board.values) {
        for (const int value : row)
            os << " " << ((value == 0) ? "." : std::to_string(value));

        os << "\n";
    }

    os << "\n";
    return os;
}

bool StandardBoard::solved() const {
    return complete() && checkRow() && checkCol() && checkReg();
}

int StandardBoard::getPossible(const Token &token) const {
    return colsPossible[token.x] & rowsPossible[token.y] & regsPossible[token.z];
}

void StandardBoard::calculatePossible() {
    for (int i = 0; i < RANGE; i++) {
        colsPossible[i] = MAX;
        rowsPossible[i] = MAX;
        regsPossible[i] = MAX;
    }

    // Saves values that aren't possible for each row, column and quadrant
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            const Token token{x, y};

            if (getValue(token) != 0) {
                const int bin = toBinary(getValue(token));

                colsPossible[token.x] ^= bin;
                rowsPossible[token.y] ^= bin;
                regsPossible[token.z] ^= bin;
            }
        }
    }
}

void StandardBoard::updatePossible(const Token &token) {
    const int possible = MAX ^ toBinary(getValue(token));

    colsPossible[token.x] &= possible;
    rowsPossible[token.y] &= possible;
    regsPossible[token.z] &= possible;
}

int StandardBoard::setForced(std::vector<Token> &blanks) {
    int state = UNMODIFIED;
    
    for (int i = blanks.size() - 1; i >= 0; i--) {
        Token token = blanks[i];
        assert(getValue(token) == 0);

        const int possible = getPossible(token);

        if (!possible)
            return DEAD_END;
            
        if ((setForcedToken(token, possible) == MODIFIED) || 
            (setForcedInRow(token) == MODIFIED) ||
            (setForcedInCol(token) == MODIFIED) ||
            (setForcedInReg(token) == MODIFIED)) {
            blanks.erase(blanks.begin() + i);
            state = MODIFIED;
        }
    }

    return state;
}

