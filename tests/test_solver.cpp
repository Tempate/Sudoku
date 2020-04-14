#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

#include "../src/boards/standard.h"
#include "../src/solver.h"
#include "main.h"

std::vector<StandardBoard> parse_file(const std::string filename) {
    std::ifstream infile(filename);
    std::vector<StandardBoard> boards;

    std::string line;

    while (std::getline(infile, line)) {
        StandardBoard board;
        int i = 0;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++, i++)
                board.setValue(Token{x, y}, (line[i] == '.') ? 0 : line[i] - '0');
        }

        boards.push_back(board);
    }

    return boards;
}

void test_solver() {
    const auto boards = parse_file("../tests/sudokus.txt");

    for (auto board : boards) {
        solve<StandardBoard>(board, 1);
        assert(board.solved());
    }

    std::cout << "[+] Successfully solved " << boards.size() << " sudokus" << std::endl;
}
