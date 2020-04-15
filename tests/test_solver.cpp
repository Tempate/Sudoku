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
    return "../tests/sudokus/" + size + "x" + size + ".txt";
}

std::vector<StandardBoard> parseFile(const std::string filename) {
    std::ifstream infile(filename);
    std::vector<StandardBoard> boards;

    std::string line;

    while (std::getline(infile, line)) {
        StandardBoard board;
        int i = 0;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++, i++) {
                int value = 0;
                
                if (line[i] >= '1' && line[i] <= '9')
                    value = line[i] - '0';
                else if (line[i] >= 'A' && line[i] <= 'Z')
                    value = line[i] - 'A' + 10;
                else if (line[i] >= 'a' && line[i] <= 'z')
                    value = line[i] - 'a' + 10;
                
                board.setValue(Token{x, y}, value);
            }
        }

        boards.push_back(board);
    }

    return boards;
}

void test_solver() {
    const auto boards = parseFile(genFilename());

    for (auto board : boards) {
        //std::cout << board;
        solve<StandardBoard>(board, 1);
        //std::cout << board;
        assert(board.solved());
    }

    std::cout << "[+] Successfully solved " << boards.size() << " sudokus" << std::endl;
}
