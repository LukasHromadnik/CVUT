#include "catch.hpp"

#include "small0.hpp"

TEST_CASE("Factorials are computed -- inside bounds", "[factorial]") {
    REQUIRE(factorial(1) == 1);
    REQUIRE(factorial(2) == 2);
    REQUIRE(factorial(3) == 6);
    REQUIRE(factorial(10) == 3628800);
}

TEST_CASE("Factorials are computed -- boundary values", "[factorial]") {
    REQUIRE(factorial(0) == 1);
    REQUIRE(factorial(11) == 39916800);
}
