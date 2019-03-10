#include <string>
#include <functional>
#include "catch.hpp"

TEST_CASE("Basic lambda", "lambda") {
    CHECK([]{ return 42; }() == 42);
    
    auto l1 = [] { return 42; };
    CHECK(l1() == 42);
    
    auto l2 = [] () -> std::string { return std::to_string(42); };
    CHECK(l2() == std::string{"42"});
    
    auto l3 = [] (std::string const & s) -> std::string { return s; };
    CHECK(l3("42") == std::string{"42"});
}

TEST_CASE("Lambda with captures", "lambda") {
    int a = 12;
    int b = 30;
    auto l1 = [=] { return a + b; };
    auto l2 = [&] { a = 30; b = 12; return a + b; };
    auto l3 = [&a, b] { a = 13; return a + b; }; // b is 30 here
    
    CHECK(l1() == 42);
    CHECK(l2() == 42);
    CHECK(a == 30);
    CHECK(b == 12);
    
    CHECK(l3() == 43);
    CHECK(a == 13);
    
    CHECK(l1() == 42);
}

TEST_CASE("Mutable lambda with captures", "lambda") {
    int id = 0;

    // Compiler error: increment of read-only variable ‘id’
    // without `mutable` keyword
    //auto f = [id] () { return ++id; };
    
    auto f = [id] () mutable { return ++id; };
    
    id = 42;
    
    CHECK(f() == 1);
    CHECK(f() == 2);
    CHECK(f() == 3);
    
    CHECK(id == 42);
}

std::function<int (int)> get_lambda ()
{
    int a = 12;
    return [=] (int b) { return a + b; };
}

TEST_CASE("Lambda is a functional object (functor)", "lambda") {
    CHECK(get_lambda()(30) == 42);
}
