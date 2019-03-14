#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "catch.hpp"

static int BAD_PERSON_COUNTER = 4;
static int PERSON_COUNTER = 4;

class BadPerson
{
    std::string _name;
    std::shared_ptr<BadPerson> _mother;
    std::shared_ptr<BadPerson> _father;
    std::vector<std::shared_ptr<BadPerson>> _kids;
    
public:
    BadPerson (std::string const & name
            , std::shared_ptr<BadPerson> mother = nullptr
            , std::shared_ptr<BadPerson> father = nullptr) 
        : _name{name}
        , _mother{mother}
        , _father{father}
    {}
    
    ~BadPerson () 
    {
        --BAD_PERSON_COUNTER;
    }
    
    void addKid (std::shared_ptr<BadPerson> kid)
    {
        _kids.push_back(kid);
    }
};

class Person
{
    std::string _name;
    std::shared_ptr<Person> _mother;
    std::shared_ptr<Person> _father;
    std::vector<std::weak_ptr<Person>> _kids;
    
public:
    Person (std::string const & name
            , std::shared_ptr<Person> mother = nullptr
            , std::shared_ptr<Person> father = nullptr) 
        : _name{name}
        , _mother{mother}
        , _father{father}
    {}
    
    ~Person () 
    {
        --PERSON_COUNTER;
    }
    
    void addKid (std::shared_ptr<Person> kid)
    {
        _kids.push_back(kid);
    }
};

void bad_person_process ()
{
    std::shared_ptr<BadPerson> p1 = std::make_shared<BadPerson>("Person1");
    std::shared_ptr<BadPerson> p2 = std::make_shared<BadPerson>("Person2");
    std::shared_ptr<BadPerson> p3 = std::make_shared<BadPerson>("Person3", p1, p2);
    std::shared_ptr<BadPerson> p4 = std::make_shared<BadPerson>("Person4", p1, p2);
    
    p1->addKid(p3);
    p1->addKid(p4);
    p2->addKid(p3);
    p2->addKid(p4);
}

void good_person_process ()
{
    std::shared_ptr<Person> p1 = std::make_shared<Person>("Person1");
    std::shared_ptr<Person> p2 = std::make_shared<Person>("Person2");
    std::shared_ptr<Person> p3 = std::make_shared<Person>("Person3", p1, p2);
    std::shared_ptr<Person> p4 = std::make_shared<Person>("Person4", p1, p2);
    
    p1->addKid(p3);
    p1->addKid(p4);
    p2->addKid(p3);
    p2->addKid(p4);
}

SCENARIO("Using smart pointers in complex object with cyclic refeferences", "[weak_ptr]") {
    GIVEN("Bad Family") {
        WHEN("Kids stored in vector as shared pointers") {
            bad_person_process();
            THEN("Desrtuctor will never called for any person") {
                REQUIRE(BAD_PERSON_COUNTER == 4);
            }
        }
    }
    
    GIVEN("Good Family") {
        WHEN("Kids stored in vector as weak pointers") {
            good_person_process();
            
            THEN("Desrtuctor will called successfully for all persons") {
                REQUIRE(PERSON_COUNTER == 0);
            }
        }
    }
}


