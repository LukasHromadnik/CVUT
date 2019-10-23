#include <cmath>

#include "Position.hpp"

using namespace std;

Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}

double Position::distanceFromPosition(Position position) {
    int x1 = this->x;
    int y1 = this->y;
    int x2 = position.x;
    int y2 = position.y;

    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

ostream & operator << (ostream & os, const Position & position) {
    os << "(" << position.x << " " << position.y << ")";
    return os;
}
