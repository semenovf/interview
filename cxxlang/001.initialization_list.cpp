#include <vector>
#include "catch.hpp"

TEST_CASE("Initialization list: static array", "init_list") {
    int values[] {1, 2, 3};
    
    CHECK(values[0] == 1);
    CHECK(values[1] == 2);
    CHECK(values[2] == 3);
}

TEST_CASE("Initialization list: vector", "init_list") {
    std::vector<int> values {1, 2, 3};
    
    CHECK(values[0] == 1);
    CHECK(values[1] == 2);
    CHECK(values[2] == 3);
}
