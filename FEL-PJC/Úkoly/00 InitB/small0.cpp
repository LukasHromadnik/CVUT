#include <iostream>
#include "small0.hpp"

using namespace std;

int factorial(int n) {
    int result = 1;
    for (int i = n; i > 0; i--) {
        result *= i;
    }
    return result;
}
