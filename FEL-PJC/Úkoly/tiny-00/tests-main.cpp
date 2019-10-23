#define CATCH_CONFIG_MAIN
#include "catch.hpp"

write_stats({-std::numeric_limits<double>::max(),
                     -std::numeric_limits<double>::max() },
                    sstr);