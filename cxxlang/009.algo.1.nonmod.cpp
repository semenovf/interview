#include "catch.hpp"

struct Item {
    int index;
    std::string name;
};

std::vector<Item> items = {
      { 0, "item0" }
    , { 1, "item1" }
    , { 2, "item2" }
    , { 3, "item3" }
    , { 4, "item4" }
    , { 5, "item5" }
};


TEST_CASE ("for_each", "algo") {
}

TEST_CASE ("find", "algo") {
}

TEST_CASE ("find_if", "algo") {
    CHECK(std::find_if(std::begin(items)
            , std::end(items)
            , [] (Item & item) {
                return item.index == 0;
            })->name == "item0");

    CHECK(std::find_if(std::begin(items)
            , std::end(items)
            , [] (Item & item) {
                return item.name == "item4";
            })->index == 4);

    CHECK(std::find_if(std::begin(items)
            , std::end(items)
            , [] (Item & item) {
                return item.name == "item not found";
            }) == std::end(items));
}

TEST_CASE ("find_if_not", "algo") {
}
