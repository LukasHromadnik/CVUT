#include <iostream>
#include "list.cpp"
#include "test-helpers.cpp"

using std::cout;
using std::endl;

using pjc::list;

void CHECK(bool expr) {
    cout << "CHECK " << expr << " " << ((expr) ? "+" : "-") << endl;
}

void REQUIRE(bool expr) {
    cout << "REQUIRE " << expr << " " << ((expr) ? "+" : "-") << endl;
}

void REQUIRE_FALSE(bool expr) {
    cout << "REQUIRE_FALSE " << expr << " " << ((!expr) ? "+" : "-") << endl;
}

void print_list(list l) {
    for (auto it = l.begin(); it != l.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}

void print_split(std::pair<list, list> split) {
    print_list(split.first);
    cout << "---" << endl;
    print_list(split.second);
}

template<typename T>
void print_vector(std::vector<T> values) {
    for (T value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    auto elements = std::vector<double>{ 0, 2, 2, 3, 2, 2, 3, 3, 2, 0, 1 };
    list l{ elements };
    // SECTION("Remove the first element in the list") {
    //     l.remove(0);
    //     elements.erase(std::remove(begin(elements), end(elements), 0),
    //                    end(elements));
    //     REQUIRE(list_equal(l, elements));
    // }
    // SECTION("Remove the last element in list") {
        l.remove(1);
        elements.erase(std::remove(begin(elements), end(elements), 1), end(elements));
    print_list(l);
    print_vector(elements);
        REQUIRE(list_equal(l, elements));
    // }
    // SECTION("Remove all elements from a list") {
    //     for (int i = 0; i < 4; ++i) {
    //         l.remove(i);
    //     }
    //     REQUIRE(l.empty());
    //     REQUIRE(l.size() == 0);
    // }

    return 0;
}
