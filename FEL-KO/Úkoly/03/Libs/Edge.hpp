#pragma once
struct Edge {
    int lowerBound;
    int upperBound;
    int value;
    int start;
    int end;
    int cost;

    Edge(int start, int end);

    void print() const;
};