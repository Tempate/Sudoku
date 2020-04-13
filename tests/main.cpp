#include <iostream>
#include <chrono>

#include "main.h"

int main() {
    using namespace std::chrono;
    const auto start = high_resolution_clock::now();

    test_solver();
    
    const auto current = high_resolution_clock::now();
    const double elapsed = duration_cast<milliseconds>(current - start).count();
    
    std::cout << "Time: " << (double) elapsed << " ms" << std::endl;

    return EXIT_SUCCESS;
}