#include <vector>
#include <string>
#include <iostream>
#include "catch.hpp"

std::string copy_constructor{"copy_constructor"};
std::string move_constructor{"move_constructor"};

class A
{
    std::string s;
public:
    A () {}
    A (A const &) { s = copy_constructor; }
    
    std::string const & str () const { return s; }
};

class B
{
protected:
    std::string s;
    
public:
    B () {}
    B (B const &) { s = copy_constructor; }
    B (B &&) { s = move_constructor; }
    
    std::string const & str () const { return s; }
};

class C : public B
{
public:
    C () {}
    C (C const &) = default;
    C (C &&) = default;
};

A get_A () { return A{}; }
B get_B () { return B{}; }
C get_C () { return C{}; }

TEST_CASE("Basic", "rvalue") {
    std::vector<int> values {1, 2, 3};
    std::vector<int> values_copy{values};
    
    CHECK(values.size() == 3);
    CHECK(values_copy.size() == 3);
    
    CHECK(values[0] == 1);
    CHECK(values[1] == 2);
    CHECK(values[2] == 3);
    CHECK(values_copy[0] == 1);
    CHECK(values_copy[1] == 2);
    CHECK(values_copy[2] == 3);
    
    std::vector<int> values_moved{std::move(values)};
    CHECK_FALSE(values.size() == 3);
    CHECK(values.size() == 0);
    CHECK(values_moved.size() == 3);
    CHECK(values_moved[0] == 1);
    CHECK(values_moved[1] == 2);
    CHECK(values_moved[2] == 3);
}

TEST_CASE("Custom", "rvalue") {
    A a;
    CHECK(A{a}.str() == copy_constructor);
    CHECK(A{std::move(a)}.str() == copy_constructor);

    B b;
    CHECK(B{b}.str() == copy_constructor);
    CHECK(B{std::move(b)}.str() == move_constructor);
    
    C c;
    CHECK(C{c}.str() == copy_constructor);
    CHECK(C{std::move(c)}.str() == move_constructor);
    
    CHECK(A{get_A()}.str() == std::string{}); // return value optimization (RVO)
    CHECK(B{get_B()}.str() == std::string{}); // return value optimization (RVO)
    CHECK(C{get_C()}.str() == std::string{}); // return value optimization (RVO)
}
