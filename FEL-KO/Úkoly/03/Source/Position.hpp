using namespace std;

#pragma once
struct Position {
    int x;
    int y;

    Position(int x, int y);

    double distanceFromPosition(Position position);
};

ostream & operator << (ostream &, const Position &);
