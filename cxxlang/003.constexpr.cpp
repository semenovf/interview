#include "catch.hpp"

TEST_CASE("Constexpr", "constexpr") {
    int a[std::numeric_limits<unsigned char>::max()];
    
    CHECK(sizeof(a) == std::numeric_limits<unsigned char>::max() * sizeof(int));
}
