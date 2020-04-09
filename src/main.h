#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <array>

static inline int pow2(const int n) {
    return 1 << (n - 1);
}

static inline int log2plus1(int index) {
    int val = 1;
    
    while (index >>= 1)
        ++val;
    
    return val;
}

#endif /* MAIN_H */
