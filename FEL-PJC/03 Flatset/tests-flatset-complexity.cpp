#include "flatset.hpp"
#include "tests-helpers.hpp"
#include "catch.hpp"

#include <algorithm>
#include <vector>
#include <iterator>
#include <random>
#include <chrono>
#include <climits>
#include <type_traits>
#include <cmath>
#include <iostream>
#include <functional>
#include <numeric>

using Clock = cpu_clock;

namespace {
    // Fixed seed for reproduceability
    std::mt19937 mt{ 123456 };
    std::uniform_int_distribution<> dist(-1'000'000, 1'000'000);

    std::vector<int> generate(size_t N) {
        std::vector<int> res;
        res.reserve(N);

        for (size_t i=0; i<N; ++i) {
            res.push_back(dist(mt));
        }
        return res;
    }

    enum class TestState {
        NotRun,
        Passed,
        Failed
    };

    std::ostream& operator<<(std::ostream &os, TestState ts) {
        const char *str;
        switch (ts) {
            case TestState::NotRun: str = "TestState::NotRun"; break;
            case TestState::Passed: str = "TestState::Passed"; break;
            case TestState::Failed: str = "TestState::Failed"; break;
            default: str = "?";
        }
        return os << str;
    }

    TestState g_range_constructor_complexity_test_state = TestState::NotRun;
}


// This must be first, as the following tests query its state.
TEST_CASE("Range constructor complexity", "[.long][range-construct]") {
    // If the test fails (REQUIRE throws an exception), set the state to failed.
    scope_guard guard([](){
        g_range_constructor_complexity_test_state = TestState::Failed;
    });

    // We check the number of allocations in a different test
    // So we only deal with time-complexity
    // Good implementation should just copy the elements over, sort them and
    // remove duplicates, leaving it with complexity of O(N log N).
    using namespace std::chrono_literals;
    double last_time = 0;
    for (int i = 60'000; i <= 16'000'000; i *= 4) {
        auto data = generate(i);
        auto start_time = Clock::now();
        flat_set<int> fs(begin(data), end(data));
        auto end_time = Clock::now();
        auto time_diff = end_time - start_time;
        //auto total_time = static_cast<double>(time_diff.count());
        auto time_per_item = time_diff.count() / static_cast<double>(i);
        REQUIRE(set_equal(fs, data));
        if (last_time != 0) {
            // The coefficient is a rough approximation.
            // A linear time coefficient should blow out, constant time
            // jitter should remain inside some reasonable coefficient
            REQUIRE(time_per_item / last_time <= 1.55);
        }
        std::cout << "range constructor: i = " << i << " time per item = " << time_per_item << '\n';
        last_time = time_per_item;
    }
    // Set the state to passed.
    guard.dismiss();
    g_range_constructor_complexity_test_state = TestState::Passed;
}

TEST_CASE("member swap - complexity", "[.long][swap]") {
    // If the range constructor was too slow, the test would take ages.
    REQUIRE(g_range_constructor_complexity_test_state == TestState::Passed);

    // Swap should take constant time regardless of flat_set size.
    using namespace std::chrono_literals;
    double last_time = 0;
    for (int i = 1'000; i <= 1'500'000; i *= 4) {
        auto lhs_elems = generate(i);
        flat_set<int> lhs{ lhs_elems.begin(), lhs_elems.end() };
        auto rhs_elems = generate(i);
        flat_set<int> rhs{ rhs_elems.begin(), rhs_elems.end() };
        auto start_time = Clock::now();
        lhs.swap(rhs);
        auto end_time = Clock::now();
        // Add 0.5 ms to even out variability
        auto time_diff = end_time - start_time + 500us;
        auto total_time = static_cast<double>(time_diff.count());
        lhs.swap(rhs);
        REQUIRE(set_equal(lhs, lhs_elems));
        if (last_time != 0) {
            // The coefficient is a rough approximation.
            // A linear time coefficient should blow out, constant time
            // jitter should remain inside some reasonable coefficient
            REQUIRE(total_time / last_time <= 1.25);
        }
        last_time = total_time;
        std::cout << "member swap: i = " << i << " total time = " << (time_diff - 500us).count() << '\n';
    }
}

TEST_CASE("swap overload - complexity", "[.long][swap]") {
    // If the range constructor was too slow, the test would take ages.
    REQUIRE(g_range_constructor_complexity_test_state == TestState::Passed);

    // Swap should take constant time regardless of flat_set size.
    using namespace std::chrono_literals;
    double last_time = 0;
    for (int i = 1'000; i <= 1'500'000; i *= 4) {
        auto lhs_elems = generate(i);
        auto lhs = make_flat_set<int>(lhs_elems);
        auto rhs_elems = generate(i);
        auto rhs = make_flat_set<int>(rhs_elems);
        auto start_time = Clock::now();
        swap(lhs, rhs);
        auto end_time = Clock::now();
        // Add 0.5 ms to even out variability
        auto time_diff = end_time - start_time + 500us;
        auto total_time = static_cast<double>(time_diff.count());
        swap(lhs, rhs);
        REQUIRE(set_equal(lhs, lhs_elems));
        if (last_time != 0) {
            // The coefficient is a rough approximation.
            // A linear time coefficient should blow out, constant time
            // jitter should remain inside some reasonable coefficient
            REQUIRE(total_time / last_time <= 1.25);
        }
        last_time = total_time;
        std::cout << "swap overload: i = " << i << " total time = " << (time_diff - 500us).count() << '\n';
    }
}

TEST_CASE("find complexity", "[.long][find]") {
    // If the range constructor was too slow, the test would take ages.
    REQUIRE(g_range_constructor_complexity_test_state == TestState::Passed);

    double last_time = 0;
    const int searches = 1'000;
    for (int i = 1'000'000; i <= 16'000'000; i *= 2) {
        auto data = generate(i);
        flat_set<int> fs(begin(data), end(data));

        auto start_time = Clock::now();
        for (int j = 0; j < searches; ++j) {
            const auto factor = i / searches;
            REQUIRE(fs.find(data[j * factor]) != fs.end());
        }
        auto end_time = Clock::now();
        // Static factor to avoid jitter
        auto total_time = static_cast<double>((end_time - start_time).count());
        if (last_time != 0) {
            // We are doubling the size, thus linear find would double
            // up tim as well. 1.5 seems like a good upper bound on
            // O(log n) growth
            REQUIRE(total_time / last_time <= 1.5);
        }
        std::cout << "find: i = " << i << " total time for " << searches << " items = " << total_time << '\n';
        last_time = total_time;
    }
}

TEST_CASE("lower_bound complexity", "[.long][lower-bound]") {
    // If the range constructor was too slow, the test would take ages.
    REQUIRE(g_range_constructor_complexity_test_state == TestState::Passed);

    double last_time = 0;
    const int searches = 1'000;
    for (int i = 1'000'000; i <= 16'000'000; i *= 2) {
        auto data = generate(i);
        flat_set<int> fs(begin(data), end(data));

        auto start_time = Clock::now();
        for (int j = 0; j < searches; ++j) {
            const auto factor = i / searches;
            REQUIRE(*fs.lower_bound(data[j * factor]) == data[j * factor]);
        }
        auto end_time = Clock::now();
        // Static factor to avoid jitter
        auto total_time = static_cast<double>((end_time - start_time).count());
        if (last_time != 0) {
            // We are doubling the size, thus linear find would double
            // up tim as well. 1.5 seems like a good upper bound on
            // O(log n) growth
            REQUIRE(total_time / last_time <= 1.5);
        }
        std::cout << "find: i = " << i << " total time for " << searches << " items = " << total_time << '\n';
        last_time = total_time;
    }
}

TEST_CASE("upper_bound complexity", "[.long][upper-bound]") {
    // If the range constructor was too slow, the test would take ages.
    REQUIRE(g_range_constructor_complexity_test_state == TestState::Passed);

    double last_time = 0;
    const int searches = 1'000;
    for (int i = 1'000'000; i <= 16'000'000; i *= 2) {
        auto data = generate(i);
        flat_set<int> fs(begin(data), end(data));

        auto start_time = Clock::now();
        for (int j = 0; j < searches; ++j) {
            const auto factor = i / searches;
            auto it = fs.upper_bound(data[j * factor]);
            REQUIRE(*it > data[j * factor]);
            REQUIRE(*(it - 1) == data[j * factor]);
        }
        auto end_time = Clock::now();
        // Static factor to avoid jitter
        auto total_time = static_cast<double>((end_time - start_time).count());
        if (last_time != 0) {
            // We are doubling the size, thus linear find would double
            // up tim as well. 1.5 seems like a good upper bound on
            // O(log n) growth
            REQUIRE(total_time / last_time <= 1.5);
        }
        std::cout << "find: i = " << i << " total time for " << searches << " items = " << total_time << '\n';
        last_time = total_time;
    }
}
