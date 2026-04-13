#pragma once

#include <random>

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr(0,1);

inline bool coin() {

    int const result = distr(gen);
    if (result == 1) {
        return true;
    }
    return false;
}