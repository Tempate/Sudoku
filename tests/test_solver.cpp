#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

#include "../src/board.h"
#include "../src/solver.h"

std::vector<Board> parse_file(const std::string filename) {
    std::ifstream infile(filename);
    std::vector<Board> boards;

    std::string line;

    while (std::getline(infile, line)) {
        Board board;
        int i = 0;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++, i++)
                board.values[y][x] = (line[i] == '.') ? 0 : line[i] - '0';
        }

        boards.push_back(board);
    }

    return boards;
}

void test_solver() {
    const auto boards = parse_file("tests/sudokus.txt");

    for (auto board : boards) {
        solve(board, 1);
        assert(board.check());
    }

    std::cout << "[+] Successfully solved " << boards.size() << " sudokus" << std::endl;
}
