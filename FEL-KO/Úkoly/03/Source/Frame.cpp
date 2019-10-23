#include "Frame.hpp"

using namespace std;

void Frame::addPosition(int x, int y) {
    Position position = Position(x, y);
    positions.push_back(position);
}
