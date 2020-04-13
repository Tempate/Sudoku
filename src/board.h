#ifndef BOARD_H
#define BOARD_H

#define RANGE 9
#define HEIGHT RANGE
#define WIDTH RANGE

#define REGION 3
#define REGIONS WIDTH * HEIGHT / (REGION * REGION)

#define MAX (1 << RANGE) - 1

enum {BLANK, NOT_BLANK};
enum {UNMODIFIED, MODIFIED, DEAD_END};

#include <vector>
#include <array>

static inline int calcRegion(const int x, const int y) {
    return REGION * (y / REGION) + x / REGION;
}

struct Token {
    int x;
    int y;
    int z;

    Token(const int x, const int y): x{x}, y{y} {
        z = calcRegion(x, y);
    }
};

class Board {
    std::array<std::array<int, WIDTH>, HEIGHT> values;

    std::array<int, HEIGHT>  colsPossible = {0};
    std::array<int, WIDTH>   rowsPossible = {0};
    std::array<int, REGIONS> regsPossible = {0};

    public:
    Board();

    Board(const std::array<std::array<int, WIDTH>, HEIGHT> values):
    values{values} {};

    void print() const;
    bool complete() const;
    bool check() const;

    int getValue(const Token &token) const;
    void setValue(const Token &token, const int value);
    std::vector<Token> getTokens(const int type) const;
    
    void calculatePossible();
    int getPossible(const Token &token) const;
    void updatePossible(const Token &token);
    void setRandomValue(const Token &token);
    int nextPossibleValue(const Token &token, const int value) const;

    int setForced(std::vector<Token> &blanks);
    int setForcedToken(const Token &token, const int possible);

    int setForcedInRow(const Token &token);
    int setForcedInCol(const Token &token);
    int setForcedInReg(const Token &token);
};

#endif /* BOARD_H */
