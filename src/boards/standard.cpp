#include "../main.h"
#include "../board.h"
#include "standard.h"


StandardBoard::StandardBoard(const std::string &fen) {
    int i = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++, i++) {
            int value;
            
            if (fen[i] == '.')
                value = 0;
            else if (fen[i] >= '0' && fen[i] <= '9')
                value = fen[i] - '0';
            else if (fen[i] >= 'A' && fen[i] <= 'Z')
                value = fen[i] - 'A' + 10;
            else if (fen[i] >= 'a' && fen[i] <= 'z')
                value = fen[i] - 'a' + 10;
            
            setValue(Token{x, y}, value);
        }
    }
}

std::string StandardBoard::toString() const {
    std::string fen;
    
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            const int value = getValue(Token{x, y});

            if (value == 0)
                fen += ".";
            else
                fen += std::to_string(value);
        }
    }

    return fen;
}

std::ostream& operator<<(std::ostream& os, const StandardBoard &board) {
    os << "\n";

    for (const auto &row : board.values) {
        for (const int value : row) {
            assert(value >= 0 && value <= RANGE);
            if (value == 0) {
                const char size = RANGE / 10 + 1;
                const std::string blanks(size, ' ');
                os << blanks << ".";
            } else {
                const char size = 1 + RANGE / 10 - value / 10;
                const std::string blanks(size, ' ');
                os << blanks << value;
            }
        }

        os << "\n";
    }

    os << "\n";
    return os;
}

bool StandardBoard::solved() const {
    return complete() && checkRow() && checkCol() && checkReg();
}

inline int StandardBoard::getPossible(const Token &token) const {
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

            if (getValue(token) == 0)
                continue;

            const int bin = toBinary(getValue(token));

            colsPossible[token.x] ^= bin;
            rowsPossible[token.y] ^= bin;
            regsPossible[token.z] ^= bin;
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
        const Token &token = blanks[i];
        assert(getValue(token) == 0);

        const int possible = getPossible(token);

        if (!possible)
            return DEAD_END;


        if (setForcedToken(token, possible) == MODIFIED) {
            blanks.erase(blanks.begin() + i);
            state = MODIFIED;
            continue;
        }

        switch (setForcedInCol(token, possible)) {
            case MODIFIED:
                blanks.erase(blanks.begin() + i);
                state = MODIFIED;
                continue;
            case DEAD_END:
                return DEAD_END;
        }

        switch (setForcedInRow(token, possible)) {
            case MODIFIED:
                blanks.erase(blanks.begin() + i);
                state = MODIFIED;
                continue;
            case DEAD_END:
                return DEAD_END;
        }

        switch (setForcedInReg(token, possible)) {
            case MODIFIED:
                blanks.erase(blanks.begin() + i);
                state = MODIFIED;
                continue;
            case DEAD_END:
                return DEAD_END;
        }
    }

    return state;
}

