#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <array>
#include <cassert>

#define UNLIMITED 0

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

template <typename T>
T randomPop(std::vector<T> &list) {
    assert(list.size() > 0);

    const int i = rand() % list.size();
    
    T value = list[i];
    list.erase(list.begin() + i);

    return value;
}

template <typename T>
T popLast(std::vector<T> &list) {
    assert(list.size() > 0);
    
    T value = list[list.size() - 1];
    list.pop_back();

    return value;
}

#endif /* MAIN_H */
