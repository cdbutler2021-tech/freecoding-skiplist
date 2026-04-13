#include <iostream>
#include "SkipList.h"

using namespace std;

int main() {
    SkipList<int> my_sl;
    for (int i=0; i<8; i++) {
        my_sl.insert(i);
    }
    return 0;
}