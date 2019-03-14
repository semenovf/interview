#include <array>
#include <utility>
#include <algorithm>
#include <functional>
#include "catch.hpp"

template <typename BidirIter, typename Less>
void insertion_sort (BidirIter first, BidirIter last, Less less)
{
    using std::swap;

    BidirIter cur = first;

    while (cur != last) {
        BidirIter prev = cur;
        BidirIter x = cur;
        ++x;

        if (x == last)
            break;

        do {
            if (less(*x, *prev))
                swap(*x, *prev);

            --x;
        } while (prev-- != first);

        ++cur;
    }
}

TEST_CASE ("Insertion sort", "algo.sort") {
    std::array<int, 0> a0{};
    std::array<int, 1> a1{10};
    std::array<int, 10> a{5, 7, 4, 2, 8, 6, 1, 9, 0, 3};

    insertion_sort(std::begin(a0), std::end(a0), std::less<int>{});
    insertion_sort(std::begin(a1), std::end(a1), std::less<int>{});
    insertion_sort(std::begin(a), std::end(a), std::less<int>{});

    CHECK(a[0] == 0);
    CHECK(a[1] == 1);
    CHECK(a[2] == 2);
    CHECK(a[3] == 3);
    CHECK(a[4] == 4);
    CHECK(a[5] == 5);
    CHECK(a[6] == 6);
    CHECK(a[7] == 7);
    CHECK(a[8] == 8);
    CHECK(a[9] == 9);
}
