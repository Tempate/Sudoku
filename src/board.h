#ifndef BOARD_H
#define BOARD_H

#define RANGE 9
#define HEIGHT RANGE
#define WIDTH RANGE

#define REGION 3
#define REGIONS RANGE

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
    protected:
    std::array<std::array<int, WIDTH>, HEIGHT> values;

    public:
    Board();
    Board(const std::array<std::array<int, WIDTH>, HEIGHT> values):
    values{values} {};

    virtual std::string toString() const = 0;
    
    virtual bool solved() const = 0;
    bool complete() const;
    bool checkRow() const;
    bool checkCol() const;
    bool checkReg() const;

    virtual inline int getPossible(const Token &token) const = 0;
    virtual void calculatePossible() = 0;
    virtual void updatePossible(const Token &token) = 0;

    int getValue(const Token &token) const;
    void setValue(const Token &token, const int value);
    std::vector<Token> getTokens(const int type) const;
    
    void setRandomValue(const Token &token);
    int nextPossibleValue(const Token &token, const int value) const;

    virtual int setForced(std::vector<Token> &blanks) = 0;

    int setForcedToken(const Token &token, const int possible);

    int setOnlyPossible(const Token &token, int possible);
    int setForcedInCol(const Token &token, int possible);
    int setForcedInRow(const Token &token, int possible);
    int setForcedInReg(const Token &token, int possible);
};

#endif /* BOARD_H */
