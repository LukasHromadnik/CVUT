#include <iostream>
#include "flatset.hpp"
#include "tests-helpers.cpp"

using namespace std;

void REQUIRE_FALSE(bool expr) {
    cout << "[REQUIRE_FALSE] " << (expr == false) << endl;
}

void REQUIRE(bool expr) {
    cout << "[REQUIRE] " << (expr == true) << endl;
}

int main() {
    using input_iter = fake_input_iterator<std::vector<tracker>::iterator, false>;
    const size_t num_elems = 1000;
    std::vector<tracker> elems; elems.reserve(num_elems);
    std::generate_n(std::back_inserter(elems), num_elems, [] () {
        static int i = 0;
        return i++;
    });
    auto oldt = tracker::cnt;
    flat_set<tracker> fs(input_iter(begin(elems)),
                            input_iter(end(elems)));
    auto newt = tracker::cnt;
    cout << oldt << endl << newt << endl;
    REQUIRE(oldt.copy_constructors < newt.copy_constructors - num_elems);
    REQUIRE(fs == make_flat_set(elems));
}
