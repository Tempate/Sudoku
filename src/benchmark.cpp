#include <iostream>
#include <chrono>

#include "tests.h"

using namespace std::chrono;

void benchmark_solver() {
    const auto start = high_resolution_clock::now();

    test_solver();
    
    const auto current = high_resolution_clock::now();
    const double elapsed = duration_cast<milliseconds>(current - start).count();
    
    std::cout << "Time: " << elapsed << std::endl;
}