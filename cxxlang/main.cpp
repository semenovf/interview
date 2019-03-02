#include <iostream>

class A 
{
public:
    A () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    ~A () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

class B : public A 
{
public:
    B () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    virtual ~B () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

class C : public B 
{
public:
    C () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    ~C () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

class D : public C 
{
public:    
    D () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
    virtual ~D () { std::cout << __PRETTY_FUNCTION__ << std::endl; }
};

int main ()
{
//    A a;
//    B b;
//     C c;
//     D d;
    
//     A * b1 = new B;
//     delete b1;

//     A * c1 = new C;
//     delete c1;

    A * d1 = new D;
    delete d1;

//     B * b2 = new B;
//     delete b2;

//     B * c2 = new C;
//     delete c2;
    
//     C * c3 = new C;
//     delete c3;

//     B * d2 = new D;
//     delete d2;

//     C * d3 = new D;
//     delete d3;

//     D * d4 = new D;
//     delete d4;
    
    return 0;
}
