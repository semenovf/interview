#include <string>
#include "catch.hpp"

namespace std {
    std::string to_string (std::string const & s) { return s; }
    
    // Attention!
    // Using std::string{1, c} refers to constructor with initialization_list
    std::string to_string (char c) { return std::string(1, c); }
}

std::string concat () { return std::string{}; }

template <typename T, typename... Types>
std::string concat (T const & value, Types const &... args)
{
    std::string r;
    r.append(std::to_string(value));
    r.append(concat(args...));
    return r;
}

TEST_CASE("Basic variadic templates", "variadic_template") {
    CHECK(concat() == std::string{});
    CHECK(concat(3) == std::string{"3"});
    CHECK(concat(3, ',') == std::string{"3,"});
    CHECK(concat(3, ',', 1) == std::string{"3,1"});
    CHECK(concat(3, ',', 1, 4) == std::string{"3,14"});
    CHECK(concat(3, ',', 1, 4, " is pi") == std::string{"3,14 is pi"});
}

