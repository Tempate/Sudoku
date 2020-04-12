#ifndef GENERATOR_H
#define GENERATOR_H

#include "token.h"
#include "board.h"

Board generateBoard();

void fillRandomNumber(Board &board, std::vector<Token> &blanks);

int randomPossibleValue(const Board &board, Token &sqr);

#endif /* GENERATOR_H */
