#include <iostream>

#include "SkipList.h"
#include <vector>

using namespace std;

SkipList<int> random_list(int min, int max, int n) {
    SkipList<int> sl;
    for (int i=0; i<n; i++) {
        std::random_device rnd;
        std::mt19937 genr(rnd());
        std::uniform_int_distribution<> dist(min,max);
        int const result = dist(genr);
        sl.insert(result);
    }
    return sl;
}

SkipList<int> sequential_list(int start, int n) {
    SkipList<int> sl;
    for (int i=start; i<start+n; i++) {
        sl.insert(i);
    }
    return sl;
}

int main() {
    SkipList<int> my_sl;
    for (int i=0; i<8; i++) {
        my_sl.insert(i);
    }
    my_sl.print();

    return 0;
}