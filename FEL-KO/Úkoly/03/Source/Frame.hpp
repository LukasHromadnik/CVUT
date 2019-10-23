#include <vector>
#include "Position.hpp"

using namespace std;

#pragma once
struct Frame {
    vector<Position> positions;

    void addPosition(int x, int y);
};
