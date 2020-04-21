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

std::string genFilename() {
    const std::string size = std::to_string(RANGE);
    return "../tests/sudokus/standard/" + size + "x" + size + ".txt";
}

std::vector<StandardBoard> parseFile(const std::string filename) {
    std::ifstream infile(filename);
    std::vector<StandardBoard> boards;

    std::string line;

    while (std::getline(infile, line))
        boards.push_back(StandardBoard{line});

    return boards;
}

void test_solver() {
    auto boards = parseFile(genFilename());

    for (auto &board : boards) {
        solve<StandardBoard>(board, 1);
        assert(board.solved());
    }

    std::cout << "[+] Successfully solved " << boards.size() << " sudokus" << std::endl;
}
