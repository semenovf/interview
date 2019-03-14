#include <array>
#include <utility>
#include <algorithm>
#include <functional>
#include "sort.bubble.hpp"
#include "sort.selection.hpp"
#include "sort.insertion.hpp"
#include "catch.hpp"

using array_t = std::array<int, 10>;
static array_t sample{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
static std::array<int, 0> empty_sample{};
static std::array<int, 1> single_sample{9};

TEST_CASE ("Bubble sort", "algo.sort") {
    array_t a{sample};

    bubble_sort(std::begin(a), std::end(a), std::less<int>{});

    for (int i = 0; i < a.size(); i++)
        CHECK(a[i] == i);

    bubble_sort(std::begin(empty_sample), std::end(empty_sample), std::less<int>{});
    bubble_sort(std::begin(single_sample), std::end(single_sample), std::less<int>{});
}

TEST_CASE ("Selection sort", "algo.sort") {
    array_t a{sample};

    selection_sort(std::begin(a), std::end(a), std::less<int>{});

    for (int i = 0; i < a.size(); i++)
        CHECK(a[i] == i);

    selection_sort(std::begin(empty_sample), std::end(empty_sample), std::less<int>{});
    selection_sort(std::begin(single_sample), std::end(single_sample), std::less<int>{});
}

TEST_CASE ("Insertion sort", "algo.sort") {
    array_t a{sample};

    insertion_sort(std::begin(a), std::end(a), std::less<int>{});

    for (int i = 0; i < a.size(); i++)
        CHECK(a[i] == i);

    insertion_sort(std::begin(empty_sample), std::end(empty_sample), std::less<int>{});
    insertion_sort(std::begin(single_sample), std::end(single_sample), std::less<int>{});
}


