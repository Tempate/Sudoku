#include <iostream>
#include <chrono>

#include "tests.h"
// #include "benchmark.h"

using namespace std::chrono;

int main() {
    test_solver();
    // benchmark_solver();
    
    return EXIT_SUCCESS;
}