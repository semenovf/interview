#include "catch.hpp"

template <typename T1, typename T2>
auto sum (T1 const & a, T2 const & b) -> decltype(a + b)
{
    return a + b;
}

TEST_CASE("Decltype", "decltype") {
    CHECK(sum(int{12}, int{30}) == 42);
    CHECK(sum(std::string{"4"}, std::string{"2"}) == "42");
    CHECK(sum(std::string{"4"}, '2') == "42");
}
