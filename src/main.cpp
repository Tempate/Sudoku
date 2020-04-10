#include <iostream>
#include <chrono>

#include "main.h"
#include "board.h"
#include "tests.h"
#include "benchmark.h"

using namespace std::chrono;

int main() {
    benchmark_solver();
    return EXIT_SUCCESS;
}