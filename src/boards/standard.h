#include <iostream>
#include <string>
#include <array>

#include "../board.h"

class StandardBoard: public Board {
    std::array<int, HEIGHT>  colsPossible = {0};
    std::array<int, WIDTH>   rowsPossible = {0};
    std::array<int, REGIONS> regsPossible = {0};

    friend std::ostream& operator<<(std::ostream& os, const StandardBoard &board);

    public:
    using Board::Board;
    StandardBoard(const std::string &fen);

    std::string toString() const override;

    bool solved() const override;

    inline int getPossible(const Token &token) const override;
    void calculatePossible() override;
    void updatePossible(const Token &token) override;

    int setForced(std::vector<Token> &blanks) override;
};
