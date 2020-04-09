#include <iostream>
#include <chrono>

#include "tests.h"

void benchmark_solver() {
    using namespace std::chrono;
    const auto start = high_resolution_clock::now();

    test_solver();
    
    const auto current = high_resolution_clock::now();
    const double elapsed = duration_cast<milliseconds>(current - start).count();
    
    std::cout << "Time: " << elapsed << std::endl;
}