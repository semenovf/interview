#include "catch.hpp"

enum class Char : char { a = 'a', b = 'b', c = 'c' };
enum class Decimal /*: int*/ { zero, one, two, three };

TEST_CASE("Enum class", "enum_class") {
    CHECK(static_cast<char>(Char::a) == 'a');
    CHECK(static_cast<char>(Char::b) == 'b');
    CHECK(static_cast<char>(Char::c) == 'c');
    
    CHECK(std::underlying_type<Decimal>::type(Decimal::zero) 
            + std::underlying_type<Decimal>::type(Decimal::one)
            == std::underlying_type<Decimal>::type(Decimal::one));

    CHECK(std::underlying_type<Decimal>::type(Decimal::one) 
            + std::underlying_type<Decimal>::type(Decimal::two)
            == std::underlying_type<Decimal>::type(Decimal::three));
    
}

