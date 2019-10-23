#include <climits>
#include <iostream>

#include "Edge.hpp"

using namespace std;

Edge::Edge(int start, int end) {
    this->start = start;
    this->end = end;
    this->lowerBound = 0;
    this->upperBound = INT_MAX;
    this->value = 0;
    this->cost = 0;
}

void Edge::print() const {
    cout << "[EDGE] (" << start << " " << end << ") C: " << cost << " V: " << value << endl;
}
