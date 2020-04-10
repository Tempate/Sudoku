#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <array>
#include <cassert>

static inline int toBinary(const int n) {
    return 1 << (n - 1);
}

static inline int fromBinary(int index) {
    assert(index > 0);
    int val = 1;
    
    while (index >>= 1)
        ++val;
    
    return val;
}

#endif /* MAIN_H */
